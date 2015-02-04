
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		クォータニオン
 */
/* ====================================================================== */
#ifndef __MATH_QUATERNION__
#define __MATH_QUATERNION__

namespace math{

class Quaternion
{
public:
	float w;
	float x;
	float y;
	float z;

	Quaternion(float _w, float _x, float _y, float _z): w(_w), x(_x), y(_y), z(_z) {}
	Quaternion(): w(0.0f), x(0.0f), y(0.0f), z(0.0f) {}

	void SetFromRotationAxis(float axisX, float axisY, float axisZ, float angleRadian ){
		w = x = y = z = 0.0f;	// 初期化
		float norm = axisX *  axisX +  axisY *  axisY +  axisZ *  axisZ;;
		if(norm <= 0.0) return;
		norm = 1.0f / sqrtf(norm);

		axisX *= norm;
		axisY *= norm;
		axisZ *= norm;

		float cosValue = cos(0.5f * angleRadian);
		float sinValue = sin(0.5f * angleRadian);

		w = cosValue;
		x = sinValue * axisX;
		y = sinValue * axisY;
		z = sinValue * axisZ;
	}

	void SetFromMatrix(const Matrix33& matrix){

		float q[4];

		float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];

		// 体格成分をチェック
		if( trace > 0.0f ){
			float s = sqrt( trace + 1.0f );
			q[3] = s * 0.5f;
			float t = 0.5f / s;
			q[0] = ( matrix.m[2][1] - matrix.m[1][2] ) * t;
			q[1] = ( matrix.m[0][2] - matrix.m[2][0] ) * t;
			q[2] = ( matrix.m[1][0] - matrix.m[0][1] ) * t;
		}else{
			int i = 0;
			if( matrix.m[1][1] > matrix.m[0][0] ) i = 1;
			if( matrix.m[2][2] > matrix.m[i][i] ) i = 2;

			static const int NEXT[3] = { 1, 2, 0 };
			int j = NEXT[i];
			int k = NEXT[j];

			float s = sqrt( ( matrix.m[i][i] - ( matrix.m[j][j] + matrix.m[k][k] ) ) + 1.0f );

			q[i] = s * 0.5f;

			float t;
			if( s != 0.0f ) t = 0.5f / s;
			else t = s;

			q[3] = (matrix.m[k][j] - matrix.m[j][k]) * t;
			q[j] = (matrix.m[j][i] + matrix.m[i][j]) * t;
			q[k] = (matrix.m[k][i] + matrix.m[i][k]) * t;
		}

		w = q[3];
		x = q[0];
		y = q[1];
		z = q[2];
	}

	void MakeQuaternionFromAxis(const Vector3& axis, float angleRadian ){
		MakeQuaternionFromAxis( axis.x, axis.y, axis.z, angleRadian );
	}

	Matrix33 MakeMatrixFromQuaternion( )
	{
		Matrix33 mat;
		Quaternion quat = *this;

		mat.m[0][0] = 1.0f - 2.0f*quat.y*quat.y - 2.0f*quat.z*quat.z;
		mat.m[0][1] = 2.0f*quat.x*quat.y + 2.0f*quat.z*quat.w;
		mat.m[0][2] = 2.0f*quat.x*quat.z - 2.0f*quat.y*quat.w;

		mat.m[1][0] = 2.0f*quat.x*quat.y - 2.0f*quat.z*quat.w;
		mat.m[1][1] = 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.z*quat.z;
		mat.m[1][2] = 2.0f*quat.y*quat.z + 2.0f*quat.x*quat.w;

		mat.m[2][0] = 2.0f*quat.x*quat.z + 2.0f*quat.y*quat.w;
		mat.m[2][1] = 2.0f*quat.y*quat.z - 2.0f*quat.x*quat.w;
		mat.m[2][2] = 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.y*quat.y;

		return mat;
	}

	void MakeQuaternionFromVectors(const Vector3& baseVector, const Vector3& targetVector){
		Vector3 base = baseVector.GetNormalize();
		Vector3 target = targetVector.GetNormalize();

		float innerProduct = GetInnerProduct( base, target );
		
		float angle = acos( innerProduct );
		Vector3 axis = GetOuterProduce( base, target );

		MakeQuaternionFromAxis(axis, angle);
	}

	Vector3 GetVector3()const{ return Vector3(x, y, z); }
	float GetScalar()const{ return w; }
	Quaternion GetConjugate()const{ return Quaternion(w, -x, -y, -z); }



	Quaternion& operator+=(const Quaternion& q){ w+=q.w; x+=q.x; y+=q.y; z+=q.z; return *(this); }
	Quaternion& operator-=(const Quaternion& q){ w-=q.w; x-=q.x; y-=q.y; z-=q.z; return *(this); }
	Quaternion& operator*=(const Quaternion& q){ Quaternion q0 = *(this);
		w = q0.w*q.w - q0.x*q.x - q0.y*q.y - q0.z*q.z;
		x = q0.w*q.x + q0.x*q.w + q0.y*q.z - q0.z*q.y;
		y = q0.w*q.y + q0.y*q.w + q0.z*q.x - q0.x*q.z;
		z = q0.w*q.z + q0.z*q.w + q0.x*q.y - q0.y*q.x;
		return *(this); }
	Quaternion& operator*=(float value){ w*=value; x*=value; y*=value; z*=value; return *(this); }
	Quaternion& operator/=(float value){ w/=value; x/=value; y/=value; z/=value; return *(this); }

};

inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2){ return Quaternion(q1.w+q2.w, q1.x+q2.x, q1.y+q2.y, q1.z+q2.z); }
inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2){ return Quaternion(q1.w-q2.w, q1.x-q2.x, q1.y-q2.y, q1.z-q2.z); }
inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2){ return Quaternion(
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
		q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
		q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
		q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x
		); }
inline Quaternion operator*(const Quaternion& q, float value){ return Quaternion(q.w*value, q.x*value, q.y*value, q.z*value); }
inline Quaternion operator*(float value, const Quaternion& q){ return Quaternion(q.w*value, q.x*value, q.y*value, q.z*value); }
inline Quaternion operator/(const Quaternion& q, float value){ return Quaternion(q.w/value, q.x/value, q.y/value, q.z/value); }

inline Vector3  operator*( const Quaternion& quat, const Vector3& vec ){
	Quaternion v(0.0f, vec.x, vec.y, vec.z );
	Quaternion quatConj = quat.GetConjugate();
	Quaternion t = quat * v * quatConj;
	return t.GetVector3();
}


} //namespace math


#endif	// MATH_QUATERNION