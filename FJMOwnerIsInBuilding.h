// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/Decorators/BTDecorator_BlackboardBase.h>
#include "FJMOwnerIsInBuilding.generated.h"

/**
 * 
 */
UCLASS()
class WORLDWARTWO_API UFJMOwnerIsInBuilding : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	UFJMOwnerIsInBuilding(const FObjectInitializer& ObjectInitializer);

	/*Calculates whether or not the decorator should allow access to the nodes below it
	* This will get the owning character and check whether they are currently in a building, if they are return true otherwise false.*/
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
