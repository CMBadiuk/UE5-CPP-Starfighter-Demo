// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Engine.h"
#include "StarfighterPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "CollisionLibrary.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StarfighterAttributeSet.h"
#include "TargetDummy.h"
#include "Blueprint/UserWidget.h"
#include "TrackableTarget.h"

// Sets default values
AStarfighterPawn::AStarfighterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StarfighterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StarfighterMesh"));
	RootComponent = StarfighterMesh;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
	FloatingPawnMovement->UpdatedComponent = RootComponent;

	// Set up default camera properties
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->TargetArmLength = 300.0f;

	// Set up default pawn properties
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UStarfighterAttributeSet>(TEXT("AttributeSet"));

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(StarfighterMesh);

}

// Called when the game starts or when spawned
void AStarfighterPawn::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent && MachineGunAbility)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MachineGunAbility.Get(), 1 , 0));
	}
	if (AbilitySystemComponent && MissileAbility)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MissileAbility.Get(), 1 , 1));
	}

	if (HUDWidgetClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			HUDWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
			}
		}
	}
	
}

// Called every frame
void AStarfighterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::Abs(ThrustAxis) != 0.0f)
	{
		float TargetThrust = ThrustAxis > 0.0f ? MaxForwardThrust : (ThrustAxis < 0.0f ? MaxBackwardThrust : 0.0f);
		CurrentThrust = FMath::FInterpTo(CurrentThrust, TargetThrust, DeltaTime, ThrustAccel);
	}
	
	AddMovementInput(GetActorForwardVector(), CurrentThrust);

	TArray<FHitResult> HitResults;
	const FVector Start = GetMuzzleLocation()->GetComponentLocation();
	const FRotator Direction = GetActorRotation();

	UCollisionLibrary::ConeTraceMulti(
		this,                   // World Context
		Start,
		Direction,
		25000.0f,                // Cone Height (Range)
		12.5f,                  // Cone Half Angle
		ETraceTypeQuery::TraceTypeQuery1, // Or your preferred trace channel
		false,
		{this},                 // Actors to Ignore (just self)
		EDrawDebugTrace::ForDuration, // Set to None when you're done debugging
		HitResults,
		FLinearColor::Blue,
		FLinearColor::Red,
		0.0f // Draw Time of 0 means it only draws for one frame
	);

	CurrentTarget = nullptr;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UTrackableTarget>())
		{
			CurrentTarget = HitActor;
			break;
		}
	}
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,            // The key used to update a message. Use -1 for a new message.
	// 	0.5f,          // How long the message will be on screen (in seconds).
	// 	FColor::Yellow,  // The color of the text.
	// 	FString::Printf(TEXT("Current Thrust: %f"), CurrentThrust) // The message to display.
	// );
}

// Called to bind functionality to input
void AStarfighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the Player Controller
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	// Get the Enhanced Input Subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	// Add the Input Mapping Context
	if (Subsystem && InputMappingContext)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// Bind the actions
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Triggered, this, &AStarfighterPawn::HandleThrust);
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Completed, this, &AStarfighterPawn::StopThrust);
		EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Triggered, this, &AStarfighterPawn::HandlePitch);
		EnhancedInputComponent->BindAction(YawAction, ETriggerEvent::Triggered, this, &AStarfighterPawn::HandleYaw);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AStarfighterPawn::HandleRoll);

		if (Fire1Action)
		{
			EnhancedInputComponent->BindAction(Fire1Action, ETriggerEvent::Triggered, AbilitySystemComponent, &UAbilitySystemComponent::AbilityLocalInputPressed, 0);
		}

		if (Fire2Action)
		{
			EnhancedInputComponent->BindAction(Fire2Action, ETriggerEvent::Triggered, AbilitySystemComponent, &UAbilitySystemComponent::AbilityLocalInputPressed, 1);
		}
	}

}

void AStarfighterPawn::HandleThrust(const FInputActionValue& Value)
{
	ThrustAxis = Value.Get<float>();
}

void AStarfighterPawn::StopThrust(const FInputActionValue& Value)
{
	ThrustAxis = 0.0f;
}


void AStarfighterPawn::HandlePitch(const FInputActionValue& Value)
{
	float PitchValue = Value.Get<float>();

	// Ensure there is some input before trying to rotate
	if (FMath::Abs(PitchValue) > 0.0f)
	{
		// Get DeltaTime to make rotation frame rate independent
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float RotationSpeed = 100.0f; 

		// Calculate the rotation amount
		float PitchAmount = PitchValue * RotationSpeed * DeltaTime;

		// Create a rotator with the roll amount and apply it to the actor's local space
		FRotator DeltaRotation(PitchAmount, 0.0f, 0.0f);
		AddActorLocalRotation(DeltaRotation);
	}
}

void AStarfighterPawn::HandleYaw(const FInputActionValue& Value)
{
	float YawValue = Value.Get<float>();

	// Ensure there is some input before trying to rotate
	if (FMath::Abs(YawValue) > 0.0f)
	{
		// Get DeltaTime to make rotation frame rate independent
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float RotationSpeed = 100.0f; 

		// Calculate the rotation amount
		float YawAmount = YawValue * RotationSpeed * DeltaTime;

		// Create a rotator with the roll amount and apply it to the actor's local space
		FRotator DeltaRotation(0.0f, YawAmount, 0.0f);
		AddActorLocalRotation(DeltaRotation);
	}
}

void AStarfighterPawn::HandleRoll(const FInputActionValue& Value)
{
	float RollValue = Value.Get<float>();

	// Ensure there is some input before trying to rotate
	if (FMath::Abs(RollValue) > 0.0f)
	{
		// Get DeltaTime to make rotation frame rate independent
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float RotationSpeed = 100.0f; 

		// Calculate the rotation amount
		float RollAmount = RollValue * RotationSpeed * DeltaTime;

		// Create a rotator with the roll amount and apply it to the actor's local space
		FRotator DeltaRotation(0.0f, 0.0f, RollAmount);
		AddActorLocalRotation(DeltaRotation);
	}
}

UAbilitySystemComponent* AStarfighterPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

