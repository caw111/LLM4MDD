$ErrorActionPreference = 'Stop'
$repo = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$runtime = Join-Path $repo '.tools/sysml'
New-Item -ItemType Directory -Force $runtime | Out-Null
$archive = Join-Path $runtime 'kernel.zip'
$expected = '5a015ed3f2b3d2dae14e9c3fb9602d9e1fef5f9ce33cf396c422329405cc3cb2'
if (!(Test-Path $archive)) {
    Invoke-WebRequest 'https://github.com/Systems-Modeling/SysML-v2-Pilot-Implementation/releases/download/2026-08/jupyter-sysml-kernel-0.62.0.zip' -OutFile $archive
}
if ((Get-FileHash $archive -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expected) { throw 'SysML archive checksum mismatch' }
Expand-Archive -LiteralPath $archive -DestinationPath (Join-Path $runtime 'kernel') -Force
if (!(Test-Path (Join-Path $runtime 'java/*/bin/java.exe'))) {
    $release = Invoke-RestMethod 'https://api.adoptium.net/v3/assets/latest/21/hotspot?architecture=x64&image_type=jdk&os=windows&vendor=eclipse'
    $pkg = $release[0].binary.package
    $jdkArchive = Join-Path $runtime 'jdk.zip'
    Invoke-WebRequest $pkg.link -OutFile $jdkArchive
    if ((Get-FileHash $jdkArchive -Algorithm SHA256).Hash.ToLowerInvariant() -ne $pkg.checksum) { throw 'JDK checksum mismatch' }
    Expand-Archive -LiteralPath $jdkArchive -DestinationPath (Join-Path $runtime 'java') -Force
}
Write-Output 'Installed SysML Pilot Implementation 2026-08 / 0.62.0 with its standard libraries and a project-local Java 21.'
