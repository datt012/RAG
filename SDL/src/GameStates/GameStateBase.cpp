#include "GameStateBase.h"
#include "GameStates/GSIntro.h"
#include "GSMenu.h"
#include "GSPlay.h"
#include "Credit.h"
#include "Setting.h"
#include "GameOver.h"
#include "Complete.h"
GameStateBase::GameStateBase(StateType stateType) : m_stateType(stateType)
{
}

std::shared_ptr<GameStateBase> GameStateBase::CreateState(StateType stt)
{
	std::shared_ptr<GameStateBase> gs = nullptr;
	switch (stt)
	{
	case StateType::STATE_INVALID:
		break;
	case StateType::STATE_INTRO:
		gs = std::make_shared<GSIntro>();
		//GSINTRO;
		break;
	case StateType::STATE_MENU:
		gs = std::make_shared<GSMenu>();
		//GSMENU
		break;
	case StateType::STATE_PLAY:
		gs = std::make_shared<GSPlay>();
		//GSPLAY
		break;
	case StateType::STATE_OVER:
		gs = std::make_shared<GameOver>();
		//GSPLAY
		break;
	case StateType::STATE_COMPLETE:
		gs = std::make_shared<Complete>();
		//GSPLAY
		break;
	case StateType::STATE_CREDIT:
		gs = std::make_shared<Credit>();
		break;
	case StateType::STATE_OPTION:
		gs = std::make_shared<Setting>();
		break;
	default:
		break;
	}
	return gs;
}

StateType GameStateBase::GetGameStateType()
{
	return m_stateType;
}