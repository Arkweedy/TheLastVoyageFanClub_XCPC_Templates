# XCPC special-purpose scratch paper

This directory contains the generator for a separate A4 scratch-paper pack. It
is intentionally not assembled into `main.pdf`: teams can print only the paper
patterns they expect to need.

The current review set contains four grayscale pages:

1. `GRID`: 5 mm squares with a darker guide every 25 mm.
2. `CHECKER`: 8 mm squares with alternating cells filled at 16% black, leaving
   handwritten ink clearly visible.
3. `HEX`: a regular honeycomb with 6 mm side length.
4. `TRIANGULAR`: equilateral triangles with 6 mm side length and darker guides
   every five intervals.

All pages use a 10 mm printer-safe horizontal margin, a small problem/sheet
header, and a real 50 mm calibration ruler. Print at `Actual size` or `100%`.

## Generate

From the repository root:

```powershell
python .\printables\scratch-paper\generate_scratch_paper.py
```

The combined vector PDF is written to
`output/pdf/xcpc-specialized-scratch-paper-preview.pdf`. When `pdftocairo` is
available, 150 dpi page previews and a contact sheet are also written under
`output/pdf/previews/`.
