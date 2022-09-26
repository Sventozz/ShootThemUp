// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
//#include "Dev/STUFireDamageType.h"
//#include "Dev/STUIceDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }

    // OnTakeAnyDamage.AddDynamic(this, &ASTUBaseCharacter::OnTakeAnyDamageHandle);
}

void USTUHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                                          class AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld())
    {
        return;
    }

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer((HealTimerHandle));

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
                                               true, HealDelay);
    }

    // UE_LOG(HealthComponentLog, Display, TEXT("Damage: %f"), Damage);

    /* if (DamageType)
     {
         if (DamageType->IsA<USTUFireDamageType>())
         {
             UE_LOG(HealthComponentLog, Display, TEXT("So hoooooot !!!"));
         }
         else if (DamageType->IsA<USTUIceDamageType>())
         {
             UE_LOG(HealthComponentLog, Display, TEXT("So cooooold !!!"));
         }
     }*/
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer((HealTimerHandle));
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}