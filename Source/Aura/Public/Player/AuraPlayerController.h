// Copyright XiaoYao.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/ObjectPtr.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraUserWidget;
class AAuraEnemy;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController, public FSelfRegisteringExec
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	UFUNCTION(Exec)
	void LogActivateGameplayAbilityEffect() const;
	
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	void CurseTrace();

	UPROPERTY(VisibleAnywhere)
	AAuraEnemy* HighlightEnemy;
};
