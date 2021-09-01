// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "FirstGamePlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Materials/Material.h"
#include "Blueprint/UserWidget.h"	
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	// Set the sphere's collision profile name to "Projectile".
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	
	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 350.0f;
	ProjectileMovementComponent->MaxSpeed = 350.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->Friction = 0.0f;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetConstraintMode(EDOFMode::XYPlane);
	ProjectileMeshComponent->SetEnableGravity(false);
	ProjectileMeshComponent->SetSimulatePhysics(false);
	ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	
	ProjectileMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/TopDownCPP/Blueprints/Bullet_FBX.Bullet_FBX'"));
	if (Mesh.Succeeded())
	{
		ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/TopDownCPP/Blueprints/Bullet_Material.Bullet_Material'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}

	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	this->Tags.Add(FName("Bullet"));

	fDamage = 0.f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// Event called when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void ABullet::FireInDirection(const FVector& ShootDirection)
{
	//ProjectileMovementComponent->Velocity = FVector(ShootDirection.X, ShootDirection.Y, 0) * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	/*
	if (OtherActor != this && OtherActor->ActorHasTag("Enemy"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("The bullet hit the actor!\n")));
		OtherActor->TakeDamage(fDamage, FDamageEvent(), this->GetInstigatorController(), this);
	}
	*/
	//if (OtherActor != this && !OtherActor->ActorHasTag("Enemy") || !OtherActor->ActorHasTag("Bullet"))
	//if(!OtherActor->ActorHasTag("Enemy") && this->ActorHasTag("FireBall"))
	Destroy();
}
void ABullet::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (this->ActorHasTag("FireBall"))
		{
			if (!EnemiesHitted.Contains(OtherActor))
			{
				EnemiesHitted.Add(OtherActor);
				OtherActor->TakeDamage(fDamage, FDamageEvent(), this->GetInstigatorController(), this);
			}
		}
		else
		{
			OtherActor->TakeDamage(fDamage, FDamageEvent(), this->GetInstigatorController(), this);
			Destroy();
		}
	}
	else
	{
		UWorld* World = this->GetWorld();
		if (IsValid(World))
		{
			AFirstGamePlayerController* Controller = Cast<AFirstGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
			if (IsValid(Controller))
			{
				AActor* Pawn = Controller->GetPawn();
				if (IsValid(Pawn))
				{
					if (OtherActor != Pawn && !OtherActor->ActorHasTag("Bullet"))
					{
						Destroy();
					}
				}
			}
		}
	}
}
