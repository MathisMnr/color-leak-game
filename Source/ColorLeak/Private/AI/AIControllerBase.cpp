// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AIControllerBase.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIPaintEater.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
	// Create BehaviorTree Component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	MyBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast your Character/Pawn to get access to the attributes
	if (auto AICharactere = Cast<AAIPaintEater>(InPawn))
	{
		// Check if the assets has been selected in the editor
		if (AICharactere->TreeAsset ) { // && AICharactere->TreeAsset->BlackboardAsset
			// Initialize the blackboard with the blackboard asset
			MyBlackboard->InitializeBlackboard(*AICharactere->TreeAsset->BlackboardAsset);

			// Run the behavior tree
			BehaviorTreeComponent->StartTree(*AICharactere->TreeAsset);
		}
	}
}

