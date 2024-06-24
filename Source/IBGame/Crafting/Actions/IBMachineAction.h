// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IBMachineAction.generated.h"

class AIBCraftingMachine;
/**
 * NOTE: It would be better to set this system up to use EditInlineNew on this class and have it instanced in the recipes
 * as is we will need to create a child in blueprints for every single outcome and spawn the object each time we need to perform an action
 */
UCLASS(BlueprintType, Blueprintable)
class IBGAME_API UIBMachineAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="IB Action")
	void PerformAction(AIBCraftingMachine* CraftingMachine);
	virtual void PerformAction_Implementation(AIBCraftingMachine* CraftingMachine);

protected:
	UPROPERTY(BlueprintReadOnly, Category="IB Action")
	AIBCraftingMachine* CraftingMachineRef;
};
