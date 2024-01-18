// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionPlane.h"
#include <cmath>

#include "VectorTypes.h"
#include "Components/BoxComponent.h"

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

	if (PlaneMeshAsset.Succeeded())
	{
		PlaneMesh->SetStaticMesh(PlaneMeshAsset.Object);
		PlaneMesh->SetRelativeLocation(FVector(0.0f,0.0f, -0.0f));
		PlaneMesh->SetWorldScale3D(FVector(1.f));
	}

	point1 = CreateDefaultSubobject<UBoxComponent>("point1");
	point1->SetupAttachment(PlaneMesh);
	point1->SetWorldScale3D(FVector(0.01f));
	point1->SetRelativeLocation(FVector(-1.f,0.0f,0.f));

	point2 = CreateDefaultSubobject<UBoxComponent>("point2");
	point2->SetupAttachment(PlaneMesh);
	point2->SetWorldScale3D(FVector(0.01f));
	point2->SetRelativeLocation(FVector(0.0f,1.f,0.f));
}

void ACollisionPlane::Collision(ABall* Ball)
{
	const auto a = point1->GetComponentLocation();
	const auto b = point2->GetComponentLocation();
	const auto c = GetActorLocation();
	FVector Normal = GetNormal(a, b, c);
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
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Normal, FColor::Blue);
	if (VCLength <= VLength)
	{
		if (acos(CosOfAngleS) < (PI/2))
		{
		
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HIT")));
			float dotProduct = DotProduct(BallVelocity, Normal);
			FVector velocityUnitVector= (Ball->GetBallVelocity() / VLength);
			FVector newVelocityUnitVector = (2 *Normal) * DotProduct(Normal, -velocityUnitVector) + velocityUnitVector;
			FVector NewVelocity = newVelocityUnitVector * VLength;
			//FVector NewVelocity = BallVelocity - ((2 * Normal) * DotProduct(BallVelocity, Normal));
			Ball->SetVelocity(NewVelocity);
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

FVector ACollisionPlane::GetNormal(FVector A, FVector B, FVector C)
{
	FVector D = B - C;
	FVector E = A - C;
	float NormalX = (D.Y * E.Z) - (D.Z * E.Y);
	float NormalY = (D.Z * E.X) - (D.X * E.Z);
	float NormalZ = (D.X * E.Y) - (D.Y * E.X);
	FVector Norm (NormalX, NormalY, NormalZ);
	Norm = Norm / LengthOfVector(Norm);
	return Norm;
}

float ACollisionPlane::DotProduct(FVector A, FVector B)
{
	return (A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z);
}

