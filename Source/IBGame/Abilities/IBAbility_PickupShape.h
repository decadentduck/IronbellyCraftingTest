// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "IBAbility_PickupShape.generated.h"

class AIBShapeActor;

/**
 *	Ability called when interacting with a shape
 */
UCLASS(Blueprintable, BlueprintType)
class IBGAME_API UIBAbility_PickupShape : public ULyraGameplayAbility
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Attaches or detaches an item */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Called to drop the item */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
protected:
	/** Cached shape we are holding */
	UPROPERTY()
	AIBShapeActor* Shape = nullptr;
};
