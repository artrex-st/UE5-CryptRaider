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
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		const FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		//Get the physical object held by the PhysicsHandle in the grab function and adjust its transformation according to the correct physics.
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();

	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physicshandler not found"));
		return;
	}

	FHitResult HitResult;

	if (GetGrabbableInReach(HitResult))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
		
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		//Get the Physics object hit by the sweep and add it to the PhysicsHandle's grabbed objects.
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
		AActor* Actor = HitResult.GetActor();
		Actor->Tags.Add("Grabbed");
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		Primitive->SetSimulatePhysics(true);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandleComponent = GetPhysicsHandle();

	if (PhysicsHandleComponent && PhysicsHandleComponent->GetGrabbedComponent())
	{
		PhysicsHandleComponent->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		//Release grabbable objects from PhysicsHandler.
		PhysicsHandleComponent->ReleaseComponent();
	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	if (GetOwner()->FindComponentByClass<UPhysicsHandleComponent>())
	{
		return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	}

	return nullptr;
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const
{
	UE_LOG(LogTemp, Display, TEXT("Start grabber!"));
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	return GetWorld()->SweepSingleByChannel(HitResult, Start,End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}
