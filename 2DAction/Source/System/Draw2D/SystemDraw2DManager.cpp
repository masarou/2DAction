/* ====================================================================== */
/**
 * @brief  ÇQÇcï`âÊÇÇ‹Ç∆ÇﬂÇƒçsÇ§ÉNÉâÉX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "SystemDraw2DManager.h"
#include "SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

Draw2DManager *Draw2DManager::s_pInstance = NULL;

Draw2DManager::Draw2DManager(void)
: m_useBilinear(false)
{
}

Draw2DManager::~Draw2DManager(void)
{
}

void Draw2DManager::DeleteDraw2DManager()
{
	SAFE_DELETE( s_pInstance );
}

void Draw2DManager::Create()
{
	if( s_pInstance ){
		DEBUG_ASSERT( 0, "s_pInstance is Already Created");
	}
	else{
		s_pInstance = NEW Draw2DManager();
	}
}

Draw2DManager *Draw2DManager::GetInstance()
{
	if( !s_pInstance ){
		DEBUG_ASSERT( 0, "s_pInstance is NULL");
		return NULL;
	}
	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	ï`âÊÉ^ÉXÉNí«â¡
 */
/* ================================================ */
void Draw2DManager::PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const PRIORITY &priority )
{
	// âÊñ ì‡Ç…ï\é¶Ç∑ÇÈÇ‡ÇÃÇ≈Ç†ÇÍÇŒï`âÊ
	if( IsPositionInWindowArea(texInfo) ){
		PushDrawInfoMain( texInfo, handle, priority );
	}
}

void Draw2DManager::PushDrawInfoMain( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const PRIORITY &priority )
{
	DRAW2D task;

	task.Init();
	task.m_info		= texInfo;
	task.m_handle	= handle;

	m_vDrawTask.push_back(task);
}


/* ================================================ */
/**
 * @brief	ï`âÊó\ñÒÉLÉÉÉìÉZÉã
 */
/* ================================================ */
void Draw2DManager::DeleteDrawInfo( const int32_t &handle )
{
	std::vector<DRAW2D>::iterator it = m_vDrawTask.begin();
	for( uint32_t i = 0; i < m_vDrawTask.size(); ++i ){
		if( handle == m_vDrawTask.at(i).m_handle ){
			it = m_vDrawTask.erase(it);
			break;
		}
		++it;
	}
}
void Draw2DManager::DeleteDrawInfo( const char *jsonFile )
{
	std::vector<DRAW2D>::iterator it = m_vDrawTask.begin();
	for( uint32_t i = 0; i < m_vDrawTask.size(); ++i ){
		if( m_vDrawTask.at(i).m_info.m_fileName.compare( jsonFile ) == 0 ){
			it = m_vDrawTask.erase(it);
		}
		else{
			++it;
		}
	}
}

/* ================================================ */
/**
 * @brief	èáèòÇåàÇﬂÇƒï`âÊ
 */
/* ================================================ */
void Draw2DManager::Action()
{
	for( uint32_t i = 0; i < PRIORITY_MAX; ++i ){
		for( uint32_t j = 0; j < m_vDrawTask.size(); ++j ){
			if( m_vDrawTask.at(j).m_info.m_prioity == static_cast<PRIORITY>(i) ){
				DrawTexture(j);
			}
		}
	}

	m_vDrawTask.clear();
}

/* ================================================ */
/**
 * @brief	ï`âÊéwé¶
 */
/* ================================================ */
void Draw2DManager::DrawTexture( const uint32_t &drawIndex )
{
	if( drawIndex > m_vDrawTask.size() ){
		return;
	}

	DRAW2D &drawInfo = m_vDrawTask.at(drawIndex);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, drawInfo.m_info.m_alpha );

	// âÊëúÇÃï`âÊà íuéÊìæ
	math::Vector2 pos = math::Vector2( drawInfo.m_info.m_pos.x, drawInfo.m_info.m_pos.y );
	if( drawInfo.m_info.m_usePlayerOffset ){
		pos -= GetPlayerOffsetPos();
	}

	// ï`âÊâÊëúÇÃÉTÉCÉYéÊìæ
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( drawInfo.m_info.m_fileName.c_str() );

	DrawRotaGraph3(
		pos.x
		, pos.y
		, ( drawInfo.m_info.m_arrangeOrigin.x == INVALID_FVALUE ) ? texInfo.m_sizeWidth / 2 : drawInfo.m_info.m_arrangeOrigin.x
		, ( drawInfo.m_info.m_arrangeOrigin.y == INVALID_FVALUE ) ? texInfo.m_sizeHeight / 2 : drawInfo.m_info.m_arrangeOrigin.y
		, static_cast<uint32_t>(drawInfo.m_info.m_scale.x)
		, static_cast<uint32_t>(drawInfo.m_info.m_scale.y)
		, static_cast<uint32_t>(drawInfo.m_info.m_rot.GetRadian())
		, drawInfo.m_handle
		, true
		, false
		);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}
