# XCPC Templates

This repository is a LaTeX-based XCPC template collection.

## Build

Use the PowerShell build script from the repository root:

```powershell
.\scripts\build.ps1
```

The final document is written to:

```text
main.pdf
```

The script also refreshes the cover PDFs used by `main.tex`. This is an intentional extension over the original VSCode LaTeX Workshop recipe, which only compiled `main.tex` and reused the existing cover PDFs:

```text
cover/dist/cover.pdf
cover/dist/signature.pdf
```

## Requirements

- TeX Live with `xelatex` and `makeindex`
- Python/Pygments with `pygmentize`; the build script prefers the Python environment that provides `pygmentize`, which avoids `latexminted` issues with incompatible Python versions.
- Fonts referenced by `main.tex`, such as `Linux Libertine O`, `Microsoft YaHei`, `SimHei`, `KaiTi`, and `Consolas`

## Manual Build

If you need to run the commands manually, use the repository root as the working directory:

```powershell
$env:PYTHONIOENCODING = "utf-8"
$env:PYTHONUTF8 = "1"

xelatex -interaction=nonstopmode -halt-on-error -file-line-error -shell-escape -output-directory=cover/dist cover/src/cover.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -shell-escape -output-directory=cover/dist cover/src/cover.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -output-directory=cover/dist cover/src/signature.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -output-directory=cover/dist cover/src/signature.tex
xelatex -shell-escape -synctex=1 -interaction=nonstopmode -halt-on-error -file-line-error main.tex
xelatex -shell-escape -synctex=1 -interaction=nonstopmode -halt-on-error -file-line-error main.tex
```

Pass `-KeepAux` to keep LaTeX auxiliary files for debugging:

```powershell
.\scripts\build.ps1 -KeepAux
```
