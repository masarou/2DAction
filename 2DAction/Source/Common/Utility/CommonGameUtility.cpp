#include "CommonGameUtility.h"


void gameUtility::AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!�v���C���[���擾
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}
math::Vector2 gameUtility::GetPlayerOffsetPos()
{
	//!�v���C���[���擾
	math::Vector2 ret = GameAccesser::GetInstance()->GetPlayerOffSet();
	return ret;
}
