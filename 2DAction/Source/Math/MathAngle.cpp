/* ====================================================================== */
/**
 * @brief  
 *
 * @note 角度を扱うクラス
 *		
 */
/* ====================================================================== */
#include "MathCommon.h"
#include "MathAngle.h"


namespace math{

//!<角度のチェックに使う
void CheckAngle(float &angle){
	if( angle <  0.0f ){ angle += ANGLE_FULL; }
	if( angle >= ANGLE_FULL ){ angle -= ANGLE_FULL; }
}

float Angle::GetDegree() const
{
	return m_degree;
}

float Angle::GetDegreeLR() const
{
	return ((m_degree > ANGLE_HALF)? m_degree-ANGLE_FULL : m_degree);
}

float Angle::GetRadian() const
{
	return DegreeToRadian(m_degree);
}

float Angle::GetRadianLR() const
{
	return DegreeToRadian(GetDegreeLR());
}

//!<operator
Angle Angle::operator-()const{ Angle angle(-m_degree); return angle; }
Angle& Angle::operator=(float angle){ m_degree=angle; CheckAngle(m_degree); return *(this);  }
Angle& Angle::operator+=(float angle){ m_degree+=angle; CheckAngle(m_degree); return *(this); }
Angle& Angle::operator+=(const Angle& angle){ m_degree+=angle.GetDegree(); CheckAngle(m_degree); return *(this); }
Angle& Angle::operator-=(float angle){ m_degree = angle; CheckAngle(m_degree); return *(this); }
Angle& Angle::operator-=(const Angle& angle){ m_degree -= angle.GetDegree(); CheckAngle(m_degree); return *(this); }
Angle& Angle::operator*=(float value){ m_degree *= value; CheckAngle(m_degree); return *(this); }
Angle& Angle::operator/=(float value){ m_degree /= value; CheckAngle(m_degree); return *(this); }

//!<角度をラジアンへ
float DegreeToRadian(float degree){
	return ((degree) * PI / ANGLE_HALF);	
}

//!<ラジアンから角度へ
float RadianToDegree(float rad){
	return ((rad) * ANGLE_HALF / PI);
}

//!<角度の差分を返す
float GetAngleSubLR(const Angle &baseAngle, const Angle &subAngle)
{
	Angle sub( subAngle.GetDegree() - baseAngle.GetDegree() );
	return sub.GetDegreeLR();
}

//!<角度の差分を絶対値で返す
float GetAngleSubAbs(const Angle &baseAngle, const Angle &subAngle)
{
	return math::Absf( GetAngleSubLR( baseAngle, subAngle ) );
}

//! 三角関数
float Sin(const Angle &angle){
	return sinf(angle.GetRadian());	
}
float Cos(const Angle &angle){
	return cosf(angle.GetRadian());
}
float Tan(const Angle &angle){
	return tanf(angle.GetRadian());
}
Angle ArcSin(float value){
	return Angle( RadianToDegree(asinf(value)) );
}
Angle ArcCos(float value){
	return Angle( RadianToDegree(acosf(value)) );
}
Angle ArcTan(float value){
	return Angle( RadianToDegree(atanf(value)) );
}
Angle ArcTan2(float y, float x){
	return Angle( RadianToDegree(atan2f(y, x)) );
}

} //namespace math

