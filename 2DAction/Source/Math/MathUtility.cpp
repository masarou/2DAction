/* ====================================================================== */
/**
 * @brief  
 *
 * @note �p�x�������N���X
 *		
 */
/* ====================================================================== */

#include "Math/MathCommon.h"
#include "Math/MathAngle.h"
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

/* ====================================================================== */
/**
 * @brief  �O���[�o��(���_)���W�����[�J�����W(�e�I�u�W�F�N�g��)�ɕϊ�
 *
 * @note
 *		angle		:�����Ǝw�W�ƂȂ�x�N�^�[�̐����p(�x)
		checkOrigin	:����
 */
/* ====================================================================== */
Vector2	ChangeCoordinateToRocal( const float &angleToOrigin, const Vector2 &checkOrigin )
{
	float	x,y;
	Angle angle = Angle( -1*angleToOrigin );

	x = checkOrigin.x * cos( angle.GetRadian() ) + checkOrigin.y * sin( angle.GetRadian() );
	y = -checkOrigin.x * sin( angle.GetRadian() ) + checkOrigin.y * cos( angle.GetRadian() );

	return Vector2( x, y );
}

}