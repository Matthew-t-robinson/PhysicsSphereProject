// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class PHYSICSSPHEREPROJECT_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

	UFUNCTION(BlueprintCallable)
	void Collision(ABall* CollidingBall);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool AreColliding(ABall* CollidingBall);
	UStaticMeshComponent* CubeMesh;
	float Scale = 2.f;
};
