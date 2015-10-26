// Fill out your copyright notice in the Description page of Project Settings.

#include "KIM.h"
#include "KIMGameMode.h"
#include "KIMCharacter.h"
#include "KIMPlayerController.h"

AKIMGameMode::AKIMGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	// use our custom PlayerController class
	PlayerControllerClass = AKIMPlayerController::StaticClass();
	DefaultPawnClass = AKIMCharacter::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<AKIMCharacter> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character_BP"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassBPClass(TEXT("/Game/Blueprint/UI/KIMHUD_BP"));
	if (HUDClassBPClass.Class != NULL) {
		HUDClass = HUDClassBPClass.Class;
	}
}


