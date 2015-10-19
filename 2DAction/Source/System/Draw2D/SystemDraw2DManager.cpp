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
void Draw2DManager::PushDrawString( const std::string &str, const math::Vector2 &pos, const uint32_t &color )
{
	DRAWSTR task;

	task.Init();
	task.m_str		= str;
	task.m_drawPos	= pos;
	task.m_color	= color;

	m_vDrawStringTask.push_back(task);
}


void Draw2DManager::PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const Common::PRIORITY &priority )
{
	if( texInfo.m_fileName.compare("") == 0 ){
		DEBUG_ASSERT( 0,"jsonファイル名がNULL");
		return;
	}

	// 画面内に表示するものであれば描画
	if( texInfo.m_drawForce || Utility::IsPositionInWindowArea(texInfo) ){
		PushDrawInfoMain( texInfo, handle, priority );
	}
}

void Draw2DManager::PushDrawInfoMain( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const Common::PRIORITY &priority )
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
	// 予約テクスチャ描画
	for( uint32_t i = 0; i < Common::PRIORITY_MAX; ++i ){
		for( uint32_t j = 0; j < m_vDrawTask.size(); ++j ){
			if( m_vDrawTask.at(j).m_info.m_prioity == static_cast<Common::PRIORITY>(i) ){
				DrawTexture(j);
			}
		}
		// 文字列は優先度固定( PRIORITY_HIGH )
		if( Common::PRIORITY_HIGH == static_cast<Common::PRIORITY>(i) ){
			for( uint32_t i = 0; i < m_vDrawStringTask.size(); ++i ){
				DrawString( m_vDrawStringTask.at(i) );
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
		math::Vector2 tmp = m_drawCircle.at(i).pos - Utility::GetPlayerOffsetPos();
		DrawCircle( static_cast<int>(tmp.x), static_cast<int>(tmp.y), m_drawCircle.at(i).radius, Color, false);
	}
	m_drawCircle.clear();
	for( uint32_t i = 0; i < m_drawBox.size(); ++i ){
		DrawBox( m_drawBox.at(i).leftUpper.x, m_drawBox.at(i).leftUpper.y
			, m_drawBox.at(i).leftUpper.x + m_drawBox.at(i).sideDistance, m_drawBox.at(i).leftUpper.y + m_drawBox.at(i).sideDistance, Color, false );
	}

#endif

	m_vDrawTask.clear();
	m_vDrawStringTask.clear();
}

/* ================================================ */
/**
 * @brief	描画指示
 */
/* ================================================ */
void Draw2DManager::DrawString( const DRAWSTR &drawTask )
{
	std::istringstream iss( drawTask.m_str );
	std::vector< std::string > strArray;

	std::string tmpStr = "";
	while( std::getline( iss, tmpStr, '\n' ) ){
		strArray.push_back( tmpStr );
	};

	for( uint32_t i = 0; i < strArray.size() ; ++i ){
		DrawFormatString( 
			static_cast<uint32_t>( drawTask.m_drawPos.x ),
			static_cast<uint32_t>( drawTask.m_drawPos.y + (i*FONT_DEFAULT_SIZE+i*10) ),
			drawTask.m_color,
			strArray.at(i).c_str()
			);
	}
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
		, ( drawInfo.m_info.m_arrangeOrigin.x == INVALID_FVALUE ) ? texInfo.m_sizeWidth / 2 : static_cast<int>( drawInfo.m_info.m_arrangeOrigin.x )
		, ( drawInfo.m_info.m_arrangeOrigin.y == INVALID_FVALUE ) ? texInfo.m_sizeHeight / 2 : static_cast<int>( drawInfo.m_info.m_arrangeOrigin.y )
		, static_cast<double>(drawInfo.m_info.m_scale.x)
		, static_cast<double>(drawInfo.m_info.m_scale.y)
		, static_cast<double>(drawInfo.m_info.m_rot.GetRadian())
		, drawInfo.m_handle
		, static_cast<int>(true)
		, static_cast<int>(false)
		);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}
