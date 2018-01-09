// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableInterface.h"
#include "IntruderMulti/Characters/FP_Characters/Guard.h"
#include "IntruderMulti/Characters/TP_Characters/Thief.h"
#include "GameFramework/Character.h"

UUsableInterface::UUsableInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

// This function can be called to know if the object can be used or not by th given controller
bool IUsableInterface::CanBeUsed(ACharacter* User)
{
	return false;
}

// This function will be called when the user uses the object
void IUsableInterface::OnUsed(ACharacter* Newuser)
{

}