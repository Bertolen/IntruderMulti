// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatText.h"

void UChatText::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UChatText, ActualText);
}