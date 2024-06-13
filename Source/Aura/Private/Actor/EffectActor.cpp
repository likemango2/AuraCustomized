// Copyright XiaoYao.


#include "Actor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC) return;

	if(!GameplayEffectClass) return;
	
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	// TargetASC->ApplyGameplayEffectToSelf(EffectSpecHandle.Data.Get()->Def, 1.0f, EffectContextHandle);

	/*
	 * there are different functions, but finally they will call ApplyGameplayEffectSpecToSelf
	 */
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

/*
void AEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		UE_LOG(LogAura, Warning, TEXT("OnBeginOverlap, hit actor: %s"), *OtherActor->GetName());

		// if(AAuraCharacterBase* AuraCharacterBase = Cast<AAuraCharacterBase>(OtherActor))
		// {
		// 	UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(AuraCharacterBase->GetAttributeSet());
		// 	AttributeSet->SetHealth(80);
		// }

		if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			const UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
			UAuraAttributeSet* MutableAttributeSet = const_cast<UAuraAttributeSet*>(AttributeSet);
			UE_LOG(LogAura, Warning, TEXT("Health = %f"), AttributeSet->GetHealth())
			MutableAttributeSet->SetHealth(40.f);
			UE_LOG(LogAura, Warning, TEXT("Modified Health = %f"), AttributeSet->GetHealth())
			
			UE_LOG(LogAura, Warning, TEXT("Mana = %f"), AttributeSet->GetMana())
			MutableAttributeSet->SetMana(AttributeSet->GetMana() - 25.f);
			UE_LOG(LogAura, Warning, TEXT("Modified Mana = %f"), AttributeSet->GetMana())
			Destroy();
		} 
	}
}*/
