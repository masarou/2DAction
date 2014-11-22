/* ====================================================================== */
/**
 * @brief  �Q�[���ŋ��L���K�v�ȏ��i�[�N���X
 *
 * @note
 *		�V�[���A�N���X���܂����ŏ���ێ��������ꍇ�ɃZ�b�g
 */
/* ====================================================================== */

#ifndef __CMN_ACCESER__
#define __CMN_ACCESER__

#include "Math/MathVector.h"

class GameAccesser
{

public:

	~GameAccesser();

	static void Create();
	static GameAccesser *GetInstance();

	// ������
	void InitAll();

	// �v���C���[���Z�b�g
	void SetPlayerOffSet(const math::Vector2 &setPos);
	void SetPlayerOffSet(const float &posX, const float &posY);
	void AddPlayerOffSet(const math::Vector2 &addPos);
	void AddPlayerOffSet(const float posX, const float posY);

	// �v���C���[���擾
	const math::Vector2 &GetPlayerOffSet();
	void GetPlayerOffSet( float &posX, float &posY);

private:

	GameAccesser();

	static GameAccesser *m_pInstance;

	//! �����I�ȃv���C���[���W
	math::Vector2	m_offsetPlayer;

};

#endif