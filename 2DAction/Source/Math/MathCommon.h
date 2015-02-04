
/* ====================================================================== */
/**
 * @brief  
 *
 * @note ���ʂ̐��w�n�͂�����
 *	
 */
/* ====================================================================== */
#ifndef __MATH_COMMON__
#define __MATH_COMMON__

#include "System/SystemDefine.h"

namespace math{

const float PI = 3.1415926535897932384626f;		// ��
const float ANGLE_HALF = 180.0f;				// 1���̔���
const float ANGLE_FULL = 360.0f;				// 1��

//!<������ 
float Sqrtf(float value);
//!<��Βl
int32_t Abs(int32_t value);
float Absf(float value);


//!<�ő�l �傫���ق���Ԃ�
template<class T> T Max(const T& value, const T& max);

//!<�ŏ��l �������ق���Ԃ�
template<class T> T Min(const T& value, const T& min);

//!<�N�����v min�ȏ�Amax�ȉ��ɔ[�߂�
template<class T> T Clamp(const T& value, const T& min, const T& max);



} //namespace math

#endif // MATH_COMMON

