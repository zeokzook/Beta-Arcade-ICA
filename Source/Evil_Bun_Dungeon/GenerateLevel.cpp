// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevel.h"

// Sets default values for this component's properties
UGenerateLevel::UGenerateLevel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBlueprint> PutNameHere(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/Example_Hallway.Example_Hallway'"));
	if (PutNameHere.Object) 
	{
		BlueprintVar = (UClass*)PutNameHere.Object->GeneratedClass;
	}
	// ...
}


// Called when the game starts
void UGenerateLevel::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	

	ARoom* room1 = World->SpawnActor<ARoom>(BlueprintVar, FVector(2000, 0, 30), FRotator(0, 0, 0));
	/*ARoom room1;
	FTransform thisTransform = this. GetTransform();
	UEditorEngine::AddActor(this->GetWorld(), room1, );*/

	// ...
	
}


// Called every frame
void UGenerateLevel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

