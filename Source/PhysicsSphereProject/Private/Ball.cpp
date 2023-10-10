// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	radius = 20;
	mass = 1;
	SphereComponent->InitSphereRadius(radius);
	SetRootComponent(SphereComponent);

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualRepresentation");
	SphereMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

    if (SphereMeshAsset.Succeeded())
    {
	    SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
    	SphereMesh->SetRelativeLocation(FVector(0.f,0.f,-radius));
    }
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->SetSphereRadius(radius);
	SphereMesh->SetRelativeLocation(FVector(0.f,0.f,-radius));
	float scale = radius / 50;
	SphereMesh->SetWorldScale3D(FVector(scale));
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveBall();
}

void ABall::Collision(ABall* OtherBall)
{
	FVector ball1Position = GetActorLocation();
	FVector ball2Position = OtherBall->GetActorLocation();
	FVector ball2Velocity = OtherBall->GetBallVelocity();
	float distance = sqrt(pow(ball1Position.X - ball2Position.X, 2) + pow(ball1Position.Y - ball2Position.Y, 2) + pow(ball1Position.Z - ball2Position.Z, 2));
	if (((abs(velocity.X) > 0.0001f || abs(velocity.Y) > 0.0001f || abs(velocity.Z) > 0.0001f) && (abs(ball2Velocity.X) > 0.0001f || abs(ball2Velocity.Y) > 0.0001f || abs(ball2Velocity.Z) > 0.0001f)) && distance >= radius + OtherBall->GetRadius())
	{
		movingCollision(ball1Position, ball2Position, ball2Velocity, OtherBall);
	}
	else
	{
		FVector A{ball2Position.X - ball1Position.X, ball2Position.Y - ball1Position.Y, ball2Position.Z - ball1Position.Z};
		float aLength = sqrt(pow(A.X, 2) + pow(A.Y, 2) + pow(A.Z, 2));
		float cosa = CosOfAngle(A, velocity);
		float alpha = acos(cosa);
		float d = sin(alpha) * aLength;
		if (d <= radius + OtherBall->GetRadius())
		{
			float e = sqrt(pow(radius + OtherBall->GetRadius(), 2) - pow(d, 2));
			float CollisionDistance = cosa * aLength - e;
			if (CollisionDistance >= 0 && CollisionDistance <= sqrt((pow(velocity.X, 2) + pow(velocity.Y, 2) + pow(velocity.Z, 2))))
			{
				float length = sqrt(pow(ball2Position.X - ball1Position.X, 2) + pow(ball2Position.Y - ball1Position.Y, 2) + pow(ball2Position.Z - ball1Position.Z, 2));
				FVector Fd{ (ball2Position.X - ball1Position.X) / length, (ball2Position.Y - ball1Position.Y) / length, (ball2Position.Z - ball1Position.Z) / length};
				float VelocityLength = sqrt(pow(velocity.X, 2) + pow(velocity.Y, 2) + pow(velocity.Z, 2));
				float cosa2 = ((Fd.X * velocity.X) + (Fd.Y * velocity.Y) + (Fd.Z * velocity.Z)) / (VelocityLength);
				float V2X = (cosa2 * sqrt(pow(velocity.X * mass,2) + pow(velocity.Y * mass, 2) + pow(velocity.Z * mass, 2)) * Fd.X) / OtherBall->GetMass();
				float V2Y = (cosa2 * sqrt(pow(velocity.X * mass,2) + pow(velocity.Y * mass, 2) + pow(velocity.Z * mass, 2)) * Fd.Y) / OtherBall->GetMass();
				float V2Z = (cosa2 * sqrt(pow(velocity.X * mass,2) + pow(velocity.Y * mass, 2) + pow(velocity.Z * mass, 2)) * Fd.Z) / OtherBall->GetMass();
				FVector V2{V2X, V2Y, V2Z};
				OtherBall->SetVelocity(V2);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f = FloatVariable"), V2X));
				FVector V1{(mass * velocity) - (OtherBall->GetMass() * V2)};
				SetVelocity(V1);
			}
		}
	}
}

void ABall::SetVelocity(FVector v)
{
	velocity = v;
}

void ABall::MoveBall()
{
	SetActorLocation(GetActorLocation() += velocity);
}

