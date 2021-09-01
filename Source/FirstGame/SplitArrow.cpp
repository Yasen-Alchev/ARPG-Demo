// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitArrow.h"
#include "Bullet.h"
#include "FirstGameCharacter.h"
#include "FirstGamePlayerController.h"

USplitArrow::USplitArrow()
{
	fRange = 400.f;
	fDamage = 50.f;
	fCooldown = 3.f;
}

void USplitArrow::CastAbility(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	UWorld* World = Player->GetWorld();
	if (IsValid(World))
	{

		FVector Location = Player->GetActorLocation() + Player->GetActorForwardVector() * 50.f;
		FRotator PlayerRotation = Player->GetActorRotation();
		FVector Location2 = Location + PlayerRotation.RotateVector(FVector(0, -50, 0));
		FVector Location3 = Location + PlayerRotation.RotateVector(FVector(0, 50, 0));
		FActorSpawnParameters SpawnInfo;

		ABullet* Projectile = World->SpawnActor<ABullet>(Location, PlayerRotation, SpawnInfo);
		ABullet* Projectile2 = World->SpawnActor<ABullet>(Location2, PlayerRotation, SpawnInfo);
		ABullet* Projectile3 = World->SpawnActor<ABullet>(Location3, PlayerRotation, SpawnInfo);


		if (IsValid(Projectile))
		{
			Projectile->fDamage = fDamage;
		}
		if (IsValid(Projectile2))
		{
			Projectile2->fDamage = fDamage;
		}
		if (IsValid(Projectile3))
		{
			Projectile3->fDamage = fDamage;
		}
	}
}
