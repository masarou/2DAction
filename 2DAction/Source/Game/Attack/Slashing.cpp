/* ====================================================================== */
/**
 * @brief  �a���A�j��
 *
 * @note
 *		���Đ������E ����Unit�Ƃ̓����蔻�肪����̂ő����̂��̂��p��
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "Slashing.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

Slashing::Slashing( const Common::OWNER_TYPE ownerType, const TYPE_SLASHING &type, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage, bool deleteBullet )
: TaskUnit( "Slashing" )
, m_ownerType( ownerType )
, m_slashingType( type )
, m_liveTime( 0 )
, m_IsDeleteBullet( deleteBullet )
, m_bladeDamage( damage )
{
	std::string jsonStr = GetJsonFileStr();
	m_drawTexture.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	const TEX_INIT_INFO &slashingInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	std::string setAnim = "";
	math::Vector2 vecOffset = math::Vector2();	// �v���C���[�̈ʒu�ɂ��̂܂܏o���̂ł͂Ȃ������ɂ���đ������炷
	switch( Utility::GetDirection( vec.x, vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		setAnim = "up";
		vecOffset = math::Vector2( 0.0f, slashingInfo.m_sizeHeight / -3.0f );
		break;
	case InputWatcher::BUTTON_DOWN:
		setAnim = "down";
		vecOffset = math::Vector2( 0.0f, slashingInfo.m_sizeHeight / 3.0f );
		break;
	case InputWatcher::BUTTON_LEFT:
		setAnim = "left";
		vecOffset = math::Vector2( slashingInfo.m_sizeWidth / -3.0f, 0.0f );
		break;
	case InputWatcher::BUTTON_RIGHT:
		setAnim = "right";
		vecOffset = math::Vector2( slashingInfo.m_sizeWidth / 3.0f, 0.0f );
		break;
	}
	m_drawTexture.m_pTex2D->SetAnim( setAnim );
	
	//!�����ʒu�Z�b�g
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_fileName = jsonStr;
	drawInfo.m_posOrigin = pos + vecOffset;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );
}

Slashing::~Slashing(void)
{
}

bool Slashing::DieMain()
{
	return true;
}

bool Slashing::Init()
{
	// �����蔻��t���[�����擾���Ă���
	GetHitFrame();
	SetInvalidCollisionFlag( true );

	return true;
}

const TEX_DRAW_INFO &Slashing::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�e�̕`��N���X��NULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void Slashing::Update()
{
	// �����蔻��t���[�����ǂ����`�F�b�N
	bool needCollision = false;
	for( uint32_t i = 0; i < m_hitFrame.size() ; ++i ){
		if( m_drawTexture.m_pTex2D->GetCurrentAnimFrame() == m_hitFrame.at(i) ){
			needCollision = true;
			break;
		}
	}
	SetInvalidCollisionFlag( !needCollision );

	++m_liveTime;
}

void Slashing::DrawUpdate()
{
	if( std::string("").compare(m_drawTexture.m_pTex2D->GetPlayAnim()) == 0 ){
		// �A�j���͏I������̂Ŏ��E�A�e�ɂ��`����
		Message msg = Message( SLASHING_ANIM_END );
		SendMessageToParent( msg );
		TaskStartDie();
		return;
	}

	// �`��
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void Slashing::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_ENEMY_SLIME:	// �G�ɓ�������
	case Common::EVENT_HIT_ENEMY_SLIME_ANOTHER:	// �G�ɓ�������
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
		if( m_ownerType == Common::OWNER_PLAYER ){
			//m_liveTime = BULLET_LIVE_TIME;
			//TaskStartDie();
		}
		break;
		
	case Common::EVENT_HIT_PLAYER:
		if( m_ownerType == Common::OWNER_ENEMY ){
			//m_liveTime = BULLET_LIVE_TIME;
			//TaskStartDie();
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̃N���X�̎�ރZ�b�g
 */
/* ================================================ */
const Common::TYPE_OBJECT Slashing::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_BLADE_PLAYER;
	if( m_ownerType == Common::OWNER_ENEMY ){
		type = Common::TYPE_BLADE_ENEMY;
	}
	return type;
}

/* ================================================ */
/**
 * @brief	�����蔻�肪�L���ƂȂ�t���[���擾
 */
/* ================================================ */
void Slashing::GetHitFrame()
{
	std::string path = JSON_GAME2D_PATH;
	path += m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName;
	idxfstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	picojson::value sceneData = root.get("hitInfo");
	picojson::value null;
	for( uint32_t i = 0;; ++i ){
		if( sceneData.get(0).get("hitFrame").get(i) == null ){
			break;
		}
		
		uint32_t frame = static_cast<uint32_t>(sceneData.get(0).get("hitFrame").get(i).get<double>());
		m_hitFrame.push_back( frame );
	}
}

/* ================================================ */
/**
 * @brief	�ǂݍ��ނׂ��t�@�C�����擾
 */
/* ================================================ */
const std::string Slashing::GetJsonFileStr()
{
	std::string retStr = "";
	
	switch( m_slashingType ){
	default:
		DEBUG_ASSERT( 0, "�z��O�̎a���^�C�v" );
		/* fall-through */
	case TYPE_1ST:
		retStr = "Slashing1st.json";
		break;
	case TYPE_2ND:
		retStr = "Slashing2nd.json";
		break;
	case TYPE_3RD:
		retStr = "Slashing3rd.json";
		break;
	}
	return retStr;
}
