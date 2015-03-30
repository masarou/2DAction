/* ====================================================================== */
/**
 * @brief  
 *
 * @note �p�x�������N���X
 *		
 */
/* ====================================================================== */

#include "Math/MathCommon.h"
#include "MathUtility.h"

namespace math{

// ��_�̋����̗ݏ��Ԃ�
float GetDistance( const Vector2 &vecA, const Vector2 &vecB )
{
	float ret = pow( (vecA.x-vecB.x), 2 ) + pow( (vecA.y-vecB.y), 2 );
	return ret;
}

// ��_�̋������w��͈͂��Ȃ����ǂ���
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range )
{
	return IsInRange( vecA, vecB, static_cast<float>(range) );
}
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range )
{
	float distance = GetDistance( vecA, vecB );
	return ( distance < (range*range) ? true : false );
}

}