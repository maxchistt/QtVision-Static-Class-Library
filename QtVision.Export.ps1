Write-Output "Exporting QtVision"

function Get-DevEnvExecutableFilePath {
    $DevEnvExecutableFilePath = 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe'
    if (!(Test-Path -Path $DevEnvExecutableFilePath)) {
        [bool] $vsSetupExists = $null -ne (Get-Command Get-VSSetupInstance -ErrorAction SilentlyContinue)
        if (!$vsSetupExists) {
            Write-Verbose "Installing the VSSetup module..."
            Install-Module VSSetup -Scope CurrentUser -Force
        }
        [string] $visualStudioInstallationPath = (Get-VSSetupInstance | Select-VSSetupInstance -Latest -Require Microsoft.Component.MSBuild).InstallationPath
        $DevEnvExecutableFilePath = (Get-ChildItem $visualStudioInstallationPath -Recurse -Filter "devenv.exe" | Select-Object -First 1).FullName
    }
    return $DevEnvExecutableFilePath
}

$TempDir = [System.IO.Path]::GetTempPath()
$TempQtVisionExportPath = "$TempDir\QtVision-Ex-Temp"
$ZipPath = "..\QtVision.zip"
$DevEnv = Get-DevEnvExecutableFilePath
$BuildDebugArgs = ".\QtVision.sln /build Debug"
$BuildReleaseArgs = ".\QtVision.sln /build Release"

$noKernel = 0
$noVision = 0
If (!(Test-Path -Path ".\C3D\Kernel\Release\")) { $noKernel = 1 }
If (!(Test-Path -Path ".\C3D\Vision\Release\")) { $noVision = 1 }
If (!(Test-Path -Path ".\C3D\Kernel\Debug\")) { $noKernel = 1 }
If (!(Test-Path -Path ".\C3D\Vision\Debug\")) { $noVision = 1 }
If (!(Test-Path -Path ".\C3D\Kernel\Include\")) { $noKernel = 1 }
If (!(Test-Path -Path ".\C3D\Vision\Include\")) { $noVision = 1 }
if ($noKernel) { Write-Output "No C3D Kernel" }
if ($noVision) { Write-Output "No C3D Vision" }
if ($noKernel -or $noVision) { 
    Read-Host -Prompt "Press Enter to exit. "
    exit
}

Write-Output "Start building Debug"
Start-Process -FilePath $DevEnv -ArgumentList $BuildDebugArgs -Wait
Write-Output "End building Debug"
Write-Output "Start building Release"
Start-Process -FilePath $DevEnv -ArgumentList $BuildReleaseArgs -Wait
Write-Output "End building Release"

Write-Output "Start creating .zip distributive"
If (Test-Path -Path $TempQtVisionExportPath ) { Remove-Item -Path $TempQtVisionExportPath -Recurse }
If (Test-Path -Path $ZipPath ) { Remove-Item -Path $ZipPath -Recurse }
Copy-Item .\x64\Debug -Filter QtVision.lib -Recurse $($TempQtVisionExportPath + "\QtVision\Debug") -Force -Container:$false
Copy-Item .\x64\Release -Filter QtVision.lib -Recurse $($TempQtVisionExportPath + "\QtVision\Release") -Force -Container:$false
Copy-Item .\QtVision\Include -Filter *.h -Recurse $($TempQtVisionExportPath + "\QtVision\Include") -Force -Container:$false
Copy-Item .\Shared -Filter *.h -Recurse $($TempQtVisionExportPath + "\Shared") -Force -Container:$false
Copy-Item .\Shared -Filter *.cpp -Recurse $($TempQtVisionExportPath + "\Shared") -Force -Container:$false
Compress-Archive -Path $($TempQtVisionExportPath + "\QtVision") -DestinationPath $ZipPath -CompressionLevel Optimal -Update
Compress-Archive -Path $($TempQtVisionExportPath + "\Shared") -DestinationPath $ZipPath -CompressionLevel Optimal -Update
Remove-Item $TempQtVisionExportPath -Recurse
Write-Output "End creating .zip distributive"

Invoke-Item $ZipPath
Read-Host -Prompt "Press Enter to exit. "