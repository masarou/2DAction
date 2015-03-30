
/* ====================================================================== */
/**
 * @brief  
 *
 * @note Vector�N���X
 *		
 */
/* ====================================================================== */
#ifndef __MATH_VECTOR__
#define __MATH_VECTOR__

namespace math{

class Angle;

class Vector2
{
public:
	float x;
	float y;

	Vector2(float _x=0.0f, float _y=0.0f): x(_x), y(_y){}

	//!<�����̐ݒ�
	void Set(float _x, float _y);

	//!<�����A�����̂Q��l���擾
	float GetLength() const;
	float GetLengthSquare()	const;

	//!<�p�x�擾
	Angle GetAngle() const;	/// ���_���玩�g�̍��W�ւ̊p�x
	Angle GetAngle(const Vector2 &vec) const;/// ���g�̍��W����vec�ւ̊p�x
	
	//!<���g�𐳋K��	
	void Normalize();
	//!<���g�𐳋K�������x�N�g����Ԃ�
	Vector2	GetNormalize() const;
	
	//!<operator
	Vector2 operator-() const;
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(float value);
	Vector2& operator/=(float value);
};

//!<���ϒl���擾
float GetInnerProduct(const Vector2& vec1, const Vector2& vec2);
//!<vec1����vec2�ւ̋����A������2��l��Ԃ�
float GetLength(const Vector2 &vec1, const Vector2 &vec2);
float GetLengthSquare(const Vector2 &vec1, const Vector2 &vec2);
//<!2�̃x�N�g���̂Ȃ��p
Angle GetBetweenAngle(const Vector2 &vec1, const Vector2 &vec2);
//<!��]�������x�N�g����Ԃ�
Vector2 GetRotateVec( const Vector2 &vec, float degree );

//!<operator
inline Vector2 operator+(const Vector2& vec1, const Vector2& vec2){ return Vector2(vec1.x+vec2.x, vec1.y+vec2.y); }
inline Vector2 operator-(const Vector2& vec1, const Vector2& vec2){ return Vector2(vec1.x-vec2.x, vec1.y-vec2.y); }
inline Vector2 operator*(const Vector2& vec, float value){ return Vector2(vec.x*value, vec.y*value); }
inline Vector2 operator*(float value, const Vector2& vec){ return Vector2(vec.x*value, vec.y*value); }
inline Vector2 operator/(const Vector2& vec, float value){ return Vector2(vec.x/value, vec.y/value); }
inline bool operator==(const Vector2& vec1, const Vector2& vec2){ return ( (vec1.x==vec2.x) && (vec1.y==vec2.y) ); }
inline bool operator!=(const Vector2& vec1, const Vector2& vec2){ return ( (vec1.x!=vec2.x) || (vec1.y!=vec2.y) ); }




class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3(float _x=0.0f, float _y=0.0f, float _z=0.0f): x(_x), y(_y), z(_z){}
	
	//!<�����̐ݒ�
	void Set(float _x, float _y, float _z);

	//!<�����A�����̂Q��l���擾
	float GetLength() const;
	float GetLengthSquare() const;
	float GetLengthXZ() const;
	float GetLengthXZSquare() const;

	//!<�p�x�擾
	Angle GetAngle() const;
	Angle GetAngle(const Vector3 &vec) const;

	//!<���g�𐳋K��	
	void Normalize();
	//!<���g�𐳋K�������x�N�g����Ԃ�
	Vector3	GetNormalize() const;
	
	//!<operator
	Vector3 operator-() const;
	Vector3& operator+=(const Vector3& vec);
	Vector3& operator-=(const Vector3& vec);
	Vector3& operator*=(float value);
	Vector3& operator/=(float value);
};

//!<���ϒl���擾
float GetInnerProduct(const Vector3& vec1, const Vector3& vec2);
//!<�O�ϒl���擾
Vector3 GetOuterProduce(const Vector3& vec1, const Vector3& vec2);
//!<vec1����vec2�ւ̋����A������2��l��Ԃ�
float GetLength(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthSquare(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthXZ(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthSquareXZ(const Vector3 &vec1, const Vector3 &vec2);
//<!2�̃x�N�g���̂Ȃ��p
Angle GetBetweenAngle(const Vector3 &vec1, const Vector3 &vec2);

//!<operator
inline Vector3 operator+(const Vector3& vec1, const Vector3& vec2){ return Vector3(vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z); }
inline Vector3 operator-(const Vector3& vec1, const Vector3& vec2){ return Vector3(vec1.x-vec2.x, vec1.y-vec2.y, vec1.z-vec2.z); }
inline Vector3 operator*(const Vector3& vec, float value){ return Vector3(vec.x*value, vec.y*value, vec.z*value); }
inline Vector3 operator*(float value, const Vector3& vec){ return Vector3(vec.x*value, vec.y*value, vec.z*value); }
inline Vector3 operator/(const Vector3& vec, float value){ return Vector3(vec.x/value, vec.y/value, vec.z/value); }
inline bool operator==(const Vector3& vec1, const Vector3& vec2){ return ( (vec1.x==vec2.x) && (vec1.y==vec2.y) && (vec1.z==vec2.z) ); }
inline bool operator!=(const Vector3& vec1, const Vector3& vec2){ return ( (vec1.x!=vec2.x) || (vec1.y!=vec2.y) || (vec1.z!=vec2.z)); }

} //namespace math


#endif // MATH_VECTOR