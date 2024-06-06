// Copyright XiaoYao.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Aura/Aura.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraMappingContext);
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(AuraMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeGameAndUI);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// AD left / right, Y axis
	// WS forward / backward, X axis, character forward

	FVector2D AxisValue = InputActionValue.Get<FVector2D>();

	float ForwardBackValue = AxisValue.X;
	float LeftRightValue = AxisValue.Y;
	
	UE_LOG(LogAura, Warning, TEXT("ForwardBackValue %s"), *FString::SanitizeFloat(ForwardBackValue));
	UE_LOG(LogAura, Warning, TEXT("LeftRightValue %s"), *FString::SanitizeFloat(LeftRightValue))
	
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator = FRotator(0, Rotator.Yaw, 0);
	// get rotation matrix 's axis
	const FVector ControllerForward = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector ControllerRight = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
	
	UE_LOG(LogAura, Warning, TEXT("GetControlRotation %s"), *Rotator.ToString());
	UE_LOG(LogAura, Warning, TEXT("ControllerForward %s"), *ControllerForward.ToString());
	UE_LOG(LogAura, Warning, TEXT("ControllerRight %s"), *ControllerRight.ToString())
	
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControllerForward, ForwardBackValue);
		ControlledPawn->AddMovementInput(ControllerRight, LeftRightValue);
	}
}
