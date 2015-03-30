/* ====================================================================== */
/**
 * @brief  ２Ｄ描画をまとめて行うクラス
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
	s_pInstance = NULL;
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
 * @brief	描画タスク追加
 */
/* ================================================ */
void Draw2DManager::PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const PRIORITY &priority )
{
	if( texInfo.m_fileName.compare("") == 0 ){
		DEBUG_ASSERT( 0,"jsonファイル名がNULL");
		return;
	}

	// 画面内に表示するものであれば描画
	if( Utility::IsPositionInWindowArea(texInfo) ){
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
 * @brief	順序を決めて描画
 */
/* ================================================ */
#include "Game/GameRecorder.h"
void Draw2DManager::Action()
{
	for( uint32_t i = 0; i < PRIORITY_MAX; ++i ){
		for( uint32_t j = 0; j < m_vDrawTask.size(); ++j ){
			if( m_vDrawTask.at(j).m_info.m_prioity == static_cast<PRIORITY>(i) ){
				DrawTexture(j);
			}
		}
	}


#ifdef _DEBUG
	int Color = GetColor( 255 , 255 , 255 );
	float xx = 0.0f;
	float yy = 0.0f;
	GameAccesser::GetInstance()->GetPlayerOffSet(xx, yy);
	DrawFormatString( 0, 10, Color, "PlayerX = %.1f, PlayerY = %.1f\n", xx + WINDOW_WIDTH/2, yy + WINDOW_HEIGHT/2);

	// デバッグ用目印
	for( uint32_t i = 0; i < m_drawCircle.size() ; ++i ){
		math::Vector2 tmp = m_drawCircle.at(i) - Utility::GetPlayerOffsetPos();
		DrawCircle( tmp.x, tmp.y, 3, Color, false);
	}
	m_drawCircle.clear();
#endif

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

	DRAW2D &drawInfo = m_vDrawTask.at(drawIndex);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, drawInfo.m_info.m_alpha );

	// 画像の描画位置取得
	math::Vector2 pos = math::Vector2( drawInfo.m_info.m_posOrigin.x, drawInfo.m_info.m_posOrigin.y );
	if( drawInfo.m_info.m_usePlayerOffset ){
		pos -= Utility::GetPlayerOffsetPos();
	}

	// 描画画像のサイズ取得
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( drawInfo.m_info.m_fileName.c_str() );

	DrawRotaGraph3(
		static_cast<int32_t>(pos.x)
		, static_cast<int32_t>(pos.y)
		, ( drawInfo.m_info.m_arrangeOrigin.x == INVALID_FVALUE ) ? texInfo.m_sizeWidth / 2 : drawInfo.m_info.m_arrangeOrigin.x
		, ( drawInfo.m_info.m_arrangeOrigin.y == INVALID_FVALUE ) ? texInfo.m_sizeHeight / 2 : drawInfo.m_info.m_arrangeOrigin.y
		, static_cast<double>(drawInfo.m_info.m_scale.x)
		, static_cast<double>(drawInfo.m_info.m_scale.y)
		, static_cast<double>(drawInfo.m_info.m_rot.GetRadian())
		, drawInfo.m_handle
		, true
		, false
		);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}
