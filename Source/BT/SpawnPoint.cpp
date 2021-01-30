// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "SpawnPoint.h"

// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// Will spawn the actor of the class chosen in the BP
	GeneratedSprungWheel = GetWorld()->SpawnActorDeferred<AActor>(SpawnedClass, GetComponentTransform());
	GeneratedSprungWheel->SetActorHiddenInGame(true);

	if(!ensure(GeneratedSprungWheel)) return;
	// The new created Actor now will be attached (or better his root component) to this SpawnPoint Scene Component
	GeneratedSprungWheel->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	//Don't use KeepRelativeTransform because we spawn the actor in the component location, and when attached if we keep the relative, the distance will double up

	UGameplayStatics::FinishSpawningActor(GeneratedSprungWheel, GetComponentTransform());
	
}

AActor* USpawnPoint::GetSprungWheel()
{
	return GeneratedSprungWheel;
}

// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

