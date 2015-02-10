/* ====================================================================== */
/**
 * @brief  �G�̐�����A�C�e��������ݒ�ɂȂ���ĊǗ�����N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include "System/Task/SystemTaskUnit.h"

class GameManager : public TaskUnit
{
public:

	enum STAGE_TYPE{
		TYPE_TIME,
		TYPE_DESTROY,
	};

	static GameManager *CreateGameManager( const Common::GAME_FLOW &currentKind );

	~GameManager(void);
	// �������Ԃ��߂������ǂ���
	bool	IsGameOver() const;

	// �G��|���������J�E���g
	void AddDestroyCount();

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	GameManager( const Common::GAME_FLOW &currentKind );
	
	void LoadGameSettings( const char *jsonFile );					// �Q�[�����̓ǂݍ���
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// �G�̐������f
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// �A�C�e���̐������f

	STAGE_TYPE		m_type;				// �X�e�[�W�N���A�̃^�C�v

	uint32_t		m_gameTimer;		// �Q�[���̃v���C����
	uint32_t		m_gameTimeMax;		// �Q�[���I���܂ł̎���(/fps)

	uint32_t		m_destroyNum;		// ���݂܂ł̓G�r�Ő�
	uint32_t		m_destroyMax;		// �Q�[���I���܂ł̓G�r�Ő�

	uint32_t		m_enemyMax;			// �ő�o���G��
	uint32_t		m_enemyFrequency;	// �G�̏o����(10�i�K0~9)
	uint32_t		m_itemMax;			// �ő�o���A�C�e����
	uint32_t		m_itemFrequency;	// �A�C�e���̏o����(10�i�K0~9)
	std::string		m_settingFileStr;	// �ǂݍ��ރX�e�[�W�ݒ�t�@�C��
};

#endif