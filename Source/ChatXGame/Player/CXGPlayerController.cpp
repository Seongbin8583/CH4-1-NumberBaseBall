#include "Player/CXGPlayerController.h"
#include "UI/CXGUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ChatXGame/ChatXGame.h"
#include "Game/CXGGameModeBase.h"
#include "CXGPlayerState.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ACXGPlayerController::ACXGPlayerController()
{
	bReplicates = true;
}

void ACXGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UCXGUserWidget>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
				NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ACXGPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	//PrintChatMessageString(ChatMessageString);

	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessageString);
		ACXGPlayerState* CXGPlayerState = GetPlayerState<ACXGPlayerState>();
		if (IsValid(CXGPlayerState) == true)
		{
			FString CombinedMessageString = CXGPlayerState->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ACXGPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	ChatXGameFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
	
	//기존에 사용하던 코드
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 10.0f);

	//디버그 로깅용
	//FString NetModeString = ChatXGameFunctionLibrary::GetNetModeString(this);
	//FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	//ChatXGameFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
}

void ACXGPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ACXGPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	//for (TActorIterator<ACXGPlayerController> It(GetWorld()); It; ++It)
	//{
	//	ACXGPlayerController* CXGPlayerController = *It;
	//	if (IsValid(CXGPlayerController) == true)
	//	{
	//		CXGPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	//	}
	//}

	AGameModeBase* GrandMaster = UGameplayStatics::GetGameMode(this);
	if (IsValid(GrandMaster) == true)
	{
		ACXGGameModeBase* CXGrandMaster = Cast<ACXGGameModeBase>(GrandMaster);
		if (IsValid(CXGrandMaster) == true)
		{
			CXGrandMaster->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ACXGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}



