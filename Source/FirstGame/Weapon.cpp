// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Enemy.h"
#include "FirstGameCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fRange = 100.f;
	fDamage = 40.f;
	fSpeed = 100.f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

