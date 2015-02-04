/* ====================================================================== */
/**
 * @brief  
 *
 * @note �p�x�������N���X
 *		
 */
/* ====================================================================== */
#ifndef __MATH_ANGLE__
#define __MATH_ANGLE__

#include "Math/MathCommon.h"

namespace math{

//!<�p�x�̃`�F�b�N�Ɏg���BAngle�N���X�̊O����͌Ă΂Ȃ���
void CheckAngle(float &angle);

class Angle
{
public:
	Angle(float angle=0.0f): m_degree(angle){ CheckAngle(m_degree); }

	~Angle(){}

	// �p�x�擾�i0.0 �` 360.0f�j
	float GetDegree() const;
	// �p�x�擾�i-180.0 �` 180.0f�j
	float GetDegreeLR() const;
	// �p�x�擾�i0.0f �` 2�΁j
	float GetRadian() const;
	// �p�x�擾�i-�� �` �΁j
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



//!<�p�x�����W�A����
float DegreeToRadian(float degree);
//!<���W�A������p�x��
float RadianToDegree(float rad);

//!<�p�x�̍�����Ԃ�
float GetAngleSubLR(const Angle &baseAngle, const Angle &subAngle);
//!<�p�x�̍������Βl�ŕԂ�
float GetAngleSubAbs(const Angle &baseAngle, const Angle &subAngle);


//! �O�p�֐�
float Sin(const Angle &angle);
float Cos(const Angle &angle);
float Tan(const Angle &angle);
Angle ArcSin(float value);			/// ����(-1.0f�`1.0f)  (-90 ~  90)��Ԃ�
Angle ArcCos(float value);			/// ����(-1.0f�`1.0f)  (  0 ~ 180)��Ԃ�
Angle ArcTan(float value);			/// (-90 ~  90)��Ԃ�
Angle ArcTan2(float y, float x);	/// (-180 ~  180)��Ԃ�


} //namespace math

#endif MATH_ANGLE
