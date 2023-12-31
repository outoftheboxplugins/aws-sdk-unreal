# What is this

This repository is a wrapper for [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp) for Unreal Engine.
It handles multiple problems regarding the setup required:
- automatically builds the AWS SDK binaries during the Unreal Header Tool parsing - eliminates repetetive steps during setup (e.g.: downloading vpckg, installing the aws-cpp-sdk, copy-pasting the binaries)
- uses AWS SDK custom memory allocator to leverage Unreal's memory allocation system - prevents any potential memory issues caused by mismatched allocator (e.g.: iOS)
- uses AWS custom HTTP client to leverage Unreal's HTTP module - enables cross platform HTTP request support (e.g.: fixes Android and Linux SSL problems & uses MacOS native requets system)
- forwards logs from the aws-sdk directly into Unreal's native logging system

# How to use

## Setup

Clone the repo
Specify modules inside the build.cs file

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