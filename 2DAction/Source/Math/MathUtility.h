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

// ��_�̋����̗ݏ��Ԃ�
float GetDistance( const Vector2 &vecA, const Vector2 &vecB );

// ��_�̋������w��͈͂��Ȃ����ǂ���
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range );
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range );

/* ====================================================================== */
/**
 * @brief  �O���[�o��(���_)���W�����[�J�����W(�e�I�u�W�F�N�g��)�ɕϊ�
 *
 * @note
 *		angle		:�����Ǝw�W�ƂȂ�x�N�^�[�̐����p(�x)
		checkOrigin	:����
 */
/* ====================================================================== */
Vector2	ChangeCoordinateToRocal( const float &angleToOrigin, const Vector2 &checkOrigin );

}

#endif // MATH_UTILITY