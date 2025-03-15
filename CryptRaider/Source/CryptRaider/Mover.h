// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Mover.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:
	UMover();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mover", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mover", meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* Platform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mover")
	UCurveFloat* MovementCurve;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MovePlatform();

	UFUNCTION(BlueprintCallable)
	void StopMovePlatform();

	UFUNCTION()
	void ProcessMovementTimeline(float Value);

	UFUNCTION()
	void OnEndMovementTimeline();
	
private:
	UPROPERTY()
	FTimeline MovementTimeline;
};