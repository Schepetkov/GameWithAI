// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameData.generated.h"

UENUM(BlueprintType)
enum class EGameObjectType : uint8
{
	Bot,
	Item,
	Obstacle
};

UCLASS()
class HIDEANDSEEKWITHAI_API UGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<EGameObjectType, int32> GameObjectsData;

};
