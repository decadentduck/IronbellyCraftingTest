// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBAbility_PickupShape.h"
#include "NativeGameplayTags.h"
#include "Crafting/IBShapeActor.h"
#include "GameFramework/Character.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(IB_Action_Pickup, "Ability.Type.Action.Pickup");

UIBAbility_PickupShape::UIBAbility_PickupShape(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityTags.AddTag(IB_Action_Pickup);
}

void UIBAbility_PickupShape::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(TriggerEventData->Instigator); Character != nullptr)
	{
		if (Shape = Cast<AIBShapeActor>(TriggerEventData->Target); Shape != nullptr)
		{
			Shape->GetStaticMeshComponent()->SetSimulatePhysics(false);
			Shape->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Shape->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Pickup"));
		}
	}
}

void UIBAbility_PickupShape::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (Shape != nullptr)
	{
		Shape->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		Shape->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

		if (IsValid(Shape))
		{
			Shape->GetStaticMeshComponent()->SetSimulatePhysics(true);

			// Give the dropped actor a little push
			if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor); Character != nullptr)
			{
				Shape->GetStaticMeshComponent()->AddImpulse(Character->GetActorRotation().Vector() * 300.0f, NAME_None, true);
			}
		}

		Shape = nullptr;
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

