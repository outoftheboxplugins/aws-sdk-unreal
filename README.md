# What is this

This repository is a wrapper for [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp) for Unreal Engine.
It handles multiple problems regarding the setup required:
- automatically builds the AWS SDK binaries during the Unreal Header Tool parsing - eliminates repetetive steps during setup (e.g.: downloading vpckg, installing the aws-cpp-sdk, copy-pasting the binaries)
- uses AWS SDK custom memory allocator to leverage Unreal's memory allocation system - prevents any potential memory issues caused by mismatched allocator (e.g.: iOS)
- uses AWS custom HTTP client to leverage Unreal's HTTP module - enables cross platform HTTP request support (e.g.: fixes Android and Linux SSL problems & uses MacOS native requets system)
- forwards logs from the aws-sdk directly into Unreal's native logging system

# How to use

## Setup

Requirements:
- Windows: Powershell
- MacOS: pwsh

Clone the repo
Specify modules inside the build.cs file


## Chose AWS libraries

Inside `Plugins/aws-sdk-unreal/Source/ThirdParty/AwsSdkLibrary/AwsSdkLibrary.Build.cs` change the `AwsLibraries` variable to include all the modules you want.
Alternatively, you can set it to `AwsLibraries = { "*" };` to include everything

## Extend system

Recommandation: create one subsystem for each aws module
go over the example thingy

# Developer notes

## How is the binaries building process automated

## Why is the cusotm allocator used

## Third party includes

```
THIRD_PARTY_INCLUDES_START
#include <aws/core/utils/logging/FormattedLogSystem.h>
THIRD_PARTY_INCLUDES_END
```

## iOS static library is build with a newer clang than UE supports 

```
LogPlayLevel: UAT: iOS Static Library:'libcrypto.a' is built with a version of clang newer than UE supports (1500.1.0.2 > 1400.0.29.202).
LogPlayLevel: UAT: Please rebuild /Users/alexandruoprea/Documents/UnrealProjects/MyProject53/Plugins/aws-sdk-unreal/Source/ThirdParty/AwsSdkLibrary/arm64-ios-unreal/lib/libcrypto.a with the minimum supported version of Xcode/clang.
```

Solution: Set the desired clang executable using `CC` & `CXX` environment variables. Either globally or inside the build script (`Plugins/aws-sdk-unreal/Extras/Scripts/build-vcpkg.ps1`) with:

```
$env:CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
$env:CXX="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++"
```