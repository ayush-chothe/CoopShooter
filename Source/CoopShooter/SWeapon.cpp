// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "SWeapon.h"

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	SocketName = "MuzzleSocket";
	TrailTargetName = "BeamEnd";
}


void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		
		FVector ShotDirection = EyeRotation.Vector();
		
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FVector TrailEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TrailEndPoint = Hit.ImpactPoint;

			UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, Hit, HitActor->GetInstigatorController(), this, DamageType);
		}

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.f, 0, 1.f);

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, SocketName);
		}

		if (TrailEffect)
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(SocketName);

			UParticleSystemComponent* Trail = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailEffect, MuzzleLocation);

			if (Trail)
			{
				Trail->SetVectorParameter(TrailTargetName, TrailEndPoint);
			}

		}

		APawn* MyOwnerPawn = Cast<APawn>(MyOwner);
		if (MyOwnerPawn)
		{
			APlayerController* PC = Cast<APlayerController>(MyOwnerPawn->GetController());
			if (PC)
			{
				PC->ClientPlayCameraShake(CameraShaker);
			}
		}

	}
}



