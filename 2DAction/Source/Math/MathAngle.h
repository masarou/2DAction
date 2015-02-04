/* ====================================================================== */
/**
 * @brief  
 *
 * @note 角度を扱うクラス
 *		
 */
/* ====================================================================== */
#ifndef __MATH_ANGLE__
#define __MATH_ANGLE__

#include "Math/MathCommon.h"

namespace math{

//!<角度のチェックに使う。Angleクラスの外からは呼ばないで
void CheckAngle(float &angle);

class Angle
{
public:
	Angle(float angle=0.0f): m_degree(angle){ CheckAngle(m_degree); }

	~Angle(){}

	// 角度取得（0.0 ～ 360.0f）
	float GetDegree() const;
	// 角度取得（-180.0 ～ 180.0f）
	float GetDegreeLR() const;
	// 角度取得（0.0f ～ 2π）
	float GetRadian() const;
	// 角度取得（-π ～ π）
	float GetRadianLR() const;
	
	//!<operator
	Angle operator-() const;
	Angle& operator=(float angle);
	Angle& operator+=(float angle);
	Angle& operator+=(const Angle& angle);
	Angle& operator-=(float angle);
	Angle& operator-=(const Angle& angle);
	Angle& operator*=(float value);
	Angle& operator/=(float value);

private:
	float m_degree;
};

//!<operator
inline Angle operator+(const Angle& angle1, const Angle& angle2){ return Angle(angle1.GetDegree()+angle2.GetDegree()); }
inline Angle operator+(float angle1, const Angle& angle2){ return Angle(angle1+angle2.GetDegree());}
inline Angle operator+(const Angle& angle1, float angle2){ return Angle(angle1.GetDegree()+angle2);}
inline Angle operator-(const Angle& angle1, const Angle& angle2){ return Angle(angle1.GetDegree()-angle2.GetDegree());}
inline Angle operator-(float angle1, const Angle& angle2){ return Angle(angle1-angle2.GetDegree()); }
inline Angle operator-(const Angle& angle1, float angle2){ return Angle(angle1.GetDegree()-angle2); }
inline Angle operator*(const Angle& angle, float value){ return Angle(angle.GetDegree()*value); }
inline Angle operator*(float value, const Angle& angle){ return Angle(angle.GetDegree()*value); }
inline Angle operator/(const Angle& angle, float value){ return Angle(angle.GetDegree()/value); }
inline bool operator==(const Angle& angle1, const Angle& angle2){ return (angle1.GetDegree() == angle2.GetDegree()); }
inline bool operator==(float angle1, const Angle& angle2){ return (Angle(angle1).GetDegree() == angle2.GetDegree()); }
inline bool operator==(const Angle& angle1, float angle2){ return (angle1.GetDegree() == Angle(angle2).GetDegree()); }
inline bool operator!=(const Angle& angle1, const Angle& angle2){ return (angle1.GetDegree() != angle2.GetDegree()); }
inline bool operator!=(float angle1, const Angle& angle2){ return (Angle(angle1).GetDegree() != angle2.GetDegree()); }
inline bool operator!=(const Angle& angle1, float angle2){ return (angle1.GetDegree() != Angle(angle2).GetDegree()); }



//!<角度をラジアンへ
float DegreeToRadian(float degree);
//!<ラジアンから角度へ
float RadianToDegree(float rad);

//!<角度の差分を返す
float GetAngleSubLR(const Angle &baseAngle, const Angle &subAngle);
//!<角度の差分を絶対値で返す
float GetAngleSubAbs(const Angle &baseAngle, const Angle &subAngle);


//! 三角関数
float Sin(const Angle &angle);
float Cos(const Angle &angle);
float Tan(const Angle &angle);
Angle ArcSin(float value);			/// 引数(-1.0f～1.0f)  (-90 ~  90)を返す
Angle ArcCos(float value);			/// 引数(-1.0f～1.0f)  (  0 ~ 180)を返す
Angle ArcTan(float value);			/// (-90 ~  90)を返す
Angle ArcTan2(float y, float x);	/// (-180 ~  180)を返す


} //namespace math

#endif MATH_ANGLE
