// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Enemy.h"
#include "FirstGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FirstGamePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABoss_Enemy::ABoss_Enemy()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("Boss_Enemy"));

	bDead = false;
	fScore = 500.f;
	fMax_Health = 100.0f;
}

// Called when the game starts or when spawned
void ABoss_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoss_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDead)
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AFirstGamePlayerController* PlayerController = Cast<AFirstGamePlayerController>(Player->GetInstigatorController());
		if (IsValid(PlayerController))
		{
			UE_LOG(LogTemp, Warning, TEXT("You Win!\nGame Ended!"));
			UE_LOG(LogTemp, Warning, TEXT("Your Score: %f"), PlayerController->Score);
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		}
	}
}

// Called to bind functionality to input
void ABoss_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
