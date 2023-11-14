// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/Actor.h"
#include "CollisionPlane.generated.h"

class UBoxComponent;
UCLASS()
class PHYSICSSPHEREPROJECT_API ACollisionPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollisionPlane();

	UFUNCTION(BlueprintCallable)
	void Collision(ABall* Ball);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* PlaneMesh;

	float CosOfAngle(FVector AVector, FVector BVector);
	float LengthOfVector(FVector Vector);
	FVector GetNormal(FVector A, FVector B, FVector C);
	float DotProduct(FVector A, FVector B);

	UBoxComponent* point1;
	UBoxComponent* point2;
};
