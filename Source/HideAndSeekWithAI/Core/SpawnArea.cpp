#include "SpawnArea.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

ASpawnArea::ASpawnArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box_Component");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = BoxComponent;
}

FVector ASpawnArea::GetRandomPoint()
{
	FVector TestVector = FVector(0.f,0.f,0.f);
	
	if (BoxComponent)
	{
		TestVector = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxComponent->GetUnscaledBoxExtent());
	}
	
	return TestVector;
}
