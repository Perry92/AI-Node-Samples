// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarTwo/AI/Services/Movement/FJMUpdateTeamSoldierFormationPos.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "WorldWarTwo/Controllers/FJMAIController.h"
#include "WorldWarTwo/Characters/FJMBaseAISoldier.h"
#include "WorldWarTwo/SoldierGroups/FJMBaseSoldierTeam.h"
#include <Kismet/KismetSystemLibrary.h>
#include "WorldWarTwo/Library/StaticFunctions.h"
#include <Kismet/KismetMathLibrary.h>
#include <NavigationSystem.h>

UFJMUpdateTeamSoldierFormationPos::UFJMUpdateTeamSoldierFormationPos(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Soldier Formation Pos";
}

void UFJMUpdateTeamSoldierFormationPos::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerController = Cast<AFJMAIController>(OwnerComp.GetAIOwner());
	if (ensure(OwnerController))
	{
		ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
	}
}

void UFJMUpdateTeamSoldierFormationPos::GetBlackBoardVariables(AFJMBaseAISoldier *& OUTTeamLeaderPawn, ETeamFormationType & OUTFormationType)
{
	if (ensure(OwnerController))
	{
		auto TeamLeaderObject = OwnerController->GetBlackboardComponent()->GetValueAsObject(TeamLeaderPawnKey.SelectedKeyName);
		OUTTeamLeaderPawn = Cast<AFJMBaseAISoldier>(TeamLeaderObject);

		uint8 TeamEnum = OwnerController->GetBlackboardComponent()->GetValueAsEnum(FormationTypeKey.SelectedKeyName);
		OUTFormationType = static_cast<ETeamFormationType>(TeamEnum);
	}
}

void UFJMUpdateTeamSoldierFormationPos::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	OwnerController = Cast<AFJMAIController>(OwnerComp.GetAIOwner());
	if (OwnerController)
	{
		ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
		if (ControlledSoldier)
		{
			AFJMBaseAISoldier* LeaderPawn;
			ETeamFormationType FormationType;

			GetBlackBoardVariables(LeaderPawn, FormationType);

			FVector PositionToUse;

			switch (FormationType)
			{
			case ETeamFormationType::Easy:
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->AtEaseFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->AtEaseFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->AtEaseFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			case ETeamFormationType::Column:
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->ColumnFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->ColumnFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->ColumnFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			case ETeamFormationType::SkirmishLine:
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->SkirmishFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->SkirmishFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->SkirmishFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			case ETeamFormationType::Wedge:
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->WedgeFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->WedgeFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->WedgeFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			case ETeamFormationType::InteriorSearch:
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->BuildingFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->BuildingFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->BuildingFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			case ETeamFormationType::TeamTalk:
				OwnerController->SetFocus(LeaderPawn);
				switch (ControlledSoldier->GetSoldierTeamPosition())
				{
				case ESoldierFormationPosition::PositionA:
					PositionToUse = ControlledSoldier->GetParentTeam()->SceneFormation.PositionA;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionB:
					PositionToUse = ControlledSoldier->GetParentTeam()->SceneFormation.PositionB;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				case ESoldierFormationPosition::PositionC:
					PositionToUse = ControlledSoldier->GetParentTeam()->SceneFormation.PositionC;
					FindFormationPositionInWorld(PositionToUse, LeaderPawn);
					break;
				}
				break;
			}
		}
	}
}

