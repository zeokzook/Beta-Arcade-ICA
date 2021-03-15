// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Room.h"
#include "Editor/EditorEngine.h"
#include "GenerateLevel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVIL_BUN_DUNGEON_API UGenerateLevel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGenerateLevel();

	TSubclassOf<ARoom> BlueprintVar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int roomsToGenerate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
