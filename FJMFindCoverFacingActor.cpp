// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarTwo/AI/Tasks/Cover/FJMFindCoverFacingActor.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "WorldWarTwo/CommonObjects/FJMBaseCoverObject.h"
#include "WorldWarTwo/SoldierGroups/FJMBaseSoldierTeam.h"
#include "WorldWarTwo/Components/Characters/Combat/Cover/FJMAICoverComponent.h"
#include "WorldWarTwo/Components/Building/FJMInteriorCoverComponent.h"
#include "WorldWarTwo/Controllers/FJMAIController.h"
#include "WorldWarTwo/Characters/FJMBaseAISoldier.h"
#include "WorldWarTwo/Vehicles/FJMBaseVehicle.h"
#include "WorldWarTwo/Library/StaticFunctions.h"
#include "WorldWarTwo/Library/FJMStructs.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetArrayLibrary.h>

UFJMFindCoverFacingActor::UFJMFindCoverFacingActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Find Cover Facing Actor";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UFJMFindCoverFacingActor::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerController = Cast<AFJMAIController>(OwnerComp.GetAIOwner());

	if (OwnerController)
	{
		ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
		if (ControlledSoldier)
		{
			UFJMAICoverComponent* AICoverComp = Cast<UFJMAICoverComponent>(ControlledSoldier->GetComponentByClass(UFJMAICoverComponent::StaticClass()));
			if (AICoverComp)
			{
				AActor* CurrentTargetEnemy = Cast<AActor>(OwnerController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
				if (CurrentTargetEnemy)
				{
					AFJMBaseHumanCharacter* TargetEnemy = Cast<AFJMBaseHumanCharacter>(CurrentTargetEnemy);
					AFJMBaseVehicle* TargetVehicle = Cast<AFJMBaseVehicle>(CurrentTargetEnemy);

					if (TargetEnemy != nullptr && TargetEnemy->GetIsDead())
					{
						OwnerController->ClearAndCheckForNewTarget();
						return EBTNodeResult::Failed;
					}
					else if (TargetVehicle != nullptr && TargetVehicle->GetIsDestroyed())
					{
						OwnerController->ClearAndCheckForNewTarget();
						return EBTNodeResult::Failed;
					}
					else
					{
						FCoverPositionData* CompareCover = nullptr;
						FCoverPositionData* OccupiedCover = AICoverComp->GetOccupiedCoverData();
						FCoverPositionData* DesiredCover = AICoverComp->GetDesiredCoverData();

						if (OccupiedCover) { CompareCover = OccupiedCover; }
						else if (DesiredCover) { CompareCover = DesiredCover; }

						if (CompareCover)
						{
							if (CoverIsSuitable(CompareCover, CurrentTargetEnemy))
							{
								// current cover is acceptable, return failed as no new cover has been found.
								return EBTNodeResult::Failed;
							}
							else
							{
								OwnerController->SetBBCoverStatus(ESoldierCoverStatus::NotInCover);
								ControlledSoldier->GetCharCoverComp()->ExitCover();
								ControlledSoldier->GetCharCoverComp()->ClearDesiredCover();
								OwnerController->GetBlackboardComponent()->ClearValue(CurrentCoverLocationKey.SelectedKeyName);
								return FindNewCover(CurrentTargetEnemy, AICoverComp);
							}
						}
						else
						{
							if (TargetEnemy) { return FindNewCover(TargetEnemy, AICoverComp); }
							else if (TargetVehicle) { return FindNewCover(TargetVehicle, AICoverComp); }
							
						}
					}
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UFJMFindCoverFacingActor::FindNewCover(AActor* TargetEnemy, UFJMAICoverComponent* AICoverComp)
{
	if (OwnerController)
	{
		UBlackboardComponent* BBComp = OwnerController->GetBlackboardComponent();
		ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
		if (ControlledSoldier && BBComp)
		{
			TArray<FCoverPositionData*> NearCover = AICoverComp->FindNearbyCoverToUse(MaximumDistanceToCover);
			TArray<FCoverPositionData*> SuitableCover;
			if (NearCover.Num() > 0)
			{
				for (FCoverPositionData* PotentialCover : NearCover)
				{
					if (CoverIsSuitable(PotentialCover, TargetEnemy))
					{
						SuitableCover.Add(PotentialCover);
					}
				}

				if (SuitableCover.Num() > 0)
				{
					TArray<FCoverPositionData*> ClosestCover;
					UStaticFunctions::OrderCoverDataByDistance(ControlledSoldier, SuitableCover, ClosestCover);

					if (ClosestCover.Num() > 3)
					{
						ClosestCover.SetNum(3);
					}

					int32 RandomI = UKismetMathLibrary::RandomIntegerInRange(0, (ClosestCover.Num() - 1));
					FCoverPositionData* NewCover = ClosestCover[RandomI];

					if (NewCover)
					{
						if (ControlledSoldier->GetCharCoverComp()->GetIsTakingCover())
						{
							//exit cover and set new desired cover pos.
							OwnerController->SetBBCoverStatus(ESoldierCoverStatus::NotInCover);
							ControlledSoldier->GetCharCoverComp()->ExitCover();
							ControlledSoldier->GetCharCoverComp()->SetNewDesiredCover(NewCover);
							BBComp->SetValueAsVector(CurrentCoverLocationKey.SelectedKeyName, NewCover->WorldPosition);
							return EBTNodeResult::Succeeded;
						}
						else
						{
							ControlledSoldier->GetCharCoverComp()->SetNewDesiredCover(NewCover);
							BBComp->SetValueAsVector(CurrentCoverLocationKey.SelectedKeyName, NewCover->WorldPosition);
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

bool UFJMFindCoverFacingActor::CoverIsSuitable(FCoverPositionData* CoverPosition, AActor* TargetEnemy)
{
	if (OwnerController)
	{
		ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
		if (ControlledSoldier)
		{
			UFJMAICoverComponent* AICoverComp = Cast<UFJMAICoverComponent>(ControlledSoldier->GetComponentByClass(UFJMAICoverComponent::StaticClass()));
			if (AICoverComp)
			{
				float Distance = (TargetEnemy->GetActorLocation() - CoverPosition->WorldPosition).Size();
				if (Distance > 500.f)
				{
					bool CanAttackEnemyFromCover = AICoverComp->CanAttackActorFromCover(CoverPosition, TargetEnemy);
					if (CanAttackEnemyFromCover)
					{
						// current cover is acceptable, return failed as no new cover has been found.
						return true;
					}
					else
					{
						AActor* NewEnemyTarget;
						bool EnemiesVisFromCover = AICoverComp->EnemiesVisibleFromCover(CoverPosition, 10000, NewEnemyTarget);
						if (EnemiesVisFromCover)
						{
							//other enemies are visible from cover, change enemy to one of those.
							if (AFJMBaseHumanCharacter* NewTarget = Cast<AFJMBaseHumanCharacter>(NewEnemyTarget))
							{
								OwnerController->SetNewEnemyTarget(NewTarget);
								// current cover is acceptable, return failed as no new cover has been found.
								return true;
							}
							else if (AFJMBaseVehicle* NewVehTarget = Cast<AFJMBaseVehicle>(NewEnemyTarget))
							{
								OwnerController->SetNewEnemyTarget(NewVehTarget);
								// current cover is acceptable, return failed as no new cover has been found.
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
