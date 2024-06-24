// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBAttributeSet.h"
#include "UnrealNetwork.h"

UIBAttributeSet::UIBAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UIBAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UIBAttributeSet, IBAttribute, COND_OwnerOnly, REPNOTIFY_Always);
}

void UIBAttributeSet::OnRep_IBAttribute(const FGameplayAttributeData& OldIBAttribute)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UIBAttributeSet, IBAttribute, OldIBAttribute);
}
