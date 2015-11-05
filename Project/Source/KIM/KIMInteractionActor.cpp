// Fill out your copyright notice in the Description page of Project Settings.

#include "KIM.h"
#include "KIMInteractionActor.h"


// Sets default values
AKIMInteractionActor::AKIMInteractionActor() {
	InteractionType = EKIMInteractionTypes::NONE;

	Weight = 50;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKIMInteractionActor::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AKIMInteractionActor::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	if (IsAnimationEnabled){
		if (IsPickedUp) {
			AnimatePickUp(DeltaTime*10);
		}
		else {
			AnimateLayBack(DeltaTime * 10);
		}
	}
}


void AKIMInteractionActor::Interacted(AKIMCharacter* Character) {
	switch (InteractionType)
	{
	case (EKIMInteractionTypes::NONE) :
		break;
	case (EKIMInteractionTypes::OnCombine) :
		/*if (Character->PickedUpItem && GetName().Contains("Station",ESearchCase::IgnoreCase)) {
			PickUpAbleItem = (AKIMInteractionActor*) Character->PickedUpItem;
			InteractionType = EKIMInteractionTypes::NONE;
		}
		if (Character->PickedUpItem) {
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem->AttachRootComponentToActor(this, NAME_None, EAttachLocation::SnapToTarget);
			Character->PickedUpItem = NULL;
			Activated();
		}*/
		if (Character->PickedUpItem) {
			Character->PickedUpItem->Destroy();
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem = NULL;
		}
		break;
	case (EKIMInteractionTypes::OnPickUp) :
		/*if (GetName().Contains("Station", ESearchCase::IgnoreCase)) {
			PickUpAbleItem->DetachRootComponentFromParent(true);
			PickUpAbleItem->AttachRootComponentToActor(Character, NAME_None, EAttachLocation::KeepWorldPosition);
			Character->PickedUpItem = PickUpAbleItem;
			PickUpAbleItem = NULL;
			InteractionType = EKIMInteractionTypes::OnCombine;
		}
		else {
			AttachRootComponentToActor(Character, NAME_None, EAttachLocation::KeepWorldPosition);
			Character->PickedUpItem = this;
		}*/
		PickedUp();
		AttachRootComponentTo(Character->ObjectAttachmentPoint, NAME_None, (Character->IsItemSnapping ? EAttachLocation::SnapToTarget : EAttachLocation::KeepWorldPosition));
		Character->PickedUpItem = this;
		break;
	case (EKIMInteractionTypes::OnPressed) :
		Activated();
		break;
	case (EKIMInteractionTypes::OnRotation) :
		IsAnimationEnabled = true;
		StoredTransform = GetTransform();
		TargetTransform = Character->ObjectAttachmentPoint->GetComponentTransform();
		IsPickedUp = true;
		Character->IsInRoationState = IsPickedUp;
		Character->PickedUpItem = this;
		break;
	}
}

void AKIMInteractionActor::FinishCharging() {
	InteractionType = EKIMInteractionTypes::OnPickUp;
}


void AKIMInteractionActor::AnimatePickUp(float DeltaSeconds) {
	SetActorLocation(FMath::Lerp(GetActorLocation(), TargetTransform.GetLocation(), DeltaSeconds));
	SetActorRotation(FMath::Lerp(GetActorRotation().Quaternion(), TargetTransform.GetRotation(), DeltaSeconds));

	if (GetActorLocation().Equals(TargetTransform.GetLocation(), 1) && GetActorRotation().Equals(TargetTransform.GetRotation().Rotator(), 1)) {
		SetActorLocation(TargetTransform.GetLocation());
		SetActorRotation(TargetTransform.GetRotation());
		IsAnimationEnabled = false;
		UE_LOG(LogClass, Warning, TEXT("PickUp Animation Finished"));
	}
}

void AKIMInteractionActor::AnimateLayBack(float DeltaSeconds) {
	SetActorLocation(FMath::Lerp(GetActorLocation(), StoredTransform.GetLocation(), DeltaSeconds));
	SetActorRotation(FMath::Lerp(GetActorRotation().Quaternion(), StoredTransform.GetRotation(), DeltaSeconds));
	if (GetActorLocation().Equals(StoredTransform.GetLocation(), 1) && GetActorRotation().Equals(StoredTransform.GetRotation().Rotator(), 1)) {
		SetActorLocation(StoredTransform.GetLocation());
		SetActorRotation(StoredTransform.GetRotation());
		IsAnimationEnabled = false;
		UE_LOG(LogClass, Warning, TEXT("LayBack Animation Finished"));
	}
}


void AKIMInteractionActor::LayBack() {
	IsAnimationEnabled = true;
	IsPickedUp = false;
}