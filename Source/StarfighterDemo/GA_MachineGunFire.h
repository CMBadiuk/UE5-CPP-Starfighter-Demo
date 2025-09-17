// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_MachineGunFire.generated.h"

/**
 * 
 */
UCLASS()
class STARFIGHTERDEMO_API UGA_MachineGunFire : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MachineGunFire();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
		) override;

	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AActor> ProjectileClass;

	// UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	// TSubclassOf<class UGameplayEffect> CooldownGameplayEffectClass;
};
