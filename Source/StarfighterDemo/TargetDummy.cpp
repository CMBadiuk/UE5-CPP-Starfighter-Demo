// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDummy.h"
#include "AbilitySystemComponent.h"
#include "StarfighterAttributeSet.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UStarfighterAttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void ATargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

class UAbilitySystemComponent* ATargetDummy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
