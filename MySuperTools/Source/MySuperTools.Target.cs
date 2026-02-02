// Copyright lostpanda. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MySuperToolsTarget : TargetRules
{
	public MySuperToolsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("MySuperTools");
	}
}
