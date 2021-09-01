// Fill out your copyright notice in the Description page of Project Settings.
#include "Gun.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Math/Vector.h"
#include "DrawDebugHelpers.h"
#include "FirstGameCharacter.h"
#include "FirstGamePlayerController.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fDamage = 20.f;
	fRange = 400.f;
	fSpeed = 200.f;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Attack(AFirstGameCharacter* Player, AEnemy* Enemy)
{
	/*
	if (IsValid(Arrow_Class))
	{
		UWorld* World = Player->GetWorld();
		if(IsValid(World))
		{
			FVector Location = Player->GetActorLocation() + Player->GetActorForwardVector() * 50.f;
			FRotator PlayerRotation = Player->GetActorRotation();
			FActorSpawnParameters SpawnInfo;


			AFirstGamePlayerController* PController = Cast<AFirstGamePlayerController>(Player->GetInstigatorController());
			if (IsValid(PController))
			{
				if (Player->bAbilityActivated)
				{
					Player->bAbilityActivated = false;
					//World->GetTimerManager().ClearTimer(PController->AbilityHandle);
					//Vector Location2 = Player->GetActorLocation() + (Player->GetActorForwardVector() + FVector(-1, 0, 0)) * 50.f;
					//FVector Location3 = Player->GetActorLocation() + (Player->GetActorForwardVector() + FVector(1, 0, 0)) * 50.f;

					FVector Location2 = Location + PlayerRotation.RotateVector(FVector(0, -50, 0));
					FVector Location3 = Location + PlayerRotation.RotateVector(FVector(0, 50, 0));
					
					if (World)
					{
						DrawDebugLine(World, Player->GetActorLocation(), Location, FColor::Red, false, 2, 10, 10);
						DrawDebugLine(World, Player->GetActorLocation(), Location2, FColor::Blue, false, 2, 10, 10);
						DrawDebugLine(World, Player->GetActorLocation(), Location3, FColor::Cyan, false, 2, 10, 10);
					}

					ABullet* Projectile = World->SpawnActor<ABullet>(Arrow_Class, Location, PlayerRotation, SpawnInfo);
					ABullet* Projectile2 = World->SpawnActor<ABullet>(Arrow_Class, Location2, PlayerRotation, SpawnInfo);
					ABullet* Projectile3 = World->SpawnActor<ABullet>(Arrow_Class, Location3, PlayerRotation, SpawnInfo);

					if (IsValid(Projectile))
					{
						Projectile->fDamage = fDamage;
						//Projectile->FireInDirection(Player->GetActorForwardVector());
					}
					if (IsValid(Projectile2))
					{
						Projectile2->fDamage = fDamage;
						//Projectile2->FireInDirection(Player->GetActorForwardVector());
					}
					if (IsValid(Projectile3))
					{
						Projectile3->fDamage = fDamage;
						//Projectile3->FireInDirection(Player->GetActorForwardVector());
					}
				}
				else
				{
					ABullet* Projectile = World->SpawnActor<ABullet>(Arrow_Class, Location, PlayerRotation, SpawnInfo);
					if (IsValid(Projectile))
					{
						Projectile->fDamage = fDamage;
						Projectile->FireInDirection(Player->GetActorForwardVector());
					}
				}
			}
		}
	}
	*/
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
				Projectile->fDamage = fDamage;
			}
		}
	}
}

