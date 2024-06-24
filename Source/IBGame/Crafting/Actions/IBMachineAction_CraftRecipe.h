// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crafting/Actions/IBMachineAction.h"
#include "IBMachineAction_CraftRecipe.generated.h"

struct FIBCraftingShape;
class AIBShapeActor;
/**
 * 
 */
UCLASS()
class IBGAME_API UIBMachineAction_CraftRecipe : public UIBMachineAction
{
	GENERATED_BODY()

public:
	virtual void PerformAction_Implementation(AIBCraftingMachine* CraftingMachine) override;
	
	/**
	 * \brief Spawns in a new shape from a craft
	 * 
	 * \param ShapeClass The type of shape to spawn
	 * \param Amount The amount to spawn
	 */
	void SpawnShapeActors(TSubclassOf<AIBShapeActor> ShapeClass, int32 Amount);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CraftRecipe")
	TSubclassOf<AIBShapeActor> ShapeClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CraftRecipe")
	float Amount = 1;
};
