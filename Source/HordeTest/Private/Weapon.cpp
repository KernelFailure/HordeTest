// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "..\Public\Weapon.h"
#include "BulletProjectile.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Firing!!"));
	if (ProjectileClass) {
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		//ActorSpawnParams.Instigator = this;

		FVector MuzzleLocation = MeshComp->GetSocketLocation(WeaponMuzzleSocketName);

		FRotator MuzzleRotation = MeshComp->GetSocketRotation(WeaponMuzzleSocketName);

		GetWorld()->SpawnActor<ABulletProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
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