void UFJMUpdateTeamSoldierFormationPos::FindFormationPositionInWorld(FVector RelativeFormationPosition, AFJMBaseAISoldier* LeaderPawn)
{
	if (ensure(OwnerController))
	{
		FVector CurrentFormationPos = OwnerController->GetBlackboardComponent()->GetValueAsVector(NewPositionKey.SelectedKeyName);

		if (ControlledSoldier)
		{
			AFJMBaseAISoldier * OUTTeamLeaderPawn;
			ETeamFormationType  OUTFormationType;

			GetBlackBoardVariables(OUTTeamLeaderPawn, OUTFormationType);

			FVector TeamLeaderForward = LeaderPawn->GetActorForwardVector();
			FVector TeamLeaderRight = LeaderPawn->GetActorRightVector();

			FVector WorldFormationVector = ((TeamLeaderForward * RelativeFormationPosition.X) + (TeamLeaderRight * RelativeFormationPosition.Y)) + LeaderPawn->GetActorLocation();
			float DistanceBetweenPositions = UStaticFunctions::DistanceBetweenVectors(CurrentFormationPos, WorldFormationVector);
			float DistanceToLeader = UStaticFunctions::DistanceBetweenVectors(LeaderPawn->GetActorLocation(), ControlledSoldier->GetActorLocation());

			bool isLevelHeightWithLeader = UKismetMathLibrary::NearlyEqual_FloatFloat(ControlledSoldier->GetActorLocation().X, LeaderPawn->GetActorLocation().X, 10.f);
			bool IsInFrontOfLeader = UStaticFunctions::ActorIsFacingVector(LeaderPawn, ControlledSoldier->GetActorLocation(), 70) && isLevelHeightWithLeader;
			bool IsFacingLeader = UStaticFunctions::ActorIsFacingVector(ControlledSoldier, LeaderPawn->GetActorLocation(), 70);

			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

			if (DistanceBetweenPositions > 75)
			{
				if (false)//IsInFrontOfLeader && DistanceToLeader < 400)
				{
					float DistanceToEscape = UStaticFunctions::DistanceBetweenVectors(CurrentFormationPos, ControlledSoldier->GetActorLocation());
					if (DistanceToEscape > 300 || DistanceToEscape < 150)
					{
						if (LeaderPawn->GetBreachDoorway() || ControlledSoldier->GetBreachDoorway())
						{
							if (IsFacingLeader)
							{
								FVector NewPos = ControlledSoldier->GetActorForwardVector() * -200;
								FNavLocation ResultLocation;
								bool found = NavSys->GetRandomReachablePointInRadius(NewPos, 75.f, ResultLocation);
								if (!ResultLocation.Location.IsNearlyZero())
								{
									OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, ResultLocation);
								}
							}
							else
							{
								FVector NewPos = ControlledSoldier->GetActorForwardVector() * 200;
								FNavLocation ResultLocation;
								bool found = NavSys->GetRandomReachablePointInRadius(NewPos, 75.f, ResultLocation);
								if (!ResultLocation.Location.IsNearlyZero())
								{
									OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, ResultLocation);
								}
							}

						}
						else
						{
							FVector NewPos = ControlledSoldier->GetActorLocation();
							FNavLocation ResultLocation;
							bool found = NavSys->GetRandomReachablePointInRadius(NewPos, 300.f, ResultLocation);
							if (!ResultLocation.Location.IsNearlyZero())
							{
								OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, ResultLocation);
							}
						}
					}
				}
				else
				{
					TArray<TEnumAsByte<EObjectTypeQuery>> AllowedObjectTypes;
					AllowedObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
					TArray<AActor*> IgnoreActors;
					IgnoreActors.Add(ControlledSoldier);

					//then create an array that 'ComponentOverlapComponents' can fill with any overlapped comps that match the filters defined above and run the overlap check.
					TArray<UPrimitiveComponent*> OUTOverlappedComponents;

					//TODO - check null is okay here.
					bool HasOverlapped = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), WorldFormationVector, 20.f, AllowedObjectTypes, NULL, IgnoreActors, OUTOverlappedComponents);

					FNavLocation OUTLocation;
					bool Navigable = NavSys->GetNavigationSystem(this)->ProjectPointToNavigation(WorldFormationVector, OUTLocation, FVector(25, 25, 250), NavSys->MainNavData);
					if (Navigable)
					{
						OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, WorldFormationVector);
					}
					else
					{
						OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, LeaderPawn->GetActorLocation());
					}
				}
			}
			else if (OUTFormationType != ETeamFormationType::TeamTalk && IsInFrontOfLeader)
			{
				FVector TeamLeaderRightPos = (TeamLeaderRight * 100) + LeaderPawn->GetActorLocation();
				FVector TeamLeaderLeftPos = (TeamLeaderRight * -100) + LeaderPawn->GetActorLocation();

				TArray<UPrimitiveComponent*> OUTOverlappedComponents;
				TArray<TEnumAsByte<EObjectTypeQuery>> AllowedObjectTypes;
				AllowedObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
				TArray<AActor*> IgnoreActors;

				bool HasOverlappedRight = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), TeamLeaderRightPos, 20.f, AllowedObjectTypes, NULL, IgnoreActors, OUTOverlappedComponents);
				bool HasOverlappedLeft = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), TeamLeaderLeftPos, 20.f, AllowedObjectTypes, NULL, IgnoreActors, OUTOverlappedComponents);

				if (!HasOverlappedRight)
				{
					float NewDistance = UStaticFunctions::DistanceBetweenVectors(CurrentFormationPos, TeamLeaderRightPos);
					if (NewDistance > 50)
					{
						if (!TeamLeaderRightPos.IsNearlyZero())
						{
							OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, TeamLeaderRightPos);
						}
					}
				}
				else if (!HasOverlappedLeft)
				{
					float NewDistance = UStaticFunctions::DistanceBetweenVectors(CurrentFormationPos, TeamLeaderLeftPos);
					if (NewDistance > 50)
					{
						if (!TeamLeaderLeftPos.IsNearlyZero())
						{
							OwnerController->GetBlackboardComponent()->SetValueAsVector(NewPositionKey.SelectedKeyName, TeamLeaderLeftPos);
						}
					}
				}
			}
		}
	}
}
