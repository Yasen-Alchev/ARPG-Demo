// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstGamePlayerController.h"
#include "Enemy.h"
#include "Hunter.h"
#include "Weapon.h"
#include "Warrior.h"
#include "Ability.h"
#include "Sorceress.h"
#include "StrongSwing.h"
#include "Math/Vector.h"
#include "Engine/World.h"
#include "FirstGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

AFirstGamePlayerController::AFirstGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	Score = 0;
	Target = nullptr;
}

void AFirstGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (Pawn == nullptr)
	{
		if (GetPawn()->GetName().Contains("Hunter"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Hunter")));
			dynamic_cast<AHunter*>(Pawn);
			Pawn = Cast<AHunter>(GetPawn());
		}
		else if (GetPawn()->GetName().Contains("Sorceress"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Sorceress")));
			dynamic_cast<ASorceress*>(Pawn);
			Pawn = Cast<ASorceress>(GetPawn());
		}
		else if (GetPawn()->GetName().Contains("Warrior"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Warrior")));
			dynamic_cast<AWarrior*>(Pawn);
			Pawn = Cast<AWarrior>(GetPawn());
		}
		else { return; }
	}

	if (!UAIBlueprintHelperLibrary::GetCurrentPath(this) && IsValid(Target))
	{
		if (IsValid(Pawn))
		{
			UWorld* World = GetWorld();
			if (IsValid(World))
			{
				if (!World->GetTimerManager().IsTimerActive(handle))
				{
					FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Target->GetActorLocation());
					PlayerRot.Pitch = 0;
					PlayerRot.Roll = 0;
					Pawn->SetActorRotation(PlayerRot);
					Pawn->Attack(Cast<AEnemy>(Target));
					World->GetTimerManager().SetTimer(handle, [this, World]()
						{
							World->GetTimerManager().ClearTimer(handle);
						}, 0.5f, false);
				}
			}
		}
	}

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AFirstGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AFirstGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AFirstGamePlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("Ability", IE_Pressed, this, &AFirstGamePlayerController::Ability);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFirstGamePlayerController::Fire);
}

void AFirstGamePlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		if (!Hit.GetActor()->ActorHasTag("Enemy")) 
		{
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AFirstGamePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AFirstGamePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	Target = nullptr;
	bMoveToMouseCursor = true;
}

void AFirstGamePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AFirstGamePlayerController::Ability()
{
	if (IsValid(Pawn))
	{
		if (IsValid(Pawn->Special_Ability))
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(AbilityHandle))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Ability")));
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("Coolldown %f"), Pawn->Special_Ability->fCooldown));
				Pawn->bAbilityActivated = true;
				GetWorld()->GetTimerManager().SetTimer(AbilityHandle, [](){}, Pawn->Special_Ability->fCooldown, false);
			}
		}
	}
}

void AFirstGamePlayerController::Fire() 
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		if (IsValid(Hit.GetActor()) && Hit.GetActor()->ActorHasTag("Enemy"))
		{
			AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());
			if(IsValid(Enemy))
			{
				Target = Enemy;

				/*
				AFirstGameCharacter* MyPawn = NULL;
				if (GetPawn()->GetName().Contains("Hunter"))
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Hunter")));
					dynamic_cast<AHunter*>(MyPawn);
					MyPawn = Cast<AHunter>(GetPawn());
				}
				else if (GetPawn()->GetName().Contains("Sorceress"))
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Sorceress")));
					dynamic_cast<ASorceress*>(MyPawn);
					MyPawn = Cast<ASorceress>(GetPawn());
				}
				else if (GetPawn()->GetName().Contains("Warrior"))
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("Warrior")));
					dynamic_cast<AWarrior*>(MyPawn);
					MyPawn = Cast<AWarrior>(GetPawn());
				}
				else
				{
					return; 
				}
				*/

				if (IsValid(Pawn))
				{
					float Dist = FVector::Dist(Pawn->GetActorLocation(), Enemy->GetActorLocation());
					UWorld* World = this->GetWorld();
					
					if (IsValid(World) && IsValid(Pawn->Player_Weapon))
					{
						float WeaponRange = Pawn->Player_Weapon->fRange;
						FVector Direction = Enemy->GetActorLocation() - Pawn->GetActorLocation();
						FVector Destination = Pawn->GetActorLocation();
						Direction.Normalize();

						if (Pawn->bAbilityActivated && IsValid(Pawn->Special_Ability))
						{
							float AbilityRange = Pawn->Special_Ability->fRange;
								
								/*
								if (IsValid(Pawn->CollisionComponent)){.....}
								if (TSubclassOf<UStrongSwing>(Pawn->Special_Ability_Class))
								{
									Pawn->CollisionComponent->SetSphereRadius(AbilityRange/2 - 30.f);
								}
								else
								{
									Pawn->CollisionComponent->SetSphereRadius(AbilityRange / 2 - 80.f);
								}
								*/

							if (abs(Dist - AbilityRange) > 120.f)
							{
								if (Dist > AbilityRange - 120.f)
								{
									Destination += Direction * (Dist - AbilityRange + 57.f);
								}
								else
								{
									Direction *= -1;
									Destination += Direction * (AbilityRange - Dist + 57.f);
								}
								UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
							}
							else if (!World->GetTimerManager().IsTimerActive(handle))
							{
								Pawn->Attack(Enemy);
								World->GetTimerManager().SetTimer(handle, [this, World]() 
									{
										World->GetTimerManager().ClearTimer(handle);
									}, 0.5f, false);
							}
						}
						else if (abs(Dist - WeaponRange) > 120.f)
						{
							if (Dist > WeaponRange)
							{
								Destination += Direction * (Dist - WeaponRange + 57.f);
							}
							else
							{
								Direction *= -1;
								Destination += Direction * (WeaponRange - Dist + 57.f);
							}

							UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
						}
						else if (!World->GetTimerManager().IsTimerActive(handle))
						{
							FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Hit.ImpactPoint);
							PlayerRot.Pitch = 0;
							PlayerRot.Roll = 0;
							Pawn->SetActorRotation(PlayerRot);

							Pawn->Attack(Enemy);
							World->GetTimerManager().SetTimer(handle, [this, World]()
								{
									World->GetTimerManager().ClearTimer(handle);
								}, 0.5f, false);
						}
					}
				}
			}
		}
	
	}
}

