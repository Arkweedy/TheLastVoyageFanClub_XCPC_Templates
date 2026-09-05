# XCPC scratch paper

This directory contains only the source and usage notes for the standalone A4
scratch-paper pack. The generated PDF is a release artifact: it is written to
the repository root as `xcpc-scratch-paper.pdf` and is ignored by Git.

## Contents

The PDF contains seven grayscale styles with ten consecutive sides per style,
for 70 A4 pages total:

| Pages | Style | Layout |
| --- | --- | --- |
| 1-10 | `GRID` | 5 mm squares with 25 mm guides |
| 11-20 | `CHECKER` | 8 mm squares with alternating 16% gray cells |
| 21-30 | `HEX` | regular honeycomb with 6 mm side length |
| 31-40 | `TRIANGULAR` | 6 mm equilateral triangles with 30 mm guides |
| 41-50 | `DOT GRID` | 5 mm orthogonal dots with 25 mm anchors |
| 51-60 | `ISOMETRIC DOTS` | 6 mm triangular dot lattice |
| 61-70 | `CARTESIAN` | centered axes on a 5 mm square grid |

Because every group contains an even number of sides, duplex printing keeps
style boundaries between physical sheets: each style occupies exactly five
sheets. Single-sided printing uses ten sheets per style.

## Generate

From the repository root:

```powershell
python .\printables\scratch-paper\generate_scratch_paper.py
```

The script requires `reportlab`. If `pdftocairo` is available, it also renders
the first and last pages into a system temporary directory as a smoke test.
Those PNGs and the temporary directory are removed automatically; the only
persistent output is the root-level `xcpc-scratch-paper.pdf`.

## Print and release

Print at `Actual size` or `100%`. Use long-edge duplex mode when printing on
both sides. Do not commit the PDF; attach it to a GitHub Release together with
`main.pdf`.
