// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KIMInteractionStates.generated.h"

UENUM(BlueprintType)
enum class EKIMInteractionTypes : uint8 {
	NONE = 0		UMETA(DisplayName = "NONE"),
	OnCombine = 1		UMETA(DisplayName = "OnCombine"),
	OnPickUp = 2		UMETA(DisplayName = "OnPickUp"),
	OnPressed = 3		UMETA(DisplayName = "OnPressed"),
	OnRotation = 4		UMETA(DisplayName = "OnRotation"),
};
