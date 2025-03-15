// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "Logging/StructuredLog.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->GetComponentByClass<UPhysicsHandleComponent>();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Hold();
}

void UGrabber::Grab()
{
	if (PhysicsHandle == nullptr || PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		return;
	}

	FHitResult HitResult;
	if (!GetGrabbableInReach(HitResult))
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();

	UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
	if (!HitComponent->IsSimulatingPhysics())
	{
		return;
	}

	UE_LOGFMT(LogTemp, Log, "Grabbing Actor: {0}", *HitActor->GetActorNameOrLabel());

	FRotator ActorRotation = HitActor->GetActorRotation();
	FRotator ComponentRotation = GetComponentRotation();
	ActorRotation.Pitch = 0;
	ActorRotation.Roll = 0;
	ActorRotation.Add(-ComponentRotation.Pitch, 0, -ComponentRotation.Roll);
	HitActor->SetActorRotation(ActorRotation);

	HitActor->Tags.Add("Grabbed");

	HitComponent->WakeAllRigidBodies();
	PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, ComponentRotation);
}

void UGrabber::Hold()
{
	if (PhysicsHandle == nullptr || PhysicsHandle->GetGrabbedComponent() == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Release()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	UPrimitiveComponent* grabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (grabbedComponent == nullptr)
	{
		return;
	}


	grabbedComponent->GetOwner()->Tags.Remove("Grabbed");

	grabbedComponent->WakeAllRigidBodies();
	PhysicsHandle->ReleaseComponent();
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const
{
	FVector startPosition = GetComponentLocation();
	FVector targetLocation = startPosition + GetForwardVector() * MaxGrabDistance;

	const UWorld* World = GetWorld();
	if (ShowGizmos)
	{
		DrawDebugLine(World, startPosition, targetLocation, FColor::Cyan);
		DrawDebugSphere(World, targetLocation, GrabberRadius, 16, FColor::Cyan, false, 5);
	}

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabberRadius);
	return World->SweepSingleByChannel(HitResult, startPosition, targetLocation, FQuat::Identity,
									   ECollisionChannel::ECC_GameTraceChannel2, Sphere);
}
