// Fill out your copyright notice in the Description page of Project Settings.

#include "KIM.h"
#include "KIMCharacter.h"
#include "KIMInteractionActor.h"
#include "KIMPlayerController.h"


// Sets default values
AKIMCharacter::AKIMCharacter() {
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	CameraComponent->AttachTo(RootComponent);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->AddRelativeLocation(FVector(0, 0, 0));

	ObjectAttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionObjectPoint"));
	ObjectAttachmentPoint->AttachTo(CameraComponent);
	ObjectAttachmentPoint->AddRelativeLocation(FVector(0, 0, 0));


	// Rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	InteractionDistance = 250.f;
	BaseLookRate = 45.f;
	BaseRotationRate = 45.f;
	MovementSpeed = 1;  
	ThrowIntensity = 5000;
	PickedUpItem = NULL;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
#pragma region Basic Input
// Called to bind functionality to input
void AKIMCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AKIMCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AKIMCharacter::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AKIMCharacter::LookUp);
	InputComponent->BindAxis("LookRight", this, &AKIMCharacter::LookRight);
	
	InputComponent->BindAction("Throw", IE_Released, this, &AKIMCharacter::ThrowObject);
	InputComponent->BindAction("Interact", IE_Released, this, &AKIMCharacter::Interact);

}

void AKIMCharacter::MoveForward(float Value) {
	if (Value < -0.09f || Value > 0.09f) {
		if (IsInRoationState) {
			RotateUp(Value * 10);
			return;
		}
		UE_LOG(LogClass, Warning, TEXT("Value = %.2f"), Value);
		if (PickedUpItem) {
			if (Value < -0.08f) {
				Value += FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, abs(Value));
			}
			else if (Value > 0.08f) {
				Value -= FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, abs(Value));
			}
			else return;
		}
		Value *= MovementSpeed; 
		MovedForward(1 + 0.1*MovementSpeed);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AKIMCharacter::MoveRight(float Value) {
	if (Value < -0.09f || Value > 0.09f) {
		if (IsInRoationState) {
			RotateRight(Value*10);
			return;
		}
		if (PickedUpItem) {
			if (Value < -0.09f) {
				Value += FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, abs(Value));
			}
			else if (Value > 0.09f) {
				Value -= FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f,  abs(Value));
			}
			else return;
		}

		Value *= MovementSpeed;

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AKIMCharacter::LookUp(float Value) {
	if (IsInRoationState) {
		return;
	}
	CameraComponent->AddRelativeRotation(FRotator(Value * BaseLookRate * GetWorld()->GetDeltaSeconds(), 0, 0));
	//AddControllerPitchInput(Value * BaseLookRate * GetWorld()->GetDeltaSeconds());
}

void AKIMCharacter::LookRight(float Value) {
	if (IsInRoationState) {
		return;
	}
	//CameraComponent->AddRelativeRotation(FRotator(0, Value * BaseLookRate * GetWorld()->GetDeltaSeconds(), 0));
	AddControllerYawInput(Value * BaseLookRate * GetWorld()->GetDeltaSeconds());
}

void AKIMCharacter::RotateUp(float Value) {
	if (((AKIMInteractionActor*)PickedUpItem)->IsAnimationEnabled || ((AKIMInteractionActor*)PickedUpItem)->IsLerpingToDesiredRotation || ((AKIMInteractionActor*)PickedUpItem)->FoundDesiredRotation) return;
	PickedUpItem->AddActorWorldRotation(FRotator(0, 0, Value * BaseRotationRate * GetWorld()->GetDeltaSeconds()));
}

void AKIMCharacter::RotateRight(float Value) {
	if (((AKIMInteractionActor*)PickedUpItem)->IsAnimationEnabled || ((AKIMInteractionActor*)PickedUpItem)->IsLerpingToDesiredRotation || ((AKIMInteractionActor*)PickedUpItem)->FoundDesiredRotation) return;
	PickedUpItem->AddActorWorldRotation(FRotator(0, Value * BaseRotationRate * GetWorld()->GetDeltaSeconds(), 0));
}

void AKIMCharacter::ThrowObject() {
	if (PickedUpItem && !IsInRoationState) {
		PickedUpItem->DetachRootComponentFromParent(true);
		((AKIMInteractionActor*)PickedUpItem)->Thrown(ThrowIntensity);
		((AKIMInteractionActor*)PickedUpItem)->InteractionType = EKIMInteractionTypes::OnPickUp;
		UE_LOG(LogClass, Warning, TEXT("Threw %s"), *PickedUpItem->GetName());

		PickedUpItem = NULL;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("Nothing to throw"));
	}
}

#pragma endregion

// Called when the game starts or when spawned
void AKIMCharacter::BeginPlay() {
	Super::BeginPlay();
	if (GetController()) {
		((AKIMPlayerController*)GetController())->bShowMouseCursor = true;
	}
}

// Called every frame
void AKIMCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	CheckTraceDistance();
}

void AKIMCharacter::Interact() {
	if (IsInRoationState && PickedUpItem) {
			((AKIMInteractionActor*)PickedUpItem)->LayBack(this);
			return;
	}

	FHitResult outHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(PickedUpItem);

	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComponent->GetForwardVector() * InteractionDistance);

	GetWorld()->LineTraceSingleByChannel(outHit, TraceStart, TraceEnd, ECollisionChannel::ECC_PhysicsBody, params);

	if (outHit.GetActor() != NULL && outHit.GetActor()->GetClass()->IsChildOf(AKIMInteractionActor::StaticClass())) {
		AKIMInteractionActor* InteractionActor = ((AKIMInteractionActor*)outHit.GetActor());
		InteractionActor->Interacted(this, outHit.GetComponent());
		UE_LOG(LogClass, Warning, TEXT("Interacted with %s"), *outHit.GetActor()->GetName());
		UE_LOG(LogClass, Warning, TEXT("(%s)"), *outHit.GetComponent()->GetName());
	}
	else if (outHit.GetActor() == NULL && PickedUpItem) {
		PickedUpItem->DetachRootComponentFromParent(true);
		((AKIMInteractionActor*)PickedUpItem)->Droped();
		((AKIMInteractionActor*)PickedUpItem)->InteractionType = EKIMInteractionTypes::OnPickUp;
		UE_LOG(LogClass, Warning, TEXT("Droped %s"), *PickedUpItem->GetName());
		PickedUpItem = NULL;
	}
}


void AKIMCharacter::CheckTraceDistance() {
	if (IsInRoationState) {
		return;
	}

	FHitResult outHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(PickedUpItem);

	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComponent->GetForwardVector() * InteractionDistance);

	GetWorld()->LineTraceSingleByChannel(outHit, TraceStart, TraceEnd, ECollisionChannel::ECC_PhysicsBody, params);
	if (outHit.GetActor() != NULL && outHit.GetActor()->GetClass()->IsChildOf(AKIMInteractionActor::StaticClass())) {
		if (((AKIMInteractionActor*)outHit.GetActor())->InteractionType != StoredType){
			StoredType = ((AKIMInteractionActor*)outHit.GetActor())->InteractionType;
			SwitchIconState(StoredType);
		}
	}
	else {
		if (StoredType != EKIMInteractionTypes::NONE) {
			StoredType = EKIMInteractionTypes::NONE;
			SwitchIconState(StoredType);
		}
	}

}