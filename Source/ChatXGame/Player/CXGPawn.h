#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CXGPawn.generated.h"

UCLASS()
class CHATXGAME_API ACXGPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};
