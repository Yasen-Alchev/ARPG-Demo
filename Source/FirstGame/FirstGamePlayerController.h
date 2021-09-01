// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstGamePlayerController.generated.h"

UCLASS()
class AFirstGamePlayerController : public APlayerController
{
	GENERATED_BODY()

		bool bIsMoving;

public:
	AFirstGamePlayerController();
	
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Score;

	class AFirstGameCharacter* Pawn;
	AActor* Target;
	FTimerHandle AbilityHandle;
	FTimerHandle handle;

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/* Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();


	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void Ability();
	void Fire();
};


