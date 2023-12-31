// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

using UnrealBuildTool;

public class AwsSdkIntegration : ModuleRules
{
	public AwsSdkIntegration(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Projects",
			"Engine",

			"AwsSdkLibrary", 
		});
	}
}