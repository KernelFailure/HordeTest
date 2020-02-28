// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "..\Public\Weapon.h"
#include "BulletProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	WeaponMuzzleSocketName = "MuzzleSocket";
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Fire()
{

	AActor* MyOwner = GetOwner();
	if (MyOwner) {
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector AdjustedEyeLocation = EyeLocation + FVector(0.0f, 150.0f, 0.0f);
		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = AdjustedEyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, AdjustedEyeLocation, TraceEnd, ECC_GameTraceChannel1, QueryParams)) {
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit something"));
			AActor* HitActor = Hit.GetActor();
			float TestDamage = 20.0f;
			UGameplayStatics::ApplyPointDamage(HitActor, TestDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint);
		}
	}

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::StartFire()
{
	Fire();
}

void AWeapon::StopFire()
{

}

