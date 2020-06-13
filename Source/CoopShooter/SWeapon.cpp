// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SWeapon.h"

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	SocketName = "MuzzleSocket";
	TrailTargetName = "BeamEnd";

	BaseDamage = 20.0f;
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
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TrailEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;

			if (SurfaceType == SurfaceType2)
			{
				ActualDamage *= 4;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, HitActor->GetInstigatorController(), this, DamageType);

			UParticleSystem* SelectedEffect;

			switch (SurfaceType)
			{
			case SurfaceType1: // FleshDefault
			case SurfaceType2: // FleshVunerable
				SelectedEffect = FleshImpactEffect;
				break;
			default:
				SelectedEffect = ImpactEffect;
				break;
			}

			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}


			TrailEndPoint = Hit.ImpactPoint;

			

			
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



