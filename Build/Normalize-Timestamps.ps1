param(
    [Parameter(Mandatory=$true)]
    [string]$Root
)

$ErrorActionPreference = "Stop"
$now = Get-Date

$allowedExtensions = @(
    ".c", ".cc", ".cpp", ".cxx",
    ".h", ".hh", ".hpp",
    ".rc", ".cmake"
)

$files = @()

$cmakeLists = Join-Path $Root "CMakeLists.txt"
if (Test-Path $cmakeLists) {
    $files += Get-Item $cmakeLists
}

$sourceRoot = Join-Path $Root "Source"
if (Test-Path $sourceRoot) {
    $files += Get-ChildItem $sourceRoot -Recurse -File |
        Where-Object { $allowedExtensions -contains $_.Extension.ToLowerInvariant() }
}

foreach ($file in $files) {
    if ($file.LastWriteTime -gt $now.AddSeconds(2)) {
        $file.LastWriteTime = $now
        Write-Host "Normalized future timestamp:" $file.FullName
    }
}
