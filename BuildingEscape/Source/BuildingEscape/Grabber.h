// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	//How far in front of the player can we reach?
	float Reach = 100.0f;
	AActor* GrabberOwner;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent*  GrabberInput = nullptr;


	//Raycast and Grab what's in reach
	void Grab();

	//Called when Grab key is released
	void Release();

	//Find attached Physics Handle
	void FindPhysicsHandleComponent();

	//Setup attached Input Component
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
