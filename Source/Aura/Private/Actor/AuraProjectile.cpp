// Copyright XiaoYao.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionObjectType(ECC_Projectile);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp->InitialSpeed = 550.f;
	ProjectileMovementComp->MaxSpeed = 550.f;
	ProjectileMovementComp->ProjectileGravityScale = .0f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetLifeSpan(LiftSpan);
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	}
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent()); 
}

void AAuraProjectile::Destroyed()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	
	if(LoopingSoundComponent)
		LoopingSoundComponent->Stop();
	
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(DamageGameplayEffectSpecHandle.Data.IsValid())
	{
		const AActor* EffectCauser = DamageGameplayEffectSpecHandle.Data->GetContext().GetEffectCauser();
		
		// for now, same as same.
		// auto a2 = DamageGameplayEffectSpecHandle.Data->GetContext().GetInstigatorAbilitySystemComponent();
		// auto a3 = DamageGameplayEffectSpecHandle.Data->GetContext().GetOriginalInstigatorAbilitySystemComponent();
		// auto a4 = DamageGameplayEffectSpecHandle.Data->GetContext().GetInstigator();
		// auto a5 = DamageGameplayEffectSpecHandle.Data->GetContext().GetOriginalInstigator();

		if(EffectCauser == OtherActor) return;
	}
	
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageGameplayEffectSpecHandle.Data.Get());
	}	
	Destroy();
}
