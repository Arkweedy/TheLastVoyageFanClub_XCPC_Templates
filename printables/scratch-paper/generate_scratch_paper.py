#!/usr/bin/env python3
"""Generate the final A4 XCPC scratch-paper print pack."""

from __future__ import annotations

import math
import shutil
import subprocess
import tempfile
from pathlib import Path

from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas


REPO_ROOT = Path(__file__).resolve().parents[2]
PDF_PATH = REPO_ROOT / "xcpc-scratch-paper.pdf"
COPIES_PER_STYLE = 10

PAGE_WIDTH, PAGE_HEIGHT = A4
LEFT = 10 * mm
RIGHT = PAGE_WIDTH - 10 * mm
BOTTOM = 18 * mm
TOP = PAGE_HEIGHT - 21.5 * mm

PAPER_STYLES = (
    ("GRID", "5 mm cells / 25 mm guides"),
    ("CHECKER", "8 mm cells / 16% gray"),
    ("HEX", "6 mm side length"),
    ("TRIANGULAR", "6 mm side / 30 mm guides"),
    ("DOT GRID", "5 mm spacing / 25 mm anchors"),
    ("ISOMETRIC DOTS", "6 mm triangular spacing"),
    ("CARTESIAN", "1 cell = 1 unit / 5 mm cells"),
)


def draw_page_frame(pdf: canvas.Canvas, title: str, detail: str, marker: str) -> None:
    """Draw a small identification strip without taking much writing space."""
    pdf.saveState()
    pdf.setFillGray(0.12)
    pdf.setFont("Helvetica-Bold", 9)
    pdf.drawString(LEFT, PAGE_HEIGHT - 14.2 * mm, title)

    pdf.setFillGray(0.42)
    pdf.setFont("Helvetica", 7.5)
    title_width = pdf.stringWidth(title, "Helvetica-Bold", 9)
    detail_x = max(LEFT + 27 * mm, LEFT + title_width + 7 * mm)
    pdf.drawString(detail_x, PAGE_HEIGHT - 14.2 * mm, detail)
    pdf.drawRightString(
        RIGHT,
        PAGE_HEIGHT - 14.2 * mm,
        f"PROBLEM: ______   SHEET: ______   {marker}",
    )

    pdf.setStrokeGray(0.72)
    pdf.setLineWidth(0.35)
    pdf.line(LEFT, PAGE_HEIGHT - 17.2 * mm, RIGHT, PAGE_HEIGHT - 17.2 * mm)

    # A real-length ruler makes accidental "fit to page" scaling easy to detect.
    ruler_x = RIGHT - 50 * mm
    ruler_y = 11.7 * mm
    pdf.setStrokeGray(0.34)
    pdf.setFillGray(0.34)
    pdf.setLineWidth(0.45)
    pdf.line(ruler_x, ruler_y, RIGHT, ruler_y)
    for step in range(6):
        x = ruler_x + step * 10 * mm
        tick = 2.2 * mm if step in (0, 5) else 1.4 * mm
        pdf.line(x, ruler_y - tick / 2, x, ruler_y + tick / 2)
    pdf.setFont("Helvetica", 6.5)
    pdf.drawRightString(RIGHT, 14.2 * mm, "50 mm - print at actual size (100%)")
    pdf.restoreState()


