// Copyright XiaoYao.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacter.h"
#include "Character/AuraEnemy.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interface/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CurseTrace();
	AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;

	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CacheDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
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

	// UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed,
		&AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

bool AAuraPlayerController::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return HasAnyFlags(RF_ClassDefaultObject) ? false : ProcessConsoleExec(Cmd, Ar, nullptr);
}

void AAuraPlayerController::LogActivateGameplayAbilityEffect() const
{
	if(AAuraCharacter* AuraCharacter = Cast<AAuraCharacter>(GetCharacter()))
	{
		if(UAbilitySystemComponent* AbilitySystemComponent = AuraCharacter->GetAbilitySystemComponent())
		{
			const FActiveGameplayEffectsContainer& ActiveGameplayEffectsContainer = AbilitySystemComponent->GetActiveGameplayEffects();
			for(auto It = ActiveGameplayEffectsContainer.CreateConstIterator(); It; ++It)
			{
				FGameplayEffectSpec Value = It->Spec;
				UE_LOG(LogAura, Warning, TEXT("Current GE : %s"), *Value.Def.GetName());
			}
		}
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// AD left / right, Y axis
	// WS forward / backward, X axis, character forward

	FVector2D AxisValue = InputActionValue.Get<FVector2D>();

	float ForwardBackValue = AxisValue.X;
	float LeftRightValue = AxisValue.Y;
	
	// UE_LOG(LogAura, Warning, TEXT("ForwardBackValue %s"), *FString::SanitizeFloat(ForwardBackValue));
	// UE_LOG(LogAura, Warning, TEXT("LeftRightValue %s"), *FString::SanitizeFloat(LeftRightValue))
	
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator = FRotator(0, Rotator.Yaw, 0);
	// get rotation matrix 's axis
	const FVector ControllerForward = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector ControllerRight = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
	
	// UE_LOG(LogAura, Warning, TEXT("GetControlRotation %s"), *Rotator.ToString());
	// UE_LOG(LogAura, Warning, TEXT("ControllerForward %s"), *ControllerForward.ToString());
	// UE_LOG(LogAura, Warning, TEXT("ControllerRight %s"), *ControllerRight.ToString())
	
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControllerForward, ForwardBackValue);
		ControlledPawn->AddMovementInput(ControllerRight, LeftRightValue);
	}
}

void AAuraPlayerController::CurseTrace()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	bool bHitEnemy = false;
	if(HitResult.bBlockingHit)
	{
		// UE_LOG(LogAura, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName())

		// check if it should respond to highlight
		if(HitResult.GetActor()->Implements<UEnemyInterface>())
		{
			if(AAuraEnemy* EnemyCharacter =  Cast<AAuraEnemy>(HitResult.GetActor()))
			{
				bHitEnemy = true;
				if(!EnemyCharacter->GetHighlight())
				{
					if(HighlightEnemy)
					{
						HighlightEnemy->SetHighlight(false);
					}
					HighlightEnemy = EnemyCharacter;
					HighlightEnemy->SetHighlight(true);
				}
			}
		}
	}
	if(!bHitEnemy)
	{
		if(HighlightEnemy)
		{
			HighlightEnemy->SetHighlight(false);
			HighlightEnemy = nullptr;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = HighlightEnemy ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
	// if(InputTag.IsValid())
	// {
	// 	GetASC()->AbilityInputTagReleased(InputTag);
	// }
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if(bTargeting)
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheDestination))
			{
				Spline->ClearSplinePoints();
				for(const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
	// if(InputTag.IsValid())
	// {
	// 	GetASC()->AbilityInputTagHeld(InputTag);
	// }
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if(bTargeting)
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if(GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CacheDestination = Hit.ImpactPoint;
		}
		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CacheDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}
