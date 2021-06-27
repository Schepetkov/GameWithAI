#include "GameObjectBase.h"

AGameObjectBase::AGameObjectBase(const FObjectInitializer& Obj)
	: Super(Obj)
{
	GameObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_GameObject");
	RootComponent = GameObjectMesh;
}

void AGameObjectBase::BeginPlay()
{
	SpawnLocation = GetActorLocation();
	
	Super::BeginPlay();
}

void AGameObjectBase::DestroyActor()
{
	if (SpotRef)
	{
		SpotRef->Destroy();
	}
	
	Destroy();
}

UStaticMeshComponent* AGameObjectBase::GetMesh()
{
	return GameObjectMesh;
}

