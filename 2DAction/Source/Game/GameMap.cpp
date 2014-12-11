

#include "GameMap.h"
#include "System/picojson.h"
#include "System/Sound/SystemSoundManager.h"

#define MAP_TIP_SIZE	32		// マップチップ一つのドットサイズ

GameMap *GameMap::CreateGameMap()
{
	return NEW GameMap();
}

GameMap::GameMap(void)
: TaskUnit("GameMap")
{
	m_texInfo.Init();
	m_mapInfo.Init();
	m_vTileInfo.clear();

	LoadMapInfo("mapInfo.json");
}


GameMap::~GameMap(void)
{
}

void GameMap::Update()
{

}

void GameMap::DrawUpdate()
{
	for(uint32_t i = 0; i < m_mapInfo.m_height; ++i){
		for(uint32_t j = 0; j < m_mapInfo.m_width; ++j){

			//!プレイヤー情報取得
			float offsetx = 0.0f;
			float offsety = 0.0f;
			GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);

			//!現在描画したい端(画面外でも一マス分では描画)
			int32_t WidthLower	= -1 * m_texInfo.m_sizeHeight;
			int32_t WidthUpper	= WINDOW_WIDTH + m_texInfo.m_sizeHeight;
			int32_t HeightLower = -1 * MAP_TIP_SIZE;
			int32_t HeightUpper = WINDOW_HEIGHT + m_texInfo.m_sizeHeight;

			//!マップチップの大きさ
			int32_t MapHeight	= m_texInfo.m_sizeHeight;
			int32_t MapWidht	= m_texInfo.m_sizeWidth;

			//!描画しようとしている位置
			int32_t posY = (i*m_texInfo.m_sizeHeight) - static_cast<uint32_t>(offsety);
			int32_t posX = (j*m_texInfo.m_sizeWidth) - static_cast<uint32_t>(offsetx);

			//!画面内かどうか判定
			if(posX < WidthUpper && posX > WidthLower
				&& posY < HeightUpper && posY > HeightLower){

					uint32_t index = m_mapInfo.m_vTileKind[i*m_mapInfo.m_width + j];
					TILE_INFO tmpInfo = m_vTileInfo.at(index-1);
					DrawRotaGraph(
					posX
					, posY
					, 1.0f
					, 0.0f
					, tmpInfo.m_tileHandle
					, true);
			}
		}
	}
}

