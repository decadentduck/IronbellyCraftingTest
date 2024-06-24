// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "Crafting/Actions/IBMachineAction.h"
#include "IBMachineAction_GameplayEffect.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class IBGAME_API UIBMachineAction_GameplayEffect : public UIBMachineAction
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category="GameplayEffect")
	TArray<APawn*> Players;
	
	virtual void PerformAction_Implementation(AIBCraftingMachine* CraftingMachine) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category="IB Action")
	void GrantEffect();
	virtual void GrantEffect_Implementation();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GameplayEffect")
	float EffectLevel = 0.0f;
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContext;
};
