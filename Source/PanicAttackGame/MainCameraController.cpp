// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCameraController.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"

AMainCameraController::AMainCameraController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCameraController::BeginPlay()
{
	Super::BeginPlay();

	totalCameraMovements = cameraMovements.Num();
	camera->SetActorLocation(startPosition);

	if (waitTimeBeforeMoving == 0.0f)
		StartNextCameraMovement();
	else
	{
		FTimerDelegate startNewRoundTimerDelegate = FTimerDelegate::CreateUObject(this, &AMainCameraController::StartNextCameraMovement);
		FTimerHandle startNewRoundTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(startNewRoundTimerHandle, startNewRoundTimerDelegate, waitTimeBeforeMoving, false);
	}
}

void AMainCameraController::StartNextCameraMovement()
{
	if (currentMovementIndex == totalCameraMovements - 1)
		return;

	currentMovementIndex++;
	currentMovementTime = 0.0f;
	currentStartPosition = camera->GetActorLocation();
	cameraMovementIsActive = true;
}

void AMainCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!cameraMovementIsActive)
		return;

	currentMovementTime += DeltaTime;

	FCameraMovement cameraMovement = cameraMovements[currentMovementIndex];
	float t = currentMovementTime / cameraMovement.MoveTime;

	FVector newCameraPosition = Slerp(currentStartPosition, cameraMovement.TargetPosition, t);
	camera->SetActorLocation(newCameraPosition);

	if (t >= 1.0f)
	{
		cameraMovementIsActive = false;

		if (cameraMovement.WaitTimeAfterMoving == 0.0f)
			StartNextCameraMovement();
		else
		{
			FTimerDelegate startNewRoundTimerDelegate = FTimerDelegate::CreateUObject(this, &AMainCameraController::StartNextCameraMovement);
			FTimerHandle startNewRoundTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(startNewRoundTimerHandle, startNewRoundTimerDelegate, cameraMovement.WaitTimeAfterMoving, false);
		}
	}
}

FVector AMainCameraController::Slerp(const FVector& a, const FVector& b, const float t)
{
	float omega = FGenericPlatformMath::Acos(FVector::DotProduct(a.GetSafeNormal(), b.GetSafeNormal()));
	float sinOmega = FGenericPlatformMath::Sin(omega);

	FVector termOne = a * (FGenericPlatformMath::Sin(omega * (1.0 - t)) / sinOmega);
	FVector termTwo = b * (FGenericPlatformMath::Sin(omega * (t)) / sinOmega);

	return termOne + termTwo;
}
