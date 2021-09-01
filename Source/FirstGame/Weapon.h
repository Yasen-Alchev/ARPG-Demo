// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FIRSTGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, Category = "WeaponSpeed")
		float fSpeed;

	UPROPERTY(VisibleAnywhere, Category = "WeaponDamage")
		float fDamage;

	UPROPERTY(VisibleAnywhere, Category = "WeaponRange")
		float fRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(class AFirstGameCharacter* Player, class AEnemy* Enemy){};
};
