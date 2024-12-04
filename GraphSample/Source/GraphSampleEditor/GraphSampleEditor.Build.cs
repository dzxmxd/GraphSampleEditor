// Copyright https://github.com/dzxmxd/GraphSampleEditor

using UnrealBuildTool;

public class GraphSampleEditor : ModuleRules
{
	public GraphSampleEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GraphSample",
				"AssetTools"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"ToolMenus",
				"GraphEditor",
				"UnrealEd",
				"EditorFramework",
				"AssetTools",
				"BlueprintGraph",
				"DeveloperSettings",
				"ApplicationCore",
				"Json",
				"InputCore",
				"Projects",
				"ToolMenus",
				"ToolWidgets",
				"KismetWidgets"
			}
		);
	}
}