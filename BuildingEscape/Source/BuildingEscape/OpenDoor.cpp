// Copyright Sjors Jessen 2018.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

UOpenDoor::UOpenDoor()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Add up all the actors which are in the pressure plate and opens if the weight is high enough.
	if (GetTotalMassOfActorsOnPlate() > 50.f) //Todo change to parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	//Find all the overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for(const auto& Actor: OverlappingActors)
	{

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s On Presurre Plate "), *Actor->GetName())
	}
	//Iterate through them adding their masses 
	return TotalMass;
}
