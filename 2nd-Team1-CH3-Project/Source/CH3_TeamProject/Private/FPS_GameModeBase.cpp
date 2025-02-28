#include "FPS_GameModeBase.h"
#include "MyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPS_GameModeBase::AFPS_GameModeBase()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BP/BP_MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}