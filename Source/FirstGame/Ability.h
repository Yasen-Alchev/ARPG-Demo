// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAME_API UAbility : public UObject
{
	GENERATED_BODY()

	
public:
	UAbility();

	float fRange;
	float fDamage;

	UPROPERTY(EditAnywhere, Category = AbilityCooldown)
		float fCooldown;

	virtual void CastAbility(class AFirstGameCharacter* Player, class AEnemy* Enemy) {};
};
