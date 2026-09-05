#!/usr/bin/env python3
"""Generate A4 review and print sets of special-purpose XCPC scratch paper."""

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
PRINT_PDF_PATH = OUTPUT_DIR / "xcpc-specialized-scratch-paper-print-70-sides.pdf"
PRINT_STYLE_COUNT = 7
PRINT_COPIES = 10

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
    ("05-dot-grid", "DOT GRID", "5 mm spacing / 25 mm anchors"),
    ("06-isometric-dots", "ISOMETRIC DOTS", "6 mm triangular spacing"),
    ("07-cartesian", "CARTESIAN", "1 cell = 1 unit / 5 mm cells"),
    ("08-polar", "POLAR", "10 mm rings / 15 degree spokes"),
    ("09-mixed", "MIXED", "blank upper half / 5 mm lower grid"),
    ("10-graph-nodes", "GRAPH NODES", "15 mm staggered hollow nodes"),
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


def draw_polar(pdf: canvas.Canvas) -> None:
    center_x = (LEFT + RIGHT) / 2
    center_y = (BOTTOM + TOP) / 2
    outer_radius = 90 * mm

    pdf.saveState()
    for radius_mm in range(10, 91, 10):
        major = radius_mm % 30 == 0
        pdf.setStrokeGray(0.53 if major else 0.78)
        pdf.setLineWidth(0.55 if major else 0.25)
        pdf.circle(center_x, center_y, radius_mm * mm, stroke=1, fill=0)

    for angle in range(0, 180, 15):
        radians = math.radians(angle)
        major = angle % 45 == 0
        pdf.setStrokeGray(0.46 if major else 0.75)
        pdf.setLineWidth(0.62 if major else 0.28)
        dx = outer_radius * math.cos(radians)
        dy = outer_radius * math.sin(radians)
        pdf.line(center_x - dx, center_y - dy, center_x + dx, center_y + dy)

    pdf.setFillGray(0.32)
    pdf.setFont("Helvetica", 6)
    label_gap = 2.3 * mm
    pdf.drawString(center_x + outer_radius + label_gap, center_y - 0.8 * mm, "0")
    pdf.drawCentredString(center_x, center_y + outer_radius + label_gap, "90")
    pdf.drawRightString(center_x - outer_radius - label_gap, center_y - 0.8 * mm, "180")
    pdf.drawCentredString(center_x, center_y - outer_radius - label_gap - 1.5 * mm, "270")
    pdf.setStrokeGray(0.30)
    pdf.setLineWidth(0.72)
    pdf.circle(center_x, center_y, outer_radius, stroke=1, fill=0)
    pdf.restoreState()


def draw_mixed(pdf: canvas.Canvas) -> None:
    step = 5 * mm
    grid_rows = 25
    split_y = BOTTOM + grid_rows * step

    pdf.saveState()
    pdf.setStrokeGray(0.80)
    pdf.setLineWidth(0.25)
    columns = int((RIGHT - LEFT) // step)
    for index in range(columns + 1):
        if index % 5:
            x = LEFT + index * step
            pdf.line(x, BOTTOM, x, split_y)
    for index in range(grid_rows + 1):
        if index % 5:
            y = BOTTOM + index * step
            pdf.line(LEFT, y, RIGHT, y)

    pdf.setStrokeGray(0.52)
    pdf.setLineWidth(0.56)
    for index in range(0, columns + 1, 5):
        x = LEFT + index * step
        pdf.line(x, BOTTOM, x, split_y)
    for index in range(0, grid_rows + 1, 5):
        y = BOTTOM + index * step
        pdf.line(LEFT, y, RIGHT, y)

    pdf.setStrokeGray(0.32)
    pdf.setLineWidth(0.72)
    pdf.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM, stroke=1, fill=0)
    pdf.line(LEFT, split_y, RIGHT, split_y)
    pdf.restoreState()


def draw_graph_nodes(pdf: canvas.Canvas) -> None:
    step = 15 * mm
    columns = 12
    rows = 17
    used_width = (columns - 1) * step + step / 2
    used_height = (rows - 1) * step
    x0 = LEFT + ((RIGHT - LEFT) - used_width) / 2
    y0 = BOTTOM + ((TOP - BOTTOM) - used_height) / 2

    pdf.saveState()
    pdf.setStrokeGray(0.57)
    pdf.setLineWidth(0.42)
    for row in range(rows):
        offset = (row & 1) * step / 2
        for column in range(columns):
            x = x0 + offset + column * step
            y = y0 + row * step
            pdf.circle(x, y, 1.35 * mm, stroke=1, fill=0)
    pdf.setStrokeGray(0.84)
    pdf.setLineWidth(0.45)
    pdf.rect(LEFT, BOTTOM, RIGHT - LEFT, TOP - BOTTOM, stroke=1, fill=0)
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
        draw_polar,
        draw_mixed,
        draw_graph_nodes,
    )


