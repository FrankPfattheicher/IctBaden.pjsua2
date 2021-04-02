#
# Project IctBaden.pjsua2.net
#
# Build script to compile native pjsua.dll for Windows (x64)
#
# (C) 2021 Frank Pfattheicher
#

$pjproject = "pjproject"
$pjsipRepo = "https://github.com/pjsip/pjproject.git"

######################################################################
Write-Host "Build pjsua2.dll for Windows x64" -ForegroundColor Yellow

$path = $PSScriptRoot
Write-Host "Script path: $path"

######################################################################
Write-Host "Get Target Package Version" -ForegroundColor Yellow
Write-Host ""
$ReleaseNotesFileName = [System.IO.Path]::Combine($path, "ReleaseNotes.md")
$semVer = "\* (?<semVer>\d+\.\d+\.\d+(\.\d+)?)\s+(-\s+)?(?<relNotes>.*)"
$lines = Get-Content $ReleaseNotesFileName
$version = $lines | Select-String -Pattern $semVer | Select-Object -First 1
$ok = $version -match $semVer
If($ok -ne $true) {
    Write-Host "FAIL: Could not find release notes with current version" -fore magenta
    return
}

$packageVersion = $Matches.semVer
$releaseNotes = $Matches.relNotes.Replace(' - ', '')

Write-Host "The current version is: $packageVersion" -fore yellow
Write-Host "Release notes: $releaseNotes"
Write-Host ""

######################################################################
Write-Host "Cleanup existing PJSIP sources" -ForegroundColor Yellow

$pjsipPath =  [System.IO.Path]::Combine($path, $pjproject)

If(Test-Path -Path $pjsipPath) {
    Remove-Item $pjsipPath -Force -Recurse
}
Write-Host ""

######################################################################
Write-Host "Get current version of PJSIP sources" -ForegroundColor Yellow

git clone $pjsipRepo $pjproject 2>$null

Write-Host ""

######################################################################
Write-Host "Set config_site.h" -ForegroundColor Yellow

$src = [System.IO.Path]::Combine($path, "config_site.h")
$dst = [System.IO.Path]::Combine($pjsipPath, "pjlib\include\pj")
Copy-Item $src $dst


######################################################################
Write-Host "Set Environment Variables for Build Tools" -ForegroundColor Yellow

. .\set-vs-vars.ps1

Write-Host ""

######################################################################
Write-Host "Retarget Project Platform Toolset" -ForegroundColor Yellow

$projects = (Get-ChildItem -Path $pjsipPath -Filter *.vcxproj -Recurse -ErrorAction SilentlyContinue -Force).FullName

$old1 = "<PlatformToolset>v140</PlatformToolset>"
$new1 = "<PlatformToolset>v142</PlatformToolset>"
$old2 = "<PlatformToolset>`$(BuildToolset)</PlatformToolset>"
$new2 = "<PlatformToolset>v142</PlatformToolset>"

foreach($project in $projects) {
    (Get-Content $project).replace($old1, $new1).replace($old2, $new2) | Set-Content $project
}

######################################################################
Write-Host "Build " -ForegroundColor Yellow

$solution = [System.IO.Path]::Combine($pjsipPath, "pjproject-vs14.sln")

#msbuild $solution /p:Configuration=Release /p:Platform="x64"
