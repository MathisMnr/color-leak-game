// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTDecorator_IsChasing.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsChasing::UBTDecorator_IsChasing()
{
	NodeName = "Check If IsChasing";
}

bool UBTDecorator_IsChasing::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Récupère le Blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	// Récupère la valeur de "IsChasing"
	bool bIsChasing = BlackboardComp->GetValueAsBool("IsChasing");

	// Retourne true si l'IA doit poursuivre, sinon false
	return bIsChasing;
}