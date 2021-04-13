#
# Project IctBaden.pjsua2.net
#
# Build script to pack nuget package on Windows
#
# (C) 2021 Frank Pfattheicher
#

######################################################################
Write-Host ""
Write-Host "*********************" -ForegroundColor Yellow
Write-Host " Build nuget package " -ForegroundColor Yellow
Write-Host "*********************" -ForegroundColor Yellow
Write-Host ""

$path = $PSScriptRoot
Write-Host "Script path: $path"
Write-Host ""

$pjproject = "pjproject"
$pjsipPath = [System.IO.Path]::Combine($path, $pjproject)


######################################################################
Write-Host "Get Target Package Version" -ForegroundColor Yellow
Write-Host ""
$ReleaseNotesFileName = [System.IO.Path]::Combine($path, "ReleaseNotes.md")
$semVer = "\* (?<semVer>\d+\.\d+\.\d+(\.\d+)?)\s+(-\s+)?(?<relNotes>.*)"
$lines = Get-Content $ReleaseNotesFileName
$version = $lines | Select-String -Pattern $semVer | Select-Object -First 1
$ok = $version -match $semVer
If($ok -ne $true) {
    Write-Host "FAIL: Could not find release notes with current version" -ForegroundColor Magenta
    return
}

$packageVersion = $Matches.semVer
$releaseNotes = $Matches.relNotes.Replace(' - ', '')

Write-Host "The current version is: $packageVersion" -fore yellow
Write-Host "Release notes: $releaseNotes"
Write-Host ""


######################################################################
Write-Host "Get PJSIP Version" -ForegroundColor Yellow
Write-Host ""
$VersionMakeFileName = [System.IO.Path]::Combine($pjsipPath, "version.mak")

$VersionMajor = "0"
$VersionMinor = "0"
$VersionRev   = ""
$VersionSuffix = ""
foreach($line in Get-Content $VersionMakeFileName) {
    if($line -match "PJ_VERSION_MAJOR  := ") {$VersionMajor = ($line -split " ")[-1] }
    if($line -match "PJ_VERSION_MINOR  := ") {$VersionMinor = ($line -split " ")[-1] }
    if($line -match "PJ_VERSION_REV    := ") {$VersionRev   = ($line -split " ")[-1] }
    if($line -match "PJ_VERSION_SUFFIX := ") {$VersionSuffix = ($line -split " ")[-1] }
}

$pjsipVersion = "$VersionMajor.$VersionMinor"
if($VersionRev -ne "") { $pjsipVersion = $pjsipVersion + ".$VersionRev" } 
if($VersionSuffix -ne "") { $pjsipVersion = $pjsipVersion + $VersionSuffix } 

Write-Host "The current PJSIP version is: $pjsipVersion"
Write-Host ""


######################################################################
Write-Host "Build Nuget Packets" -ForegroundColor Yellow

$packagePath = [System.IO.Path]::Combine($path, "package")

.\nuget.exe pack IctBaden.pjsua2.nuspec -Version $packageVersion -OutputDirectory $packagePath -properties PjsipVersion=$pjsipVersion

Write-Host ""


######################################################################
Write-Host ""
Write-Host "*********" -ForegroundColor Green
Write-Host " SUCCESS " -ForegroundColor Green
Write-Host "*********" -ForegroundColor Green
Write-Host ""

