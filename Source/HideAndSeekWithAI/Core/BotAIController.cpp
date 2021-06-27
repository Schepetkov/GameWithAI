// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "Pawns/Bots/BotBase.h"
#include "GameObjects/ItemBase.h"
#include "Pawns/Player/PlayerBase.h"

#include <GenericTeamAgentInterface.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BehaviorTree.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/RotatingMovementComponent.h>

ABotAIController::ABotAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");

	AISight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
	AISight->DetectionByAffiliation.bDetectEnemies = true;

	AIHearingt = CreateDefaultSubobject<UAISenseConfig_Hearing>("Sight_Hearing");
	AIHearingt->DetectionByAffiliation.bDetectEnemies = true;

	AIPerceptionComponent->ConfigureSense(*AISight);
	AIPerceptionComponent->ConfigureSense(*AIHearingt);
	AIPerceptionComponent->SetDominantSense(AISight->GetSenseImplementation());
	
	SetGenericTeamId(FGenericTeamId(1));
}

void ABotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (InPawn == nullptr)
	{
		return;
	}

	if (BTAsset == nullptr)
	{
		return;
	}

	if (RunBehaviorTree(BTAsset))
	{
		ControlledBot = Cast<ABotBase>(InPawn);

		BotBlackboard = GetBlackboardComponent();

		if (AIPerceptionComponent && ControlledBot && BotBlackboard && ControlledBot)
		{
			BotBlackboard->SetValueAsVector("BotSpawnLocation", ControlledBot->GetActorLocation());
			
			AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABotAIController::OnStimulusReaction);
		}
	}
}

void ABotAIController::ResetBot()
{
	BotBlackboard->SetValueAsBool("NewNoise", false);
	BotBlackboard->SetValueAsBool("HearNoise", false);
	BotBlackboard->SetValueAsBool("BotLostPlayer", false);
}

void ABotAIController::BotLostPlayer()
{
	BotBlackboard->SetValueAsBool("SeePlayer", false);
	BotBlackboard->SetValueAsBool("BotLostPlayer", true);
	
	if (ControlledBot && ControlledBot->GetBotTarget())
	{
		
		ControlledBot->GetBotTarget()->ChangeCaughtBotsState(false);
		ControlledBot->SetBotTarget(nullptr);
	}
}

bool ABotAIController::CanChasePlaer()
{
	if (ControlledBot == nullptr)
	{
		return false;
	}
	
	if (BotBlackboard->GetValueAsBool("SeePlayer") && ControlledBot->GetBotTarget())
	{
		BotBlackboard->SetValueAsVector("PlayerLocation", ControlledBot->GetBotTarget()->GetActorLocation());
		BotBlackboard->SetValueAsVector("LastSeePlayerLocation", ControlledBot->GetBotTarget()->GetActorLocation());
		
		return true;
	}

	return false;
}

void ABotAIController::StartLookAround()
{
	if (ControlledBot == nullptr)
	{
		return;
	}
	
	float LookAroundTime = UKismetMathLibrary::RandomFloatInRange(float(ControlledBot->GetAISettingsRowHandle().LooksAroundMinTime), float(ControlledBot->GetAISettingsRowHandle().LooksAroundMaxTime));
	BotBlackboard->SetValueAsFloat("LookAroundTime", LookAroundTime);

	ControlledBot->StartLookAround();
}

void ABotAIController::OnStimulusReaction(AActor* Actor, FAIStimulus Stimulus)
{
	if (ControlledBot == nullptr)
	{
		return;
	}

	if (BotBlackboard->GetValueAsBool("SeePlayer") && !Stimulus.WasSuccessfullySensed())
	{
		BotLostPlayer();
		return;
	}

	if (BotBlackboard->GetValueAsBool("SeePlayer"))
	{
		return;
	}

	if (!BotBlackboard->GetValueAsBool("SeePlayer") && Stimulus.Tag == "Noise")
	{
		if (ControlledBot->GetCurrentItem())
		{
			ControlledBot->ThrowItem();
		}

		if (Actor)
		{
			if (AItemBase* SpotItem = Cast<AItemBase>(Actor))
			{
				if (SpotItem->GetLastItemOwner() == ControlledBot)
				{
					BotBlackboard->SetValueAsVector("NoiseLocation", SpotItem->GetActorLocation());
					BotBlackboard->SetValueAsBool("HearNoise", true);
				}
				else
				{
					BotBlackboard->SetValueAsObject("NoiseItemToPickUp", SpotItem);
					BotBlackboard->SetValueAsVector("NoiseLocation", SpotItem->GetActorLocation());
					BotBlackboard->SetValueAsBool("HearNoise", true);
					BotBlackboard->SetValueAsBool("NewNoise", true);
				}
			}
		}
	}

	if (Stimulus.WasSuccessfullySensed() && Actor && Actor->ActorHasTag(FName("Player")))
	{
		BotBlackboard->SetValueAsBool("SeePlayer", true);
		BotBlackboard->SetValueAsBool("HearNoise", false);

		BotBlackboard->SetValueAsVector("PlayerLocation", Stimulus.StimulusLocation);
		BotBlackboard->SetValueAsVector("LastSeePlayerLocation", Stimulus.StimulusLocation);

		if (APlayerBase* NewBotTarget = Cast<APlayerBase>(Actor))
		{
			NewBotTarget->ChangeCaughtBotsState(true);
			ControlledBot->SetBotTarget(NewBotTarget);
		}
		
		if (ControlledBot->GetCurrentItem())
		{
			ControlledBot->ThrowItem();
		}
	}
}

void ABotAIController::PickUpNoiseItem()
{
	if (BotBlackboard->GetValueAsObject("NoiseItemToPickUp") && ControlledBot)
	{
		if (AItemBase* ItemToPickUp = Cast<AItemBase>(BotBlackboard->GetValueAsObject("NoiseItemToPickUp")))
		{
			BotBlackboard->SetValueAsVector("ItemSpawnLocation", ItemToPickUp->SpawnLocation);
			
			ControlledBot->PickUpItem(ItemToPickUp);
		}
	}
}

ETeamAttitude::Type ABotAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Hostile;
}
