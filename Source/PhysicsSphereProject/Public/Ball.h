// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class PHYSICSSPHEREPROJECT_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Collision(ABall* OtherBall);

	void SetVelocity(FVector v);

	UFUNCTION(BlueprintCallable)
	void MoveBall();

	float GetMass();

	float GetRadius();
	
	UFUNCTION(BlueprintCallable)
	FVector GetBallVelocity() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Specifications")
	FVector velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Specifications")
	float mass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Specifications")
	float radius;

private:

	void friction();
	
	float CosOfAngle(FVector AVector, FVector BVector);

	void movingCollision(FVector Ball1Pos, FVector Ball2Pos, FVector Ball2Velocity, ABall* OtherBall);

	UStaticMeshComponent* SphereMesh;
};
