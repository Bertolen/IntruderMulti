// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IntruderMulti.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayGS.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERMULTI_API AGameplayGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGameplayGS(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());
	
	//Gives clients the chance to do something when time gets updates
	virtual void UpdateServerTimeSeconds();
};
