// Fill out your copyright notice in the Description page of Project Settings.

#include "KIM.h"
#include "KIMCharacter.h"
#include "KIMInteractionActor.h"


// Sets default values
AKIMCharacter::AKIMCharacter() {
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	CameraComponent->AttachTo(RootComponent);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->AddRelativeLocation(FVector(0, 0, 0));

	// Rotate when the controller rotates
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseLookRate = 45.f;

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

	InputComponent->BindAction("Interact", IE_Released, this, &AKIMCharacter::Interact);

}

void AKIMCharacter::MoveForward(float Value) {
	if (PickedUpItem) {
		if (Value < -0.1f) {
			Value += FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, Value * (Value / abs(Value)));
		}
		else if (Value > 0.1f){
			Value -= FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, Value * (Value / abs(Value)));
		}
		else return;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AKIMCharacter::MoveRight(float Value) {
	if (PickedUpItem) {
		if (Value < -0.1f) {
			Value += FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, Value * (Value / abs(Value)));
		}
		else if (Value > 0.1f) {
			Value -= FMath::Clamp((((AKIMInteractionActor*)PickedUpItem)->Weight / 100), 0.f, Value * (Value / abs(Value)));
		}
		else return;
	}
	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	AddMovementInput(Direction, Value);
}

void AKIMCharacter::LookUp(float Value) {
	AddControllerPitchInput(Value * BaseLookRate * GetWorld()->GetDeltaSeconds());
}

void AKIMCharacter::LookRight(float Value) {
	AddControllerYawInput(Value * BaseLookRate * GetWorld()->GetDeltaSeconds());
}
#pragma endregion

// Called when the game starts or when spawned
void AKIMCharacter::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AKIMCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AKIMCharacter::Interact() {
	FHitResult outHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(PickedUpItem);
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComponent->GetForwardVector() * 250);
	GetWorld()->LineTraceSingleByChannel(outHit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldDynamic, params);
	GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(GetController(), outHit.Location);
	if (outHit.GetActor() != NULL && outHit.GetActor()->GetClass()->IsChildOf(AKIMInteractionActor::StaticClass())) {
		AKIMInteractionActor* InteractionActor = ((AKIMInteractionActor*)outHit.GetActor());
		InteractionActor->Interacted(this);
		UE_LOG(LogClass, Warning, TEXT("Interacted with %s"), *outHit.GetActor()->GetName());
	}
	else if (outHit.GetActor() == NULL && PickedUpItem) {
		PickedUpItem->DetachRootComponentFromParent(true);
		UE_LOG(LogClass, Warning, TEXT("Droped %s"), *PickedUpItem->GetName());
		PickedUpItem = NULL;
	}
}