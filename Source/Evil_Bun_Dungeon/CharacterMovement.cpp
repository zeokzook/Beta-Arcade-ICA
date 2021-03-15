// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement.h"
#include "Projectile.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/LineBatchComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACharacterMovement::ACharacterMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->SetUsingAbsoluteRotation(true); 
	CameraBoom->TargetArmLength = 800.f;
	//CameraBoom->SetRelativeRotation(FRotator(-1000.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileBP(TEXT("Blueprint'/Game/Blueprints/Projectile_BP.Projectile_BP'"));
	if (ProjectileBP.Object)
		ProjectileClass = (UClass*)ProjectileBP.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void ACharacterMovement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CursorToWorld != nullptr)
	{
		//if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		//{
		//	/*if (UWorld* World = GetWorld())
		//	{
		//		FHitResult HitResult;
		//		FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
		//		FVector StartLocation = FollowCamera->GetComponentLocation();
		//		FVector EndLocation = FollowCamera->GetComponentRotation().Vector() * 2000.0f;
		//		Params.AddIgnoredActor(this);
		//		World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
		//		FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
		//		CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
		//	}*/
		//}
		/*else*/ if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

}

// Called to bind functionality to input
void ACharacterMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterMovement::MoveRight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ACharacterMovement::Shoot);

}

void ACharacterMovement::MoveForward(float Axis)
{
	FRotator Rotation = CameraBoom->GetTargetRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
}

void ACharacterMovement::MoveRight(float Axis)
{
	FRotator Rotation = CameraBoom->GetTargetRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}

void ACharacterMovement::Shoot()
{
	// Attempt to fire a projectile.
	APlayerController* PC = Cast<APlayerController>(GetController());

	FHitResult TraceHitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	
	const FRotator R = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), FVector(TraceHitResult.Location.X, TraceHitResult.Location.Y, 100.0f));
	const FRotator SpawnRotation = FRotator(0.0f, R.Yaw, R.Roll);
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = this->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		// Spawn the projectile at the muzzle.
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Projectile)
		{
			// Set the projectile's initial trajectory.
			FVector LaunchDirection = SpawnRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);
		}
	}

	//DrawRayLine(this->GetActorLocation(), TraceHitResult.Location, 3);
}

void ACharacterMovement::DrawRayLine(FVector StartPos, FVector EndPos, float fLifeTime)
{
	ULineBatchComponent* const LineBatcher = GetWorld()->PersistentLineBatcher;
	const float LifeTime = fLifeTime;
	if (LineBatcher != NULL)
	{
		float LineLifeTime = (LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime;

		LineBatcher->DrawLine(StartPos, EndPos, FLinearColor::Red, 10, 0.f, LineLifeTime);
	}
}

