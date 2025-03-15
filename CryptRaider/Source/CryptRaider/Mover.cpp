// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Logging/StructuredLog.h"
#include "Math/UnrealMathUtility.h"

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMover::BeginPlay()
{
	Super::BeginPlay();
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

void UMover::MovePlatform()
{
	if (SplineComponent == nullptr || Platform == nullptr)
	{
		UE_LOGFMT(LogTemp, Log, "Cannot Move Platform.");
		return;
	}

	UE_LOGFMT(LogTemp, Log, "Moving Platform");

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, TEXT("ProcessMovementTimeline"));
	MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

	FOnTimelineEvent OnTimelineFinished;
	OnTimelineFinished.BindUFunction(this, TEXT("OnEndMovementTimeline"));
	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinished);

	MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	MovementTimeline.Play();
}

void UMover::StopMovePlatform()
{
	if (MovementTimeline.IsReversing())
	{
		return;
	}

	MovementTimeline.Reverse();
}

void UMover::ProcessMovementTimeline(float Value)
{
	const float SplineLength = SplineComponent->GetSplineLength();

	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	// making platform always straight
	CurrentSplineRotation.Pitch = 0.f;

	Platform->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

void UMover::OnEndMovementTimeline()
{
}
