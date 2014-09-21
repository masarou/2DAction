/* ====================================================================== */
/**
 * @brief  
 *
 * @note �p�x�������N���X
 *		
 */
/* ====================================================================== */

#include "MathUtility.h"

namespace math{

// ��_�̋����̗ݏ��Ԃ�
uint32_t GetDistance( const Vector2 &vecA, const Vector2 &vecB )
{
	uint32_t ret = vecA.x*vecA.x + vecB.y*vecB.y;
	return ret;
}

// ��_�̋������w��͈͂��Ȃ����ǂ���
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range )
{
	return IsInRange( vecA, vecB, static_cast<float>(range) );
}
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range )
{
	uint32_t distance = GetDistance( vecA, vecB );
	return ( static_cast<float>(distance) < (range*range) ? true : false );
}

}