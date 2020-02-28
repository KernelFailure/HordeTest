// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "..\Public\PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	WeaponAttachSocketName = "WeaponSocket";

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	// Spawn weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon) {
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
}

void APlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
}

void APlayerCharacter::BeginZoom()
{
	bWantsToZoom = true;
	UE_LOG(LogTemp, Warning, TEXT("Wants to zoom"));
}

void APlayerCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void APlayerCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied) {
		UE_LOG(LogTemp, Warning, TEXT("Just died"));
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
		//StopFire();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	//float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	//CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);

	//PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::BeginZoom);
	//PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::EndZoom);
}

void APlayerCharacter::StartFire()
{
	if (CurrentWeapon) { CurrentWeapon->StartFire(); }
}

void APlayerCharacter::StopFire()
{
	if (CurrentWeapon) { CurrentWeapon->StopFire(); }
}

/* 
This makes sure that when we do our line trace for the weapon that 
we start aiming from the camera not the player mesh
*/
FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

