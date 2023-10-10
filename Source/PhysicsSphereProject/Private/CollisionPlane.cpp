// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionPlane.h"
#include <cmath>

// Sets default values
ACollisionPlane::ACollisionPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	mesh->SetWorldScale3D(FVector{5.f});
	SetRootComponent(mesh);

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualRepresentation");
	PlaneMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

	if (CubeMeshAsset.Succeeded())
	{
		PlaneMesh->SetStaticMesh(CubeMeshAsset.Object);
	}
}

void ACollisionPlane::Collision(ABall* Ball)
{
	FVector Normal = GetActorUpVector();
	FVector BallVelocity = Ball->GetBallVelocity();
	FVector PlaneLocation = GetActorLocation();
	FVector BallLocation = Ball->GetActorLocation();
	FVector P{BallLocation - PlaneLocation};
	float CosAngle = CosOfAngle(P, Normal);
	float q1 = acos(CosAngle);
	float q2 = (PI / 2) - q1;
	float d = sin(q2) * LengthOfVector(P);
	float CosOfAngleS = CosOfAngle(BallVelocity, -Normal);
	float VCLength = (d - Ball->GetRadius()) / CosOfAngleS;
	float VLength = LengthOfVector(BallVelocity);
	if (VCLength <= VLength)
	{
		if (acos(CosOfAngleS)< (PI/2))
		{
			Ball->SetVelocity(BallVelocity - FVector{0.0f,0.0f, BallVelocity.Z * 2});
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("COLLIDING"));
		}
	}
	
}

// Called when the game starts or when spawned
void ACollisionPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollisionPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACollisionPlane::CosOfAngle(FVector AVector, FVector BVector)
{
	float aL = LengthOfVector(AVector);
	float bL = LengthOfVector(BVector);
	float answer = ((AVector.X * BVector.X) + (AVector.Y * BVector.Y) + (AVector.Z * BVector.Z)) / (aL * bL);
	return answer;
}

float ACollisionPlane::LengthOfVector(FVector Vector)
{
	return sqrt(pow(Vector.X, 2) + pow(Vector.Y, 2) + pow(Vector.Z, 2));
}

