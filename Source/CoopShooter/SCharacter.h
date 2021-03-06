// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USHealthComponent;

UCLASS()
class COOPSHOOTER_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void BeginZoom();

	void StartFire();

	void StopFire();

	void EndZoom();

	void DoJump();

	UFUNCTION()
	void OnHealthChange(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASWeapon> StarterWeapon;

	UPROPERTY(VisibleDefaultsOnly)
	FName WeaponAttachedSocketName;

	ASWeapon* CurrentWeapon;

	bool bWantsToZoom;

	UPROPERTY(BlueprintReadWrite)
	bool bDied;

	UPROPERTY(EditDefaultsOnly)
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomedInterpedSpeed;

	float DefaultFOV;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

};
