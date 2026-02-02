// Copyright lostpanda. All Rights Reserved.

using UnrealBuildTool;

public class SuperTools : ModuleRules
{
	public SuperTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"SuperToolsLibrary",
				"Projects"
			}
		);
	}
}
