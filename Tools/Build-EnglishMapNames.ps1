[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][ValidateSet('ChernarusPlus','Enoch','Sakhal','Nasdara')][string]$World,
    [string]$OutputRoot=(Join-Path (Split-Path $PSScriptRoot -Parent) 'EnglishMapNames'),
    [switch]$DryRun
)
$ErrorActionPreference='Stop'
$source=Join-Path (Split-Path $PSScriptRoot -Parent) 'EnglishMapNames\Names.json'
$table=Get-Content -LiteralPath $source -Raw | ConvertFrom-Json
$names=$table.$World
if(!$names){throw "No English map names for $World"}
$dependencies=@{ChernarusPlus='DZ_Worlds_Chernarusplus_World';Enoch='DZ_Worlds_Enoch';Sakhal='DZ_Worlds_Sakhal';Nasdara='DZ_Worlds_Nasdara'}
$modRoot=Join-Path ([IO.Path]::GetFullPath($OutputRoot)) ('@COMEnglishMapNames_'+$World)
if($DryRun){$modRoot;return}
$addonDir=Join-Path $modRoot 'Addons'
New-Item -ItemType Directory -Force $addonDir | Out-Null
$lines=New-Object 'System.Collections.Generic.List[string]'
$lines.Add('class CfgPatches { class COMEnglishMapNames_'+$World+' { units[]={}; weapons[]={}; requiredVersion=0.1; requiredAddons[]={"'+$dependencies[$World]+'"}; }; };')
$lines.Add('class CfgWorlds { class CAWorld; class '+$World+': CAWorld { class Names {')
foreach($entry in $names.PSObject.Properties){
    if($entry.Name -notmatch '^[A-Za-z_][A-Za-z0-9_]*$'){throw "Invalid config class: $($entry.Name)"}
    $label=([string]$entry.Value).Replace('"','""')
    $lines.Add('class '+$entry.Name+' { name="'+$label+'"; };')
}
$lines.Add('}; }; };')
$config=$lines -join "`r`n"
$utf8=New-Object Text.UTF8Encoding($false)
$payload=$utf8.GetBytes($config)
# Rebuild the small configuration-only add-on shipped with the project.
$stream=New-Object IO.MemoryStream
$writer=New-Object IO.BinaryWriter($stream)
$writer.Write([Text.Encoding]::ASCII.GetBytes("config.cpp`0"))
$writer.Write([uint32]0);$writer.Write([uint32]0);$writer.Write([uint32]0);$writer.Write([uint32]0);$writer.Write([uint32]$payload.Length)
$writer.Write([byte]0)
1..5|ForEach-Object {$writer.Write([uint32]0)}
$writer.Write($payload);$writer.Flush()
$body=$stream.ToArray()
$sha=[Security.Cryptography.SHA1]::Create()
$checksum=$sha.ComputeHash($body)
$writer.Write([byte]0);$writer.Write($checksum);$writer.Flush()
[IO.File]::WriteAllText((Join-Path $modRoot 'config.cpp'),$config,$utf8)
[IO.File]::WriteAllBytes((Join-Path $addonDir ('COMEnglishMapNames_'+$World+'.pbo')),$stream.ToArray())
$sha.Dispose();$writer.Dispose();$stream.Dispose()
$modRoot
