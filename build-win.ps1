#
# Project IctBaden.pjsua2.net
#
# Build script to compile native pjsua.dll for Windows (x64)
#
# (C) 2021 Frank Pfattheicher
#

$pjproject = "pjproject"
$pjsipRepo = "https://github.com/pjsip/pjproject.git"
$toolset = "v142"

######################################################################
Write-Host ""
Write-Host "**********************************" -ForegroundColor Yellow
Write-Host " Build pjsua2.dll for Windows x64 " -ForegroundColor Yellow
Write-Host "**********************************" -ForegroundColor Yellow
Write-Host ""

$path = $PSScriptRoot
Write-Host "Script path: $path"
Write-Host ""


######################################################################
Write-Host "Detect SWIG installation" -ForegroundColor Yellow

$swig = (Get-ChildItem -Path "$ENV:ProgramFiles" -Filter "swigwin*").FullName
If($swig -eq $null) {
    Write-Host "FAIL: Could not find SWIG installation" -fore magenta
    Write-Host "      Should be placed in an folder under $ENV:ProgramFiles" -fore magenta
    return
}

$swig = [System.IO.Path]::Combine($swig, "swig.exe")

Write-Host "SWIG found in $swig"
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
Write-Host "Cleanup existing PJSIP sources" -ForegroundColor Yellow

$pjsipPath = [System.IO.Path]::Combine($path, $pjproject)

try {

    If(Test-Path -Path $pjsipPath) {
        Remove-Item $pjsipPath -Force -Recurse
    }

} catch {

    Write-Host "FAIL: Could not remove PJSIP folder" -ForegroundColor Magenta
    return
}

Write-Host ""

######################################################################
Write-Host "Get current version of PJSIP sources" -ForegroundColor Yellow

git clone $pjsipRepo $pjproject 2>$null

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
Write-Host "Set config_site.h" -ForegroundColor Yellow

$src = [System.IO.Path]::Combine($path, "config_site.h")
$dst = [System.IO.Path]::Combine($pjsipPath, "pjlib\include\pj")
Copy-Item $src $dst
Write-Host ""


######################################################################
Write-Host "Set Environment Variables for Build Tools" -ForegroundColor Yellow

. .\set-vs-vars.ps1

Write-Host ""

######################################################################
Write-Host "Retarget Project Platform Toolset" -ForegroundColor Yellow

$projects = (Get-ChildItem -Path $pjsipPath -Filter *.vcxproj -Recurse -ErrorAction SilentlyContinue -Force).FullName

$old1 = "<PlatformToolset>v140</PlatformToolset>"
$new1 = "<PlatformToolset>$toolset</PlatformToolset>"
$old2 = "<PlatformToolset>`$(BuildToolset)</PlatformToolset>"
$new2 = "<PlatformToolset>$toolset</PlatformToolset>"

foreach($project in $projects) {
    (Get-Content $project).replace($old1, $new1).replace($old2, $new2) | Set-Content $project
}
Write-Host ""


######################################################################
Write-Host "Fix C++ code issues" -ForegroundColor Yellow

$source = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\pjsua\pjsua_app_cli.c")
$old1 = "PJ_DEF(void) cli_get_info"
$new1 = "void cli_get_info"
(Get-Content $source).replace($old1, $new1) | Set-Content $source
Write-Host ""


######################################################################
Write-Host "Use SWIG to generate a wrapper for the PJSUA2 library" -ForegroundColor Yellow

$include1 = [System.IO.Path]::Combine($pjsipPath, "pjlib\include")
$include2 = [System.IO.Path]::Combine($pjsipPath, "pjlib-util\include")
$include3 = [System.IO.Path]::Combine($pjsipPath, "pjmedia\include")
$include4 = [System.IO.Path]::Combine($pjsipPath, "pjsip\include")
$include5 = [System.IO.Path]::Combine($pjsipPath, "pjnath\include")
$swig_i = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\swig\pjsua2.i")

