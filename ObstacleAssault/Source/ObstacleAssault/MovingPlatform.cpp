// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	if (Velocity.IsZero())
	{
		return;
	}

	if (ShouldReturnPlatform())
	{
		ReturnPlatform();
		return;
	}

	FVector currentLocation = GetActorLocation();
	currentLocation += Velocity * DeltaTime;
	SetActorLocation(currentLocation);
}

void AMovingPlatform::ReturnPlatform()
{
	StartPosition += Velocity.GetSafeNormal() * MaxMoveDistance;
	SetActorLocation(StartPosition);
	Velocity = -Velocity;
}

void AMovingPlatform::RotatePlatform(float DelatTime)
{
	if (Rotator.IsZero())
	{
		return;
	}

	FRotator currentRotation = GetActorRotation();
	AddActorLocalRotation(Rotator * DelatTime);
}

bool AMovingPlatform::ShouldReturnPlatform() const
{
	return GetDistancedMoved() > MaxMoveDistance;
}

float AMovingPlatform::GetDistancedMoved() const
{
	return FVector::Dist(StartPosition, GetActorLocation());
}
