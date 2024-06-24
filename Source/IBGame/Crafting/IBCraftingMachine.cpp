// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBCraftingMachine.h"

#include "EnhancedInputComponent.h"
#include "IBCrafingCommon.h"
#include "IBShapeActor.h"
#include "UnrealNetwork.h"
#include "Components/SphereComponent.h"

AIBCraftingMachine::AIBCraftingMachine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	RootComponent = Scene;

	CraftResultLocation = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ResultLocation"));
	CraftResultLocation->SetupAttachment(Scene);
	CraftResultLocation->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

	Sphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere"));
	Sphere->SetupAttachment(Scene);
	Sphere->SetSphereRadius(350.0f);

	bReplicates = true;
}

void AIBCraftingMachine::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AIBCraftingMachine::OnOverlapBegin);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &AIBCraftingMachine::OnEndOverlap);

		if (ShouldMachineStartActive)
		{
			SetMachineState(true);
		}
	}

	//Get All Player Actions we need to listen for
	FString ContextString = "Crafting Machine Begin Play";
	TArray<FIBCraftingRecipe*> Recipes;
	
	for(UDataTable* RecipeTable: RecipeDataTables)
	{
		RecipeTable->GetAllRows(ContextString, Recipes);
		
		for(const FIBCraftingRecipe* Recipe : Recipes)
		{
			for(const FIBItemAmount &Condition : Recipe->Conditions)
			{
				if( FIBCraftingPlayerConditions* PlayerCondition =
					Condition.Item.DataTable->FindRow<FIBCraftingPlayerConditions>(Condition.Item.RowName, ContextString))
				{
					PlayerActions.AddUnique(PlayerCondition->InputAction);
				}
			}
		}
	}
}

void AIBCraftingMachine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (AIBShapeActor* Shape = Cast<AIBShapeActor>(OtherActor); Shape != nullptr)
	{
		AddShapeOverlap(Shape);
	}
	else if(APawn* Player = Cast<APawn>(OtherActor))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Player->GetComponentByClass(UEnhancedInputComponent::StaticClass())))
		{
			for(UInputAction* Action : PlayerActions)
			{
				EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, &AIBCraftingMachine::OnPlayerActionTriggered);
			}
		}
	}
}

void AIBCraftingMachine::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AIBShapeActor* Shape = Cast<AIBShapeActor>(OtherActor); Shape != nullptr)
	{
		RemoveShapeOverlap(Shape);
	}
	if(APawn* Player = Cast<APawn>(OtherActor))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Player->GetComponentByClass(UEnhancedInputComponent::StaticClass())))
		{
			EnhancedInputComponent->ClearBindingsForObject(this);
		}
	}
}

void AIBCraftingMachine::AddShapeOverlap(AIBShapeActor* Shape)
{
	CurrentShapes.Add(Shape);

	if (IsCrafting == false)
	{
		// Update and cache our current ingredients list
		UpdateIngredients();

		// Check for a valid recipe and craft it if needed
		AttemptToCraftRecipes();
	}
}

void AIBCraftingMachine::RemoveShapeOverlap(AIBShapeActor* Shape)
{
	CurrentShapes.Remove(Shape);

	if (IsCrafting == false)
	{
		// Update and cache our current ingredients list
		UpdateIngredients();
	}
}

void AIBCraftingMachine::UpdateIngredients()
{
	Ingredients.Empty();
	
	for (const AIBShapeActor* Shape : CurrentShapes)
	{
		if (IsValid(Shape))
		{
			int32& Amount = Ingredients.FindOrAdd(Shape->GetClass());
			Amount++;
		}
	}
}

void AIBCraftingMachine::AttemptToCraftRecipes()
{
	// Early out if we have no ingredients
	if (Ingredients.Num() == 0 && ActionsDone.Num() == 0)
	{
		return;
	}
	
	for (const UDataTable* RecipeDataTable : RecipeDataTables)
	{
		if (IsValid(RecipeDataTable))
		{
			TArray<FIBCraftingRecipe*> Recipes;
			RecipeDataTable->GetAllRows<FIBCraftingRecipe>(TEXT("AttemptToCraftRecipes"), Recipes);

			for (const FIBCraftingRecipe* Recipe : Recipes)
			{
				if (CanCraftRecipe(*Recipe))
				{
					PerformAction(*Recipe);
					return;
				}
			}
		}
	}
}

