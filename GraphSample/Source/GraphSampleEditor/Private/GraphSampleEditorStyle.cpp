// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "GraphSampleEditorStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define IMAGE_BRUSH_SVG(RelativePath, ...) FSlateVectorImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FGraphSampleEditorStyle::StyleSet = nullptr;

FName FGraphSampleEditorStyle::GetStyleSetName()
{
	static FName GraphSampleEditorStyleName(TEXT("GraphSampleEditorStyle"));
	return GraphSampleEditorStyleName;
}

void FGraphSampleEditorStyle::Initialize()
{
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("GraphSampleEditorStyle")));

	const FVector2D Icon16(16.0f, 16.0f);
	const FVector2D Icon20(20.0f, 20.0f);
	const FVector2D Icon30(30.0f, 30.0f);
	const FVector2D Icon40(40.0f, 40.0f);
	const FVector2D Icon64(64.0f, 64.0f);

	// engine assets
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate/"));

	StyleSet->Set("GraphSampleToolbar.TestUICommand", new IMAGE_BRUSH("Icons/icon_Placement_FilterMisc_20px", Icon20));

	// GraphSample assets
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("GraphSample"))->GetBaseDir() / TEXT("Resources"));
	StyleSet->Set("GraphSampleGraphNode.Node.ActiveShadow", new BOX_BRUSH("Border_Green", FMargin(18.0f/64.0f)));
	StyleSet->Set("GraphSampleGraphNode.Node.FailedShadow", new BOX_BRUSH("Border_Red", FMargin(18.0f/64.0f)));
	StyleSet->Set("GraphSampleGraphNode.Node.DefaultShadow", new BOX_BRUSH("Border_Grey", FMargin(18.0f/64.0f)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

void FGraphSampleEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	ensure(StyleSet.IsUnique());
	StyleSet.Reset();
}

#undef BORDER_BRUSH
#undef BOX_BRUSH
#undef IMAGE_BRUSH
#undef IMAGE_BRUSH_SVG