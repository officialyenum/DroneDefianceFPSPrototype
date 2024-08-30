// Copyright Chukwuyenum Opone @officialyenum 2024

using UnrealBuildTool;
using System.Collections.Generic;

public class BrotherhoodFPSEditorTarget : TargetRules
{
	public BrotherhoodFPSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "BrotherhoodFPS" } );
	}
}
