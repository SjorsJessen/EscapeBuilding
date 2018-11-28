// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get Player Viewpoint each tick

	FVector PlayerLocation;
	FRotator PlayerRotation;

	

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	
	
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString());

	FVector LineTraceDirection = PlayerRotation.Vector();
	FVector LineTraceEnd = PlayerLocation + (LineTraceDirection * Reach);

	//Draw a red trace in the world to visualize 
	DrawDebugLine(GetWorld(), PlayerLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.0f, 10.0f);
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//RayCast out to reach distance
	FHitResult Hit;
	//AActor* ObjectHit = ;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters);
	
	//See what we hit 
	AActor* ActorHit = Hit.GetActor();

	if(ActorHit) //Check if raycast hits something, will crash if all non-hits are logging also.
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *ActorHit->GetName());
	}	
}

