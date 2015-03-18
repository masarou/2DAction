/* ====================================================================== */
/**
 * @brief  �������ԃX�e�[�W�̎c�莞�Ԃ�\���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_STAGE_TIMER__
#define __GAME_STAGE_TIMER__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class GameCountDownTimer;

class StageTimer : public TaskUnit
{

public:

	static StageTimer *CreateStageTimer( uint32_t startTime );
	~StageTimer(void);

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

private:
	
	StageTimer( uint32_t startTime );

	// �w��p�[�c�̏��擾
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	uint32_t			m_startTime;		// �J�E���g�J�n��

	// �`��
	Texture2D			m_plateStageTimer;	// �X�e�[�W�^�C�}�[�̉��n
	GameCountDownTimer	*m_pCountDown;		// ���ۂɃJ�E���g�_�E�����鐔���N���X
	Texture2D			m_animClock;		// �����v

	// �p�[�c�}�b�v
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
