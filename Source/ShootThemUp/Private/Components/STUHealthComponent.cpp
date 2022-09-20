// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;

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
    Health -= Damage;
}