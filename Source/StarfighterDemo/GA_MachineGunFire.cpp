
// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MachineGunFire.h"
#include "StarfighterPawn.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

UGA_MachineGunFire::UGA_MachineGunFire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MachineGunFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (ProjectileClass)
	{
		AStarfighterPawn* Starfighter = Cast<AStarfighterPawn>(ActorInfo->AvatarActor.Get());
		if (Starfighter && Starfighter->GetMuzzleLocation())
		{
			FTransform SpawnTransform = Starfighter->GetMuzzleLocation()->GetComponentTransform();
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Machine Gun Ability Activated!"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

UGameplayEffect* UGA_MachineGunFire::GetCooldownGameplayEffect() const
{
	return CooldownGameplayEffectClass
	? CooldownGameplayEffectClass->GetDefaultObject<UGameplayEffect>()
	: nullptr;
}
