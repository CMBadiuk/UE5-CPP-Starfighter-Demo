// StarfighterAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "StarfighterAttributeSet.generated.h"

// This is a helper macro to add required boilerplate functions for attributes.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class STARFIGHTERDEMO_API UStarfighterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Declare a Health attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UStarfighterAttributeSet, Health);

	// Declare a MaxHealth attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UStarfighterAttributeSet, MaxHealth);

	// Declare an Energy attribute (for weapons)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UStarfighterAttributeSet, Energy);
    
	// Declare a MaxEnergy attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UStarfighterAttributeSet, MaxEnergy);

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};