def centered_lattice(step: float) -> tuple[float, float, int, int]:
    """Return an integer-cell rectangle centered inside the writable area."""
    columns = int((RIGHT - LEFT) // step)
    rows = int((TOP - BOTTOM) // step)
    width = columns * step
    height = rows * step
    return (
        LEFT + ((RIGHT - LEFT) - width) / 2,
        BOTTOM + ((TOP - BOTTOM) - height) / 2,
        columns,
        rows,
    )


def draw_grid(pdf: canvas.Canvas) -> None:
    step = 5 * mm
    x0, y0, columns, rows = centered_lattice(step)
    x1 = x0 + columns * step
    y1 = y0 + rows * step

    pdf.saveState()
    pdf.setStrokeGray(0.79)
    pdf.setLineWidth(0.25)
    for index in range(columns + 1):
        if index % 5:
            x = x0 + index * step
            pdf.line(x, y0, x, y1)
    for index in range(rows + 1):
        if index % 5:
            y = y0 + index * step
            pdf.line(x0, y, x1, y)

    pdf.setStrokeGray(0.50)
    pdf.setLineWidth(0.58)
    for index in range(0, columns + 1, 5):
        x = x0 + index * step
        pdf.line(x, y0, x, y1)
    for index in range(0, rows + 1, 5):
        y = y0 + index * step
        pdf.line(x0, y, x1, y)

    pdf.setStrokeGray(0.36)
    pdf.setLineWidth(0.75)
    pdf.rect(x0, y0, x1 - x0, y1 - y0, stroke=1, fill=0)
    pdf.restoreState()


def draw_checker(pdf: canvas.Canvas) -> None:
    step = 8 * mm
    x0, y0, columns, rows = centered_lattice(step)
    x1 = x0 + columns * step
    y1 = y0 + rows * step

    pdf.saveState()
    pdf.setFillGray(0.84)
    for row in range(rows):
        for column in range(columns):
            if (row + column) % 2 == 0:
                pdf.rect(
                    x0 + column * step,
                    y0 + row * step,
                    step,
                    step,
                    stroke=0,
                    fill=1,
                )

    pdf.setStrokeGray(0.61)
    pdf.setLineWidth(0.34)
    for index in range(columns + 1):
        x = x0 + index * step
        pdf.line(x, y0, x, y1)
    for index in range(rows + 1):
        y = y0 + index * step
        pdf.line(x0, y, x1, y)

    pdf.setStrokeGray(0.34)
    pdf.setLineWidth(0.72)
    pdf.rect(x0, y0, x1 - x0, y1 - y0, stroke=1, fill=0)
    pdf.restoreState()


def clipped_pattern_area(pdf: canvas.Canvas) -> None:
    clip = pdf.beginPath()
    clip.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM)
    pdf.clipPath(clip, stroke=0, fill=0)


def draw_hex(pdf: canvas.Canvas) -> None:
    side = 6 * mm
    height = math.sqrt(3) * side
    x_step = 1.5 * side
    edges: set[tuple[tuple[int, int], tuple[int, int]]] = set()

    # Generate one extra ring of cells, then clip it to a clean rectangular field.
    first_column = -2
    last_column = math.ceil((RIGHT - LEFT) / x_step) + 2
    first_row = -2
    last_row = math.ceil((TOP - BOTTOM) / height) + 2
    for column in range(first_column, last_column + 1):
        cx = LEFT + column * x_step
        offset = (column & 1) * height / 2
        for row in range(first_row, last_row + 1):
            cy = BOTTOM + row * height + offset
            vertices = [
                (
                    cx + side * math.cos(math.radians(60 * vertex)),
                    cy + side * math.sin(math.radians(60 * vertex)),
                )
                for vertex in range(6)
            ]
            for vertex in range(6):
                a = vertices[vertex]
                b = vertices[(vertex + 1) % 6]
                # Integer micro-points make coincident shared edges hash identically.
                aa = (round(a[0] * 1000), round(a[1] * 1000))
                bb = (round(b[0] * 1000), round(b[1] * 1000))
                edges.add(tuple(sorted((aa, bb))))

    pdf.saveState()
    clipped_pattern_area(pdf)
    pdf.setStrokeGray(0.61)
    pdf.setLineWidth(0.38)
    for a, b in edges:
        pdf.line(a[0] / 1000, a[1] / 1000, b[0] / 1000, b[1] / 1000)
    pdf.restoreState()

    pdf.saveState()
    pdf.setStrokeGray(0.34)
    pdf.setLineWidth(0.72)
    pdf.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM, stroke=1, fill=0)
    pdf.restoreState()


