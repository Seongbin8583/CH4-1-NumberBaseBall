#include "Game/CXGGameModeBase.h"
#include "CXGGameStateBase.h"
#include "Player/CXGPlayerController.h"
#include "Player/CXGPlayerState.h"
#include "EngineUtils.h"

void ACXGGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACXGPlayerController* CXGPlayerController = Cast<ACXGPlayerController>(NewPlayer);
	if (IsValid(CXGPlayerController) == true)
	{
		CXGPlayerController->NotificationText = FText::FromString(TEXT("Connected Game Server"));
		AllPlayerControllers.Add(CXGPlayerController);
		ACXGPlayerState* CXGPlayerState = CXGPlayerController->GetPlayerState<ACXGPlayerState>();
		if (IsValid(CXGPlayerState) == true)
		{
			CXGPlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		ACXGGameStateBase* CXGGameStateBase = GetGameState<ACXGGameStateBase>();
		if (IsValid(CXGGameStateBase) == true)
		{
			CXGGameStateBase->MulticastRPCBroadcastLoginMessage(CXGPlayerState->PlayerNameString);
		}
	}
}

//어우 어렵다 어려워~~~ 0o0!!!

FString ACXGGameModeBase::GenerateSecretNumber() 
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1); //키보드 R키가 맛탱이가 갔는데?????? 큰일이다...
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index); //rrrrrrrrrrrrrrrrrRRRrrrrrrrrrr
	}
	return Result;
}

bool ACXGGameModeBase::IsGuessNumberString(const FString& InNumberString) 
{
	bool bCanPlay = false;
	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		if (bIsUnique == false)
		{
			break;
		}
		bCanPlay = true;
	}
	while (false);

	return bCanPlay;
}

FString ACXGGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString) //r키 중복입력으로 오류 발생해서 한참 찾음....Secr"r"ect
{
	int32 StrikeCount = 0, BallCount = 0;
	for (int32 i = 0; i < 3; i++)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACXGGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SecretNumberString = GenerateSecretNumber();
}

void ACXGGameModeBase::PrintChatMessageString(ACXGPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<ACXGPlayerController> It(GetWorld()); It; ++It)
		{
			ACXGPlayerController* CXGPlayerController = *It;
			if (IsValid(CXGPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" --> ") + JudgeResultString;
				CXGPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString); //Print를 내가 P"o"int라고 지정 했었구나 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ 

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACXGPlayerController> It(GetWorld()); It; ++It)
		{
			ACXGPlayerController* CXGPlayerController = *It;
			if (IsValid(CXGPlayerController) == true)
			{
				CXGPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
} //r키가 채터링이 너무 심해!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 미치겠네 미치겠어!!!!!!

void ACXGGameModeBase::IncreaseGuessCount(ACXGPlayerController* InChattingPlayerController)
{
	ACXGPlayerState* CXGPlayerState = InChattingPlayerController->GetPlayerState<ACXGPlayerState>();
	if (IsValid(CXGPlayerState) == true)
	{
		CXGPlayerState->CurrentGuessCount++;
	}
}

void ACXGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	for (const auto& CXGPlayerController : AllPlayerControllers)
	{
		ACXGPlayerState* CXGPlayerState = CXGPlayerController->GetPlayerState<ACXGPlayerState>();
		if (IsValid(CXGPlayerState) == true)
		{
			CXGPlayerState->CurrentGuessCount = 0;
		}
	}
}

void ACXGGameModeBase::JudgeGame(ACXGPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACXGPlayerState* CXGPlayerState = InChattingPlayerController->GetPlayerState<ACXGPlayerState>();
		for (const auto& CXGPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXGPlayerState) == true)
			{
				FString CombinedMessageString = CXGPlayerState->PlayerNameString + TEXT(" You win ");
				CXGPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXGPlayerController : AllPlayerControllers)
		{
			ACXGPlayerState* CXGPlayerState = CXGPlayerController->GetPlayerState<ACXGPlayerState>();
			if (IsValid(CXGPlayerState) == true)
			{
				if (CXGPlayerState->CurrentGuessCount < CXGPlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		if (true == bIsDraw)
		{
			for (const auto& CXGPlayerController : AllPlayerControllers)
			{
				CXGPlayerController->NotificationText = FText::FromString(TEXT("DRAW!!!!!"));
				ResetGame();
			}
		}
	}
}
