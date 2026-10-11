#include "Game/CXGGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CXGPlayerController.h"

void ACXGGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACXGPlayerController* CXGPC = Cast<ACXGPlayerController>(PC);
			if (IsValid(CXGPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" FBI OPEN UP!!!!");
				CXGPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

