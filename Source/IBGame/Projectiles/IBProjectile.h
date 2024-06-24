// Copyright Ironbelly Studios, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "IBProjectile.generated.h"

/**
 *	Generic actor with a projectile movement component and handles for generic projectile weapons
 */
UCLASS(Blueprintable, BlueprintType)
class IBGAME_API AIBProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Initialize the projectile */
	virtual void BeginPlay() override;

	/** Either play a bounce effect, or blow up on contact */
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	/** Called when the projectile explodes to destroy it on server */
	UFUNCTION()
	void DestroyProjectile(UNiagaraComponent* PS);

	/** Called shortly after BeginPlay to set up full collision */
	UFUNCTION()
	void PostLaunch();

	/** Called when the timer ends to explode the projectile */
	UFUNCTION()
	void Explode();

	/** Delay the destroy so the TornOff message can be replicated */
	UFUNCTION()
	void DelayedExplode();
	
	/** Setup the trail effect */
	void SetupVFX();

	/** Called */
	virtual void TornOff() override;
	
protected:
	/** Component that handles the movement logic */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	
	/** Static mesh representing the projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh = nullptr;

	/** Gameplay cue to play on explosion */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ExplosionCueTag;

	/** Explosion damage radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionRadius = 450.0f;

	/** Lifetime of the projectile before it explodes automatically */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionTime = 1.5f;

	/** Damage trace channel to use */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECC_Visibility;

	/** Damage effect to apply */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageGE = nullptr;

	/** Optional effect to use as a trail */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* GrenadeTrail = nullptr;
	
	/** The particle representing the trail */
	UPROPERTY()
	UNiagaraComponent* NSTrail = nullptr;

	/** Timers */
	FTimerHandle ExplosionTimer;
	FTimerHandle PostLaunchTimer;
	FTimerHandle DelayedDestroyTimer;

	/** Track how many times this projectile has bounced */
	int32 NumBounces = 0;

	/** If this projectile bounces, this is the number of bounces before exploding */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BouncesBeforeExploding = 3;
};
