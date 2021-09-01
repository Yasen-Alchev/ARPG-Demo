// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "Enemy.h"
#include "FirstGameCharacter.h"

UAbility::UAbility()
{
	fRange = 400.f;
	fDamage = 50.f;
	fCooldown = 2.0f;
}
