// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

using System;
using System.IO;
using System.Linq;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;

public class AwsSdkLibrary : ModuleRules
{
	// string[] AwsLibraries = {"core", "s3", "dynamodb"};
	string[] AwsLibraries = {"core", "ec2" };

	public AwsSdkLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		// This is a blocking operation which halts generation until libraries are build and copied over
		BuildAwsLibraries();

		// Gives access to the AWS C++ SDK header files to modules referencing this
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		// We will be using Unreal Engine allocators, so we need this define to make sure we are using the correct types
		PublicDefinitions.Add("USE_AWS_MEMORY_MANAGEMENT");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			AddPrecompiledLibraries(Path.Combine(ModuleDirectory, "lib"), "*.lib");
			AddPrecompiledDlls(Path.Combine(ModuleDirectory, "bin"), "*.dll");
		}
	}

	private void BuildAwsLibraries()
	{
		string AwsLibrariesArg = AwsLibraries.Aggregate((current, next) => $"{current},{next}");

		string ScriptPath = Path.Combine(PluginDirectory, "Extras/Scripts/build-vcpkg.ps1");

		// Prepare a new external process instance to build and copy binaries
		Process Process = new Process();
		Process.StartInfo.FileName = "powershell.exe";
		Process.StartInfo.Arguments = $"-NoProfile -ExecutionPolicy Bypass -File \"{ScriptPath}\" -awsLibraries {AwsLibrariesArg}";
		Process.StartInfo.UseShellExecute = false;

		Console.WriteLine(Name + " running script: " + ScriptPath);

		// Start and block until process ends to ensure the Build.cs file links binaries after they are successfully built
		Process.Start();
		Process.WaitForExit();
	}

	private void AddPrecompiledLibraries(string FolderPath, string Extension)
	{
		List<string> Files = Directory.GetFiles(FolderPath, Extension, SearchOption.AllDirectories).ToList();
		foreach (string File in Files)
		{
			PublicAdditionalLibraries.Add(File);
			Console.WriteLine(Name + " adding library: " + File);
		}
	}

	private void AddPrecompiledDlls(string FolderPath, string Extension)
	{
		List<string> Files = Directory.GetFiles(FolderPath, Extension, SearchOption.AllDirectories).ToList();
		foreach (string File in Files)
		{
			string FileName = Path.GetFileName(File);
			RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)", FileName), File);
			PublicDelayLoadDLLs.Add(File);
			Console.WriteLine(Name + " adding dll: " + File);
		}
	}
}
