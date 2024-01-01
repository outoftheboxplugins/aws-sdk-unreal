# What is this

This repository is a wrapper for [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp) for Unreal Engine.
It handles multiple problems regarding the setup required:
- automatically builds the AWS SDK binaries during the Unreal Header Tool parsing - eliminates repetetive steps during setup (e.g.: downloading vpckg, installing the aws-sdk-cpp, copy-pasting the binaries)
- uses AWS SDK custom memory allocator to leverage Unreal's memory allocation system - prevents any potential memory issues caused by mismatched allocator (e.g.: iOS)
- uses AWS custom HTTP client to leverage Unreal's HTTP module - enables cross platform HTTP request support (e.g.: fixes Android and Linux SSL problems & uses MacOS native requets system)
- forwards logs from the aws-sdk directly into Unreal's native logging system

# How to use

## Setup

Requirements:
- [git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- Windows: [powershell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows?view=powershell-7.4)
- MacOS: [pwsh](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-macos?view=powershell-7.4)
- Linux: [pwsh](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-linux?view=powershell-7.4)

Clone this repository inside your project's `Plugins` folder. If you don't a `Plugins` folder, just create one.

## AWS libraries

Inside `Plugins/aws-sdk-unreal/Source/ThirdParty/AwsSdkLibrary/AwsSdkLibrary.Build.cs` change the `AwsLibraries` variable to include all the modules you want.
Alternatively, you can set it to `AwsLibraries = { "*" };` to include everything

## Extend system

To keep things modular, my recommandation is to create a `GameInstanceSubsystem` for each aws-sdk-cpp you want to implement. I've left an example inside `ExampleAwsEc2Subsystem`.

Inside `ExampleAwsEc2Subsystem.h`: you can see how the `GameInstanceSubsystem` type was implemented and how a function is exposed to blueprints (`PrintEc2Instances`).
Inside `ExampleAwsEc2Subsystem.cpp`: you can see how we deal with race conditions with the AWS initialization (`UExampleAwsEc2Subsystem::Initialize`), how third-party includes are added and how types are used (`UExampleAwsEc2Subsystem::PrintEc2Instances`).

You are free to modify the plugin's code directly or extend via other modules it by adding to `PublicDependencyModuleNames` the `AwsSdkLibrary` and `AwsSdkIntegration` modules:

```
PublicDependencyModuleNames.AddRange(new string[]
{
	"AwsSdkLibrary", 
	"AwsSdkIntegration", 
});
```

# Developer notes

## How is the binaries building process automated

The whole process is described in [How to Integrate the AWS C++ SDK with Unreal Engine](https://aws.amazon.com/blogs/gametech/how-to-integrate-the-aws-c-sdk-with-unreal-engine/).

The `Compiling AWS C++ SDK` section is automated inside the `Plugins/aws-sdk-unreal/Extras/Scripts/build-vcpkg.ps1`:
- it clones and initializes `vcpkg` in a temporary directory to avoid cluttering up your project.
- it compiles the desired `aws-sdk-cpp` modules
- it copies the resulting folders to your project

The `Creating a Plugin from the SDK` section is automated inside the `Plugins/aws-sdk-unreal/Source/ThirdParty/AwsSdkLibrary/AwsSdkLibrary.Build.cs`
- the plugin is already created
- the binaries are discovered by checking the resulting folder instead of added manually
- the module file already contains the necessary setup based on your platform

The AWS initialization and shutdown are handled by `AwsIntegrationSubsystem` during the game's Initialization and Deinitialization.

**Bonus**: The `build-vcpkg.ps1` script runs every the time Unreal Header Tool (UHT) updates the module's files. This means you will get updated binaries every time you modify the `AwsLibraries` variable.

## Why is the cusotm allocator used

TODO:

## Why is a custom HTTP client used

TODO:

# Common issues

## Third party includes

```
0>event_loop.h(29,5): Error  : 'AWS_USE_IO_COMPLETION_PORTS' is not defined, evaluates to 0 [-Werror,-Wundef]
```

C++ handles undefined macros by replacing them with `0`, however Unreal errors out when it faces an undefined macro.

Solution: To avoid this wrap the `#include` directives betweeen `THIRD_PARTY_INCLUDES_START` and `THIRD_PARTY_INCLUDES_END` like:

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