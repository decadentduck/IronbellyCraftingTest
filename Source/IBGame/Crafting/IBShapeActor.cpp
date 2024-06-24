// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBShapeActor.h"
#include "GameFramework/Character.h"

AIBShapeActor::AIBShapeActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetSimulatePhysics(true);

	GetStaticMeshComponent()->SetCollisionProfileName("PhysicsActor", true);

	bReplicates = true;
}

void AIBShapeActor::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}