bool AIBCraftingMachine::CanCraftRecipe(const FIBCraftingRecipe& Recipe) const
{
	for (const FIBItemAmount& Ingredient : Recipe.Conditions)
	{
		if(FIBCraftingPlayerConditions* CraftingAction = Ingredient.Item.GetRow<FIBCraftingPlayerConditions>(TEXT("CanCraftRecipe")))
		{
			const int32* Amount = ActionsDone.Find(CraftingAction->InputAction->GetFName());
			
			// Invalid ingredient
			if (Amount == nullptr)
			{
				return false;
			}

			// Not enough shapes, recipe fails
			if (*Amount < Ingredient.Amount)
			{
				return false;
			}
		}
		else if (const FIBCraftingShape* CraftingShape = Ingredient.Item.GetRow<FIBCraftingShape>(TEXT("CanCraftRecipe"));
			CraftingShape != nullptr)
		{
			const int32* Amount = Ingredients.Find(CraftingShape->ShapeClass);
			
			// Invalid ingredient
			if (Amount == nullptr)
			{
				return false;
			}

			// Not enough shapes, recipe fails
			if (*Amount < Ingredient.Amount)
			{
				return false;
			}
		}
		
	}

	return true;
}

void AIBCraftingMachine::PerformAction(const FIBCraftingRecipe& Recipe)
{
	if (IsMachineActive == false)
	{
		return;
	}
	
	IsCrafting = true;

	// Remove ingredients
	for (const FIBItemAmount& Ingredient : Recipe.Conditions)
	{
		if(const FIBCraftingPlayerConditions* Action = Ingredient.Item.GetRow<FIBCraftingPlayerConditions>(TEXT("CraftRecipe")))
		{
			ActionsDone[Action->InputAction->GetFName()] -= Ingredient.Amount;
		}
		else if (const FIBCraftingShape* CraftingShape = Ingredient.Item.GetRow<FIBCraftingShape>(TEXT("CraftRecipe"));
			CraftingShape != nullptr)
		{
			DestroyShapeActors(CraftingShape->ShapeClass, Ingredient.Amount);
		}
	}

	// Spawn results
	for (TSubclassOf<UIBMachineAction> ActionClass : Recipe.Actions)
	{
		UIBMachineAction* Action = NewObject<UIBMachineAction>(this, ActionClass);
		Action->PerformAction(this);
	}

	IsCrafting = false;
	
	UpdateIngredients();
	
	// Try to craft another recipe right away
	AttemptToCraftRecipes();
}

void AIBCraftingMachine::DestroyShapeActors(TSubclassOf<AIBShapeActor> ShapeClass, int32 Amount)
{
	// Generate a list of actors of this type
	TArray<AIBShapeActor*> ShapesToConsider;
	for (AIBShapeActor* Shape : CurrentShapes)
	{
		if (IsValid(Shape))
		{
			if (Shape->IsA(ShapeClass))
			{
				ShapesToConsider.Add(Shape);
			}
		}
	}
	
	for (int32 i = 0; i < Amount && i < ShapesToConsider.Num(); i++)
	{
		if (AIBShapeActor* ShapeToDestroy = ShapesToConsider[i]; ShapeToDestroy != nullptr)
		{
			CurrentShapes.Remove(ShapeToDestroy);
			ShapeToDestroy->Destroy();
		}
	}
}

void AIBCraftingMachine::SetMachineState(bool IsActive)
{
	if (IsActive != IsMachineActive)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(AIBCraftingMachine, IsMachineActive, this);
		IsMachineActive = IsActive;

		OnRep_IsMachineActive();

		// Update our crafting status
		if (IsMachineActive)
		{
			UpdateIngredients();
			AttemptToCraftRecipes();
		}
	}
}

void AIBCraftingMachine::OnRep_IsMachineActive()
{
	if (IsMachineActive)
	{
		OnMachineTurnedOn();
	}
	else
	{
		OnMachineTurnedOff();
	}
}

void AIBCraftingMachine::OnPlayerActionTriggered(const FInputActionInstance& InputActionInstance)
{
	ActionsDone.FindOrAdd(InputActionInstance.GetSourceAction()->GetFName())++;
	AttemptToCraftRecipes();
}

void AIBCraftingMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AIBCraftingMachine, IsMachineActive, SharedParams);
}

