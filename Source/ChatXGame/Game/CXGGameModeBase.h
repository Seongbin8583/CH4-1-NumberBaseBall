#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGGameModeBase.generated.h"

class ACXGPlayerController; //즈어언바바앙선언

UCLASS()
class CHATXGAME_API ACXGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecrrectNumerString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	void PrintChatMessageString(ACXGPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(ACXGPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgeGame(ACXGPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ACXGPlayerController>> AllPlayerControllers;
};
