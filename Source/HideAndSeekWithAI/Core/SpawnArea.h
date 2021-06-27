// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

class UBoxComponent;

UCLASS()
class HIDEANDSEEKWITHAI_API ASpawnArea : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawnArea();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UBoxComponent* BoxComponent = nullptr;

public:
	//Get random point inside this area
	UFUNCTION(BlueprintCallable)
	FVector GetRandomPoint();

};