float ABall::GetMass()
{
	return mass;
}

float ABall::GetRadius()
{
	return radius;
}

FVector ABall::GetBallVelocity() const
{
	return velocity;
}

void ABall::movingCollision(FVector Ball1Pos, FVector Ball2Pos, FVector Ball2Velocity, ABall* OtherBall)
{
	float xp = Ball2Pos.X - Ball1Pos.X;
	float yp = Ball2Pos.Y - Ball1Pos.Y;
	float zp = Ball2Pos.Z - Ball1Pos.Z;
	float xv = Ball2Velocity.X - velocity.X;
	float yv = Ball2Velocity.Y - velocity.Y;
	float zv = Ball2Velocity.Z - velocity.Z;
	float a = (pow(xv, 2)) + (pow(yv,2)) + (pow(zv,2));
	float b = (2 * xp * xv) + (2 * yp * yv) + (2 * zp * zv);
	float c = (pow(xp, 2) + pow(yp, 2) + pow(zp, 2)) - (pow(radius + OtherBall->GetRadius(), 2));
	float t1 = (-b + sqrt((pow(b, 2)) - (4 * a * c))) / (2 * a);
	float t2 = (-b - sqrt((pow(b, 2)) - (4 * a * c))) / (2 * a);
	if ((t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1))
	{
		float cosA = CosOfAngle((Ball1Pos - Ball2Pos), Ball2Velocity);
		float cosB = CosOfAngle((Ball2Pos - Ball1Pos), velocity);

		float division = sqrt(pow(Ball1Pos.X - Ball2Pos.X, 2) + pow(Ball1Pos.Y - Ball2Pos.Y, 2) + pow(Ball1Pos.Z - Ball2Pos.Z, 2));
		FVector FD1{(Ball1Pos - Ball2Pos) / division};
		FVector FD2{-FD1};
		float v1Length = sqrt(pow(Ball2Velocity.X * OtherBall->GetMass(), 2) + pow(Ball2Velocity.Y * OtherBall->GetMass(), 2) + pow(Ball2Velocity.Z * OtherBall->GetMass(), 2)); // this works out the ball 1 velocity length
		float v2Length = sqrt(pow(velocity.X * mass, 2) + pow(velocity.Y * mass, 2) + pow(velocity.Z * mass, 2));
		FVector f12{(cosA * v1Length * FD1) / mass};
		FVector f21{(cosB * v2Length * FD2) / OtherBall->GetMass()};
		FVector V2FD{velocity + f12 - f21};
		FVector V1FD{((Ball2Velocity * OtherBall->GetMass()) + (velocity * mass) - (mass* V2FD)) / OtherBall->GetMass()};
		OtherBall->SetVelocity(V1FD);
		SetVelocity(V2FD);
	}
}

void ABall::friction()
{
	if ((0.01 > velocity.X) && (velocity.X > -0.01))
	{
		velocity.X = 0;
	}
	if ((0.01 > velocity.Y) && (velocity.Y > -0.01))
	{
		velocity.Y = 0;
	}
	if ((0.01 > velocity.Z) && (velocity.Z > -0.01))
	{
		velocity.Z = 0;
	}
	if (velocity.X > 0)
	{
		velocity.X = velocity.X * 0.99;
	}
	if (velocity.X < 0)
	{
		velocity.X = velocity.X * 0.99;
	}
	if (velocity.Y > 0)
	{
		velocity.Y = velocity.Y * 0.99;
	}
	if (velocity.Y < 0)
	{
		velocity.Y = velocity.Y * 0.99;
	}
	if (velocity.Z > 0)
	{
		velocity.Z = velocity.Z * 0.99;
	}
	if (velocity.Z < 0)
	{
		velocity.Z = velocity.Z * 0.99;
	}
}

float ABall::CosOfAngle(FVector AVector, FVector BVector)
{
	float aL = sqrt(pow(AVector.X, 2) + pow(AVector.Y, 2) + pow(AVector.Z, 2));
	float bL = sqrt(pow(BVector.X, 2) + pow(BVector.Y, 2) + pow(BVector.Z, 2));
	float answer = ((AVector.X * BVector.X) + (AVector.Y * BVector.Y) + (AVector.Z * BVector.Z)) / (aL * bL);
	return answer;
}

