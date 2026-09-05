#!/usr/bin/env python3
"""Generate an A4 review set of special-purpose XCPC scratch paper."""

from __future__ import annotations

import math
import shutil
import subprocess
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas


REPO_ROOT = Path(__file__).resolve().parents[2]
OUTPUT_DIR = REPO_ROOT / "output" / "pdf"
PREVIEW_DIR = OUTPUT_DIR / "previews"
PDF_PATH = OUTPUT_DIR / "xcpc-specialized-scratch-paper-preview.pdf"

PAGE_WIDTH, PAGE_HEIGHT = A4
LEFT = 10 * mm
RIGHT = PAGE_WIDTH - 10 * mm
BOTTOM = 18 * mm
TOP = PAGE_HEIGHT - 21.5 * mm

PAGES = (
    ("01-grid", "GRID", "5 mm cells / 25 mm guides"),
    ("02-checker", "CHECKER", "8 mm cells / 16% gray"),
    ("03-hex", "HEX", "6 mm side length"),
    ("04-triangular", "TRIANGULAR", "6 mm side / 30 mm guides"),
)


def draw_page_frame(pdf: canvas.Canvas, title: str, detail: str, page_no: int) -> None:
    """Draw a small identification strip without taking much writing space."""
    pdf.saveState()
    pdf.setFillGray(0.12)
    pdf.setFont("Helvetica-Bold", 9)
    pdf.drawString(LEFT, PAGE_HEIGHT - 14.2 * mm, title)

    pdf.setFillGray(0.42)
    pdf.setFont("Helvetica", 7.5)
    pdf.drawString(LEFT + 27 * mm, PAGE_HEIGHT - 14.2 * mm, detail)
    pdf.drawRightString(
        RIGHT,
        PAGE_HEIGHT - 14.2 * mm,
        f"PROBLEM: ______   SHEET: ______   {page_no}/4",
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


def generate_pdf() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    pdf = canvas.Canvas(str(PDF_PATH), pagesize=A4, pageCompression=1)
    pdf.setTitle("XCPC Special-purpose Scratch Paper Preview")
    pdf.setAuthor("XCPC Templates")
    pdf.setSubject("A4 grayscale review set: grid, checker, hex, triangular")

    drawers = (draw_grid, draw_checker, draw_hex, draw_triangular)
    for page_no, ((_, title, detail), drawer) in enumerate(zip(PAGES, drawers), start=1):
        draw_page_frame(pdf, title, detail, page_no)
        drawer(pdf)
        pdf.showPage()
    pdf.save()


def render_previews() -> bool:
    renderer = shutil.which("pdftocairo")
    if renderer is None:
        print("pdftocairo was not found; PDF generated without PNG previews.")
        return False

    PREVIEW_DIR.mkdir(parents=True, exist_ok=True)
    temporary_prefix = PREVIEW_DIR / "page"
    subprocess.run(
        [renderer, "-png", "-r", "150", str(PDF_PATH), str(temporary_prefix)],
        check=True,
    )

    preview_paths: list[Path] = []
    for page_no, (slug, _, _) in enumerate(PAGES, start=1):
        rendered = PREVIEW_DIR / f"page-{page_no}.png"
        target = PREVIEW_DIR / f"{slug}.png"
        if target.exists():
            target.unlink()
        rendered.replace(target)
        preview_paths.append(target)

    make_contact_sheet(preview_paths)
    return True


def make_contact_sheet(preview_paths: list[Path]) -> None:
    thumb_width = 780
    gap = 34
    label_height = 54
    outer = 40
    font = ImageFont.load_default(size=26)
    pages: list[Image.Image] = []

    for path in preview_paths:
        with Image.open(path) as source:
            page = source.convert("RGB")
            page.thumbnail((thumb_width, 1120), Image.Resampling.LANCZOS)
            pages.append(page.copy())

    cell_width = max(page.width for page in pages)
    cell_height = max(page.height for page in pages) + label_height
    sheet = Image.new(
        "RGB",
        (outer * 2 + cell_width * 2 + gap, outer * 2 + cell_height * 2 + gap),
        "#e9e9e9",
    )
    draw = ImageDraw.Draw(sheet)

    for index, (page, (_, title, _)) in enumerate(zip(pages, PAGES)):
        column = index % 2
        row = index // 2
        x = outer + column * (cell_width + gap)
        y = outer + row * (cell_height + gap)
        draw.rectangle(
            (x - 1, y - 1, x + page.width, y + page.height),
            fill="white",
            outline="#707070",
            width=2,
        )
        sheet.paste(page, (x, y))
        draw.text((x, y + page.height + 12), title, fill="#222222", font=font)

    sheet.save(PREVIEW_DIR / "contact-sheet.png", optimize=True)


def main() -> None:
    generate_pdf()
    rendered = render_previews()
    print(f"Generated: {PDF_PATH}")
    if rendered:
        print(f"Rendered previews: {PREVIEW_DIR}")


if __name__ == "__main__":
    main()
