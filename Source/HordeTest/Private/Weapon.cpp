// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "..\Public\Weapon.h"
#include "BulletProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "HordeTest.h"

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

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_GameTraceChannel1, QueryParams)) {
			AActor* HitActor = Hit.GetActor();
			float TestDamage = 20.0f;
			UGameplayStatics::ApplyPointDamage(HitActor, TestDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			if (SurfaceType == SURFACE_FLESHDEFAULT) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FleshImpactEffect, Hit.ImpactPoint);
				UE_LOG(LogTemp, Warning, TEXT("Hit flesh"));
			}
			else {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint);
				UE_LOG(LogTemp, Warning, TEXT("Hit default surface"));
			}
			
			//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Yellow, false, 1.0f, 0, 1.0f);
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

