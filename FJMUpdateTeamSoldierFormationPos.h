// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/Services/BTService_BlackboardBase.h>
#include "WorldWarTwo/Library/FJMEnumerations.h"
#include "FJMUpdateTeamSoldierFormationPos.generated.h"

/**
 * 
 */
UCLASS()
class WORLDWARTWO_API UFJMUpdateTeamSoldierFormationPos : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UFJMUpdateTeamSoldierFormationPos(const FObjectInitializer& ObjectInitializer);
	
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//when the service comes into action - set player character here.
	void OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

public:

	//the corresponding blackboard key for the team leader - BB Key set in editor.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TeamLeaderPawnKey;

	//the corresponding blackboard key for the team formation - BB Key set in editor.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector FormationTypeKey;

	//the corresponding blackboard key for the position this soldier should move to - BB Key set in editor.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector NewPositionKey;

private:
	class AFJMAIController* OwnerController;
	class AFJMBaseAISoldier* ControlledSoldier;

	//gets the actualy values from the blacboard for the leader pawn and formation type.
	void GetBlackBoardVariables(AFJMBaseAISoldier*& OUTTeamLeaderPawn, ETeamFormationType& OUTFormationType);

	//Sets the new formation position by calculating the world position from the relative formation position and the team leaders location.
	void FindFormationPositionInWorld(FVector RelativeFormationPosition, class AFJMBaseAISoldier* LeaderPawn);
};
