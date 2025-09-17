// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissileProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AHomingMissileProjectile::AHomingMissileProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 20000.0f;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 8000.0f;
}

// Called when the game starts or when spawned
void AHomingMissileProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HomingTarget != nullptr)
	{
		ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
	}
	else
	{
		ProjectileMovementComponent->bIsHomingProjectile = false;
	}
}

// Called every frame
void AHomingMissileProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

