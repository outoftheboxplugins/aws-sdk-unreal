Param(
    [String] $awsLibraries
)

# Get the directory containing the .uplugin file
$parentDir = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)

# Spawn the vpckg directly inside the temp folder to have a shorter path
$vpckgDir = "$env:TEMP/unreal-vcpkg"

# Print desired libraries for visibility
Write-Host "Installing aws-sdk libraries: $awsLibraries in:$vpckgDir to:$parentDir"

if (Test-Path -Path "$vpckgDir") {
    Write-Host "VCPKG already present, pulling."
    Push-Location "$vpckgDir"
    git pull 
    Pop-Location
}
else {
    Write-Host "VCPKG missing, cloning."
    git clone https://github.com/Microsoft/vcpkg.git "$vpckgDir"
}

# Clone the microsoft VCPKG repo

# Bootstrap the new copy
& "$vpckgDir/bootstrap-vcpkg.bat"

# Uninstall the library to ensure only the required packages are built
& "$vpckgDir/vcpkg.exe" remove "aws-sdk-cpp" --triplet x64-windows-unreal --overlay-triplets "$parentDir"

# Install AWS binaries
& "$vpckgDir/vcpkg.exe" install "aws-sdk-cpp[$awsLibraries]" --recurse --triplet x64-windows-unreal --overlay-triplets "$parentDir/Extras/Triplets"

# Remove past includes
Remove-Item -Path "$parentDir/Source/ThirdParty/AWSSDKLibrary/bin" -Recurse -Force
Remove-Item -Path "$parentDir/Source/ThirdParty/AWSSDKLibrary/lib" -Recurse -Force
Remove-Item -Path "$parentDir/Source/ThirdParty/AWSSDKLibrary/include" -Recurse -Force

# Copy the binaries and header files to the plugin 
Copy-Item -Path "$vpckgDir/installed/x64-windows-unreal/bin"        -Destination "$parentDir/Source/ThirdParty/AWSSDKLibrary/bin"       -Recurse -Force
Copy-Item -Path "$vpckgDir/installed/x64-windows-unreal/lib"        -Destination "$parentDir/Source/ThirdParty/AWSSDKLibrary/lib"       -Recurse -Force
Copy-Item -Path "$vpckgDir/installed/x64-windows-unreal/include"    -Destination "$parentDir/Source/ThirdParty/AWSSDKLibrary/include"   -Recurse -Force