// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstGameCharacter.generated.h"

UCLASS(Blueprintable)
class AFirstGameCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AFirstGameCharacter();

	float fHitRadius;
	
	bool bShooting;
	bool bDead;
	bool bAbilityActivated;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float fMax_Health;

	/*
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnBeginOverlapMesh(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	*/

	UFUNCTION()
		void Attack(AEnemy* Enemy);

	UMaterial* Default_Cursor;
	UMaterial* Enemy_Cursor;

	
	UPROPERTY(EditAnywhere, Category = "Player Weapon", meta=(AllowedClasses ="Gun, Sword, Staff"))
		TSubclassOf<class AWeapon> Player_Weapon_Class;
	class AWeapon* Player_Weapon;
	
	UPROPERTY(EditAnywhere, Category = "Class Ability", meta = (AllowedClasses = "FireBall, StrongSwing, SplitArrow"))
		TSubclassOf<class UAbility> Special_Ability_Class;
	class UAbility* Special_Ability;

	UPROPERTY(EditAnywhere, Category = CollisionSphere)
		class USphereComponent* CollisionComponent;

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere, Category = "Health HUD")
		TSubclassOf<class UUserWidget> Player_Health_Widget_Class;
	class UUserWidget* Player_Health_Widget;
};



