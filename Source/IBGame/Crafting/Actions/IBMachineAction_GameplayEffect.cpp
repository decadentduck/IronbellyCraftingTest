// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Actions/IBMachineAction_GameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Crafting/IBCraftingMachine.h"

void UIBMachineAction_GameplayEffect::PerformAction_Implementation(AIBCraftingMachine* CraftingMachine)
{
	Super::PerformAction_Implementation(CraftingMachine);

	TArray<AActor*> Actors;
	CraftingMachineRef->Sphere->GetOverlappingActors(Actors);

	for(AActor* Actor : Actors)
	{
		if(APawn* Pawn = Cast<APawn>(Actor))
			Players.Add(Pawn);
	}

	GrantEffect();
}

void UIBMachineAction_GameplayEffect::GrantEffect_Implementation()
{
	for (APawn* Pawn : Players)
	{
		//I intended to use the ability system however I don't want to mess with the pawn so I'll just override this function in BP to do something different...
		if(UAbilitySystemComponent* AbilitySystemComp = Cast<UAbilitySystemComponent>(Pawn->GetComponentByClass(UAbilitySystemComponent::StaticClass())))
			AbilitySystemComp->BP_ApplyGameplayEffectToSelf(GameplayEffect, EffectLevel, EffectContext);
	}
}
