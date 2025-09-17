// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "StarfighterPawn.generated.h"


// Forward Declarations
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class STARFIGHTERDEMO_API AStarfighterPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStarfighterPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// GET FUNCTIONS
	USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }

	AActor* GetCurrentTarget() const { return CurrentTarget; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* HUDWidget;

private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StarfighterMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	// INPUT VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrustAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* YawAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Fire1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Fire2Action;

	// THRUST VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrust", meta = (AllowPrivateAccess = "true"))
	float CurrentThrust;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrust", meta = (AllowPrivateAccess = "true"))
	float MaxForwardThrust = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrust", meta = (AllowPrivateAccess = "true"))
	float MaxBackwardThrust = -2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrust", meta = (AllowPrivateAccess = "true"))
	float ThrustAccel = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrust", meta = (AllowPrivateAccess = "true"))
	float ThrustAxis = 0.0f;

	// GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UStarfighterAttributeSet* AttributeSet;

	// ABILITIES
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> MachineGunAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> MissileAbility;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Targeting", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentTarget;
	
	// INPUT FUNCTIONS
	void HandleThrust(const FInputActionValue& Value);
	void StopThrust(const FInputActionValue& Value);
	void HandlePitch(const FInputActionValue& Value);
	void HandleYaw(const FInputActionValue& Value);
	void HandleRoll(const FInputActionValue& Value);

};
