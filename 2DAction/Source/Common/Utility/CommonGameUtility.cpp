#include "CommonGameUtility.h"
#include "Math/MathUtility.h"

void AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!�v���C���[���擾
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}
const math::Vector2 GetPlayerOffsetPos()
{
	//!�v���C���[���擾
	math::Vector2 ret = GameAccesser::GetInstance()->GetPlayerOffSet();
	return ret;
}

const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{

	return false;
}