def generate_pdf() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    pdf = canvas.Canvas(str(PDF_PATH), pagesize=A4, pageCompression=1)
    pdf.setTitle("XCPC Special-purpose Scratch Paper Preview")
    pdf.setAuthor("XCPC Templates")
    pdf.setSubject("A4 grayscale review set: ten scratch-paper styles")

    drawers = paper_drawers()
    for page_no, ((_, title, detail), drawer) in enumerate(zip(PAGES, drawers), start=1):
        draw_page_frame(pdf, title, detail, f"{page_no}/{len(PAGES)}")
        drawer(pdf)
        pdf.showPage()
    pdf.save()


def generate_print_pdf() -> None:
    """Generate ten consecutive printable sides for each accepted paper style."""
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    pdf = canvas.Canvas(str(PRINT_PDF_PATH), pagesize=A4, pageCompression=1)
    pdf.setTitle("XCPC Special-purpose Scratch Paper - 70 Printable Sides")
    pdf.setAuthor("XCPC Templates")
    pdf.setSubject("Seven accepted A4 grayscale styles, ten sides per style")

    accepted_pages = PAGES[:PRINT_STYLE_COUNT]
    accepted_drawers = paper_drawers()[:PRINT_STYLE_COUNT]
    for style_no, ((_, title, detail), drawer) in enumerate(
        zip(accepted_pages, accepted_drawers), start=1
    ):
        for copy_no in range(1, PRINT_COPIES + 1):
            if copy_no == 1:
                bookmark = f"style-{style_no}"
                pdf.bookmarkPage(bookmark)
                pdf.addOutlineEntry(title, bookmark, level=0, closed=False)
            draw_page_frame(pdf, title, detail, f"COPY {copy_no}/{PRINT_COPIES}")
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
    for stale_preview in PREVIEW_DIR.glob("page-*.png"):
        stale_preview.unlink()
    subprocess.run(
        [renderer, "-png", "-r", "150", str(PDF_PATH), str(temporary_prefix)],
        check=True,
    )

    rendered_pages = sorted(
        PREVIEW_DIR.glob("page-*.png"),
        key=lambda path: int(path.stem.rsplit("-", 1)[1]),
    )
    if len(rendered_pages) != len(PAGES):
        raise RuntimeError(
            f"Expected {len(PAGES)} rendered pages, found {len(rendered_pages)}"
        )

    preview_entries: list[tuple[Path, str]] = []
    for rendered, (slug, title, _) in zip(rendered_pages, PAGES):
        target = PREVIEW_DIR / f"{slug}.png"
        if target.exists():
            target.unlink()
        rendered.replace(target)
        preview_entries.append((target, title))

    make_contact_sheet(preview_entries, "contact-sheet.png")
    make_contact_sheet(preview_entries[:4], "contact-sheet-core.png")
    make_contact_sheet(preview_entries[4:], "contact-sheet-additions.png")

    print_checks = (
        (1, "print-pack-page-01-grid"),
        (PRINT_STYLE_COUNT * PRINT_COPIES, "print-pack-page-70-cartesian"),
    )
    for page_no, filename in print_checks:
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
                str(PRINT_PDF_PATH),
                str(PREVIEW_DIR / filename),
            ],
            check=True,
        )
    return True


def make_contact_sheet(preview_entries: list[tuple[Path, str]], filename: str) -> None:
    thumb_width = 780
    gap = 34
    label_height = 54
    outer = 40
    font = ImageFont.load_default(size=26)
    pages: list[tuple[Image.Image, str]] = []

    for path, title in preview_entries:
        with Image.open(path) as source:
            page = source.convert("RGB")
            page.thumbnail((thumb_width, 1120), Image.Resampling.LANCZOS)
            pages.append((page.copy(), title))

    columns = 2
    rows = math.ceil(len(pages) / columns)
    cell_width = max(page.width for page, _ in pages)
    cell_height = max(page.height for page, _ in pages) + label_height
    sheet = Image.new(
        "RGB",
        (
            outer * 2 + cell_width * columns + gap * (columns - 1),
            outer * 2 + cell_height * rows + gap * (rows - 1),
        ),
        "#e9e9e9",
    )
    draw = ImageDraw.Draw(sheet)

    for index, (page, title) in enumerate(pages):
        column = index % columns
        row = index // columns
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

    sheet.save(PREVIEW_DIR / filename, optimize=True)


def main() -> None:
    generate_pdf()
    generate_print_pdf()
    rendered = render_previews()
    print(f"Generated preview: {PDF_PATH}")
    print(f"Generated print pack: {PRINT_PDF_PATH}")
    if rendered:
        print(f"Rendered previews: {PREVIEW_DIR}")


if __name__ == "__main__":
    main()
