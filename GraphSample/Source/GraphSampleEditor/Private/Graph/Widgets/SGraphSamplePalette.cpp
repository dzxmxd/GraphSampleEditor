// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/Widgets/SGraphSamplePalette.h"
#include "Asset/GraphSampleAssetEditor.h"
#include "Graph/GraphSampleGraphSchema.h"
#include "GraphSampleAsset.h"
#include "GraphSampleEditorMacro.h"
#include "Fonts/SlateFontInfo.h"
#include "Graph/GraphSampleGraphSchema_Actions.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphPalette"

void SGraphSamplePaletteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData)
{
	const FSlateFontInfo NameFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	check(InCreateData->Action.IsValid());

	const TSharedPtr<FEdGraphSchemaAction> GraphAction = InCreateData->Action;
	ActionPtr = InCreateData->Action;

	// Get the hotkey chord if one exists for this action
	TSharedPtr<const FInputChord> HotkeyChord;

	// Find icons
	const FSlateBrush* IconBrush = FAppStyle::GetBrush(TEXT("NoBrush"));
	const FSlateColor IconColor = FSlateColor::UseForeground();
	const FText IconToolTip = GraphAction->GetTooltipDescription();
	constexpr bool bIsReadOnly = false;

	const TSharedRef<SWidget> IconWidget = CreateIconWidget(IconToolTip, IconBrush, IconColor);
	const TSharedRef<SWidget> NameSlotWidget = CreateTextSlotWidget(InCreateData, bIsReadOnly);
	const TSharedRef<SWidget> HotkeyDisplayWidget = CreateHotkeyDisplayWidget(NameFont, HotkeyChord);

	// Create the actual widget
	this->ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			IconWidget
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.VAlign(VAlign_Center)
		.Padding(3, 0)
		[
			NameSlotWidget
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		[
			HotkeyDisplayWidget
		]
	];
}

TSharedRef<SWidget> SGraphSamplePaletteItem::CreateHotkeyDisplayWidget(const FSlateFontInfo& NameFont, const TSharedPtr<const FInputChord> HotkeyChord) const
{
	FText HotkeyText;
	if (HotkeyChord.IsValid())
	{
		HotkeyText = HotkeyChord->GetInputText();
	}

	return SNew(STextBlock)
		.Text(HotkeyText)
		.Font(NameFont);
}

FText SGraphSamplePaletteItem::GetDisplayText() const
{
	return FText::Format(ActionPtr.Pin()->GetMenuDescription(), ActionPtr.Pin()->GetTooltipDescription());
}

FText SGraphSamplePaletteItem::GetItemTooltip() const
{
	return ActionPtr.Pin()->GetTooltipDescription();
}

EVisibility SGraphSamplePaletteItem::GetEditButtonVisibility() const
{
	if (FGraphSampleGraphSchemaAction_NewNode::StaticGetTypeId().IsEqual(ActionPtr.Pin()->GetTypeId()))
	{
		return IsHovered() ? EVisibility::Visible : EVisibility::Collapsed;
	}
	return EVisibility::Collapsed;
}