const uint32_t GameMap::GetTileHeight( const uint32_t &posX, const uint32_t &posY ) const
{
	return GetTileHeight( math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

const uint32_t GameMap::GetTileHeight( const math::Vector2 &pos ) const
{
	if( m_vTileInfo.size() == 0 || m_mapInfo.m_vTileKind.size() == 0 ){
		return 0;
	}

	uint32_t retValue = 0;
	uint32_t row	= ( static_cast<uint32_t>(pos.y) / m_texInfo.m_sizeHeight);	// マップ全体で縦何番目か
	uint32_t column	= ( static_cast<uint32_t>(pos.x) / m_texInfo.m_sizeWidth);	// 横何番目か
	uint32_t index	= row*m_mapInfo.m_width + column;

	if( m_mapInfo.m_vTileKind.size() > index ){
		uint32_t tileKind = m_mapInfo.m_vTileKind.at(index);
		for( uint32_t i = 0; i < m_vTileInfo.size() ; ++i ){
			if( m_vTileInfo.at(i).m_tileTileKind == tileKind - 1 ){
				retValue = m_vTileInfo.at(i).m_tileHeight;
				break;
			}
		}
	}
	return retValue;
}

// 二進数表示の全てのビットの前に0をつける
// 例) 引数 5(101) → 17(010001)
DWORD BitSeparate32( uint32_t n )
{
	n = (n|(n<<8)) & 0x00ff00ff;	// 0000 0000 1111 1111 0000 0000 1111 1111
	n = (n|(n<<4)) & 0x0f0f0f0f;	// 0000 1111 0000 1111 0000 1111 0000 1111
	n = (n|(n<<2)) & 0x33333333;	// 0011 0011 0011 0011 0011 0011 0011 0011
	return (n|(n<<1)) & 0x55555555;	// 0101 0101 0101 0101 0101 0101 0101 0101
}

const uint32_t GameMap::Get2DMortonNumber( const math::Vector2 &pos ) const
{
	return (BitSeparate32(static_cast<uint32_t>(pos.x)) | (BitSeparate32(static_cast<uint32_t>(pos.y))<<1));
}

const uint32_t GameMap::GetBelongArea( const math::Vector2 &pos ) const
{
	uint32_t retValue = 0;

	uint32_t columnNum = m_mapInfo.m_vTileKind.size() / m_mapInfo.m_width;
	uint32_t rowNum = m_mapInfo.m_vTileKind.size() / m_mapInfo.m_height;

	// 調べるエリア最小の一片の長さ
	float width = (columnNum*MAP_TIP_SIZE) / pow(static_cast<double>(2), static_cast<double>(HIT_AREA_SPLIT_MAX));
	float height = (rowNum*MAP_TIP_SIZE) / pow(static_cast<double>(2), static_cast<double>(HIT_AREA_SPLIT_MAX));

	uint32_t numberC = static_cast<uint32_t>(pos.x / height);
	uint32_t numberR = static_cast<uint32_t>(pos.y / width);

	retValue = Get2DMortonNumber( math::Vector2(static_cast<float>(numberC), static_cast<float>(numberR)) );
	return retValue;
}

void GameMap::LoadTextureInfo(const char *jsonFile)
{
	std::string path = JSON_GAME2D_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	//!基本となる情報取得
	picojson::value sceneData = root.get("basicInfo");
	m_texInfo.m_fileName	= sceneData.get(0).get("fileName").get<std::string>();
	m_texInfo.m_splitNum	= static_cast<uint32_t>(sceneData.get(0).get("total").get<double>());
	m_texInfo.m_splitWidth	= static_cast<uint32_t>(sceneData.get(0).get("splitW").get<double>());
	m_texInfo.m_splitHeight	= static_cast<uint32_t>(sceneData.get(0).get("splitH").get<double>());
	m_texInfo.m_sizeWidth	= static_cast<uint32_t>(sceneData.get(0).get("sizeW").get<double>());
	m_texInfo.m_sizeHeight	= static_cast<uint32_t>(sceneData.get(0).get("sizeH").get<double>());

	//!テクスチャ読み込み
	int32_t	texHandle[SPLIT_MAX];
	path = TEXTURE_PATH;
	path += m_texInfo.m_fileName;
	LoadDivGraph( path.c_str(), m_texInfo.m_splitNum
	, m_texInfo.m_splitWidth, m_texInfo.m_splitHeight
	, m_texInfo.m_sizeWidth, m_texInfo.m_sizeHeight, texHandle);

	//!タイル情報格納
	picojson::value mapData = root.get("mapinfo");
	for(uint32_t i = 0;; ++i){
		picojson::value null;
		if(mapData.get(i) == null){
			break;
		}
		TILE_INFO tileInfo;
		tileInfo.Init();

		tileInfo.m_tileTileKind = static_cast<uint32_t>(mapData.get(i).get("kind").get<double>());
		tileInfo.m_tileHandle = texHandle[i];
		tileInfo.m_tileHeight = static_cast<uint32_t>(mapData.get(i).get("height").get<double>());
		m_vTileInfo.push_back(tileInfo);
	}
}


void GameMap::LoadMapInfo(const char *jsonFile)
{
	std::string path = JSON_GAME2D_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	//!基本となる情報取得
	picojson::value mapData = root.get("layers");

	// マップの縦横
	m_mapInfo.m_height	= static_cast<uint32_t>(mapData.get(0).get("height").get<double>());
	m_mapInfo.m_width	= static_cast<uint32_t>(mapData.get(0).get("width").get<double>());

	// マップのチップデータ取得
	picojson::value null;
	for( uint32_t i = 0;;++i ){
		if( mapData.get(0).get("data").get(i) == null ){
			break;
		}
		MAP_INFO mapsTile;
		uint32_t tileKind = static_cast<uint32_t>(mapData.get(0).get("data").get(i).get<double>());
		m_mapInfo.m_vTileKind.push_back(tileKind);
	}

	// 使用しているマップテクスチャ名取得
	picojson::value tileData = root.get("tilesets");
	std::string texName	= tileData.get(0).get("image").get<std::string>();
	// 名前と紐付いたjsonファイルを取得
	std::string extentionPNG = ".png";
	std::string extentionJSON = ".json";
	uint32_t extentionIndex = texName.find( extentionPNG, 0 );
	texName.replace( extentionIndex, extentionJSON.length(), extentionJSON );
	LoadTextureInfo( texName.c_str() );
}
