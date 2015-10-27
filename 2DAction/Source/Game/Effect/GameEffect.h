/* ====================================================================== */
/**
 * @brief  �G�t�F�N�g�N���X ��{���Đ�+���E
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/Game2DBase.h"
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"


#ifndef __GAME_EFFECT__
#define __GAME_EFFECT__

class GameEffect : public TaskUnit
{

public:

	// �e�֓����郁�b�Z�[�W��`
	static const uint32_t EFFECT_ANIM_END = 0;

	enum EFFECT_KIND{
		EFFECT_BOMB,			// �G�����ꂽ���̔���
		EFFECT_PRE_EXPLOSION,	// �����\��
		EFFECT_DAMAGE,			// �_���[�W
		EFFECT_EXCLAMATION,		// �т�����}�[�N
		EFFECT_SLASHING_HIT,	// �a��HIT
		EFFECT_DASH_SMOKE,		// DASH��
		EFFECT_INVALID_DAMAGE,	// �_���[�W����
		EFFECT_WORP,			// ���[�v
		EFFECT_PRE_FIRE_WALL,	// ��ʑS�̉����o
		EFFECT_FIRE_WALL,		// ��ʑS�̉����o
		EFFECT_SUMMON,			// ����
		EFFECT_FIRE_FLOWER,		// �N���A��ԉ�
		EFFECT_HEALING,			// �q�[���G�t�F�N�g
	};

	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );
	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );

	void SetEffectAnim( const std::string &animStr );
	void SetDrawFlag( const bool &isDraw ){ m_isInvalid = !isDraw; }

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	GameEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffect(void);

	std::string SelectEffectFile() const;		// �ǂݍ��݃t�@�C���I�ʊ֐�

	bool			m_isInvalid;		// �`�斳���t���O
	EFFECT_KIND		m_kind;				// ���o�̎��
	Texture2D		m_textureEffect;	// �G�t�F�N�g

};

class PreFireWall : public GameEffect
{
public:

	PreFireWall( const EFFECT_KIND &kind, const math::Vector2 &pos );

protected:

	virtual void Update() override;

private:

	virtual ~PreFireWall(void);

	uint32_t	m_liveTime;	// �������Ă���̎���
};

// �����蔻��t���Q�[���G�t�F�N�g
class GameEffectWithCollision : public TaskUnit, public Collision2DUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_EXPLOSION,	// ����
		EFFECT_FIRE,		// ��
		EFFECT_POISON,		// ��

		EFFECT_MAX
	};
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );

protected:

	GameEffectWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffectWithCollision(void);
	
	virtual bool Init() override;
	virtual void DrawUpdate() override;
	
	// ���̃N���X�̎�ރZ�b�g
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:

	std::string SelectEffectFile() const;		// �ǂݍ��݃t�@�C���I�ʊ֐�
	
	Common::OWNER_TYPE	m_ownerType;
	EFFECT_KIND			m_kind;				// ���o�̎��

};

class FireWithCollision : public GameEffectWithCollision
{

public:

	FireWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );

protected:

	virtual void Update() override;

private:

	virtual ~FireWithCollision(void);

	uint32_t	m_liveTime;	// �������Ă���̎���
};

class GameEffectDamage : public TaskUnit
{

public:

	static GameEffectDamage *GetInstance();

	void CreateEffectDamage( const uint32_t &value, const int32_t &posX, const int32_t &posY, bool isPlayer = false );
	virtual ~GameEffectDamage(void);

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

private:

	GameEffectDamage();

	struct EFFECT_DAMAGE_INFO{
		uint32_t m_value;
		uint32_t m_liveTime;
		std::vector<Texture2D>	m_array2D;

		void Init(){
			m_value		= 0;
			m_liveTime	= 0;
			m_array2D.clear();
		}
	};

	static GameEffectDamage	*s_pInstance;

	std::vector<EFFECT_DAMAGE_INFO> m_damageArray;
};

class GameEffectLoop : public TaskUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_STATUS_ICON		// �ُ��Ԃ�\���A�C�R��
	};

	static GameEffectLoop *CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );
	static GameEffectLoop *CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );

	void SetEffectAnim( const std::string &animStr );
	void SetDrawFlag( const bool &isDraw ){ m_isInvalid = !isDraw; }

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	GameEffectLoop( const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffectLoop(void);

	std::string SelectEffectFile() const;		// �ǂݍ��݃t�@�C���I�ʊ֐�

	bool			m_isInvalid;		// �`�斳���t���O
	EFFECT_KIND		m_kind;				// ���o�̎��
	Texture2D		m_textureEffect;	// �G�t�F�N�g

};

#endif