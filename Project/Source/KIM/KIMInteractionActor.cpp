// Fill out your copyright notice in the Description page of Project Settings.

#include "KIM.h"
#include "KIMInteractionActor.h"


// Sets default values
AKIMInteractionActor::AKIMInteractionActor() {
	InteractionType = EKIMInteractionTypes::NONE;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AKIMInteractionActor::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AKIMInteractionActor::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

}


void AKIMInteractionActor::Interacted(AKIMCharacter* Character) {
	switch (InteractionType)
	{
	case (EKIMInteractionTypes::NONE) :

		break;
	case (EKIMInteractionTypes::OnCombine) :
		if (Character->PickedUpItem && GetName().Contains("Station",ESearchCase::IgnoreCase)) {
			PickUpAbleItem = (AKIMInteractionActor*) Character->PickedUpItem;
			InteractionType = EKIMInteractionTypes::NONE;
		}
		if (Character->PickedUpItem) {
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem->AttachRootComponentToActor(this, NAME_None, EAttachLocation::SnapToTarget);
			Character->PickedUpItem = NULL;
			Activated();
		}
		break;
	case (EKIMInteractionTypes::OnPickUp) :
		if (GetName().Contains("Station", ESearchCase::IgnoreCase)) {
			PickUpAbleItem->DetachRootComponentFromParent(true);
			PickUpAbleItem->AttachRootComponentToActor(Character, NAME_None, EAttachLocation::KeepWorldPosition);
			Character->PickedUpItem = PickUpAbleItem;
			PickUpAbleItem = NULL;
			InteractionType = EKIMInteractionTypes::OnCombine;
		}
		else {
			AttachRootComponentToActor(Character, NAME_None, EAttachLocation::KeepWorldPosition);
			Character->PickedUpItem = this;
		}
		break;
	case (EKIMInteractionTypes::OnPressed) :
		Activated();
		break;
	}
}


void AKIMInteractionActor::FinishCharging() {
	InteractionType = EKIMInteractionTypes::OnPickUp;
}