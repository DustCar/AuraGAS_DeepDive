// Copyright Dustin Carmelo


#include "Actor/AGASEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AGASAttributeSet.h"
#include "Components/SphereComponent.h"


AAGASEffectActor::AAGASEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("OverlapSphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAGASEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change the body to apply a Gameplay Effect rather than directly modifying.
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAGASAttributeSet* AGASAttributeSet = Cast<UAGASAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAGASAttributeSet::StaticClass()));
		UAGASAttributeSet* MutableAGASAttributeSet = const_cast<UAGASAttributeSet*>(AGASAttributeSet);
		MutableAGASAttributeSet->SetHealthPoints(AGASAttributeSet->GetHealthPoints() + 25.f);
		Destroy();
	}
}

void AAGASEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AAGASEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndOverlap);
}

