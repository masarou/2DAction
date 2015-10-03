/* ====================================================================== */
/**
 * @brief  �e��ʃp�[�c�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemMenuParts.h"
#include "SystemMenuPartsFactory.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

MenuParts *MenuParts::Create( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
{
	return NEW MenuParts( partsStr, jsonStr, priority, originalPos );
}

MenuParts::MenuParts( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
: m_invalidDraw( false )
, m_partsNameStr( partsStr )
, m_readJsonStr( jsonStr )
, m_originPos( originalPos )
, m_priority( priority )
{
	m_texMine.Init();
	m_partsArray.clear();
	m_partsMap.clear();

	// �����Ńp�[�c�̂��ׂĂ��Z�b�g�A�b�v!!
	SetupParts();
}


MenuParts::~MenuParts(void)
{
}

/* ================================================ */
/**
 * @brief	��ʃp�[�c�̍X�V�g���K�[ �q�p�[�c���܂ߍX�V�w��
 */
/* ================================================ */
void MenuParts::UpdatePartsRecursive()
{
	UpdateParts();

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at(i)->UpdatePartsRecursive();
	}
}

/* ================================================ */
/**
 * @brief	��ʃp�[�c�̕`��g���K�[ �q�p�[�c���܂ߕ`��w��
 */
/* ================================================ */
void MenuParts::DrawPartsRecursive()
{
	if( !m_invalidDraw && m_texMine.m_pTex2D ){
		m_texMine.m_pTex2D->DrawUpdate2D();
	}

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at(i)->DrawPartsRecursive();
	}
}

/* ================================================ */
/**
 * @brief	�q���܂߂ăp�[�c�폜
 */
/* ================================================ */
MenuParts *MenuParts::GetPartsRecursive( const std::string &partsStr )
{
	// �T���Ă���̂������ł͂Ȃ����`�F�b�N
	if( m_partsNameStr.compare( partsStr ) == 0 ){
		return this;
	}

	// �����łȂ���Ύq�N���X��T��
	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		MenuParts *pSearchResult = m_partsArray.at(i)->GetPartsRecursive( partsStr );
		if( pSearchResult ){
			return pSearchResult;
		}
	}

	// ������Ȃ�����
	return NULL;
}


/* ================================================ */
/**
 * @brief	�`�悷�邩�ǂ����t���O�Z�b�g
 */
/* ================================================ */
void MenuParts::SetDrawFlag( const bool &isDraw )
{
	m_invalidDraw = !isDraw;
	
	// �q�N���X�ɂ��`�B
	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at( i )->SetDrawFlag( isDraw );
	}
}

/* ================================================ */
/**
 * @brief	�q���܂߂ăp�[�c�폜
 */
/* ================================================ */
bool MenuParts::DeletePartsAll()
{
	auto it = m_partsArray.begin();
	while( it != m_partsArray.end() ){
		MenuParts *parts = *it;

		// �p�[�c�ɍ폜���ėǂ������f��
		bool isFinish = parts->DeletePartsAll();
		if( !isFinish ){
			return false;
			//break;
		}

		// �폜�����ł����̂ō폜
		it = m_partsArray.erase( it );
		SAFE_DELETE( parts );
	}
	
	// �����폜
	m_texMine.DeleteAndInit();
	return true;
}

/* ================================================ */
/**
 * @brief	�w��p�[�c����T���{�w�胉�x���Z�b�g�ċA�֐�
 */
/* ================================================ */
bool MenuParts::SetPartsAnim( const std::string &partsName, const std::string &animStr )
{
	if( m_partsNameStr.compare( partsName ) == 0 ){
		m_texMine.m_pTex2D->SetAnim( animStr );
		return true;
	}

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		if( m_partsArray.at(i)->SetPartsAnim( partsName, animStr ) ){
			return true;
		}
	}
	return false;
}

/* ================================================ */
/**
 * @brief	��ʃZ�b�g�A�b�v�����ċA�֐�
 */
/* ================================================ */
void MenuParts::SetupParts()
{
	// �X�e�[�^�X���j���[�̃p�[�c���擾
	Utility::GetPartsInfoFromJson( m_readJsonStr, m_partsMap );
	auto it = m_partsMap.begin();
	for(;;)
	{
		if( it == m_partsMap.end() ){
			break;
		}
		MenuParts *parts = CreatePartsFactory( it->second.m_type, it->first, it->second.m_jsonStr, m_priority, it->second.m_pos + m_originPos );
		m_partsArray.push_back(parts);
		++it;
	}

	if( m_partsNameStr.compare("root") != 0 || m_partsArray.empty() ){
		// root(��ԏ�)�łȂ���Ύ��g�̃p�[�c�Z�b�g
		// ����Ȃ����Ƃ�����̂�NULL�`�F�b�N���s��
		m_texMine.m_pTex2D = Game2DBase::CreateWithCheck( m_readJsonStr.c_str() );
		if( m_texMine.m_pTex2D ){
			TEX_DRAW_INFO drawInfo;
			drawInfo.m_fileName = m_readJsonStr.c_str();
			drawInfo.m_posOrigin.x	= m_originPos.x;
			drawInfo.m_posOrigin.y	= m_originPos.y;
			drawInfo.m_prioity		= m_priority;
			drawInfo.m_usePlayerOffset = false;
			m_texMine.m_pTex2D->SetDrawInfo( drawInfo );
		}
	}



}

