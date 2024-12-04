// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Asset/GraphSampleAssetEditor.h"

#include "EditorClassUtils.h"
#include "GraphSampleAsset.h"
#include "GraphSampleDelegates.h"
#include "GraphSampleEditorCommands.h"
#include "Asset/GraphSampleAssetEditorContext.h"
#include "Graph/Widgets/SGraphSampleGraphEditor.h"
#include "Graph/Widgets/SGraphSamplePalette.h"

#define LOCTEXT_NAMESPACE "GraphSampleAssetEditor"

const FName FGraphSampleAssetEditor::DetailsTab(TEXT("Details"));
const FName FGraphSampleAssetEditor::GraphTab(TEXT("Graph"));
const FName FGraphSampleAssetEditor::PaletteTab(TEXT("Palette"));

FGraphSampleAssetEditor::FGraphSampleAssetEditor()
	: GraphSampleAsset(nullptr) {}

FGraphSampleAssetEditor::~FGraphSampleAssetEditor() {}

FName FGraphSampleAssetEditor::GetToolkitFName() const
{
	return FName("GraphSampleEditor");
}

FText FGraphSampleAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("GraphSampleAssetEditor_ToolkitName", "GraphSampleAsset Editor");
}

FString FGraphSampleAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("GraphSampleAssetEditor_WorldCentricTabPrefix", "GraphSampleAsset").ToString();
}

FLinearColor FGraphSampleAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FGraphSampleAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_GraphSampleAssetEditor", "GraphSample Editor"));
	const auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(DetailsTab, FOnSpawnTab::CreateSP(this, &FGraphSampleAssetEditor::SpawnTab_Details))
				.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
				.SetGroup(WorkspaceMenuCategoryRef)
				.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(GraphTab, FOnSpawnTab::CreateSP(this, &FGraphSampleAssetEditor::SpawnTab_Graph))
				.SetDisplayName(LOCTEXT("GraphTab", "Graph"))
				.SetGroup(WorkspaceMenuCategoryRef)
				.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(PaletteTab, FOnSpawnTab::CreateSP(this, &FGraphSampleAssetEditor::SpawnTab_Palette))
				.SetDisplayName(LOCTEXT("PaletteTab", "Palette"))
				.SetGroup(WorkspaceMenuCategoryRef)
				.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Palette"));
}

void FGraphSampleAssetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(DetailsTab);
	InTabManager->UnregisterTabSpawner(GraphTab);
	InTabManager->UnregisterTabSpawner(PaletteTab);
}

void FGraphSampleAssetEditor::InitToolMenuContext(FToolMenuContext& MenuContext)
{
	FAssetEditorToolkit::InitToolMenuContext(MenuContext);

	UGraphSampleAssetEditorContext* Context = NewObject<UGraphSampleAssetEditorContext>();
	Context->GraphSampleAssetEditor = SharedThis(this);
	MenuContext.AddObject(Context);
}

void FGraphSampleAssetEditor::PostRegenerateMenusAndToolbars()
{
	FAssetEditorToolkit::PostRegenerateMenusAndToolbars();
	// Provide a hyperlink to view our class
	const TSharedRef<SHorizontalBox> MenuOverlayBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			.ShadowOffset(FVector2D::UnitVector)
			.Text(LOCTEXT("GraphSampleAssetEditor_AssetType", "Asset Type: "))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			FEditorClassUtils::GetSourceLink(GraphSampleAsset->GetClass())
		];

	SetMenuOverlay(MenuOverlayBox);
}

bool FGraphSampleAssetEditor::CanSaveAsset() const
{
	return FAssetEditorToolkit::CanSaveAsset();
}

bool FGraphSampleAssetEditor::IsTabFocused(const FTabId& TabId) const
{
	if (const TSharedPtr<SDockTab> CurrentGraphTab = GetToolkitHost()->GetTabManager()->FindExistingLiveTab(TabId))
	{
		return CurrentGraphTab->IsActive();
	}

	return false;
}

