/* ====================================================================== */
/**
 * @brief  ２Ｄ描画をまとめて行うクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "SystemDraw2DManager.h"
#include "Common/Utility/CommonGameUtility.h"

Draw2DManager *Draw2DManager::m_pInstance = NULL;

Draw2DManager::Draw2DManager(void)
: m_useBilinear(false)
{
}

Draw2DManager::~Draw2DManager(void)
{
}

void Draw2DManager::Create()
{
	if( m_pInstance ){
		DEBUG_ASSERT("m_pInstance is Already Created");
	}
	else{
		m_pInstance = NEW Draw2DManager();
	}
}

Draw2DManager *Draw2DManager::GetInstance()
{
	if( !m_pInstance ){
		DEBUG_ASSERT("m_pInstance is NULL");
		return NULL;
	}
	return m_pInstance;
}

/* ================================================ */
/**
 * @brief	描画タスク追加
 */
/* ================================================ */
void Draw2DManager::PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const KIND_2D &kind, const PRIORITY &priority )
{
	DRAW2D task;

	task.Init();
	task.m_info		= texInfo;
	task.m_handle	= handle;

	m_vDrawTask.push_back(task);
}

/* ================================================ */
/**
 * @brief	描画予約キャンセル
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

/* ================================================ */
/**
 * @brief	順序を決めて描画
 */
/* ================================================ */
void Draw2DManager::Action()
{
	for( uint32_t i = 0; i < KIND_MAX; ++i ){
		for( uint32_t j = 0; j < PRIORITY_MAX; ++j ){
			for( uint32_t k = 0; k < m_vDrawTask.size(); ++k ){
				if( m_vDrawTask.at(k).m_info.m_kind2D == static_cast<KIND_2D>(i)
					&& m_vDrawTask.at(k).m_info.m_prioity == static_cast<PRIORITY>(j) ){
						DrawTexture(k);
				}
			}
		}
	}

	m_vDrawTask.clear();
}

/* ================================================ */
/**
 * @brief	描画指示
 */
/* ================================================ */
void Draw2DManager::DrawTexture( const uint32_t &drawIndex )
{
	if( drawIndex > m_vDrawTask.size() ){
		return;
	}

	DRAW2D tmpInfo = m_vDrawTask.at(drawIndex);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, tmpInfo.m_info.m_alpha );

	math::Vector2 pos = math::Vector2( tmpInfo.m_info.m_pos.x, tmpInfo.m_info.m_pos.y );
	if( tmpInfo.m_info.m_usePlayerOffset ){
		pos -= GetPlayerOffsetPos();
	}

	DrawRotaGraph(
		pos.x
		, pos.y
		, tmpInfo.m_info.m_scale
		, static_cast<uint32_t>(tmpInfo.m_info.m_rot.GetRadian())
		, tmpInfo.m_handle
		, true
		);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}
