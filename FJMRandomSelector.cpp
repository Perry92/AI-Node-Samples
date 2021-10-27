// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarTwo/AI/Composites/FJMRandomSelector.h"
#include <Kismet/KismetMathLibrary.h>

UFJMRandomSelector::UFJMRandomSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Randomiser";
}

int32 UFJMRandomSelector::GetNextChildHandler(FBehaviorTreeSearchData & SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	// success = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// newly activated: start from first
		uint8 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, (GetChildrenNum() - 1));
		NextChildIdx = RandomIndex;

	}
	else if (LastResult == EBTNodeResult::Failed)
	{
		// failed = choose next child
		NextChildIdx = BTSpecialChild::ReturnToParent;
	}

	return NextChildIdx;
}

#if WITH_EDITOR
FName UFJMRandomSelector::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif