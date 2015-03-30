
/* ====================================================================== */
/**
 * @brief  
 *
 * @note Vectorクラス
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

	//!<成分の設定
	void Set(float _x, float _y);

	//!<長さ、長さの２乗値を取得
	float GetLength() const;
	float GetLengthSquare()	const;

	//!<角度取得
	Angle GetAngle() const;	/// 原点から自身の座標への角度
	Angle GetAngle(const Vector2 &vec) const;/// 自身の座標からvecへの角度
	
	//!<自身を正規化	
	void Normalize();
	//!<自身を正規化したベクトルを返す
	Vector2	GetNormalize() const;
	
	//!<operator
	Vector2 operator-() const;
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(float value);
	Vector2& operator/=(float value);
};

//!<内積値を取得
float GetInnerProduct(const Vector2& vec1, const Vector2& vec2);
//!<vec1からvec2への距離、距離の2乗値を返す
float GetLength(const Vector2 &vec1, const Vector2 &vec2);
float GetLengthSquare(const Vector2 &vec1, const Vector2 &vec2);
//<!2つのベクトルのなす角
Angle GetBetweenAngle(const Vector2 &vec1, const Vector2 &vec2);
//<!回転させたベクトルを返す
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
	
	//!<成分の設定
	void Set(float _x, float _y, float _z);

	//!<長さ、長さの２乗値を取得
	float GetLength() const;
	float GetLengthSquare() const;
	float GetLengthXZ() const;
	float GetLengthXZSquare() const;

	//!<角度取得
	Angle GetAngle() const;
	Angle GetAngle(const Vector3 &vec) const;

	//!<自身を正規化	
	void Normalize();
	//!<自身を正規化したベクトルを返す
	Vector3	GetNormalize() const;
	
	//!<operator
	Vector3 operator-() const;
	Vector3& operator+=(const Vector3& vec);
	Vector3& operator-=(const Vector3& vec);
	Vector3& operator*=(float value);
	Vector3& operator/=(float value);
};

//!<内積値を取得
float GetInnerProduct(const Vector3& vec1, const Vector3& vec2);
//!<外積値を取得
Vector3 GetOuterProduce(const Vector3& vec1, const Vector3& vec2);
//!<vec1からvec2への距離、距離の2乗値を返す
float GetLength(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthSquare(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthXZ(const Vector3 &vec1, const Vector3 &vec2);
float GetLengthSquareXZ(const Vector3 &vec1, const Vector3 &vec2);
//<!2つのベクトルのなす角
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