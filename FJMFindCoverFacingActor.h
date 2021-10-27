// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "FJMFindCoverFacingActor.generated.h"

/**
 * 
 */
UCLASS()
class WORLDWARTWO_API UFJMFindCoverFacingActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UFJMFindCoverFacingActor(const FObjectInitializer& ObjectInitializer);

	//the maximum distance the cover casn be from the character.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MaximumDistanceToCover = 1000.f;

	//the corresponding blackboard key for the Enemy we want to face - BB Key set in editor.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	//the corresponding blackboard key for the the location of cover - should be set if we find new cover.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector CurrentCoverLocationKey;

	/** starts this task, should return Succeeded, Failed or InProgress
	 *  (use FinishLatentTask() when returning InProgress)
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/*Attempts to find new cove for the character within the given maximum range for this task
	* If cover is found this method will set the cover in the characters cover compoent and add its location to the blackboard.
	* Will only return Success if cover has actually been found otherwise the task will fail*/
	EBTNodeResult::Type FindNewCover(class AActor* TargetEnemy, class UFJMAICoverComponent* AICoverComp);

private:
	class AFJMAIController* OwnerController;
	class AFJMBaseAISoldier* ControlledSoldier;

	/*Checks that the given cover poistion is not in use and is facing the target enemy
	* Returns true where the cover is suitable and is not in use by another character*/
	bool CoverIsSuitable(struct FCoverPositionData* CoverPosition, AActor* TargetEnemy);

};