def draw_triangular(pdf: canvas.Canvas) -> None:
    side = 6 * mm
    height = math.sqrt(3) * side / 2
    field_height = TOP - BOTTOM
    diagonal_run = field_height / math.sqrt(3)

    pdf.saveState()
    clipped_pattern_area(pdf)

    # Draw minor lines first, followed by every fifth guide in a darker tone.
    for major in (False, True):
        pdf.setStrokeGray(0.55 if major else 0.79)
        pdf.setLineWidth(0.55 if major else 0.25)

        row_min = -1
        row_max = math.ceil(field_height / height) + 1
        for row in range(row_min, row_max + 1):
            if (row % 5 == 0) != major:
                continue
            y = BOTTOM + row * height
            pdf.line(LEFT, y, RIGHT, y)

        start_min = math.floor(-diagonal_run / side) - 2
        start_max = math.ceil((RIGHT - LEFT + diagonal_run) / side) + 2
        for index in range(start_min, start_max + 1):
            if (index % 5 == 0) != major:
                continue
            x = LEFT + index * side
            pdf.line(x, BOTTOM, x + diagonal_run, TOP)
            pdf.line(x, BOTTOM, x - diagonal_run, TOP)

    pdf.restoreState()

    pdf.saveState()
    pdf.setStrokeGray(0.34)
    pdf.setLineWidth(0.72)
    pdf.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM, stroke=1, fill=0)
    pdf.restoreState()


def draw_dot_grid(pdf: canvas.Canvas) -> None:
    step = 5 * mm
    x0, y0, columns, rows = centered_lattice(step)

    pdf.saveState()
    for row in range(rows + 1):
        for column in range(columns + 1):
            major = row % 5 == 0 and column % 5 == 0
            pdf.setFillGray(0.38 if major else 0.59)
            radius = (0.28 if major else 0.18) * mm
            pdf.circle(x0 + column * step, y0 + row * step, radius, stroke=0, fill=1)
    pdf.setStrokeGray(0.78)
    pdf.setLineWidth(0.45)
    pdf.rect(x0, y0, columns * step, rows * step, stroke=1, fill=0)
    pdf.restoreState()


def draw_isometric_dots(pdf: canvas.Canvas) -> None:
    side = 6 * mm
    row_height = math.sqrt(3) * side / 2
    rows = int((TOP - BOTTOM) // row_height)
    y0 = BOTTOM + ((TOP - BOTTOM) - rows * row_height) / 2

    pdf.saveState()
    for row in range(rows + 1):
        offset = (row & 1) * side / 2
        column = 0
        x = LEFT + offset
        while x <= RIGHT + 0.01:
            major = row % 5 == 0 and column % 5 == 0
            pdf.setFillGray(0.38 if major else 0.59)
            radius = (0.28 if major else 0.18) * mm
            pdf.circle(x, y0 + row * row_height, radius, stroke=0, fill=1)
            column += 1
            x = LEFT + offset + column * side
    pdf.setStrokeGray(0.78)
    pdf.setLineWidth(0.45)
    pdf.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM, stroke=1, fill=0)
    pdf.restoreState()


