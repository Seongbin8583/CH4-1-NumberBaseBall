#include "Player/CXGPlayerState.h"
#include "Net/UnrealNetwork.h"

ACXGPlayerState::ACXGPlayerState()
	: PlayerNameString(TEXT("None")),
	 CurrentGuessCount(0),
	 MaxGuessCount(3)
{
	bReplicates = true;
}
void ACXGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
	//현재시각 오전8시 11분 (무수면 30시간째)
}


FString ACXGPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}


