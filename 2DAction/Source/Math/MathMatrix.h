
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�s��
 */
/* ====================================================================== */
#ifndef MATH_MATRIX
#define MATH_MATRIX

namespace math{

class Matrix33
{
public:
	float m[3][3];

	Matrix33(){
		for( int i = 0; i < 3; i++ ){
			for( int j = 0; j < 3; j++ ){
				m[i][j] = 0.0f;
			}
		}
	}
};


} //namespace math

#endif // MATH_MATRIX