// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "IBAttributeSet.generated.h"

/** Various macros for defining attributes without having to write out all functions */

/**
 * \brief Generic macro that adds the basic accessors for an attribute
 */
#define IBATTRIBUTE_ACCESSOR(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * \brief  Use this when the attribute should have a maximum value. The minimum is generally assumed to be 0.0f
 */
#define IBATTRIBUTE_ACCESSOR_WITHMAX(ClassName, PropertyName) \
IBATTRIBUTE_ACCESSOR(ClassName, PropertyName) \
IBATTRIBUTE_ACCESSOR(ClassName, PropertyName##_Max) 

/**
 * \brief  Use this when the attribute should have a minimum and maximum value
 */
#define IBATTRIBUTE_ACCESSOR_WITHMINANDMAX(ClassName, PropertyName) \
IBATTRIBUTE_ACCESSOR(ClassName, PropertyName) \
IBATTRIBUTE_ACCESSOR(ClassName, PropertyName##_Max) \
IBATTRIBUTE_ACCESSOR(ClassName, PropertyName##_Min)

/**
 *	Any attributes for the test should be declared here
 */
UCLASS()
class IBGAME_API UIBAttributeSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()
	
public:

protected:
	/** Called when a new IBAttribute value arrives over the network */
	UFUNCTION()
	virtual void OnRep_IBAttribute(const FGameplayAttributeData& OldIBAttribute);
	
public:
	/** IBAttribute attribute definitions */
	IBATTRIBUTE_ACCESSOR(UIBAttributeSet, IBAttribute);

protected:
	/** This stat is just an example and shouldn't actually be used for anything */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_IBAttribute)
	FGameplayAttributeData IBAttribute;
};
