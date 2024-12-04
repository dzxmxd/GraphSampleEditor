// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "SGraphPalette.h"

class FGraphSampleAssetEditor;

/** Widget displaying a single item  */
class GRAPHSAMPLEEDITOR_API SGraphSamplePaletteItem : public SGraphPaletteItem
{
public:
	SLATE_BEGIN_ARGS(SGraphSamplePaletteItem) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData);

private:
	TSharedRef<SWidget> CreateHotkeyDisplayWidget(const FSlateFontInfo& NameFont, const TSharedPtr<const FInputChord> HotkeyChord) const;
	virtual FText GetDisplayText() const override;
	virtual FText GetItemTooltip() const override;
	EVisibility GetEditButtonVisibility() const;
};

/** GraphSample Palette  */
class GRAPHSAMPLEEDITOR_API SGraphSamplePalette : public SGraphPalette
{
public:
	SLATE_BEGIN_ARGS(SGraphSamplePalette) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TWeakPtr<FGraphSampleAssetEditor> InGraphSampleAssetEditor);
	virtual ~SGraphSamplePalette() override;

protected:
	void Refresh();
	void UpdateCategoryNames();

	// SGraphPalette
	virtual TSharedRef<SWidget> OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData) override;
	virtual void CollectAllActions(FGraphActionListBuilderBase& OutAllActions) override;
	// --

	FString GetFilterCategoryName() const;
	void CategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnActionSelected(const TArray<TSharedPtr<FEdGraphSchemaAction>>& InActions, ESelectInfo::Type InSelectionType) const;

public:
	void ClearGraphActionMenuSelection() const;

protected:
	TWeakPtr<FGraphSampleAssetEditor> GraphSampleAssetEditor;
	TArray<TSharedPtr<FString>> CategoryNames;
	TSharedPtr<STextComboBox> CategoryComboBox;
};