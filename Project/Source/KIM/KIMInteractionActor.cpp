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
	else {
		if (IsPickedUp && InteractionType == EKIMInteractionTypes::OnRotation) {
			if (!FoundDesiredRotation) {
				CheckRotationForDesired();
			}
			if (IsLerpingToDesiredRotation) {
				LerpToDesiredLocation(DeltaTime);
			}
		}
	}
}


void AKIMInteractionActor::Interacted(AKIMCharacter* Character, UPrimitiveComponent* Component) {
	switch (InteractionType)
	{
	case (EKIMInteractionTypes::NONE) :
		break;
	case (EKIMInteractionTypes::OnCombine) :
		if (Character->PickedUpItem) {
			Character->PickedUpItem->Destroy();
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem = NULL;
		}
		break;
	case (EKIMInteractionTypes::OnPickUp) :
		if (Character->PickedUpItem){
			break;
		}
		PickedUp();
		AttachRootComponentTo(Character->ObjectAttachmentPoint, NAME_None, (Character->IsItemSnapping ? EAttachLocation::SnapToTarget : EAttachLocation::KeepWorldPosition));
		Character->PickedUpItem = this;
		break;
	case (EKIMInteractionTypes::OnPressed) :
		if (GetName().Contains("Control", ESearchCase::IgnoreCase) && IsDoorControlOpen && Character->IsBatteryAcquired){
			PlacedBattery();
			Character->IsBatteryAcquired = false;
		}
		else if (GetName().Contains("Control", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		else if (GetName().Contains("Screw", ESearchCase::IgnoreCase) && Character->PickedUpItem && Character->PickedUpItem->GetName().Contains("ScrewDriver", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		else if (GetName().Contains("Screw", ESearchCase::IgnoreCase)) {
			GetInteractionDialogue("Press");
		}
		else if (GetName().Contains("Locker", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		else if (GetName().Contains("Door", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		else if (GetName().Contains("Console", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		else if (GetName().Contains("Vent", ESearchCase::IgnoreCase) && Character->PickedUpItem && Character->PickedUpItem->GetName().Contains("DuctTape", ESearchCase::IgnoreCase)) {
			Activated(Component, false);
			Character->PickedUpItem->Destroy();
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem = NULL;
		}
		else if (GetName().Contains("Vent", ESearchCase::IgnoreCase) && Character->PickedUpItem && Character->PickedUpItem->GetName().Contains("Blanket", ESearchCase::IgnoreCase)) {
			Activated(Component, true);
			Character->PickedUpItem->Destroy();
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem = NULL;
		}
		else if (GetName().Contains("Vent", ESearchCase::IgnoreCase) && Character->PickedUpItem && Character->PickedUpItem->GetName().Contains("Socks", ESearchCase::IgnoreCase)) {
			Activated(Component, false, true);
			Character->PickedUpItem->Destroy();
			Character->PickedUpItem->DetachRootComponentFromParent(true);
			Character->PickedUpItem = NULL;
		}
		else if (GetName().Contains("Oven", ESearchCase::IgnoreCase)) {
			Activated(Component);
		}
		break;
	case (EKIMInteractionTypes::OnRotation) :
		if (Character->PickedUpItem) {
			break;
		}
		IsAnimationEnabled = true;
		StoredTransform = GetTransform();
		TargetTransform = Character->ObjectAttachmentPoint->GetComponentTransform();
		IsPickedUp = true;
		Character->IsInRoationState = IsPickedUp;
		Character->PickedUpItem = this;
		GetInteractionDialogue("OnPickup");
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
		TargetTransform.SetRotation(FRotator(GetActorRotation().Pitch * -1 + DesiredRotation.Pitch, GetActorRotation().Yaw  + DesiredRotation.Yaw, 0).Quaternion());
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

void AKIMInteractionActor::LayBack(AKIMCharacter* Character) {
	if (IsLerpingToDesiredRotation) {
		return;
	}
	if (FoundDesiredRotation && !InteractionFinished && InteractionType != EKIMInteractionTypes::NONE) {
		Activated(NULL);
	}
	else if (FoundDesiredRotation && InteractionType != EKIMInteractionTypes::NONE) {
		InteractionType = EKIMInteractionTypes::NONE;
		Activated(NULL);
		if (GetName().Contains("Ghettoblaster", ESearchCase::IgnoreCase)) {
			Character->IsBatteryAcquired = true;
		}
	}
	else {
		Character->PickedUpItem->DetachRootComponentFromParent(true);
		Character->IsInRoationState = false;
		Character->PickedUpItem = NULL;
		IsAnimationEnabled = true;
		IsPickedUp = false;
		UE_LOG(LogClass, Warning, TEXT("Layed Back %s"), *GetName());
	}
}

void AKIMInteractionActor::CheckRotationForDesired() {
	if (GetActorRotation().Equals(TargetTransform.GetRotation().Rotator(), 25)) {
		GetInteractionDialogue("FoundSpot");
		IsLerpingToDesiredRotation = true;
		FoundDesiredRotation = true;
		UE_LOG(LogClass, Warning, TEXT("Found desired rotation"));
	}
}

void AKIMInteractionActor::LerpToDesiredLocation(float DeltaSeconds) {
	SetActorRotation(FMath::Lerp(GetActorRotation().Quaternion(), TargetTransform.GetRotation(), DeltaSeconds));
	if (GetActorRotation().Equals(TargetTransform.GetRotation().Rotator(), 1)) {
		SetActorRotation(TargetTransform.GetRotation());
		IsLerpingToDesiredRotation = false;
		UE_LOG(LogClass, Warning, TEXT("Rotated to desire"));
	}
}

void AKIMInteractionActor::GetInteractionDialogue(const FName Interaction) {
	TArray<FKIMDialogue> Dialogues;
	for (FKIMDialogue InteractionDialogue : DialogueTexts) {
		if (InteractionDialogue.Interaction.Compare(Interaction) == 0) {
			Dialogues.Add(InteractionDialogue);
		}
	}

	if (Dialogues.Num() == 0) {
		StartDialogue(TEXT("No Dialogue found!"),NULL);
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, Dialogues.Num() - 1);
	StartDialogue(Dialogues[RandomIndex].Dialogue, Dialogues[RandomIndex].Audio);
}