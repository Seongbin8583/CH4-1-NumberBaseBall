#include "Player/CXGPawn.h"
#include "ChatXGame.h"


void ACXGPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = ChatXGameFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXGPawn::BeginPlay() %s [%s]"), *ChatXGameFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXGameFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ACXGPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = ChatXGameFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXGPawn::PossessedBy() %s [%s]"), *ChatXGameFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXGameFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


