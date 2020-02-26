// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class ABulletProjectile;

UCLASS()
class HORDETEST_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	virtual void Fire();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABulletProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	FName WeaponMuzzleSocketName;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartFire();

	void StopFire();

};
