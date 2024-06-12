// Copyright XiaoYao.


#include "Actor/EffectActor.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacter.h"
#include "Components/SphereComponent.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>("SphereComp");
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetupAttachment(Mesh);
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::OnEndOverlap);
}

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
}

void AEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		UE_LOG(LogAura, Warning, TEXT("OnEndOverlap, hit actor: %s"), *OtherActor->GetName());
	}
}


