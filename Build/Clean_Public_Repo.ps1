param(
    [string]$Root = (Resolve-Path (Join-Path $PSScriptRoot ".."))
)

$ErrorActionPreference = "Stop"

$removeDirs = @(
    "Config",
    "CMake",
    "src"
)

$removeFiles = @(
    "generators.txt",
    "Build\BUILD.txt",
    "Build\Package_Nyxoryth.bat",
    "Build\build_notes.txt",
    "TESTS\Release_Checklist.md",
    "TESTS\V1_RC1_Checklist.md"
)

foreach ($relative in $removeDirs) {
    $path = Join-Path $Root $relative
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force
        Write-Host "Removed:" $relative
    }
}

foreach ($relative in $removeFiles) {
    $path = Join-Path $Root $relative
    if (Test-Path $path) {
        Remove-Item $path -Force
        Write-Host "Removed:" $relative
    }
}

$sourceRoot = Join-Path $Root "Source"
$keep = @(
    (Join-Path $Root "Source\App\Main.cpp"),
    (Join-Path $Root "Source\UI\MainWindow.cpp"),
    (Join-Path $Root "Source\UI\MainWindow.h"),
    (Join-Path $Root "Source\Resources\Nyxoryth.ico"),
    (Join-Path $Root "Source\Resources\Nyxoryth.rc"),
    (Join-Path $Root "Source\Resources\Resource.h")
) | ForEach-Object { [System.IO.Path]::GetFullPath($_) }

Get-ChildItem $sourceRoot -Recurse -File | ForEach-Object {
    $full = [System.IO.Path]::GetFullPath($_.FullName)
    if ($keep -notcontains $full) {
        Remove-Item $_.FullName -Force
        Write-Host "Removed legacy source:" $_.FullName
    }
}

Get-ChildItem $sourceRoot -Recurse -Directory |
    Sort-Object FullName -Descending |
    ForEach-Object {
        if ((Get-ChildItem $_.FullName -Force | Measure-Object).Count -eq 0) {
            Remove-Item $_.FullName -Force
        }
    }

foreach ($relative in @("Assets\Backgrounds", "Assets\Themes")) {
    $path = Join-Path $Root $relative
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force
    }
}

Write-Host ""
Write-Host "Nyxoryth public-source cleanup complete."
Write-Host "Keep TESTS\1.1.0_Release_Checklist.md and TESTS\Extended_Calculation_Checklist.md."
Write-Host "Run: git status"
Write-Host "Review the deletions, then commit only if everything looks correct."
