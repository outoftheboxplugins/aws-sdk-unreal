// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using UnrealBuildTool;

public class AwsSdkLibrary : ModuleRules
{
	// string[] AwsLibraries = {"core", "s3", "dynamodb"};
	private readonly string[] AwsLibraries = { "core", "ec2" };

	public AwsSdkLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			BuildAwsLibraries("x64-windows-unreal", "powershell.exe");
			AddPrecompiledLibraries(Path.Combine(ModuleDirectory, "x64-windows-unreal", "lib"), "*.lib");
			AddPrecompiledDlls(Path.Combine(ModuleDirectory, "x64-windows-unreal", "bin"), "*.dll");
			
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "x64-windows-unreal", "include"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			BuildAwsLibraries("arm64-osx-unreal", "pwsh");
			AddPrecompiledLibraries(Path.Combine(ModuleDirectory, "arm64-osx-unreal", "lib"), "*.a");

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "arm64-osx-unreal", "include"));

			PublicFrameworks.AddRange(new[]
			{
				"Foundation",
				"SystemConfiguration"
			});
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			BuildAwsLibraries("arm64-ios-unreal", "pwsh");
			AddPrecompiledLibraries(Path.Combine(ModuleDirectory, "arm64-ios-unreal", "lib"), "*.a");

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "arm64-ios-unreal", "include"));
		}


		// We will be using Unreal Engine allocators, so we need this define to make sure we are using the correct types
		PublicDefinitions.Add("USE_AWS_MEMORY_MANAGEMENT");
	}

	private void BuildAwsLibraries(string VcpkgTriplet, string Executable)
	{
		var AwsLibrariesArg = AwsLibraries.Aggregate((current, next) => $"{current},{next}");

		var ScriptPath = Path.Combine(PluginDirectory, "Extras/Scripts/build-vcpkg.ps1");

		// Prepare a new external process instance to build and copy binaries
		var Process = new Process();
		Process.StartInfo.FileName = Executable;
		Process.StartInfo.Arguments = $"-NoProfile -ExecutionPolicy Bypass -File \"{ScriptPath}\" -vcpkgTriplet {VcpkgTriplet} -awsLibraries {AwsLibrariesArg}";
		Process.StartInfo.UseShellExecute = false;

		Console.WriteLine(Name + " running script: " + ScriptPath);

		// Start and block until process ends to ensure the Build.cs file links binaries after they are successfully built
		Process.Start();
		Process.WaitForExit();
	}

	private void AddPrecompiledLibraries(string FolderPath, string Extension)
	{
		var Files = Directory.GetFiles(FolderPath, Extension, SearchOption.AllDirectories).ToList();
		foreach (var File in Files)
		{
			PublicAdditionalLibraries.Add(File);
			Console.WriteLine(Name + " adding library: " + File);
		}
	}

	private void AddPrecompiledDlls(string FolderPath, string Extension)
	{
		var Files = Directory.GetFiles(FolderPath, Extension, SearchOption.AllDirectories).ToList();
		foreach (var File in Files)
		{
			var FileName = Path.GetFileName(File);
			RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)", FileName), File);
			PublicDelayLoadDLLs.Add(File);
			Console.WriteLine(Name + " adding dll: " + File);
		}
	}
}