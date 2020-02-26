// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "..\Public\BulletProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	
	RootComponent = CollisionComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 3000.0f;
	ProjectileMovementComp->MaxSpeed = 3000.0f;
	ProjectileMovementComp->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//OtherActor->TakeDamage(BaseDamageTest, )
	//UE_LOG(LogTemp, Warning, TEXT("Bullet actor overlapped with something"));
	float BaseDamageTest = 20.0f;
	FVector EyeLocation;
	FRotator EyeRotation;
	FVector NewDirection = GetActorForwardVector();
	//FVector NewDirection = EyeRotation.Vector();
	FHitResult Hit;
	//AController Instigator = GetInstigator();
	UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamageTest, NewDirection, Hit, nullptr, this, DamageType);
	

	
}

