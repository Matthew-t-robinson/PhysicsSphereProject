// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

#include "SelectionSet.h"
#include "Components/SphereComponent.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	mesh->SetWorldScale3D(FVector{Scale});
	SetRootComponent(mesh);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualRepresentation");
	CubeMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

    if (CubeMeshAsset.Succeeded())
    {
	    CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
    	CubeMesh->SetRelativeLocation(FVector{0.f,0.f,-(Scale * 50)});
    	CubeMesh->SetWorldScale3D(FVector(Scale));
    }
}

void ACube::Collision(ABall* CollidingBall)
{
	if (AreColliding(CollidingBall))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "COLLIDING");
	}
}

bool ACube::AreColliding(ABall* CollidingBall)
{
	float sphereXDistance = abs((CollidingBall->GetActorLocation().X + CollidingBall->GetRadius()) - (GetActorLocation().X + (50 * Scale)));
	float sphereYDistance = abs((CollidingBall->GetActorLocation().Y + CollidingBall->GetRadius()) - (GetActorLocation().Y + (50 * Scale)));
	float sphereZDistance = abs((CollidingBall->GetActorLocation().Z + CollidingBall->GetRadius()) - (GetActorLocation().Z + (50 * Scale)));

	if (sphereXDistance >= ((50 * Scale) + CollidingBall->GetRadius())) { return false; }
	if (sphereYDistance >= ((50 * Scale) + CollidingBall->GetRadius())) { return false; }
	if (sphereZDistance >= ((50 * Scale) + CollidingBall->GetRadius())) { return false; }

	if (sphereXDistance < ((50 * Scale))) { return true; } 
	if (sphereYDistance < ((50 * Scale))) { return true; }
	if (sphereZDistance < ((50 * Scale))) { return true; }

	float cornerDistance_sq = ((sphereXDistance - (50 * Scale)) * (sphereXDistance - (50 * Scale))) +
						  ((sphereYDistance - (50 * Scale)) * (sphereYDistance - (50 * Scale)) +
						  ((sphereYDistance - (50 * Scale)) * (sphereYDistance - (50 * Scale))));

	return (cornerDistance_sq < (CollidingBall->GetRadius() * CollidingBall->GetRadius()));
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

