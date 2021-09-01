// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class FIRSTGAME_API AChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Chest)
		class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = Chest)
		UStaticMeshComponent* ChestMeshComponent;

	UPROPERTY(EditAnywhere, Category = Chest)
		UStaticMesh* ChestMesh;

	UPROPERTY(EditAnywhere, Category = Movement)
		UMaterialInstance* ChestMaterialInstance;

};
