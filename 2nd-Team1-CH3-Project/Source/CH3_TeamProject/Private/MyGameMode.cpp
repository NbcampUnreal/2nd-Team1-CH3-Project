#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyGameMode::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}