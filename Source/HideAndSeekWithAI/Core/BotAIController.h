// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "BotAIController.generated.h"

class ABotBase;

UCLASS()
class HIDEANDSEEKWITHAI_API ABotAIController : public AAIController
{
	GENERATED_BODY()
	
	ABotAIController();

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPriveteAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	class UAISenseConfig_Sight* AISight = nullptr;
	class UAISenseConfig_Hearing* AIHearingt = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BTAsset = nullptr;

	//UPROPERTY(BlueprintReadWrite)
	ABotBase* ControlledBot = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UBlackboardComponent* BotBlackboard = nullptr;

protected:
	void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnStimulusReaction(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable)
	void ResetBot();

	UFUNCTION(BlueprintCallable)
	void BotLostPlayer();

	UFUNCTION(BlueprintCallable)
	bool CanChasePlaer();

	UFUNCTION(BlueprintCallable)
	void StartLookAround();

public:
	UFUNCTION(BlueprintCallable)
	void PickUpNoiseItem();
	
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

};
