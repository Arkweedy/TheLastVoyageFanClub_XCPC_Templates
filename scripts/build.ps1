param(
    [switch]$KeepAux
)

$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot = Resolve-Path (Join-Path $ScriptDir "..")
$PreviousLocation = Get-Location

function Require-Command {
    param([string]$Name)

    if (-not (Get-Command $Name -ErrorAction SilentlyContinue)) {
        throw "Required command not found: $Name"
    }
}

function Invoke-Checked {
    param(
        [string]$Command,
        [string[]]$Arguments
    )

    Write-Host "==> $Command $($Arguments -join ' ')"
    & $Command @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code ${LASTEXITCODE}: $Command"
    }
}

function Remove-IfExists {
    param([string[]]$Paths)

    foreach ($Path in $Paths) {
        if (Test-Path -LiteralPath $Path) {
            Remove-Item -LiteralPath $Path -Recurse -Force
        }
    }
}

function Remove-Glob {
    param([string[]]$Patterns)

    foreach ($Pattern in $Patterns) {
        Get-ChildItem -Path "." -Filter $Pattern -File -ErrorAction SilentlyContinue |
            Remove-Item -Force
    }
}

function Use-PygmentsPython {
    $Pygmentize = Get-Command "pygmentize" -ErrorAction SilentlyContinue
    if (-not $Pygmentize) {
        return
    }

    $ScriptsDir = Split-Path -Parent $Pygmentize.Source
    $PythonRoot = Split-Path -Parent $ScriptsDir
    $PythonExe = Join-Path $PythonRoot "python.exe"

    if (Test-Path -LiteralPath $PythonExe) {
        $env:Path = "$PythonRoot;$ScriptsDir;$env:Path"
    }
}

try {
    Set-Location $RepoRoot

    $env:PYTHONIOENCODING = "utf-8"
    $env:PYTHONUTF8 = "1"
    Use-PygmentsPython

    Require-Command "xelatex"
    Require-Command "makeindex"
    Require-Command "pygmentize"
    Require-Command "python"

    New-Item -ItemType Directory -Force -Path "cover/dist" | Out-Null

    $xelatexCommon = @(
        "-interaction=nonstopmode",
        "-halt-on-error",
        "-file-line-error"
    )

    $xelatexMain = @(
        "-shell-escape",
        "-synctex=1",
        "-interaction=nonstopmode",
        "-halt-on-error",
        "-file-line-error",
        "main.tex"
    )

    Invoke-Checked "xelatex" ($xelatexCommon + @(
        "-shell-escape",
        "-output-directory=cover/dist",
        "cover/src/cover.tex"
    ))

    Invoke-Checked "xelatex" ($xelatexCommon + @(
        "-shell-escape",
        "-output-directory=cover/dist",
        "cover/src/cover.tex"
    ))

    Invoke-Checked "xelatex" ($xelatexCommon + @(
        "-output-directory=cover/dist",
        "cover/src/signature.tex"
    ))

    Invoke-Checked "xelatex" ($xelatexCommon + @(
        "-output-directory=cover/dist",
        "cover/src/signature.tex"
    ))

    Invoke-Checked "python" @("scripts/rainbow_brackets.py")

    Invoke-Checked "xelatex" $xelatexMain

    Invoke-Checked "xelatex" $xelatexMain

    if (-not $KeepAux) {
        Remove-IfExists @(
            "_minted",
            "_minted-main",
            "build",
            "cover/dist/cover.aux",
            "cover/dist/cover.log",
            "cover/dist/signature.aux",
            "cover/dist/signature.log",
            "main.aux",
            "main.fdb_latexmk",
            "main.fls",
            "main.idx",
            "main.ilg",
            "main.ind",
            "main.log",
            "main.out",
            "main.synctex.gz",
            "main.toc",
            "main.xdv"
        )
        Remove-Glob @("*.minted")
    }

    Write-Host "Built main.pdf"
}
finally {
    Set-Location $PreviousLocation
}
