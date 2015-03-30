
/* ====================================================================== */
/**
 * @brief  
 *
 * @note Vector�N���X
 *		
 */
/* ====================================================================== */
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"

#include <cmath>

namespace math{

// Vector2 =========================================

//!<�����̐ݒ�
void Vector2::Set(float _x, float _y)
{
	x = _x;		y = _y;
}

//!<�����A�����̂Q��l���擾
float Vector2::GetLength() const
{
	return Sqrtf( GetLengthSquare() );
}

float Vector2::GetLengthSquare() const
{
	return (x*x+y*y);
}

//!<�p�x�擾
Angle Vector2::GetAngle() const
{
	return ArcTan2( y, x );
}
Angle Vector2::GetAngle(const Vector2 &vec) const
{
	return ArcTan2( vec.y-y, vec.x-x);
}

//!<���g�𐳋K��	
void Vector2::Normalize()
{
	float length = GetLength();
	if( length != 0.0f ){
		x /= length;
		y /= length;
	}
}

//!<���g�𐳋K�������x�N�g����Ԃ�
Vector2	Vector2::GetNormalize() const
{
	float length = GetLength();
	if( length == 0.0f ){
		return (*this);
	}
	return (*this)/length;
}

//!<���ϒl���擾
float GetInnerProduct(const Vector2& vec1, const Vector2& vec2)
{
	return (vec1.x*vec2.x+vec1.y*vec2.y);
}

//!<vec1����vec2�ւ̋����A������2��l��Ԃ�
float GetLength(const Vector2 &vec1, const Vector2 &vec2)
{
	return Sqrtf( GetLengthSquare(vec1, vec2) );
}

float GetLengthSquare(const Vector2 &vec1, const Vector2 &vec2)
{
	float dx = vec1.x - vec2.x;
	float dy = vec1.y - vec2.y;
	return (dx*dx+dy*dy);
}

//<!2�̃x�N�g���̂Ȃ��p
Angle GetBetweenAngle(const Vector2 &vec1, const Vector2 &vec2)
{
	Vector2 vec1Nrm = vec1.GetNormalize();
	Vector2 vec2Nrm = vec2.GetNormalize();

	float innerProduct = GetInnerProduct( vec1Nrm, vec2Nrm );
	return ArcCos( innerProduct );
}

//<!��]�������x�N�g����Ԃ�
Vector2 GetRotateVec( const Vector2 &vec, float degree )
{
	Vector2 vecNor = vec.GetNormalize();

	float cos = math::Cos( math::Angle(degree) );
	float sin = math::Sin( math::Angle(degree) );

	Vector2 retVec = Vector2();
	retVec.x = (vecNor.x * cos) - (vecNor.y * sin);
	retVec.y = (vecNor.x * sin) + (vecNor.y * cos);

	return retVec;
}

//!<operator
Vector2 Vector2::operator-() const{ return Vector2( -x, -y); }
Vector2& Vector2::operator+=(const Vector2& vec){ x+=vec.x; y+=vec.y; return *(this); }
Vector2& Vector2::operator-=(const Vector2& vec){ x-=vec.x; y-=vec.y; return *(this); }
Vector2& Vector2::operator*=(float value){ x*=value; y*=value; return *(this); }
Vector2& Vector2::operator/=(float value){ x/=value; y/=value; return *(this); }

// Vector3 =========================================

//!<�����̐ݒ�
void Vector3::Set(float _x, float _y, float _z)
{
	x = _x;	y = _y; z = _z;
}

//!<�����A�����̂Q��l���擾
float Vector3::GetLength() const
{
	return Sqrtf( GetLengthSquare() );
}

float Vector3::GetLengthSquare()	const
{
	return (x*x+y*y+z*z);
}

float Vector3::GetLengthXZ() const
{
	return Sqrtf( GetLengthXZSquare() );
}

float Vector3::GetLengthXZSquare() const
{
	return (x*x+z*z);
}

//!<�p�x�擾 
Angle Vector3::GetAngle() const
{
	return ArcTan2( x, z );
}
Angle Vector3::GetAngle(const Vector3 &vec) const
{
	return ArcTan2( vec.x-x, vec.z-z );
}

//!<���g�𐳋K��	
void Vector3::Normalize()
{
	float length = GetLength();
	if( length != 0.0f ){
		x /= length;
		y /= length;
		z /= length;
	}
}

//!<���g�𐳋K�������x�N�g����Ԃ�
Vector3	Vector3::GetNormalize() const
{
	float length = GetLength();
	if( length == 0.0f ){
		return (*this);
	}
	return (*this)/length;
}

//!<���ϒl���擾
inline float GetInnerProduct(const Vector3& vec1, const Vector3& vec2)
{
	return (vec1.x*vec2.x+vec1.y*vec2.y+vec1.z*vec2.z);
}
//!<�O�ϒl���擾
inline Vector3 GetOuterProduce(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3( vec1.y*vec2.z-vec1.z*vec2.y, vec1.z*vec2.x-vec1.x*vec2.z, vec1.x*vec2.y-vec1.y*vec2.x );
}

//!<vec1����vec2�ւ̋����A������2��l��Ԃ�
float GetLength(const Vector3 &vec1, const Vector3 &vec2)
{
	return Sqrtf( GetLengthSquare( vec1, vec2) );
}
float GetLengthSquare(const Vector3 &vec1, const Vector3 &vec2)
{
	float dx = vec2.x-vec1.x;
	float dy = vec2.y-vec1.y;
	float dz = vec2.z-vec1.z;

	return (dx*dx+dy*dy+dz*dz);
}
float GetLengthXZ(const Vector3 &vec1, const Vector3 &vec2)
{
	return Sqrtf( GetLengthSquareXZ( vec1, vec2 ) );
}
float GetLengthSquareXZ(const Vector3 &vec1, const Vector3 &vec2)
{
	float dx = vec2.x - vec1.x;
	float dz = vec2.z - vec1.z;
	return (dx*dx+dz*dz);
}

//<!2�̃x�N�g���̂Ȃ��p
Angle GetBetweenAngle(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 vec1Nrm = vec1.GetNormalize();
	Vector3 vec2Nrm = vec2.GetNormalize();

	float innerProduct = GetInnerProduct( vec1Nrm, vec2Nrm );
	return ArcCos( innerProduct );
}

//!<operator
Vector3 Vector3::operator-()const{ return Vector3(-x, -y, -z); }
Vector3& Vector3::operator+=(const Vector3& vec){ x+=vec.x; y+=vec.y; z+=vec.z; return *(this); }
Vector3& Vector3::operator-=(const Vector3& vec){ x-=vec.x; y-=vec.y; z-=vec.z; return *(this); }
Vector3& Vector3::operator*=(float value){ x*=value; y*=value; z*=value; return *(this); }
Vector3& Vector3::operator/=(float value){ x/=value; y/=value; z/=value; return *(this); }

} //namespace math