void SGraphSamplePalette::Construct(const FArguments& InArgs, TWeakPtr<FGraphSampleAssetEditor> InGraphSampleAssetEditor)
{
	GraphSampleAssetEditor = InGraphSampleAssetEditor;

	UpdateCategoryNames();
	UGraphSampleGraphSchema::OnNodeListChanged.AddSP(this, &SGraphSamplePalette::Refresh);

	struct LocalUtils
	{
		static TSharedRef<SExpanderArrow> CreateCustomExpanderStatic(const FCustomExpanderData& ActionMenuData, bool bShowFavoriteToggle)
		{
			TSharedPtr<SExpanderArrow> CustomExpander;
			// in SBlueprintSubPalette here would be a difference depending on bShowFavoriteToggle
			SAssignNew(CustomExpander, SExpanderArrow, ActionMenuData.TableRow);
			return CustomExpander.ToSharedRef();
		}
	};

	this->ChildSlot
	[
		SNew(SBorder)
		.Padding(2.0f)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() // Filter UI
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.8)
				.VAlign(VAlign_Center)
				[
					SAssignNew(CategoryComboBox, STextComboBox)
					.OptionsSource(&CategoryNames)
					.OnSelectionChanged(this, &SGraphSamplePalette::CategorySelectionChanged)
					.InitiallySelectedItem(CategoryNames[0])
				]
			]
			+ SVerticalBox::Slot() // Content list
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(GraphActionMenu, SGraphActionMenu)
				.OnActionDragged(this, &SGraphSamplePalette::OnActionDragged)
				.OnActionSelected(this, &SGraphSamplePalette::OnActionSelected)
				.OnCreateWidgetForAction(this, &SGraphSamplePalette::OnCreateWidgetForAction)
				.OnCollectAllActions(this, &SGraphSamplePalette::CollectAllActions)
				.OnCreateCustomRowExpander_Static(&LocalUtils::CreateCustomExpanderStatic, false)
				.AutoExpandActionMenu(true)
			]
		]
	];
}

SGraphSamplePalette::~SGraphSamplePalette()
{
	UGraphSampleGraphSchema::OnNodeListChanged.RemoveAll(this);
}

void SGraphSamplePalette::Refresh()
{
	const FString LastSelectedCategory = CategoryComboBox->GetSelectedItem().IsValid() ? *CategoryComboBox->GetSelectedItem().Get() : FString();

	UpdateCategoryNames();
	RefreshActionsList(true);

	// refresh list of category and currently selected category
	CategoryComboBox->RefreshOptions();
	TSharedPtr<FString> SelectedCategory = CategoryNames[0];
	if (!LastSelectedCategory.IsEmpty())
	{
		for (const TSharedPtr<FString>& CategoryName : CategoryNames)
		{
			if (*CategoryName.Get() == LastSelectedCategory)
			{
				SelectedCategory = CategoryName;
				break;
			}
		}
	}
	CategoryComboBox->SetSelectedItem(SelectedCategory);
}

void SGraphSamplePalette::UpdateCategoryNames()
{
	CategoryNames = { MakeShareable(new FString(GraphSampleEditor::Constants::SelectedAllCategory)) };
	CategoryNames.Append(UGraphSampleGraphSchema::GetGraphSampleNodeCategories());
}

TSharedRef<SWidget> SGraphSamplePalette::OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SGraphSamplePaletteItem, InCreateData);
}

void SGraphSamplePalette::CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{
	ensureAlways(GraphSampleAssetEditor.Pin() && GraphSampleAssetEditor.Pin()->GetGraphSampleAsset());
	FGraphActionMenuBuilder ActionMenuBuilder;
	UGraphSampleGraphSchema::GetPaletteActions(ActionMenuBuilder, GetFilterCategoryName());
	OutAllActions.Append(ActionMenuBuilder);
}

FString SGraphSamplePalette::GetFilterCategoryName() const
{
	if (CategoryComboBox.IsValid() && CategoryComboBox->GetSelectedItem() != CategoryNames[0])
	{
		return *CategoryComboBox->GetSelectedItem();
	}

	return FString();
}

void SGraphSamplePalette::CategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	RefreshActionsList(true);
}

void SGraphSamplePalette::OnActionSelected(const TArray<TSharedPtr<FEdGraphSchemaAction>>& InActions, ESelectInfo::Type InSelectionType) const
{
	if (InSelectionType == ESelectInfo::OnMouseClick || InSelectionType == ESelectInfo::OnKeyPress || InSelectionType == ESelectInfo::OnNavigation || InActions.Num() == 0)
	{
		// GraphSampleAssetEditor.Pin()->SetUISelectionState(FGraphSampleAssetEditor::PaletteTab);
	}
}

void SGraphSamplePalette::ClearGraphActionMenuSelection() const
{
	GraphActionMenu->SelectItemByName(NAME_None);
}

#undef LOCTEXT_NAMESPACE
