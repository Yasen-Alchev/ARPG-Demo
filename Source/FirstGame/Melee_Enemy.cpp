// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee_Enemy.h"

// Sets default values
AMelee_Enemy::AMelee_Enemy()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fSpeed = 100.f;
	fScore = 50.f;
	fMax_Health = 200.f;

	this->Tags.Add(FName("Melee_Enemy"));
}

// Called when the game starts or when spawned
void AMelee_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMelee_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMelee_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

