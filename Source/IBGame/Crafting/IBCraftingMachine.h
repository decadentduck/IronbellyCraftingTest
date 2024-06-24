// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "IBCrafingCommon.h"
#include "IBCraftingMachine.generated.h"

enum class ETriggerEvent : uint8;
struct FInputActionInstance;
struct FInputActionValue;
class AIBShapeActor;
class USphereComponent;

/**
 *	Simple actor that detects shapes nearby and attempts to convert them into other shapes via a recipe table
 */
UCLASS(Blueprintable, BlueprintType)
class IBGAME_API AIBCraftingMachine : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Initialize sphere overlap events */
	virtual void BeginPlay() override;

	/**
	 * \brief Determines if the ingredients exist for a recipe
	 *
	 * \param Recipe The recipe to check
	 *
	 * \return True if the ingredients are present
	 */
	bool CanCraftRecipe(const FIBCraftingRecipe& Recipe) const;

	/**
	 * \brief Turn the machine on or off (server only)
	 *
	 * \param IsActive True if the machine should be turned on, false if turned off
	 */
	UFUNCTION(BlueprintCallable)
	void SetMachineState(bool IsActive);
	
	/** Location to spawn crafting results */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* CraftResultLocation = nullptr;
	
	/**
	 * \brief Adds a shape to be considered for crafting. Will attempt to craft recipes after added
	 * 
	 * \param Shape The new shape to consider
	 */
	void AddShapeOverlap(AIBShapeActor* Shape);
	
	/** Collision sphere that detects shapes that enter its radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Sphere = nullptr;
	
protected:
	/** Event called when an actor overlaps our collision sphere */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	/** Event called when an overlap ends */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * \brief Removes a shape from the ingredients list
	 * 
	 * \param Shape The shape to remove
	 */
	void RemoveShapeOverlap(AIBShapeActor* Shape);

	/** Called whenever the ingredients add a new item to try and craft something automatically */
	void AttemptToCraftRecipes();

	/** Called when the shapes in range change to populate a map of ingredient amounts */
	void UpdateIngredients();

	/**
	 * \brief Takes in a recipe to craft and removes the ingredients
	 *
	 * \param Recipe The recipe to craft
	 */
	void PerformAction(const FIBCraftingRecipe& Recipe);

	/**
	 * \brief Destroy shapes when they are crafted into something else
	 * 
	 * \param ShapeClass The type of shape to destroy
	 * \param Amount The amount to destroy
	 */
	void DestroyShapeActors(TSubclassOf<AIBShapeActor> ShapeClass, int32 Amount);

	/** Called when IsMachineActive has changed */
	UFUNCTION()
	void OnRep_IsMachineActive();

	/** Event called when the machine is turned on */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMachineTurnedOn();

	/** Event called when the machine is turned off */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMachineTurnedOff();
	
protected:
	/** Root component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Scene = nullptr;

	
	/** Static mesh to represent the machine in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh = nullptr;

	/** List of all data table recipes possible at this machine */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/IBGame.IBCraftingRecipe"))
	TArray<UDataTable*> RecipeDataTables;

	/** List of shapes in range that should be considered for crafting */
	UPROPERTY()
	TArray<AIBShapeActor*> CurrentShapes;

	/** Current map of shapes and their amount in range. Refreshed every time a shape is added or removed */
	TMap<TSubclassOf<AIBShapeActor>, int32> Ingredients;

	/** Flag that is true while the craft is in progress. This prevents new recipe calculations from occurring when removing items */
	bool IsCrafting = false;

	/** Flag that determines if the machine is on or off */
	UPROPERTY(ReplicatedUsing=OnRep_IsMachineActive, BlueprintReadOnly)
	bool IsMachineActive = false;

	/** If true, the machine will activate on startup */
	UPROPERTY(EditAnywhere)
	bool ShouldMachineStartActive = true;

	UPROPERTY()
	TArray<UInputAction*> PlayerActions;

	UFUNCTION()
	void OnPlayerActionTriggered(const FInputActionInstance& InputActionInstance);
	
	UPROPERTY()
	TMap<FName, int32> ActionsDone;
};
