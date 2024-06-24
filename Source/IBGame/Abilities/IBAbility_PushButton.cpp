// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBAbility_PushButton.h"
#include "Interaction/IBButtonComponent.h"

UIBAbility_PushButton::UIBAbility_PushButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UIBAbility_PushButton::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const UIBButtonComponent* ButtonComponent = Cast<UIBButtonComponent>(TriggerEventData->OptionalObject.Get()); ButtonComponent != nullptr)
	{
		ButtonComponent->OnButtonPressed.Broadcast();
	}

	K2_EndAbility();
}