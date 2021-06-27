// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Perception/AIPerceptionTypes.h"
#include "../PawnBase.h"

#include "BotBase.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBlackboardComponent;
class APlayerBase;

UENUM(BlueprintType)
enum class EBotMovementState : uint8
{
	Chase,
	MoveToSpawnSpot,
	MoveToNoiseLocation
};

//GD settings for the bot
USTRUCT(BlueprintType)
struct FBotSetup : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	//Indicate the number of meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseSpeed;

	//Indicate the number of meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpawnSpotSpeed;

	//Indicate the number of meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveNoiseLocationSpeed;

	//Measured in degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightDegreesAngle;

	//Debug splines of FOV color
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SplinesFOVColor;

	//Minimum time for looks around in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LooksAroundMinTime;

	//Maximum time for looks around in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LooksAroundMaxTime;

	//The speed with which bots will look around
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LooksAroundSpeedRotation;

	//Bot body material
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* BodyMaterial;
};

UCLASS(Abstract)
class HIDEANDSEEKWITHAI_API ABotBase : public APawnBase
{
	GENERATED_BODY()

public:
	ABotBase(const FObjectInitializer& Obj);

	UFUNCTION(BlueprintCallable)
	const FBotSetup& GetAISettingsRowHandle() const;
	
	UFUNCTION(BlueprintCallable)
	void SetBotMovementState(EBotMovementState MovementState);

	UFUNCTION(BlueprintCallable)
	void DestroyActor();

	UFUNCTION(BlueprintCallable)
	void StartLookAround();

	void SetBotTarget(APlayerBase* InNewTarget);
	
	APlayerBase* GetBotTarget();

protected:	
	UPROPERTY(BlueprintReadOnly)
	APlayerBase* BotTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	AActor* SpotRef = nullptr;
	
protected:
	virtual void PostInitializeComponents() override;

};
