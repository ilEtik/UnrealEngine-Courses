// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 150;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 125;

	UPROPERTY(EditAnywhere)
	float GrabberRadius = 50;

	UPROPERTY(EditAnywhere)
	bool ShowGizmos = false;

private:
	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Hold();

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION()
	bool GetGrabbableInReach(FHitResult& HitResult) const;
};
