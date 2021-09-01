// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "StrongSwing.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAME_API UStrongSwing : public UAbility
{
	GENERATED_BODY()

public:
	UStrongSwing();

	void CastAbility(AFirstGameCharacter* Player, AEnemy* Enemy) override;

};
