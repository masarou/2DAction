#include "CommonGameUtility.h"


void gameUtility::AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!プレイヤー情報取得
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}
math::Vector2 gameUtility::GetPlayerOffsetPos()
{
	//!プレイヤー情報取得
	math::Vector2 ret = GameAccesser::GetInstance()->GetPlayerOffSet();
	return ret;
}
