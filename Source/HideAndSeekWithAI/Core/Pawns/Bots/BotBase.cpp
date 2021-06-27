#include "BotBase.h"
#include "../../GameObjects/ItemBase.h"

#include "Engine/EngineTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProceduralMeshComponent.h"

ABotBase::ABotBase(const FObjectInitializer& Obj)
	: Super(Obj)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

const FBotSetup& ABotBase::GetAISettingsRowHandle() const
{
	const FString Contecxt("ABotBase::GetCharacterSettingsRowHandle");
	FBotSetup* Settings = SetupDataTable.GetRow<FBotSetup>(Contecxt);
	check(Settings);
	return *Settings;
}

void ABotBase::SetBotMovementState(EBotMovementState MovementState)
{
	switch (MovementState)
	{
	case EBotMovementState::Chase:
		FloatingPawnMovement->MaxSpeed = GetAISettingsRowHandle().ChaseSpeed;
		FloatingPawnMovement->Acceleration = GetAISettingsRowHandle().ChaseSpeed;
		break;

	case EBotMovementState::MoveToSpawnSpot:
		FloatingPawnMovement->MaxSpeed = GetAISettingsRowHandle().MoveSpawnSpotSpeed;
		FloatingPawnMovement->Acceleration = GetAISettingsRowHandle().MoveSpawnSpotSpeed;
		break;

	case EBotMovementState::MoveToNoiseLocation:
		FloatingPawnMovement->MaxSpeed = GetAISettingsRowHandle().MoveNoiseLocationSpeed;
		FloatingPawnMovement->Acceleration = GetAISettingsRowHandle().MoveNoiseLocationSpeed;
		break;

	default:
		break;
	}
}

void ABotBase::DestroyActor()
{
	if (SpotRef)
	{
		SpotRef->Destroy();
	}
	
	Destroy();
}

void ABotBase::StartLookAround()
{
	if (RotatingMovementComponent == nullptr)
	{
		return;
	}
	
	if (UKismetMathLibrary::RandomBool())
	{
		RotatingMovementComponent->RotationRate.Yaw = GetAISettingsRowHandle().LooksAroundSpeedRotation;
	}
	else
	{
		RotatingMovementComponent->RotationRate.Yaw = (GetAISettingsRowHandle().LooksAroundSpeedRotation * -1);
	}
}

void ABotBase::SetBotTarget(APlayerBase* InNewTarget)
{
	BotTarget = InNewTarget;
}

APlayerBase* ABotBase::GetBotTarget()
{
	return BotTarget;
}

void ABotBase::PostInitializeComponents()
{
	if (SetupDataTable.DataTable && BodyMesh)
	{
		BodyMesh->SetMaterial(0, GetAISettingsRowHandle().BodyMaterial);
	}

	Super::PostInitializeComponents();
}
