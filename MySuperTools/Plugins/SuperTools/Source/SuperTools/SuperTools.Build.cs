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
				"CoreUObject",
				"Engine",
				"SuperToolsLibrary",
				"Projects",
				"Slate",
				"SlateCore",
				"ApplicationCore",
				"Sockets",
				"Networking",
				"DesktopPlatform",
				"ImageWrapper",
				"Json",
				"JsonUtilities",
				"HTTP",
				"RenderCore"
			}
		);
	}
}
