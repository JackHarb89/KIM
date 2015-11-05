// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KIMCharacter.h"
#include "KIMInteractionActor.generated.h"

UENUM(BlueprintType)
enum class EKIMInteractionTypes : uint8 {
	NONE		= 0		UMETA(DisplayName = "NONE"),
	OnCombine   = 1		UMETA(DisplayName = "OnCombine"),
	OnPickUp	= 2		UMETA(DisplayName = "OnPickUp"),
	OnPressed	= 3		UMETA(DisplayName = "OnPressed"),
};

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

	void Interacted(AKIMCharacter* Character);

	AKIMInteractionActor* PickUpAbleItem = NULL;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Activated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Thrown(float Intensity);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void PickedUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Droped();

	FTimerHandle handle;

	UFUNCTION(BlueprintCallable, Category = "Interaction") 
		void FinishCharging();
};
