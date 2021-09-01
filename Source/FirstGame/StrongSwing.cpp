// Fill out your copyright notice in the Description page of Project Settings.


#include "StrongSwing.h"

#include "Enemy.h"
#include "FirstGameCharacter.h"

UStrongSwing::UStrongSwing()
{
	fRange = 100.f;
	fDamage = 200.f;
	fCooldown = 2.f;
}

void UStrongSwing::CastAbility(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	Enemy->TakeDamage(fDamage, FDamageEvent(UDamageType::StaticClass()), Player->GetInstigatorController(), Player);
}