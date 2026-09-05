# XCPC special-purpose scratch paper

This directory contains the generator for a separate A4 scratch-paper pack. It
is intentionally not assembled into `main.pdf`: teams can print only the paper
patterns they expect to need.

The current review set contains ten grayscale pages:

1. `GRID`: 5 mm squares with a darker guide every 25 mm.
2. `CHECKER`: 8 mm squares with alternating cells filled at 16% black, leaving
   handwritten ink clearly visible.
3. `HEX`: a regular honeycomb with 6 mm side length.
4. `TRIANGULAR`: equilateral triangles with 6 mm side length and darker guides
   every five intervals.
5. `DOT GRID`: an orthogonal 5 mm dot grid with a larger anchor every 25 mm.
6. `ISOMETRIC DOTS`: a triangular 6 mm dot lattice for isometric sketches.
7. `CARTESIAN`: a 5 mm square grid with centered axes, arrowheads, and labels.
8. `POLAR`: a 90 mm-radius polar grid with 10 mm rings and 15-degree spokes.
9. `MIXED`: an unruled upper half and a 5 mm grid on the lower half.
10. `GRAPH NODES`: 15 mm-spaced staggered hollow circles for graph sketches.

The production print pack accepts the first seven styles (`GRID` through
`CARTESIAN`) and excludes `POLAR`, `MIXED`, and `GRAPH NODES`. Each accepted
style appears on ten consecutive sides, for 70 A4 pages total:

| Pages | Style |
| --- | --- |
| 1-10 | `GRID` |
| 11-20 | `CHECKER` |
| 21-30 | `HEX` |
| 31-40 | `TRIANGULAR` |
| 41-50 | `DOT GRID` |
| 51-60 | `ISOMETRIC DOTS` |
| 61-70 | `CARTESIAN` |

Because every group contains an even number of sides, duplex printing keeps
style boundaries between physical sheets: each style occupies exactly five
sheets. Single-sided printing uses ten sheets per style.

All pages use a 10 mm printer-safe horizontal margin, a small problem/sheet
header, and a real 50 mm calibration ruler. Print at `Actual size` or `100%`.

## Generate

From the repository root:

```powershell
python .\printables\scratch-paper\generate_scratch_paper.py
```

The combined vector PDF is written to
`output/pdf/xcpc-specialized-scratch-paper-preview.pdf`. When `pdftocairo` is
available, 150 dpi page previews are also written under
`output/pdf/previews/`. `contact-sheet.png` shows all ten pages,
`contact-sheet-core.png` keeps the original four-page overview, and
`contact-sheet-additions.png` shows the six suggested additions.

The ready-to-print 70-side pack is written to
`output/pdf/xcpc-specialized-scratch-paper-print-70-sides.pdf`. Print it at
`Actual size` or `100%`; use long-edge duplex mode when printing on both sides.
The generator also renders the first and last print-pack pages as PNGs for a
quick check that the 70-page sequence starts and ends correctly.
