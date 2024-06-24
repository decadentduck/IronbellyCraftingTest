// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "Interaction/IInteractableTarget.h"
#include "IBButtonComponent.generated.h"

/** Called when the button is pressed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIBOnButtonPressed);

/**
 *	Component that can be added to actors to give them interactable buttons
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class IBGAME_API UIBButtonComponent : public UStaticMeshComponent, public IInteractableTarget
{
	GENERATED_BODY()

public:
	/** Button has been interacted with */
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;

public:
	/** Event called when the button is pressed */
	UPROPERTY(BlueprintAssignable)
	FIBOnButtonPressed OnButtonPressed;

protected:
	/** Interaction settings */
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;
};
