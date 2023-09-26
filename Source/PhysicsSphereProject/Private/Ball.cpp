// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABall::ABall(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	b_radius = FMath::RandRange(1, 10);
	b_mass = FMath::RandRange(1, 10);
	SphereComponent->InitSphereRadius(b_radius);

	UStaticMeshComponent* SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualReprisentation");
	SphereMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

    if (SphereMeshAsset.Succeeded())
    {
    	
    }
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

