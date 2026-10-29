[CmdletBinding()]
param(
    [ValidateSet('Chernarus', 'ChernarusPlus', 'Livonia', 'Enoch', 'Sakhal', 'Nasdara')][string]$Map = 'Chernarus',
    [string]$SettingsFile,
    [switch]$DryRun,
    [Parameter(ValueFromRemainingArguments = $true)][string[]]$ExtraArguments
)
$ErrorActionPreference = 'Stop'
if ([string]::IsNullOrWhiteSpace($SettingsFile)) { $SettingsFile = Join-Path $PSScriptRoot 'LauncherSettings.json' }

function Resolve-ConfiguredPath([string]$Value, [string]$BaseDirectory) {
    if ([string]::IsNullOrWhiteSpace($Value)) { throw 'A configured folder path is empty.' }
    $Value = [Environment]::ExpandEnvironmentVariables($Value)
    if (![IO.Path]::IsPathRooted($Value)) { $Value = Join-Path $BaseDirectory $Value }
    return [IO.Path]::GetFullPath($Value)
}
function Assert-ModPath([string]$Path) {
    if ($Path.Contains(';') -or $Path.Contains('"')) { throw "Mod paths cannot contain semicolons or quotes: $Path" }
    if (!(Test-Path -LiteralPath $Path -PathType Container)) { throw "Mod folder not found: $Path" }
    if (!(Test-Path -LiteralPath (Join-Path $Path 'Addons') -PathType Container)) {
        throw "Mod folder has no Addons folder: $Path. Select the mod folder itself, not its parent or Addons folder."
    }
}

$SettingsFile = (Resolve-Path -LiteralPath $SettingsFile).Path
$settingsRoot = Split-Path -Parent $SettingsFile
$config = Get-Content -LiteralPath $SettingsFile -Raw | ConvertFrom-Json
$gameRoot = Resolve-ConfiguredPath $config.GamePath $settingsRoot
$modsRoot = Resolve-ConfiguredPath $config.ModsPath $settingsRoot
$exe = Join-Path $gameRoot 'DayZ_x64.exe'
if (!(Test-Path -LiteralPath $exe -PathType Leaf)) { throw "DayZ_x64.exe not found. Update GamePath in $SettingsFile" }
if (!(Test-Path -LiteralPath $modsRoot -PathType Container)) { throw "ModsPath does not exist: $modsRoot" }

$mods = New-Object 'System.Collections.Generic.List[string]'
if ($null -eq $config.EnabledMods) {
    $candidates = @(Get-ChildItem -LiteralPath $modsRoot -Directory | Where-Object {
        Test-Path -LiteralPath (Join-Path $_.FullName 'Addons') -PathType Container
    } | Sort-Object Name)
    foreach ($candidate in $candidates) { Assert-ModPath $candidate.FullName; $mods.Add($candidate.FullName) }
} else {
    foreach ($name in @($config.EnabledMods)) {
        $path = Resolve-ConfiguredPath ([string]$name) $modsRoot
        Assert-ModPath $path
        if (!$mods.Contains($path)) { $mods.Add($path) }
    }
}
foreach ($argument in $ExtraArguments) {
    if ($argument -match '^-mod(?:=|$)') { throw 'Select mods in EnabledMods instead of passing a second -mod argument.' }
    if ($argument.Contains('"')) { throw 'Extra arguments must not contain literal double quotes.' }
}

$world = switch ($Map) {
    'Chernarus' { 'ChernarusPlus' }
    'Livonia' { 'Enoch' }
    default { $Map }
}
if ($world -in @('Sakhal', 'Nasdara')) {
    $terrainAddons = Join-Path $gameRoot ($world + '\Addons')
    if (!(Test-Path -LiteralPath $terrainAddons -PathType Container)) {
        throw "Terrain data missing for $world. Install its matching DayZ DLC in the configured GamePath."
    }
}
$missionName = 'DayZCommunityOfflineMode.' + $world
$source = Join-Path $PSScriptRoot ('Missions\' + $missionName)
$destination = Join-Path $gameRoot ('Missions\' + $missionName)
if (!(Test-Path -LiteralPath (Join-Path $source 'init.c'))) { throw "Mission source missing: $source" }
$arguments = @('-mission=.\Missions\' + $missionName) + @('-nosplash', '-noPause', '-filePatching', '-doLogs', '-scriptDebug=true')
if ($mods.Count) { $arguments += '-mod=' + ($mods -join ';') }
if ($ExtraArguments) { $arguments += @($ExtraArguments) }
# Start-Process joins its argument array; quote every complete argument explicitly.
$quotedArguments = @($arguments | ForEach-Object { '"' + $_ + '"' })
if ($DryRun) {
    [pscustomobject]@{
        Executable = $exe; WorkingDirectory = $gameRoot; MissionSource = $source
        MissionDestination = $destination; ModsPath = $modsRoot; Mods = @($mods.ToArray())
        Arguments = $arguments; CommandLine = ('"' + $exe + '" ' + ($quotedArguments -join ' '))
    } | ConvertTo-Json -Depth 4
    return
}

# Sync mission source without deleting existing economy storage or user files.
if ([IO.Path]::GetFullPath($source).TrimEnd('\') -ine [IO.Path]::GetFullPath($destination).TrimEnd('\')) {
    New-Item -ItemType Directory -Path $destination -Force | Out-Null
    foreach ($item in Get-ChildItem -LiteralPath $source) {
        if ($item.Name -like 'storage_*') { continue }
        Copy-Item -LiteralPath $item.FullName -Destination $destination -Recurse -Force
    }
}
Write-Host ('Mods folder: ' + $modsRoot)
foreach ($mod in $mods) { Write-Host ('Loading: ' + $mod) }
if (!$mods.Count) { Write-Host 'No mods selected.' }
Start-Process -FilePath $exe -WorkingDirectory $gameRoot -ArgumentList $quotedArguments