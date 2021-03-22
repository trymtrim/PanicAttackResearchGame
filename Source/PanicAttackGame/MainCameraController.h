// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCameraController.generated.h"

USTRUCT(BlueprintType)
struct FCameraMovement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FVector TargetPosition;
	UPROPERTY(EditAnywhere) float MoveTime;
	UPROPERTY(EditAnywhere) float WaitTimeAfterMoving;
};

UCLASS()
class PANICATTACKGAME_API AMainCameraController : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainCameraController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere) ACameraActor* camera;
	UPROPERTY(EditAnywhere) FVector startPosition;
	UPROPERTY(EditAnywhere) float waitTimeBeforeMoving;
	UPROPERTY(EditAnywhere) TArray<FCameraMovement> cameraMovements;

private:
	void StartNextCameraMovement();

	FVector Slerp(const FVector& a, const FVector& b, const float t);

	int currentMovementIndex = -1;
	float currentMovementTime = 0.0f;
	FVector currentStartPosition;
	bool cameraMovementIsActive = false;

	int totalCameraMovements;
};
