// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Enemy.h"
#include "FirstGameCharacter.h"


// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fDamage = 50.f;
	fRange = 100.f;
	fSpeed = 100.f;
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASword::Attack(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	Enemy->TakeDamage(fDamage, FDamageEvent(UDamageType::StaticClass()), Player->GetInstigatorController(), Player);
}