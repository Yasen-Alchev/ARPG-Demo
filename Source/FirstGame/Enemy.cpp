// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "FirstGamePlayerController.h"


// Sets default values
AEnemy::AEnemy()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("Enemy"));

	bDead = false;

	fSpeed = 100.f;
	fScore = 50.f;
	fMax_Health = 100.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	fHealth = fMax_Health;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDead) 
	{
		this->Destroy();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnBeginOverlap(class UPrimitiveComponent* HitComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{

}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	fHealth -= DamageAmount;
	fHealth = FMath::Clamp(fHealth, 0.0f, fMax_Health);
	if (fHealth <= 0 && !bDead)
	{
		bDead = true;
		AFirstGamePlayerController* MyController = Cast<AFirstGamePlayerController>(GetWorld()->GetFirstPlayerController());
		MyController->Score += fScore;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Enemy Killed!\n +50 HP Earned")));
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Damage Taken: %f"), DamageAmount));
	return DamageAmount;
}
