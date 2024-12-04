// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "GraphSampleNode.generated.h"

#if WITH_EDITOR
UENUM(BlueprintType)
enum EGraphSampleNodeState : uint8
{
	Default = 0,
	Completed,
	Failed
};
#endif

UCLASS(Blueprintable)
class GRAPHSAMPLE_API UGraphSampleNode : public UObject
{
	GENERATED_BODY()

public:
	/** For example, the organization of runtime data depends on the specific logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UGraphSampleNode*> Children;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual FString GetNodeCategory() const;
	virtual FText GetNodeTitle() const;
	virtual FText GetNodeToolTip() const;
	EGraphSampleNodeState GetActivationState() const;
#endif

#if WITH_EDITORONLY_DATA
	/** When runtime execute node, we can use this state to changed debug style */
	EGraphSampleNodeState ActivationState = EGraphSampleNodeState::Default;
#endif
};