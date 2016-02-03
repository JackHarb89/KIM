// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "KIMInteractionStates.generated.h"

UENUM(BlueprintType)
enum class EKIMInteractionTypes : uint8 {
	NONE = 0		UMETA(DisplayName = "NONE"),
	OnCombine = 1		UMETA(DisplayName = "OnCombine"),
	OnPickUp = 2		UMETA(DisplayName = "OnPickUp"),
	OnPressed = 3		UMETA(DisplayName = "OnPressed"),
	OnRotation = 4		UMETA(DisplayName = "OnRotation"),
};

USTRUCT(BlueprintType)
struct FKIMDialogue {
	GENERATED_USTRUCT_BODY()

public:
	FKIMDialogue()
			: Interaction("")
			, Dialogue(""){
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		FName Interaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		FName Dialogue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		USoundWave* Audio;
};

UENUM(BlueprintType)
enum class EKIMConnectionColor : uint8 {
	Normal	= 0		UMETA(DisplayName = "Normal"),
	Blue	= 1		UMETA(DisplayName = "Blue"),
	Green	= 2		UMETA(DisplayName = "Green"),
	Red		= 3		UMETA(DisplayName = "Red"),
};

USTRUCT(BlueprintType)
struct FKIMConnectionDirection {
	GENERATED_USTRUCT_BODY()

public:
	FKIMConnectionDirection()
		: UpConnection(false)
		, DownConnection(false)
		, RightConnection(false)
		, LeftConnection(false) {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		bool UpConnection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		bool DownConnection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		bool RightConnection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		bool LeftConnection;
};

USTRUCT(BlueprintType)
struct FKIMGridControl {
	GENERATED_USTRUCT_BODY()

public:
	FKIMGridControl()
		: Module()
		, Row(0)
		, Column(0){
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		UUserWidget* Module;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyPuzzle")
		int32 Column;
};
