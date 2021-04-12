#
# Project IctBaden.pjsua2.net
#
# Build script to compile native pjsua.dll for Windows (x64)
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
Write-Host "Build Nuget Packets" -ForegroundColor Yellow

$packagePath = [System.IO.Path]::Combine($path, "package")

.\nuget.exe pack IctBaden.pjsua2.nuspec -Version $packageVersion -OutputDirectory $packagePath

Write-Host ""


######################################################################
Write-Host ""
Write-Host "*********" -ForegroundColor Green
Write-Host " SUCCESS " -ForegroundColor Green
Write-Host "*********" -ForegroundColor Green
Write-Host ""

