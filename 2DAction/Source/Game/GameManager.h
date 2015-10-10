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
#include "Game/Item/ItemObject.h"

class GameManager : public TaskUnit
{
public:

	enum STAGE_TYPE{
		TYPE_TIME,
		TYPE_DESTROY,
		TYPE_DESTROY_BOSS,

		TYPE_MAX,
	};

	static GameManager *CreateGameManager( const Common::GAME_FLOW &currentKind );
	~GameManager(void);

	// �X�e�[�W�N���A�����𖞂��������ǂ���
	bool	IsGameOver() const;

	// �G��|���������J�E���g
	void AddDestroyCount();

	// �X�e�[�W�^�C�v�擾
	const STAGE_TYPE GetStageType() const;

	// �c�莞��or�r�Ő��̎擾
	const uint32_t GetGameLeftTimeByFrame() const;
	const uint32_t GetGameLeftTimeBySec() const;
	const uint32_t GetGameLeftDestroy() const;
	
	// �A�C�e�������˗�
	void CreateItem( const Common::ITEM_KIND &kind, const math::Vector2 &pos = math::Vector2() );
	// �G�����˗�
	void CreateEnemy( const Common::ENEMY_KIND &kind, const uint32_t &level = 0, bool isForce = false, const math::Vector2 &enemyPos = DEFAULT_VECTOR2 );

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	struct ExistEnemyState{
		uint32_t			m_level;		// �ڈ��ƂȂ鋭��
		uint32_t			m_freequency;	// �o���p�x(���ׂĂ̓G�̒��ł̏o������%)
		Common::ENEMY_KIND	m_kind;			// ���
		math::Vector2		m_initPos;		// �����ʒu(math::Vector2�Ȃ烉���_���ʒu)
		void Init(){
			m_level = 0;
			m_freequency = 0;
			m_kind = Common::ENEMY_KIND_MAX;
			m_initPos = math::Vector2();
		}
	};

	GameManager( const Common::GAME_FLOW &currentKind );
	
	void ResetManageValue();										// ������
	void LoadGameSettings( const char *jsonFile );					// �Q�[�����̓ǂݍ���
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// �G�̐������f
	bool IsCreateEnemy( uint32_t enemyLimit	);						// �G�̐������f(�����_���v�f�Ȃ�)
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// �A�C�e���̐������f
	bool IsCreateItem( uint32_t enemyLimit );						// �A�C�e���̐������f(�����_���v�f�Ȃ�)
	Common::ENEMY_KIND GetEnemyKindFromStr( const std::string str );// �G�̎�ނ𕶎��񂩂�擾

	STAGE_TYPE		m_type;				// �X�e�[�W�N���A�̃^�C�v

	// �X�e�[�W���������Ԃ������̂������ꍇ
	uint32_t		m_gameTimer;		// �Q�[���̃v���C����
	uint32_t		m_gameTimeMax;		// �Q�[���I���܂ł̎���(/fps)

	// �X�e�[�W���G�r�Ő����N���A�����Ƃ���ꍇ
	uint32_t		m_destroyNum;		// ���݂܂ł̓G�r�Ő�
	uint32_t		m_destroyMax;		// �X�e�[�W�I���܂ł̓G�r�Ő�

	// �X�e�[�W���{�X�r�ł��N���A�����Ƃ���ꍇ
	std::vector<Common::ENEMY_KIND>	m_bossKindVec;	// �{�X�J�E���g����G�̎��

	// �X�e�[�W������ϐ�
	uint32_t		m_enemyMax;			// �ő�o���G��
	uint32_t		m_enemyFrequency;	// �G�̏o����(10�i�K0~9)
	uint32_t		m_itemMax;			// �ő�o���A�C�e����
	uint32_t		m_itemFrequency;	// �A�C�e���̏o����(10�i�K0~9)
	std::string		m_settingFileStr;	// �ǂݍ��ރX�e�[�W�ݒ�t�@�C��

	// �o������G���
	std::vector<ExistEnemyState>	m_enemyInfoVec;

	// �ŏ��ɔz�u����G���(�{�X��)
	std::vector<ExistEnemyState>	m_initEnemyInfoVec;
};

#endif