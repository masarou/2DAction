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

	GameEffect( const EFFECT_KIND &kind, const uint32_t &posX, const uint32_t &posY );
	virtual ~GameEffect(void);

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	std::string SelectEffectFile();		// �ǂݍ��݃t�@�C���I�ʊ֐�
	std::string SelectEffectAnimTag();	// �Đ��A�j���^�O�擾

	EFFECT_KIND		m_kind;				// ���o�̎��
	Texture2D		m_textureEffect;	// �G�t�F�N�g
	
};

#endif