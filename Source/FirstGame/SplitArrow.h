// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "SplitArrow.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAME_API USplitArrow : public UAbility
{
	GENERATED_BODY()

	void CastAbility(AFirstGameCharacter* Player, AEnemy* Enemy) override;

public:
	USplitArrow();
};
