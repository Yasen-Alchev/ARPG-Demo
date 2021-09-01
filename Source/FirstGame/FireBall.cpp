// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "FireBall.h"
#include "Containers/Array.h"
#include "FirstGameCharacter.h"
#include "FirstGamePlayerController.h"

UFireBall::UFireBall()
{
	fRange = 400.f;
	fDamage = 50.f;
	fCooldown = 1.f;
}

void UFireBall::CastAbility(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	UWorld* World = Player->GetWorld();
	if (IsValid(World))
	{
		FVector Location = Player->GetActorLocation() + Player->GetActorForwardVector() * 50.f;
		FRotator PlayerRotation = Player->GetActorRotation();
		FActorSpawnParameters SpawnInfo;

		ABullet* Projectile = World->SpawnActor<ABullet>(Location, PlayerRotation, SpawnInfo);
		
		if (IsValid(Projectile))
		{
			UMaterial* Material = Projectile->ProjectileMaterialInstance->GetMaterial();
			Projectile->ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material, NULL);
			Projectile->ProjectileMeshComponent->SetMaterial(0, Projectile->ProjectileMaterialInstance);
			Projectile->ProjectileMaterialInstance->SetVectorParameterValue(TEXT("BulletColor"), FLinearColor(100.f, 0, 0));

			Projectile->fDamage = fDamage;
			Projectile->Tags.Add("FireBall");
		}
	}
}
