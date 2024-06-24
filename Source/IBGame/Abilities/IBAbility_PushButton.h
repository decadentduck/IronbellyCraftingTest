// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "IBAbility_PushButton.generated.h"

/**
 *	Ability that detects nearby interactables and sends an interaction message to them
 */
UCLASS(Blueprintable, BlueprintType)
class IBGAME_API UIBAbility_PushButton : public ULyraGameplayAbility
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** Cached reference to the actor being held */
	UPROPERTY()
	AActor* HeldActor = nullptr;
};
