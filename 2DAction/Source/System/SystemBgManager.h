/* ====================================================================== */
/**
 * @brief	�w�i�Ǘ��N���X
 *			json�Ɏw�肵�Ă���ID����BG���Z�b�g
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __BG_MANAGER__
#define __BG_MANAGER__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class BgManager : public TaskUnit
{

public:

	enum BGID{
		BGID_TITLE,
		BGID_STAGE_RESULT,
		BGID_COMMON,		// �ėp
		BGID_INTERVAL,		// �C���^�[�o��(�Q�[���̐i�s�x�����ĕ\���ύX)
		BGID_NONE,			// �\���Ȃ�
		BGID_CURRENT,		// ����ێ�

		BGID_MAX,
	};

	static BgManager *Create();
	static BgManager *GetInstance();
	static void ReleaseInstance();

	BgManager(void);
	~BgManager(void);

	// ���擾
	void	SetNextBg( const std::string &bgId );
	void	SetNextBg( const BGID &bgId );

	// ���������m�F�֐�
	bool	IsShowingBG() const;

protected:

	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

private:

	enum BG_STEP{
		STEP_IDLE,
		STEP_INIT,
		STEP_CHANGING,
		STEP_WAIT,
		STEP_SHOWING,

		STEP_MAX,
	};
	
	void SetNextStep( const BG_STEP &next ){ m_currStep = next; }
	std::string GetBgJsonName( const BGID &bgId ) const;

	static BgManager	*s_pInstance;
	BG_STEP				m_currStep;		// �N���X�̏�Ԃ�\���X�e�b�v

	BGID				m_currBGID;		// ���݂�ID
	BGID				m_nextBGID;		// ���ɃZ�b�g���ꂽID
	Texture2D			m_drawTexture;	// �\���摜�f�[�^

};
#endif // __BG_MANAGER__