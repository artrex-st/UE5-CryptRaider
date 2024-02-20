// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// ...	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Start grabber!"));
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	TryGrab(Start, End);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber!"));
}

void UGrabber::TryGrab(FVector Start, FVector End)
{
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	FHitResult HitResult;

	if (GetWorld()->SweepSingleByChannel(HitResult, Start,End, FQuat::Identity, ECC_GameTraceChannel2, Sphere))
	{
		UE_LOG(LogTemp, Display, TEXT("Object Hit: %s"), *HitResult.GetActor()->GetActorLabel());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No object Hit"));
	}
}

