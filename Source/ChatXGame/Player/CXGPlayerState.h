#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXGPlayerState.generated.h"

UCLASS()
class CHATXGAME_API ACXGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACXGPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(Replicated)
	int32 MaxGuessCount;

};
