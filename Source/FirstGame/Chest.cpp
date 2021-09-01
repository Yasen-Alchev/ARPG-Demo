// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FirstGamePlayerController.h"

// Sets default values
AChest::AChest()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ChestSceneComponent"));

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Chest"));
	CollisionComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 39.0f));
	CollisionComponent->SetupAttachment(RootComponent);

	ChestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMeshComponent"));
	ChestMeshComponent->SetConstraintMode(EDOFMode::XYPlane);
	ChestMeshComponent->SetEnableGravity(false);
	ChestMeshComponent->SetSimulatePhysics(true);
	ChestMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ChestMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	ChestMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/TopDownCPP/Chest/ChestCartoon_ChestCartoon.ChestCartoon_ChestCartoon'"));
	if (Mesh.Succeeded())
	{
		ChestMeshComponent->SetStaticMesh(Mesh.Object);
		ChestMeshComponent->SetRelativeScale3D(FVector(15.0f, 15.0f, 15.0f));
		ChestMeshComponent->AddRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/TopDownCPP/Chest/M_Wood_Oak.M_Wood_Oak'"));
	if (Material.Succeeded())
	{
		ChestMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ChestMeshComponent);
	}

	ChestMeshComponent->SetMaterial(0, ChestMaterialInstance);
	ChestMeshComponent->SetupAttachment(RootComponent);

	this->Tags.Add(FName("Chest"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnBeginOverlap);
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChest::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("You got the chest!"));
		if (GEngine)
		{
			//Print debug message
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("You got the chest!\n+100 HP Earned")));
		}

		AFirstGamePlayerController* MyController = Cast<AFirstGamePlayerController>(GetWorld()->GetFirstPlayerController());
		MyController->Score += 100.0f;
		this->Destroy();
	}
}