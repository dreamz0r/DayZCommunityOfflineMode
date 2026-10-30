[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$OutputRoot,
    [switch]$DryRun
)
$ErrorActionPreference='Stop'
$source=Join-Path (Split-Path $PSScriptRoot -Parent) 'Support\@COMSupport'
$files=@(Get-Item -LiteralPath (Join-Path $source 'config.cpp')) + @(Get-ChildItem -LiteralPath (Join-Path $source 'Scripts') -Recurse -File -Filter '*.c' | Sort-Object FullName)
if($files.Count -lt 2){throw 'COM support script sources are missing.'}
$modRoot=Join-Path ([IO.Path]::GetFullPath($OutputRoot)) '@COMSupport'
if($DryRun){$modRoot; return}
$entries=@()
$fingerprint=New-Object IO.MemoryStream
$utf8=New-Object Text.UTF8Encoding($false)
foreach($file in $files){
    $name=$file.FullName.Substring($source.Length+1)
    $bytes=[IO.File]::ReadAllBytes($file.FullName)
    $entries+=@{Name=$name;Bytes=$bytes}
    $nameBytes=$utf8.GetBytes($name+[char]0)
    $fingerprint.Write($nameBytes,0,$nameBytes.Length)
    $fingerprint.Write($bytes,0,$bytes.Length)
}
$sha256=[Security.Cryptography.SHA256]::Create()
$digest=[BitConverter]::ToString($sha256.ComputeHash($fingerprint.ToArray())).Replace('-','')
$sha256.Dispose();$fingerprint.Dispose()
$addonDir=Join-Path $modRoot 'Addons'
$pbo=Join-Path $addonDir 'COMSupport.pbo'
$stamp=Join-Path $modRoot 'source.sha256'
if((Test-Path -LiteralPath $pbo) -and (Test-Path -LiteralPath $stamp) -and ([IO.File]::ReadAllText($stamp) -eq $digest)){$modRoot;return}
New-Item -ItemType Directory -Force $addonDir | Out-Null
$stream=New-Object IO.MemoryStream
$writer=New-Object IO.BinaryWriter($stream)
# PBO prefix makes virtual script paths independent of the project/cache location.
$writer.Write([byte]0)
$writer.Write([uint32]0x56657273)
1..4|ForEach-Object {$writer.Write([uint32]0)}
$writer.Write([Text.Encoding]::ASCII.GetBytes("prefix`0COMSupport`0`0"))
foreach($entry in $entries){
    $writer.Write([Text.Encoding]::ASCII.GetBytes($entry.Name+[char]0))
    1..4|ForEach-Object {$writer.Write([uint32]0)}
    $writer.Write([uint32]$entry.Bytes.Length)
}
$writer.Write([byte]0)
1..5|ForEach-Object {$writer.Write([uint32]0)}
foreach($entry in $entries){$writer.Write([byte[]]$entry.Bytes)}
$writer.Flush()
$sha1=[Security.Cryptography.SHA1]::Create()
$checksum=$sha1.ComputeHash($stream.ToArray())
$writer.Write([byte]0);$writer.Write($checksum);$writer.Flush()
[IO.File]::WriteAllBytes(($pbo+'.tmp'),$stream.ToArray())
Move-Item -LiteralPath ($pbo+'.tmp') -Destination $pbo -Force
[IO.File]::WriteAllText($stamp,$digest)
$sha1.Dispose();$writer.Dispose();$stream.Dispose()
$modRoot