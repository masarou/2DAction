
/* ====================================================================== */
/**
 * @brief  
 *
 * @note ���ʂ̐��w�n�͂�����
 *	
 */
/* ====================================================================== */
#include <cmath>
#include "Math/MathCommon.h"

namespace math{


//!<������
float Sqrtf(float value){
	return sqrtf(value);
}

//!<��Βl
int32_t Abs(int32_t value){
	return ((value<0)? -value : value);
}
float Absf(float value){
	return ((value<0.0f)? -value : value);
}

//!<�ő�l �傫���ق���Ԃ�
template<class T> T Max(const T& value, const T& max){
	return ( value > max ) ? ( value ) : ( max );
}

//!<�ŏ��l �������ق���Ԃ�
template<class T> T Min(const T& value, const T& min){
	return ( value < min ) ? ( value ) : ( min );
}

//!<�N�����v min�ȏ�Amax�ȉ��ɔ[�߂�
template<class T> T Clamp(const T& value, const T& min, const T& max){
	return (( value > max ) ? (max :( value < min ) ? ( min ) : ( value )));
}

} //namespace math

