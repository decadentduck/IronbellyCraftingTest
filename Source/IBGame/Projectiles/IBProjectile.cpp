// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#include "IBProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

AIBProjectile::AIBProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetRelativeLocation(FVector::ZeroVector);
	RootComponent = Mesh;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));

	ProjectileMovement->InitialSpeed = 2500.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->Friction = 0.8f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 25.0f;
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bInterpRotation = true;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1.0f, 0.0f, 0.15f));

	bReplicates = true;
}

void AIBProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Character = Cast<ACharacter>(GetInstigator()); Character != nullptr)
	{
		Character->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);

		Mesh->IgnoreActorWhenMoving(Character, true);
	}

	SetupVFX();

	if (GetLocalRole() == ROLE_Authority)
	{
		// Set up the explosion timer
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &AIBProjectile::Explode, ExplosionTime, false);
	}
	
	// Set up a short timer to re-enable collision and start rotating the projectile
	GetWorld()->GetTimerManager().SetTimer(PostLaunchTimer, this, &AIBProjectile::PostLaunch, 0.16f, false);
}

void AIBProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if ((BouncesBeforeExploding > 0 && NumBounces >= BouncesBeforeExploding) ||
			ProjectileMovement->bShouldBounce == false)
		{
			Explode();
		}
	}

	NumBounces++;
}

void AIBProjectile::SetupVFX()
{
	if (NSTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(GrenadeTrail, Mesh, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
		EAttachLocation::Type::KeepRelativeOffset, false, true, ENCPoolMethod::None, true);
		NSTrail != nullptr)
	{
		
	}
}

void AIBProjectile::TornOff()
{
	Super::TornOff();
	Explode();
}

void AIBProjectile::Explode()
{
	if (GetLocalRole() == ROLE_Authority && GetNetMode() != NM_Standalone)
	{
		TearOff();
	}
	
	if (ExplosionCueTag.IsValid())
	{
		FGameplayCueParameters Parameters;
		Parameters.Location = GetActorLocation();
		UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(this, ExplosionCueTag, Parameters);
	}

	Mesh->SetHiddenInGame(true, false);
	ProjectileMovement->Deactivate();

	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> Actors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius, {}, ACharacter::StaticClass(), {}, Actors);

		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
			AbilitySystemComponent != nullptr)
		{
			FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();

			for (AActor* Actor : Actors)
			{
				if (UAbilitySystemComponent* DamagedAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
					DamagedAbilitySystemComponent != nullptr)
				{
					TArray<AActor*> IgnoreActors = Actors;
					IgnoreActors.Remove(Actor);

					// Check visibility
					FCollisionQueryParams Params;
					Params.bTraceComplex = true;
					Params.AddIgnoredActors(IgnoreActors);
					Params.AddIgnoredActor(this);
				
					FHitResult Hit;
					if (DamageGE != nullptr && GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Actor->GetActorLocation(), DamageTraceChannel, Params))
					{
						Context.AddHitResult(Hit, true);
						float Level = FMath::Clamp(Hit.Distance / ExplosionRadius, 0.0f, 1.0f);
						AbilitySystemComponent->ApplyGameplayEffectToTarget(DamageGE->GetDefaultObject<UGameplayEffect>(), DamagedAbilitySystemComponent, Level, Context);
					}
				}
			}
		}

		// Wait for the trail vfx to finish before destroying
		if (NSTrail != nullptr)
		{
			NSTrail->OnSystemFinished.AddDynamic(this, &AIBProjectile::DestroyProjectile);
		}
		else
		{
			DestroyProjectile(nullptr);
		}
	}
}

void AIBProjectile::DestroyProjectile(UNiagaraComponent* PS)
{
	// Wait at least a frame so things can be replicated to clients
	GetWorld()->GetTimerManager().SetTimer(DelayedDestroyTimer, this, &AIBProjectile::DelayedExplode, 0.1f, false);
}

void AIBProjectile::DelayedExplode()
{
	Destroy();
}

void AIBProjectile::PostLaunch()
{
	if (ACharacter* Character = Cast<ACharacter>(GetInstigator()); Character != nullptr)
	{
		Character->GetCapsuleComponent()->IgnoreActorWhenMoving(this, false);

		Mesh->IgnoreActorWhenMoving(Character, false);
	}

	ProjectileMovement->bRotationFollowsVelocity = true;
}