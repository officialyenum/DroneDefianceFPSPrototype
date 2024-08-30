// Copyright Chukwuyenum Opone @officialyenum 2024

using UnrealBuildTool;
using System.Collections.Generic;

public class BrotherhoodFPSTarget : TargetRules
{
	public BrotherhoodFPSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "BrotherhoodFPS" } );
	}
}