$swig_results = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\swig")

CD $swig_results
. $swig -I"$include1" -I"$include2" -I"$include3" -I"$include4" -I"$include5" -w312 -c++ -csharp -o pjsua2_wrap.cpp $swig_i
CD $path
Write-Host ""


######################################################################
Write-Host "Copy generated C++ wrappers to pjsua2.win" -ForegroundColor Yellow

$pjsua2winPath = [System.IO.Path]::Combine($path, "pjsua2.win")

$src = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\swig\pjsua2_wrap.*")
Copy-Item $src $pjsua2winPath

$src = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\swig\pjsua2.i")
Copy-Item $src $pjsua2winPath

Write-Host ""


######################################################################
Write-Host "Copy generated C# wrappers to pjsua2.net/pjsua2" -ForegroundColor Yellow

$pjsua2netPath = [System.IO.Path]::Combine($path, "pjsua2.net\pjsua2")

$src = [System.IO.Path]::Combine($pjsipPath, "pjsip-apps\src\swig\*.cs")


If(Test-Path -Path $pjsua2netPath) {
    Remove-Item $pjsua2netPath -Force -Recurse
}

New-Item -ItemType Directory -Path $pjsua2netPath
Copy-Item $src $pjsua2netPath

Write-Host ""


######################################################################
Write-Host "Fix C# code issues" -ForegroundColor Yellow

$source = [System.IO.Path]::Combine($pjsua2netPath, "pjmedia_format_id.cs")
$old1 = "enum pjmedia_format_id"
$new1 = "enum pjmedia_format_id : uint"
(Get-Content $source).replace($old1, $new1) | Set-Content $source
Write-Host ""


######################################################################
Write-Host "Set pjsua2.dll Version Info" -ForegroundColor Yellow

$resourceFileName = [System.IO.Path]::Combine($pjsua2winPath, "pjsua2.win.rc")

$packageVersionEntry = "            VALUE ""FileVersion"", ""$packageVersion"""
$pjsipVersionEntry = "            VALUE ""ProductVersion"", ""PJSIP $pjsipVersion"""
(Get-Content $resourceFileName) `
    -replace '(.*)VALUE "FileVersion(.*)', $packageVersionEntry `
    -replace '(.*)VALUE "ProductVersion(.*)', $pjsipVersionEntry `
    | Set-Content $resourceFileName

Write-Host ""


######################################################################
Write-Host "Build pjsua2.dll Native Windows DLL" -ForegroundColor Yellow

$solution = [System.IO.Path]::Combine($pjsua2winPath, "pjsua2.win.sln")

Write-Host "Using toolset $toolset"
Write-Host ""

msbuild $solution /p:Configuration=Release-Dynamic /p:Platform="x64"
Write-Host ""


######################################################################
Write-Host "Build pjsua2.net.dll Managed Windows DLL" -ForegroundColor Yellow

$pjsua2netPath = [System.IO.Path]::Combine($path, "pjsua2.net")

CD $pjsua2netPath
dotnet build -c Release
CD $Path
Write-Host ""


######################################################################
Write-Host "Copy Assets to Packets Folder" -ForegroundColor Yellow

$assetsPath = [System.IO.Path]::Combine($pjsua2netPath, "bin\x64\Release\netcoreapp3.1")
$packetsPath = [System.IO.Path]::Combine($path, "packets")

$src = [System.IO.Path]::Combine($assetsPath, "*.dll")

If(Test-Path -Path $packetsPath) {
    Remove-Item $packetsPath -Force -Recurse
}

New-Item -ItemType Directory -Path $packetsPath
Copy-Item $src $packetsPath

Write-Host ""


######################################################################
Write-Host ""
Write-Host "*********" -ForegroundColor Green
Write-Host " SUCCESS " -ForegroundColor Green
Write-Host "*********" -ForegroundColor Green
Write-Host ""

