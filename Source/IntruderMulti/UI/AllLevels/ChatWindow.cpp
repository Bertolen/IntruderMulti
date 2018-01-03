// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWindow.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "IntruderMulti/UI/AllLevels/ChatText.h"
#include "IntruderMulti/PlayerController/LobbyPC.h"
#include "IntruderMulti/PlayerController/GameplayPC.h"

void UChatWindow::UpdateChatWindow_Implementation(const FText & Sender, const FText & Text)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChatWindow_Implementation - Begin"));

	FText Message = FText::Format(FText::FromString("{0} : {1}"), Sender, Text);

	UChatText* ChatText = CreateWidget<UChatText>(GetOwningPlayer()->GetWorld(), ChatTextClass);

	ChatText->SetText(Message);

	ChatLogWB->AddChild(ChatText);
	ChatLogWB->ScrollToEnd();

	UE_LOG(IntruderDebug, Verbose, TEXT("UpdateChatWindow_Implementation - End"));
}

void UChatWindow::CommitText(FText Text)
{
	UE_LOG(IntruderDebug, Verbose, TEXT("CommitText - Begin"));

	ALobbyPC* LobbyPC = Cast<ALobbyPC>(GetOwningPlayer());
	if (LobbyPC) {
		LobbyPC->GetChatMessage(Text);
	}
	else {
		
		AGameplayPC* GameplayPC = Cast<AGameplayPC>(GetOwningPlayer());
		if (!GameplayPC) {
			return;
		}

		GameplayPC->GetChatMessage(Text);
	}

	ChatEntryWB->SetText(FText());

	UE_LOG(IntruderDebug, Verbose, TEXT("CommitText - End"));
}