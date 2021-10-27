// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarTwo/AI/Decorators/FJMOwnerIsInBuilding.h"
#include "WorldWarTwo/Controllers/FJMAIController.h"
#include "WorldWarTwo/CommonObjects/FJMBaseBuilding.h"
#include "WorldWarTwo/Characters/FJMBaseAISoldier.h"

UFJMOwnerIsInBuilding::UFJMOwnerIsInBuilding(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Owning Character Is In Building";
}

bool UFJMOwnerIsInBuilding::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	AFJMAIController* OwnerController = Cast<AFJMAIController>(OwnerComp.GetAIOwner());

	if (OwnerController)
	{
		AFJMBaseAISoldier* ControlledSoldier = Cast<AFJMBaseAISoldier>(OwnerController->GetPawn());
		if (ControlledSoldier)
		{
			AFJMBaseBuilding* OUTBuilding;
			return ControlledSoldier->GetCharacterInBuilding(OUTBuilding);
		}
	}
	return false;
}
