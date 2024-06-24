// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Actions/IBMachineAction_CraftRecipe.h"

#include "Crafting/IBCrafingCommon.h"
#include "Crafting/IBCraftingMachine.h"
#include "Crafting/IBShapeActor.h"

void UIBMachineAction_CraftRecipe::PerformAction_Implementation(AIBCraftingMachine* CraftingMachine)
{
	Super::PerformAction_Implementation(CraftingMachine);
	
	if (ShapeClass)
	{
		SpawnShapeActors(ShapeClass, Amount);
	}
}

void UIBMachineAction_CraftRecipe::SpawnShapeActors(TSubclassOf<AIBShapeActor> ShapeClass, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = CraftingMachineRef;
		SpawnInfo.Instigator = CraftingMachineRef->GetInstigator();
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const FVector Location = CraftingMachineRef->CraftResultLocation->GetComponentLocation();
		
		if (AIBShapeActor* ShapeActor = GetWorld()->SpawnActor<AIBShapeActor>(ShapeClass, Location, FRotator::ZeroRotator, SpawnInfo);
			ShapeActor != nullptr)
		{
			CraftingMachineRef->AddShapeOverlap(ShapeActor);
		}
	}
}
