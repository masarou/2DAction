/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		操作説明演出クラス
 */
/* ====================================================================== */

#include "FlowProcessInitLastStage.h"
#include "Game/GameRegister.h"


InitLastStage *InitLastStage::Create()
{
	return NEW InitLastStage();
}

InitLastStage::InitLastStage(void)
{
}


InitLastStage::~InitLastStage(void)
{
}

bool InitLastStage::CanDie()
{
	return true;
}

bool InitLastStage::Init()
{
	return true;
}


void InitLastStage::Update()
{
	// ボス生成
	GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( Common::ENEMY_KIND_BOSS );

	SetStateNext();
}

void InitLastStage::PadEventDecide()
{
}
