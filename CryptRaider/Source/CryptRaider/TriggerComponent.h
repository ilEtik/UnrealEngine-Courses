// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLocked);


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUnlocked OnUnlocked;

	UPROPERTY(BlueprintAssignable)
	FOnLocked OnLocked;

public:
	void virtual BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	FName TriggerKeyTag = "Key_1";

	UPROPERTY(EditAnywhere)
	bool ShouldAttachKey = true;

	UPROPERTY(VisibleAnywhere)
	bool Unlocked;

private:
	UFUNCTION(BlueprintCallable)
	void UpdateLock(AActor* Actor);

	UFUNCTION()
	void AttachActor(AActor* Actor);

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
