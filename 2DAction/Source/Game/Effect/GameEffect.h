/* ====================================================================== */
/**
 * @brief  �G�t�F�N�g�N���X ��{���Đ�+���E
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Game/Game2DBase.h"

#ifndef GAME_EFFECT
#define GAME_EFFECT

class GameEffect : public TaskUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_BOMB,		// ����

	};

	GameEffect( EFFECT_KIND kind, uint32_t posX, uint32_t posY );
	~GameEffect(void);

protected:

	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	std::string SelectEffectFile();		// �ǂݍ��݃t�@�C���I�ʊ֐�
	std::string SelectEffectAnimTag();	// �Đ��A�j���^�O�擾

	EFFECT_KIND			m_kind;			// ���o�̎��
	Game2DBase			*m_pEffect;		// �`��N���X�{��
	TEX_DRAW_INFO		m_effectInfo;	// �`����
	
};

#endif