// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/Composites/BTComposite_Selector.h>
#include "FJMRandomSelector.generated.h"

/**
 * 
 */
UCLASS()
class WORLDWARTWO_API UFJMRandomSelector : public UBTComposite_Selector
{
	GENERATED_BODY()

	UFJMRandomSelector(const FObjectInitializer& ObjectInitializer);

	TArray<uint8> UsedIndeices;

	/*Selects the next child node to go to in the behaviour tree 
	* Selects a child node at random by finding a random integer corresponding to the number of children*/
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif
};