TSharedRef<SDockTab> FGraphSampleAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == DetailsTab);

	return SNew(SDockTab)
		.Label(LOCTEXT("GraphSampleDetailsTitle", "Details"))
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FGraphSampleAssetEditor::SpawnTab_Graph(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == GraphTab);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("GraphSampleGraphTitle", "Graph"));

	if (GraphEditor.IsValid())
	{
		SpawnedTab->SetContent(GraphEditor.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FGraphSampleAssetEditor::SpawnTab_Palette(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == PaletteTab);

	return SNew(SDockTab)
		.Label(LOCTEXT("GraphSamplePaletteTitle", "Palette"))
		[
			Palette.ToSharedRef()
		];
}

void FGraphSampleAssetEditor::InitGraphSampleAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	GraphSampleAsset = CastChecked<UGraphSampleAsset>(ObjectToEdit);

	BindToolbarCommands();
	RegisterDelegates();
	CreateToolbar();
	CreateWidgets();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("GraphSampleAssetEditor_Layout_v0.1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.225f)
					->AddTab(DetailsTab, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->SetHideTabWell(true)
					->AddTab(GraphTab, ETabState::OpenedTab)
				)

				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f)
					->AddTab(PaletteTab, ETabState::OpenedTab)
				)
		);

	constexpr bool bCreateDefaultStandaloneMenu = true;
	constexpr bool bCreateDefaultToolbar = true;
	InitAssetEditor(Mode, InitToolkitHost, TEXT("GraphSampleEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectToEdit, false);

	RegenerateMenusAndToolbars();
}

void FGraphSampleAssetEditor::CreateWidgets()
{
	// Details View
	{
		FDetailsViewArgs Args;
		Args.bHideSelectionTip = true;
		Args.bShowPropertyMatrixButton = false;
		Args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		DetailsView = PropertyModule.CreateDetailView(Args);
		DetailsView->SetObject(GraphSampleAsset);
	}

	// Graph
	SAssignNew(GraphEditor, SGraphSampleGraphEditor, SharedThis(this))
	.DetailsView(DetailsView);

	// Palette
	Palette = SNew(SGraphSamplePalette, SharedThis(this));
}

void FGraphSampleAssetEditor::CreateToolbar()
{
	FName ParentToolbarName;
	const FName ToolBarName = GetToolMenuToolbarName(ParentToolbarName);

	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* FoundMenu = ToolMenus->FindMenu(ToolBarName);
	if (!FoundMenu || !FoundMenu->IsRegistered())
	{
		FoundMenu = ToolMenus->RegisterMenu(ToolBarName, ParentToolbarName, EMultiBoxType::ToolBar);
	}

	if (FoundMenu)
	{
		{
			FToolMenuSection& Section = FoundMenu->AddSection("GraphSampleAsset");
			Section.InsertPosition = FToolMenuInsert("Asset", EToolMenuInsertType::After);

			// Default execute this logic when save package, no longer need this button on toolbar
			Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGraphSampleToolbarCommands::Get().TestUICommand));
		}
	}
}

void FGraphSampleAssetEditor::BindToolbarCommands()
{
	FGraphSampleToolbarCommands::Register();
	const FGraphSampleToolbarCommands& ToolbarCommands = FGraphSampleToolbarCommands::Get();

	// Editing toolbar here
	ToolkitCommands->MapAction(ToolbarCommands.TestUICommand,
		FExecuteAction::CreateSP(this, &FGraphSampleAssetEditor::TestUICommandCall),
		FCanExecuteAction::CreateStatic(&FGraphSampleAssetEditor::CanTestUICommandCall));
}

void FGraphSampleAssetEditor::RegisterDelegates()
{
	GraphSample::GraphSampleNode::Delegates::OnGraphSampleNodeChanged.AddSP(this, &FGraphSampleAssetEditor::OnGraphSampleNodeChanged);
}

void FGraphSampleAssetEditor::UnregisterDelegates() const
{
	GraphSample::GraphSampleNode::Delegates::OnGraphSampleNodeChanged.RemoveAll(this);
}

void FGraphSampleAssetEditor::TestUICommandCall() const
{
	const FString LogStr = FString::Printf(TEXT("TestUICommandCall!!!"));
	UE_LOG(LogTemp, Log, TEXT("%s:%d %s"), *FString(__FUNCTION__), __LINE__, *LogStr);
}

bool FGraphSampleAssetEditor::CanTestUICommandCall()
{
	return true;
}

void FGraphSampleAssetEditor::OnGraphSampleNodeChanged() const
{
	GraphEditor.Get()->NotifyGraphChanged();
}

#undef LOCTEXT_NAMESPACE