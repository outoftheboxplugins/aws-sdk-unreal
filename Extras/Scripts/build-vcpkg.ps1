Param(
    [String] $vcpkgTriplet,
    [String] $awsLibraries
)

# Get the directory containing the .uplugin file
$parentDir = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)

# Spawn the vcpkg directly inside the temp folder to have a shorter path
$vcpkgDir = "$env:TEMP/unreal-vcpkg"

# Print desired libraries for visibility
Write-Host "Installing aws-sdk libraries: $awsLibraries in:$vcpkgDir to:$parentDir"

if (Test-Path -Path "$vcpkgDir") {
    Write-Host "VCPKG already present, pulling."
    Push-Location "$vcpkgDir"
    git pull 
    Pop-Location
}
else {
    Write-Host "VCPKG missing, cloning."
    git clone https://github.com/Microsoft/vcpkg.git "$vcpkgDir"
}

$vcpkgBootstrap = ""
$vcpkgExecutable = ""

if ($IsLinux) {
    Write-Host "Building for Linux"
    $vcpkgBootstrap = "$vcpkgDir/bootstrap-vcpkg.sh"
    $vcpkgExecutable = "$vcpkgDir/vcpkg"
}
elseif ($IsMacOS) {
    Write-Host "Building for MacOS"
    $vcpkgBootstrap = "$vcpkgDir/bootstrap-vcpkg.sh"
    $vcpkgExecutable = "$vcpkgDir/vcpkg"
}
else {
    Write-Host "Building for Windows"
    $vcpkgBootstrap = "$vcpkgDir/bootstrap-vcpkg.bat"
    $vcpkgExecutable = "$vcpkgDir/vcpkg.exe"
}

# Bootstrap the new copy
& "$vcpkgBootstrap"

# Uninstall the library to ensure only the required packages are built
& "$vcpkgExecutable" remove "aws-sdk-cpp" --triplet $vcpkgTriplet --overlay-triplets "$parentDir/Extras/Triplets"

# Install AWS binaries
& "$vcpkgExecutable" install "aws-sdk-cpp[$awsLibraries]" --recurse --triplet $vcpkgTriplet --overlay-triplets "$parentDir/Extras/Triplets"

# Remove past includes
Remove-Item -Path "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/bin" -Recurse -Force
Remove-Item -Path "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/lib" -Recurse -Force
Remove-Item -Path "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/include" -Recurse -Force

# Copy the binaries and header files to the plugin 
Copy-Item -Path "$vcpkgDir/installed/$vcpkgTriplet/bin"        -Destination "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/bin"       -Recurse -Force
Copy-Item -Path "$vcpkgDir/installed/$vcpkgTriplet/lib"        -Destination "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/lib"       -Recurse -Force
Copy-Item -Path "$vcpkgDir/installed/$vcpkgTriplet/include"    -Destination "$parentDir/Source/ThirdParty/AwsSdkLibrary/$vcpkgTriplet/include"   -Recurse -Force