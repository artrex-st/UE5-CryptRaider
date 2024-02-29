// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
	// AActor* TheOwner = GetOwner();
	// Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FString Position = TheOwner->GetActorLocation().ToCompactString();
	// UE_LOG(LogTemp,Display, TEXT("TheOwner Address is: %p and the location is: %s"),TheOwner, *Position);
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FVector TargetLocation = StartLocation;

	if (NeedMove)
	{
		TargetLocation = MoveOffset + StartLocation;
	}

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed = MoveOffset.Length() / MoveTime;
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
}

void UMover::SetShouldMove(bool ShouldMove)
{
	NeedMove = ShouldMove;
}

