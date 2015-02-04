/* ====================================================================== */
/**
 * @brief  �Q�[���ŋ��L���K�v�ȏ��i�[�N���X
 *
 * @note
 *			�N���X���܂����ŏ���ێ��������ꍇ�ɃZ�b�g
 */
/* ====================================================================== */

#ifndef __CMN_ACCESER__
#define __CMN_ACCESER__

#include "Math/MathVector.h"

class GameAccesser
{

public:

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
	const math::Vector2 &GetPlayerOffSet() const;
	void GetPlayerOffSet( float &posX, float &posY);

	// GameAccesser���
	void DeleteGameAccesser();

private:

	GameAccesser();
	~GameAccesser();

	static GameAccesser *s_pInstance;

	//! �����I�ȃv���C���[���W
	math::Vector2	m_offsetPlayer;

};

#endif