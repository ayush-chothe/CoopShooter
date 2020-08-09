// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "SSmolEnemy.h"

// Sets default values
ASSmolEnemy::ASSmolEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCanEverAffectNavigation(false);

}

// Called when the game starts or when spawned
void ASSmolEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSmolEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
