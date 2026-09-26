param(
    [Parameter(Mandatory=$true)]
    [string]$PortableFolder
)

$ErrorActionPreference = "Stop"

$required = @(
    "Nyxoryth.exe",
    "README.md",
    "LICENSE",
    "THIRD_PARTY_NOTICES.md",
    "SECURITY.md",
    "VERSION.txt",
    "SHA256SUMS.txt"
)

foreach ($name in $required) {
    $path = Join-Path $PortableFolder $name
    if (-not (Test-Path $path -PathType Leaf)) {
        throw "Missing release file: $name"
    }
}

$forbiddenExtensions = @(".png", ".jpg", ".jpeg", ".gif", ".bmp")

$forbidden = Get-ChildItem $PortableFolder -Recurse -File |
    Where-Object { $forbiddenExtensions -contains $_.Extension.ToLowerInvariant() }

if ($forbidden) {
    $names = ($forbidden | ForEach-Object { $_.FullName }) -join [Environment]::NewLine
    throw "User background/media files must not be in the portable release:`n$names"
}

$exe = Join-Path $PortableFolder "Nyxoryth.exe"
if ((Get-Item $exe).Length -le 0) {
    throw "Nyxoryth.exe is empty."
}

Write-Host "Release verification passed."
