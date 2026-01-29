// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/Tasks/BTTaskNode_FindPlayerLocation.h"

#include "AI/Tasks/BTTaskNode_FindPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIPaintEater.h"
#include "Acryl.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskNode_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get IA 
	AAIPaintEater* AICharacter = Cast<AAIPaintEater>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AICharacter) return EBTNodeResult::Failed;

	// Get Acryl
	AAcryl* Acryl = Cast<AAcryl>(UGameplayStatics::GetActorOfClass(GetWorld(), AAcryl::StaticClass()));
	if (!Acryl) return EBTNodeResult::Failed;

	// If color match & != Uncolored
	if (Acryl->GetColorBrush("Socket_Brush") != AICharacter->Color && AICharacter->Color != EColorType::Uncolored) return EBTNodeResult::Failed;

	// If close enought
	float Distance = FVector::Dist(Acryl->GetActorLocation(), AICharacter->GetActorLocation());
	if (Distance > AICharacter->DetectionDistance) return EBTNodeResult::Failed;

	// Get Acryl Arrow component Y value
	FVector AcrylForward = Acryl->GetActorForwardVector()*100;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AcrylForward: %s"), *AcrylForward.ToString()));
	// Get Acryl position + 100 ahead
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), Acryl->GetActorLocation()+AcrylForward);

	return EBTNodeResult::Succeeded;
}
