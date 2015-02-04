/* ====================================================================== */
/**
 * @brief  �t�B�[���h�ɖ���ׂɏo�Ă���A�C�e���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "ItemObject.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

ItemObject *ItemObject::Create( const ITEM_KIND &kind, math::Vector2 pos )
{
	return NEW ItemObject( kind, pos );
}

ItemObject::ItemObject( const ITEM_KIND &kind, math::Vector2 pos )
: TaskUnit("ItemObject")
, m_kindItem(kind)
, m_liveTime(0)
{
	m_drawTexture.m_texInfo.m_posOrigin = pos;
}


ItemObject::~ItemObject(void)
{
}

bool ItemObject::Init()
{
	m_drawTexture.m_pTex2D = NEW Game2DBase( GetItemFilePath().c_str() );

	//!�����ʒu�Z�b�g
	m_drawTexture.m_texInfo.m_fileName = GetItemFilePath().c_str();
	if( m_drawTexture.m_texInfo.m_posOrigin == DEFAULT_VECTOR2 ){
		m_drawTexture.m_texInfo.m_posOrigin = Utility::GetMapRandamPos( /*allowInWindow=*/false );
	}
	m_drawTexture.m_pTex2D->SetDrawInfo( m_drawTexture.m_texInfo );

	return true;
}

bool ItemObject::DieMain()
{
	if( GameRegister::GetInstance()->GetManagerItem() ){
		GameRegister::GetInstance()->UpdateManagerItem()->RemoveItem( this );
	}
	return true;
}

void ItemObject::Update()
{
	// �������ԍX�V
	++m_liveTime;
}

void ItemObject::DrawUpdate()
{
	// ������O�b�O���炢����_�ł�����
	if( m_liveTime > ITEM_LIVE_TIME - 180
		&& m_liveTime < ITEM_LIVE_TIME ){
		if( m_liveTime%3 != 1 ){
			m_drawTexture.m_pTex2D->DrawUpdate2D();
		}
	}
	else if( m_liveTime >= ITEM_LIVE_TIME ){
		// ���S
		TaskStartDie();
	}
	else{
		// �A�C�e���`��
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

/* ================================================ */
/**
 * @brief	�`����擾�֐�
 */
/* ================================================ */
const TEX_DRAW_INFO &ItemObject::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�A�C�e���̕`��N���X��NULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	���������҂Ȃ̂��������^�C�v�擾
 */
/* ================================================ */
const Common::TYPE_OBJECT ItemObject::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_MAX;
	switch( m_kindItem ){
	default:
		DEBUG_ASSERT( 0, "�A�C�e���̎�ނ��z��O" );
		/* fall-through */
	case ITEM_KIND_RAPID_BULLET:
		type = Common::TYPE_ITEM_BULLET;
		break;
	case ITEM_KIND_LIFE_UP:
		type = Common::TYPE_ITEM_LIFE;
		break;
	case ITEM_KIND_DAMAGE_UP:
		type = Common::TYPE_ITEM_DAMAGE;
		break;
	};
	return type;
}

/* ================================================ */
/**
 * @brief	�C�x���g����
 */
/* ================================================ */
void ItemObject::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_PLAYER:
		m_liveTime = ITEM_LIVE_TIME;
		TaskStartDie();
		SoundManager::GetInstance()->PlaySE("ShootBullet");	// �擾����炷
		break;
	default:
		break;
	}
}

/* ================================================ */
/**
 * @brief	�A�C�e���̎�ނ���Ή�����json�t�@�C�������擾
 */
/* ================================================ */
std::string ItemObject::GetItemFilePath()
{
	std::string fileName = "";
	switch( m_kindItem ){
	default:
	case ITEM_KIND_RAPID_BULLET:
		fileName = "itemBullet.json";
		break;
	case ITEM_KIND_LIFE_UP:
		fileName = "itemLife.json";
		break;
	case ITEM_KIND_DAMAGE_UP:
		fileName = "itemDamage.json";
		break;
	};

	return fileName;
}