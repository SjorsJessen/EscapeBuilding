// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	GrabberOwner = GetOwner();
	PlayerLocation = GetOwner()->GetActorLocation();
	PlayerRotation = GetOwner()->GetActorRotation();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LiftUpGrabbedComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Has No PhysicsComponent! %s"), *GrabberOwner->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	GrabberInput = GetOwner()->FindComponentByClass<UInputComponent>();

	if (GrabberInput)
	{
		GrabberInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		GrabberInput->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Has No GrabberComponent!"), *GrabberOwner->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	//RayCast out to reach distance
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit,
		PlayerLocation,
		GetLineTraceResult(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

const FVector UGrabber::GetLineTraceResult()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	FVector LineTraceEnd = PlayerLocation + (PlayerRotation.Vector() * Reach);

	return LineTraceEnd;
}

void UGrabber::LiftUpGrabbedComponent()
{
	//if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceResult());
	}
}

void UGrabber::Grab()
{
	//Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(), 
			true //Allow rotation
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

