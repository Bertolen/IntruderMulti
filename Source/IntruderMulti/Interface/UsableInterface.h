// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "UsableInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class INTRUDERMULTI_API UUsableInterface :public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INTRUDERMULTI_API IUsableInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// This function can be called to know if the object can be used or not by the given character
	virtual bool CanBeUsed(class ACharacter* User);

	// This function will be called when the user uses the object
	virtual void OnUsed(class ACharacter* User);
};