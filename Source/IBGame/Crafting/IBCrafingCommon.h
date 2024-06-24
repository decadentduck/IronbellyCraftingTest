// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once
#include "Actions/IBMachineAction.h"

#include "IBCrafingCommon.generated.h"

class UInputAction;
class AIBShapeActor;

/**
 *	Helper for matching an item lookup with an amount
 */
USTRUCT(Blueprintable, BlueprintType)
struct IBGAME_API FIBItemAmount
{
	GENERATED_BODY()

public:
	/** Item reference */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = "/Script/IBGame.IBCraftingCondition"))
	FDataTableRowHandle Item;

	/** Item amount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Amount = 1;
};

/**
 *	Data table entry for a recipe
 */
USTRUCT(Blueprintable, BlueprintType)
struct IBGAME_API FIBCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** List of ingredients required */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FIBItemAmount> Conditions;

	/** List of Actions caused by the recipe */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TSubclassOf<UIBMachineAction>> Actions;
};

/**
 *	Data table entry for a crafting shape
 */
USTRUCT(Blueprintable, BlueprintType)
struct IBGAME_API FIBCraftingCondition : public FTableRowBase
{
	GENERATED_BODY()

public:
	
};

/**
 *	Data table entry for a crafting shape
 */
USTRUCT(Blueprintable, BlueprintType)
struct IBGAME_API FIBCraftingShape : public FIBCraftingCondition
{
	GENERATED_BODY()

public:
	/** Actor class of the shape */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AIBShapeActor> ShapeClass = nullptr;
};

/**
 *	Data table entry for a crafting shape
 */
USTRUCT(Blueprintable, BlueprintType)
struct IBGAME_API FIBCraftingPlayerConditions : public FIBCraftingCondition
{
	GENERATED_BODY()

public:
	/** Actor class of the shape */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* InputAction = nullptr;
};