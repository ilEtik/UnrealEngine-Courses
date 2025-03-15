// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Logging/StructuredLog.h"

void UTriggerComponent::BeginPlay()
{
	this->OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OverlapBegin);
	this->OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OverlapEnd);
}

void UTriggerComponent::UpdateLock(AActor* Actor)
{
	if (Actor == nullptr || !Actor->ActorHasTag(TriggerKeyTag) /*|| Actor->ActorHasTag("Grabbed")*/)
	{
		return;
	}

	Unlocked = !Unlocked;
	UE_LOGFMT(LogTemp, Log, "Trigger Unlocked = {0} | By = {1}!", Unlocked, Actor->GetActorNameOrLabel());

	AttachActor(Actor);

	if (Unlocked)
	{
		OnUnlocked.Broadcast();
	}
	else
	{
		OnLocked.Broadcast();
	}
}

void UTriggerComponent::AttachActor(AActor* Actor)
{
	if (!ShouldAttachKey || Actor == nullptr)
	{
		return;
	}

	UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	if (Component == nullptr)
	{
		return;
	}

	Component->SetSimulatePhysics(false);
	Actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UTriggerComponent::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UpdateLock(OtherActor);
}

void UTriggerComponent::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UpdateLock(OtherActor);
}
