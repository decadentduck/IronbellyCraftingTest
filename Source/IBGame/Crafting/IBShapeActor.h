// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Interaction/IInteractableTarget.h"
#include "IBShapeActor.generated.h"

/**
 *	Actor that represents a shape in the world
 */
UCLASS(Blueprintable, BlueprintType)
class IBGAME_API AIBShapeActor : public AStaticMeshActor, public IInteractableTarget
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Pick up when interacted with */
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;

protected:
	/** Interaction settings */
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;
};
