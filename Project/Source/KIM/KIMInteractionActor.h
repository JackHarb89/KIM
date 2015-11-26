// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KIMCharacter.h"
#include "KIMInteractionActor.generated.h"

UCLASS()
class KIM_API AKIMInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKIMInteractionActor();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		EKIMInteractionTypes InteractionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Weight;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Interacted(AKIMCharacter* Character, UPrimitiveComponent* Component);

	AKIMInteractionActor* PickUpAbleItem = NULL;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Activated(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Thrown(float Intensity);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void PickedUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Droped();

	FTimerHandle handle;

	UFUNCTION(BlueprintCallable, Category = "Interaction") 
		void FinishCharging();

	FTransform StoredTransform;
	FTransform TargetTransform;

	bool IsAnimationEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		bool IsPickedUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		bool FoundDesiredRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		bool InteractionFinished = false;

	void LayBack(AKIMCharacter* Character);
	
	void AnimatePickUp(float DeltaSeconds);
	void AnimateLayBack(float DeltaSeconds);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FRotator DesiredRotation = FRotator::ZeroRotator;

	bool IsLerpingToDesiredRotation = false;

	void CheckRotationForDesired();
	void LerpToDesiredLocation(float DeltaSeconds);

	// Room 1 Puzzle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool IsDoorControlOpen = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void PlacedBattery();


	
	// Dialogue

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		TArray<FKIMDialogue> DialogueTexts;

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
		void StartDialogue(const FName Dialogue, USoundWave* AudioWave);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void GetInteractionDialogue(const FName Interaction);

};
