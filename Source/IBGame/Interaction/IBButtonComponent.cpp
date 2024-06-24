// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBButtonComponent.h"

void UIBButtonComponent::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}
