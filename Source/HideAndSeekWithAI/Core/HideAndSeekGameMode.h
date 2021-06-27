// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Save/GameData.h"
#include "HideAndSeekGameMode.generated.h"

class ABotBase;
class ASpawnArea;
class AItemBase;
class AGameObjectBase;

UCLASS()
class HIDEANDSEEKWITHAI_API AHideAndSeekGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void SpawnActors(EGameObjectType InObjectType, int32 InCount);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ASpawnArea* SpawnArea = nullptr;

	UPROPERTY(BlueprintReadOnly)
	APawn* Player = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceBotFromPlayer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABotBase> DefaultBotToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AItemBase> DefaultItemToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGameObjectBase> DefaulObstacleToSpawn;

	UPROPERTY(BlueprintReadOnly)
	TArray<ABotBase*> AllBots;

	UPROPERTY(BlueprintReadOnly)
	TArray<AItemBase*> AllItems;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> AllObstacles;
};
