// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "KIMCharacter.generated.h"

UCLASS()
class KIM_API AKIMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKIMCharacter();

	// Components
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ObjectAttachmentPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool IsItemSnapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BaseLookRate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float ThrowIntensity;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	// Move
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	// Look
	void LookUp(float Value);
	void LookRight(float Value);
	
	void ThrowObject();

	void Interact();

	AActor* PickedUpItem;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float BaseRotationRate;

	// Rotate
	void RotateUp(float Value);
	void RotateRight(float Value);

	bool IsInRoationState = false;
};
