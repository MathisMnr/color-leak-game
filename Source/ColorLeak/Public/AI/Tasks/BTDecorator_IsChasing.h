// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsChasing.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API UBTDecorator_IsChasing : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsChasing();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
