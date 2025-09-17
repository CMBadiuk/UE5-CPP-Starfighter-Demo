// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MissileLauncher.h"
#include "HomingMissileProjectile.h"
#include "CollisionLibrary.h"
#include "MeshPaintVisualize.h"
#include "StarfighterPawn.h"
#include "DrawDebugHelpers.h" // Add this include for debug drawing
#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

UGA_MissileLauncher::UGA_MissileLauncher()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MissileLauncher::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AStarfighterPawn* Starfighter = Cast<AStarfighterPawn>(ActorInfo->AvatarActor.Get());
	if (!Starfighter || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* TargetActor = Starfighter->GetCurrentTarget();

	FTransform SpawnTransform = Starfighter->GetMuzzleLocation()->GetComponentTransform();
	AHomingMissileProjectile* Missile = GetWorld()->SpawnActor<AHomingMissileProjectile>(ProjectileClass, SpawnTransform);

	if (Missile)
	{
		if (TargetActor)
		{
			Missile->HomingTarget = TargetActor;
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

UGameplayEffect* UGA_MissileLauncher::GetCooldownGameplayEffect() const
{
	return CooldownGameplayEffectClass
	? CooldownGameplayEffectClass->GetDefaultObject<UGameplayEffect>()
	: nullptr;
}