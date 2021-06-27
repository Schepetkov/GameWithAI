#include "HideAndSeekGameMode.h"
#include "SpawnArea.h"
#include "Pawns/Bots/BotBase.h"
#include "GameObjects/ItemBase.h"
#include "GameObjects/GameObjectBase.h"
#include "Save/GameData.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Pawns/Player/PlayerBase.h"

void AHideAndSeekGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnArea = Cast<ASpawnArea>(UGameplayStatics::GetActorOfClass(this, ASpawnArea::StaticClass()));

	if (SpawnArea)
	{
		if (UGameplayStatics::DoesSaveGameExist("Config", 0))
		{
			if (UGameData* GameData = Cast<UGameData>(UGameplayStatics::LoadGameFromSlot("Config", 0)))
			{
				SpawnActors(EGameObjectType::Bot, *GameData->GameObjectsData.Find(EGameObjectType::Bot));
				SpawnActors(EGameObjectType::Item, *GameData->GameObjectsData.Find(EGameObjectType::Item));
				SpawnActors(EGameObjectType::Obstacle, *GameData->GameObjectsData.Find(EGameObjectType::Obstacle));
			}
		}
		else
		{
			SpawnActors(EGameObjectType::Bot, 6);
			SpawnActors(EGameObjectType::Item, 24);
			SpawnActors(EGameObjectType::Obstacle, 24);
		}
	}
}

void AHideAndSeekGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerController = NewPlayer;
}

void AHideAndSeekGameMode::SpawnActors(EGameObjectType InObjectType, int32 InCount)
{
	FTransform Transform;

	check(DefaultBotToSpawn);
	check(DefaultItemToSpawn);
	check(DefaulObstacleToSpawn);
	check(PlayerController)

	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundPlayers);

	TArray<AActor*> FoundBots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABotBase::StaticClass(), FoundBots);

	TArray<AActor*> FoundExit;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Exit", FoundExit);
	
	switch (InObjectType)
	{
	case EGameObjectType::Bot:
		
		if (AllBots.IsValidIndex(0))
		{
			for (ABotBase* BotToDestroy : AllBots)
			{
				BotToDestroy->DestroyActor();
			}
			
			AllBots.Empty();
		}

		if (FoundPlayers.IsValidIndex(0))
		{
			if (APlayerBase* Player = Cast<APlayerBase>(FoundPlayers[0]))
			{
				for (int32 i = 0; i < InCount;)
				{
					FVector SpawnPoint = SpawnArea->GetRandomPoint();

					float DistanceToPlayer = (SpawnPoint - Player->GetActorLocation()).Size();

					if (DistanceToPlayer <= DistanceBotFromPlayer)
					{
						continue;
					}

					i++;
					SpawnPoint.Z = 5.f;
					Transform.SetLocation(SpawnPoint);

					if (ABotBase* SpawnedBot = CastChecked<ABotBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DefaultBotToSpawn, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)))
					{
						UGameplayStatics::FinishSpawningActor(SpawnedBot, Transform);
						AllBots.Add(SpawnedBot);
					}
				}
			}
		}
		break;	
	
	case EGameObjectType::Item:
		
		if (AllItems.IsValidIndex(0))
		{
			for (AItemBase* ItemToDestroy : AllItems)
			{
				ItemToDestroy->DestroyActor();
			}

			AllItems.Empty();
		}
		
		for (int32 i = 0; i < InCount; i++)
		{
			FVector SpawnPoint = SpawnArea->GetRandomPoint();
			SpawnPoint.Z = 5.f;
			
			Transform.SetScale3D(FVector(0.2f, 0.2f, 0.2f));
			Transform.SetLocation(SpawnPoint);

			if (AItemBase* SpawnItem = CastChecked<AItemBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DefaultItemToSpawn, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)))
			{
				UGameplayStatics::FinishSpawningActor(SpawnItem, Transform);
				AllItems.Add(SpawnItem);
			}
		}
		break;

	case EGameObjectType::Obstacle:

		if (AllObstacles.IsValidIndex(0))
		{
			for (AActor* Obstacle : AllObstacles)
			{
				if (AGameObjectBase* ObstacleToDestroy = Cast<AGameObjectBase>(Obstacle))
				{
					ObstacleToDestroy->DestroyActor();
				}
			}

			AllObstacles.Empty();
		}
		
		for (int32 i = 0; i < InCount;)
		{
			FVector SpawnPoint = SpawnArea->GetRandomPoint();
			SpawnPoint.Z = 0.f;
			
			if (FoundPlayers.IsValidIndex(0) && FoundBots.IsValidIndex(0) && FoundExit.IsValidIndex(0))
			{
				if (APlayerBase* Player = Cast<APlayerBase>(FoundPlayers[0]))
				{
					float DistanceToPlayer = (SpawnPoint - Player->GetActorLocation()).Size();

					if (DistanceToPlayer <= 300.f)
					{
						continue;
					}
				}
				
				if (AActor* ExitSpot = FoundExit[0])
				{
					float DistanceToExitSpot = (SpawnPoint - ExitSpot->GetActorLocation()).Size();

					if (DistanceToExitSpot <= 300.f)
					{
						continue;
					}
				}
				
				for (AActor* FoundBot : FoundBots)
				{
					float DistanceToBot = (SpawnPoint - FoundBot->GetActorLocation()).Size();

					if (DistanceToBot <= 300.f)
					{
						continue;
					}
				}
			}
			
			i++;
			Transform.SetLocation(SpawnPoint);

			if (AGameObjectBase* ObstacleItem = CastChecked<AGameObjectBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DefaulObstacleToSpawn, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)))
			{
				UGameplayStatics::FinishSpawningActor(ObstacleItem, Transform);
				AllObstacles.Add(ObstacleItem);
			}	
		}
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("[DEBUG] GM - ObjectType is not choosed for SpawnActors();"));
		break;
	}
}