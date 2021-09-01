// Fill out your copyright notice in the Description page of Project Settings.

#include "Staff.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Math/Vector.h"
#include "FirstGameCharacter.h"
#include "FirstGamePlayerController.h"

// Sets default values
AStaff::AStaff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fDamage = 20.f;
	fRange = 400.f;
	fSpeed = 200.f;
}

// Called when the game starts or when spawned
void AStaff::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStaff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStaff::Attack(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	if (IsValid(Arrow_Class))
	{
		UWorld* World = Player->GetWorld();
		if (IsValid(World))
		{
			FVector Location = Player->GetActorLocation() + Player->GetActorForwardVector() * 50.f;
			FRotator PlayerRotation = Player->GetActorRotation();
			FActorSpawnParameters SpawnInfo;

			ABullet* Projectile = World->SpawnActor<ABullet>(Arrow_Class, Location, PlayerRotation, SpawnInfo);
			if (IsValid(Projectile))
			{
				/*
				UMaterial* Material = Projectile->ProjectileMaterialInstance->GetMaterial();
				Projectile->ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material, NULL);
				Projectile->ProjectileMeshComponent->SetMaterial(0, Projectile->ProjectileMaterialInstance);
				Projectile->ProjectileMaterialInstance->SetVectorParameterValue(TEXT("BulletColor"), FLinearColor(128.f, 0.f, 128.f));
				*/

				Projectile->fDamage = fDamage;
				Projectile->FireInDirection(Player->GetActorForwardVector());
			}
		}
	}
}

