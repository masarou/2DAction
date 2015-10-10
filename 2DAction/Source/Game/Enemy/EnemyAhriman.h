/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BBB__
#define __ENEMY_BBB__

#include "EnemyBase.h"

class AttackGun;

class EnemyBBB : public EnemyBase
{
public:

	static EnemyBBB *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_AHRIMAN;}
	void EventUpdate( Common::CMN_EVENT &eventId ) override;
	virtual const uint32_t	GetEnemyDefaultHP() const override;	// �f�t�H���gHP�擾
	virtual const float		GetEnemyDefaultSPD() const override;	// �f�t�H���gSPD�擾

private:

	EnemyBBB( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyBBB(void);

};

#endif ENEMY_BBB