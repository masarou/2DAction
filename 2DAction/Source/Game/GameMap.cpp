

#include "GameMap.h"
#include "System/picojson.h"
#include "System/Sound/SystemSoundManager.h"

#define MAP_SIZE	64			// マップチップ一つのドットサイズ

#define MAP_WIDTH	20			// マップの幅
#define MAP_HEIGHT	17			// マップの縦長さ

uint32_t MapData[ MAP_HEIGHT ][ MAP_WIDTH ] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 } ,
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ,
} ;

GameMap *GameMap::CreateGameMap()
{
	return NEW GameMap();
}

GameMap::GameMap(void)
: TaskUnit("GameMap")
{
	LoadTextureInfo("map.json");
}


GameMap::~GameMap(void)
{
}

void GameMap::Update()
{

}

void GameMap::DrawUpdate()
{
	for(int32_t i = 0; i < MAP_HEIGHT; ++i){
		for(int32_t j = 0; j < MAP_WIDTH; ++j){

			//!プレイヤー情報取得
			float offsetx = 0.0f;
			float offsety = 0.0f;
			GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);

			//!現在描画したい端(画面外でも50pixel間では描画)
			int32_t WidthLower	= - 50;
			int32_t WidthUpper	= WINDOW_WIDTH + 50;
			int32_t HeightLower = - 50;
			int32_t HeightUpper = WINDOW_HEIGHT + 50;

			//!マップチップの大きさ
			int32_t MapHeight	= m_texInfo.m_sizeHeight;
			int32_t MapWidht	= m_texInfo.m_sizeWidth;

			//!描画しようとしている位置
			int32_t posY = (i*m_texInfo.m_sizeHeight) - offsety;
			int32_t posX = (j*m_texInfo.m_sizeWidth) - offsetx;

			//!画面内かどうか判定
			if(posX < WidthUpper && posX > WidthLower
				&& posY < HeightUpper && posY > HeightLower){

					uint32_t index = MapData[i][j];
					TILE_INFO tmpInfo = m_vTileInfo.at(index);
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

		uint32_t index = static_cast<uint32_t>(mapData.get(i).get("kind").get<double>());
		tileInfo.m_tileHandle = texHandle[index];
		tileInfo.m_walkOver = mapData.get(i).get("loop").get<bool>();
		m_vTileInfo.push_back(tileInfo);
	}


}
