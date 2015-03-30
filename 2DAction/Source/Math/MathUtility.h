/* ====================================================================== */
/**
 * @brief  
 *
 * @note ���w�I��Utility
 *		
 */
/* ====================================================================== */
#ifndef __MATH_UTILITY__
#define __MATH_UTILITY__

#include "System/SystemDefine.h"
#include "Math/MathVector.h"

namespace math{

// ���ς����߂�
float GetInnerProduct( math::Vector2 vecA, math::Vector2 vecB );

// ��_�̋����̗ݏ��Ԃ�
float GetDistance( const Vector2 &vecA, const Vector2 &vecB );

// ��_�̋������w��͈͂��Ȃ����ǂ���
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range );
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range );

// �����p�����߂�
float GetAngleFormed( const math::Vector2 &vecA, const math::Vector2 &vecB);

}

#endif // MATH_UTILITY