def draw_cartesian(pdf: canvas.Canvas) -> None:
    step = 5 * mm
    columns = 38
    rows = 50
    x0 = LEFT
    y0 = BOTTOM + ((TOP - BOTTOM) - rows * step) / 2
    x1 = x0 + columns * step
    y1 = y0 + rows * step
    center_column = columns // 2
    center_row = rows // 2
    origin_x = x0 + center_column * step
    origin_y = y0 + center_row * step

    pdf.saveState()
    pdf.setStrokeGray(0.81)
    pdf.setLineWidth(0.24)
    for index in range(1, columns):
        coordinate = index - center_column
        if coordinate % 5:
            x = x0 + index * step
            pdf.line(x, y0, x, y1)
    for index in range(1, rows):
        coordinate = index - center_row
        if coordinate % 5:
            y = y0 + index * step
            pdf.line(x0, y, x1, y)

    pdf.setStrokeGray(0.58)
    pdf.setLineWidth(0.52)
    for index in range(1, columns):
        coordinate = index - center_column
        if coordinate and coordinate % 5 == 0:
            x = x0 + index * step
            pdf.line(x, y0, x, y1)
    for index in range(1, rows):
        coordinate = index - center_row
        if coordinate and coordinate % 5 == 0:
            y = y0 + index * step
            pdf.line(x0, y, x1, y)

    pdf.setStrokeGray(0.24)
    pdf.setLineWidth(0.92)
    pdf.line(x0, origin_y, x1, origin_y)
    pdf.line(origin_x, y0, origin_x, y1)
    arrow = 2.2 * mm
    pdf.line(x1, origin_y, x1 - arrow, origin_y + arrow / 2)
    pdf.line(x1, origin_y, x1 - arrow, origin_y - arrow / 2)
    pdf.line(origin_x, y1, origin_x - arrow / 2, y1 - arrow)
    pdf.line(origin_x, y1, origin_x + arrow / 2, y1 - arrow)

    pdf.setFillGray(0.30)
    pdf.setFont("Helvetica", 5.5)
    for value in range(-15, 16, 5):
        if value:
            pdf.drawCentredString(origin_x + value * step, origin_y - 2.5 * mm, str(value))
    for value in range(-20, 21, 5):
        if value:
            pdf.drawRightString(origin_x - 1.6 * mm, origin_y + value * step - 0.7 * mm, str(value))
    pdf.drawRightString(origin_x - 1.3 * mm, origin_y - 2.5 * mm, "0")
    pdf.setFont("Helvetica-Bold", 6.5)
    pdf.drawString(x1 - 3.8 * mm, origin_y + 1.5 * mm, "x")
    pdf.drawString(origin_x + 1.5 * mm, y1 - 3.8 * mm, "y")

    pdf.setStrokeGray(0.32)
    pdf.setLineWidth(0.72)
    pdf.rect(x0, y0, x1 - x0, y1 - y0, stroke=1, fill=0)
    pdf.restoreState()


def paper_drawers() -> tuple:
    return (
        draw_grid,
        draw_checker,
        draw_hex,
        draw_triangular,
        draw_dot_grid,
        draw_isometric_dots,
        draw_cartesian,
    )


def generate_pdf() -> None:
    pdf = canvas.Canvas(str(PDF_PATH), pagesize=A4, pageCompression=1)
    pdf.setTitle("XCPC Scratch Paper")
    pdf.setAuthor("XCPC Templates")
    pdf.setSubject("Seven A4 grayscale styles, ten printable sides per style")

    drawers = paper_drawers()
    for style_no, ((title, detail), drawer) in enumerate(
        zip(PAPER_STYLES, drawers), start=1
    ):
        for copy_no in range(1, COPIES_PER_STYLE + 1):
            if copy_no == 1:
                bookmark = f"style-{style_no}"
                pdf.bookmarkPage(bookmark)
                pdf.addOutlineEntry(title, bookmark, level=0, closed=False)
            draw_page_frame(pdf, title, detail, f"COPY {copy_no}/{COPIES_PER_STYLE}")
            drawer(pdf)
            pdf.showPage()
    pdf.save()


def verify_render() -> bool:
    """Render the first and last page in a temporary directory, then clean it."""
    renderer = shutil.which("pdftocairo")
    if renderer is None:
        print("pdftocairo was not found; skipped temporary render verification.")
        return False

    page_count = len(PAPER_STYLES) * COPIES_PER_STYLE
    with tempfile.TemporaryDirectory(prefix="xcpc-scratch-paper-") as temp_dir:
        temp_path = Path(temp_dir)
        for page_no in (1, page_count):
            output_prefix = temp_path / f"page-{page_no}"
            subprocess.run(
                [
                    renderer,
                    "-png",
                    "-r",
                    "150",
                    "-f",
                    str(page_no),
                    "-l",
                    str(page_no),
                    "-singlefile",
                    str(PDF_PATH),
                    str(output_prefix),
                ],
                check=True,
            )
            rendered = output_prefix.with_suffix(".png")
            if not rendered.is_file() or rendered.stat().st_size == 0:
                raise RuntimeError(f"Render verification failed for page {page_no}")
    return True


def main() -> None:
    generate_pdf()
    verified = verify_render()
    print(f"Generated: {PDF_PATH}")
    if verified:
        print("Verified first and last pages; temporary previews were removed.")


if __name__ == "__main__":
    main()
