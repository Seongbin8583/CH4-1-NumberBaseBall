#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CXGPlayerController.generated.h"

class UCXGUserWidget;
class UUserWidget;

UCLASS()
class CHATXGAME_API ACXGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACXGPlayerController();
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InchatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCXGUserWidget> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCXGUserWidget> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass; //ㅣㅁ나오류;마ㅓ눌'매ㅑ놂'ㅣㅏ눟;나ㅓ오ㅠㅍ;재댜퓨ㅗㄴ[ㅐ야ㅕ퓨ㅗㄴ'ㅣ아퓬'이ㅏㅓ퓬'ㅇ피뉴애펴
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	
public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
