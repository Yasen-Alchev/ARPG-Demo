// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstGameCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Materials/Material.h"
#include "Blueprint/UserWidget.h"	
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

#include "Gun.h"
#include "Enemy.h"
#include "Staff.h"
#include "Sword.h"
#include "Weapon.h"
#include "Ability.h"
#include "FireBall.h"
#include "SplitArrow.h"
#include "StrongSwing.h"
#include "FirstGamePlayerController.h"

AFirstGameCharacter::AFirstGameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		Default_Cursor = DecalMaterialAsset.Object;
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset2(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal_Red.M_Cursor_Decal_Red'"));
	if (DecalMaterialAsset2.Succeeded())
	{
		Enemy_Cursor = DecalMaterialAsset2.Object;
	}

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Sphere"));
	CollisionComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -12.0f));
	CollisionComponent->SetSphereRadius(50.f);
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	// GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	this->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -94.f));
	this->GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Special_Ability = nullptr;

	fHitRadius = 0;
	bDead = false;
	fMax_Health = 100.0f;
	bShooting = false;
	bAbilityActivated = false;
	this->Tags.Add(FName("Player"));
}

void AFirstGameCharacter::BeginPlay() 
{
	Super::BeginPlay();
	fHealth = fMax_Health;

	//this->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AFirstGameCharacter::OnBeginOverlapMesh);
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFirstGameCharacter::OnBeginOverlap);
//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFirstGameCharacter::OnBeginOverlap);
//CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFirstGameCharacter::OnEndOverlap);
	
	if (Player_Health_Widget_Class)
	{
		Player_Health_Widget = CreateWidget(GetWorld(), Player_Health_Widget_Class);
		Player_Health_Widget->AddToViewport();
	}

	if (IsValid(Player_Weapon_Class))
	{
		if (TSubclassOf<AGun>(Player_Weapon_Class))
		{
			dynamic_cast<AGun*>(Player_Weapon);
			Player_Weapon = NewObject<AGun>(GetTransientPackage(), Player_Weapon_Class);
			//fHitRadius = Player_Weapon->fRange / 2 - 80.f;
			//CollisionComponent->SetSphereRadius(fHitRadius);
		}
		else if (TSubclassOf<ASword>(Player_Weapon_Class))
		{
			dynamic_cast<ASword*>(Player_Weapon);
			Player_Weapon = NewObject<ASword>(GetTransientPackage(), Player_Weapon_Class);
			//fHitRadius = Player_Weapon->fRange / 2 - 30.f;
			//CollisionComponent->SetSphereRadius(fHitRadius);
		}
		else if (TSubclassOf<AStaff>(Player_Weapon_Class))
		{
			dynamic_cast<AStaff*>(Player_Weapon);
			Player_Weapon = NewObject<AStaff>(GetTransientPackage(), Player_Weapon_Class);
			//fHitRadius = Player_Weapon->fRange / 2 - 80.f;
			//CollisionComponent->SetSphereRadius(fHitRadius);
		}
	}
	if (IsValid(Special_Ability_Class))
	{
		if (TSubclassOf<USplitArrow>(Special_Ability_Class))
		{
			dynamic_cast<USplitArrow*>(Special_Ability);
			Special_Ability = NewObject<USplitArrow>(GetTransientPackage(), Special_Ability_Class);
		}
		if (TSubclassOf<UFireBall>(Special_Ability_Class))
		{
			dynamic_cast<UFireBall*>(Special_Ability);
			Special_Ability = NewObject<UFireBall>(GetTransientPackage(), Special_Ability_Class);
		}
		if (TSubclassOf<UStrongSwing>(Special_Ability_Class))
		{
			dynamic_cast<UStrongSwing*>(Special_Ability);
			Special_Ability = NewObject<UStrongSwing>(GetTransientPackage(), Special_Ability_Class);
		}
	}
}

void AFirstGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	AFirstGamePlayerController* PlayerController = Cast<AFirstGamePlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		if (bDead || fHealth <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("You Lost.\nGame Ended!"));
			UE_LOG(LogTemp, Warning, TEXT("Your Score: %f"), PlayerController->Score);
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		}
		if (CursorToWorld != nullptr)
		{
			FHitResult TraceHitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

			if (IsValid(TraceHitResult.GetActor()))
			{
				if (TraceHitResult.GetActor()->ActorHasTag("Enemy"))
				{
					CursorToWorld->SetDecalMaterial(Enemy_Cursor);
				}
				else if(CursorToWorld->GetDecalMaterial() != Default_Cursor)
				{
					CursorToWorld->SetDecalMaterial(Default_Cursor);
				}
			}
			
			FVector CursorForwardVector = TraceHitResult.ImpactNormal;
			FRotator CursorRotation = CursorForwardVector.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorRotation);
		}
	}
}

/*
void AFirstGameCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		AFirstGamePlayerController* PlayerController = Cast<AFirstGamePlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			if (PlayerController->Target == OtherActor && OtherActor != nullptr)
			{
				this->Attack(Cast<AEnemy>(OtherActor));
				PlayerController->Target = nullptr;
			}
		}
	}
}

void AFirstGameCharacter::OnEndOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		AFirstGamePlayerController* PlayerController = Cast<AFirstGamePlayerController>(GetController());
		if (IsValid(PlayerController))
		{
			if (PlayerController->Target == OtherActor && OtherActor != nullptr)
			{
				FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OtherActor->GetActorLocation());
				PlayerRot.Pitch = 0;
				PlayerRot.Roll = 0;
				this->SetActorRotation(PlayerRot);

				this->Attack(Cast<AEnemy>(OtherActor));
				PlayerController->Target = nullptr;
			}
		}
	}
}

void AFirstGameCharacter::OnBeginOverlapMesh(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnBeginOverlapMesh Activated\n")));
	if (OtherActor->ActorHasTag("Bullet"))
	{
		OtherActor->Destroy();
	}
}
*/

void AFirstGameCharacter::Attack(AEnemy* Enemy)
{
	AFirstGamePlayerController* PlayerController = Cast<AFirstGamePlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		if (IsValid(Enemy) && PlayerController->Target != nullptr)
		{
			if (bAbilityActivated)
			{
				bAbilityActivated = false;
				if (IsValid(Special_Ability))
				{
					FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Enemy->GetActorLocation());
					PlayerRot.Pitch = 0;
					PlayerRot.Roll = 0;
					this->SetActorRotation(PlayerRot);
					Special_Ability->CastAbility(this, Enemy);
				}
			}
			else if (IsValid(Player_Weapon))
			{
				Player_Weapon->Attack(this, Enemy);
			}
			PlayerController->Target = nullptr;
		}
	}
	//CollisionComponent->SetSphereRadius(fHitRadius);
}

