

#include "GameMap.h"
#include "System/picojson.h"
#include "System/Sound/SystemSoundManager.h"

#define MAP_TIP_SIZE	32		// �}�b�v�`�b�v��̃h�b�g�T�C�Y

#define MAP_WIDTH		100		// �}�b�v�̕�
#define MAP_HEIGHT		100		// �}�b�v�̏c����

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
	for(int32_t i = 0; i < MAP_HEIGHT; ++i){
		for(int32_t j = 0; j < MAP_WIDTH; ++j){

			//!�v���C���[���擾
			float offsetx = 0.0f;
			float offsety = 0.0f;
			GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);

			//!���ݕ`�悵�����[(��ʊO�ł�50pixel�Ԃł͕`��)
			int32_t WidthLower	= - MAP_TIP_SIZE;
			int32_t WidthUpper	= WINDOW_WIDTH + MAP_TIP_SIZE;
			int32_t HeightLower = - MAP_TIP_SIZE;
			int32_t HeightUpper = WINDOW_HEIGHT + MAP_TIP_SIZE;

			//!�}�b�v�`�b�v�̑傫��
			int32_t MapHeight	= m_texInfo.m_sizeHeight;
			int32_t MapWidht	= m_texInfo.m_sizeWidth;

			//!�`�悵�悤�Ƃ��Ă���ʒu
			int32_t posY = (i*m_texInfo.m_sizeHeight) - static_cast<uint32_t>(offsety);
			int32_t posX = (j*m_texInfo.m_sizeWidth) - static_cast<uint32_t>(offsetx);

			//!��ʓ����ǂ�������
			if(posX < WidthUpper && posX > WidthLower
				&& posY < HeightUpper && posY > HeightLower){

					uint32_t index = m_mapInfo.m_vTileKind[i*MAP_WIDTH + j];
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

void GameMap::LoadTextureInfo(const char *jsonFile)
{
	std::string path = JSON_GAME2D_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	//!��{�ƂȂ���擾
	picojson::value sceneData = root.get("basicInfo");
	m_texInfo.m_fileName	= sceneData.get(0).get("fileName").get<std::string>();
	m_texInfo.m_splitNum	= static_cast<uint32_t>(sceneData.get(0).get("total").get<double>());
	m_texInfo.m_splitWidth	= static_cast<uint32_t>(sceneData.get(0).get("splitW").get<double>());
	m_texInfo.m_splitHeight	= static_cast<uint32_t>(sceneData.get(0).get("splitH").get<double>());
	m_texInfo.m_sizeWidth	= static_cast<uint32_t>(sceneData.get(0).get("sizeW").get<double>());
	m_texInfo.m_sizeHeight	= static_cast<uint32_t>(sceneData.get(0).get("sizeH").get<double>());

	//!�e�N�X�`���ǂݍ���
	int32_t	texHandle[SPLIT_MAX];
	path = TEXTURE_PATH;
	path += m_texInfo.m_fileName;
	LoadDivGraph( path.c_str(), m_texInfo.m_splitNum
	, m_texInfo.m_splitWidth, m_texInfo.m_splitHeight
	, m_texInfo.m_sizeWidth, m_texInfo.m_sizeHeight, texHandle);

	//!�^�C�����i�[
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
		tileInfo.m_height = mapData.get(i).get("height").get<bool>();
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
	
	//!��{�ƂȂ���擾
	picojson::value mapData = root.get("layers");

	// �}�b�v�̏c��
	m_mapInfo.m_height	= static_cast<uint32_t>(mapData.get(0).get("height").get<double>());
	m_mapInfo.m_width	= static_cast<uint32_t>(mapData.get(0).get("width").get<double>());

	// �}�b�v�̃`�b�v�f�[�^�擾
	picojson::value null;
	for( uint32_t i = 0;;++i ){
		if( mapData.get(0).get("data").get(i) == null ){
			break;
		}
		MAP_INFO mapsTile;
		uint32_t tileKind = static_cast<uint32_t>(mapData.get(0).get("data").get(i).get<double>());
		m_mapInfo.m_vTileKind.push_back(tileKind);
	}

	// �g�p���Ă���}�b�v�e�N�X�`�����擾
	picojson::value tileData = root.get("tilesets");
	std::string texName	= tileData.get(0).get("image").get<std::string>();
	// ���O�ƕR�t����json�t�@�C�����擾
	std::string extentionPNG = ".png";
	std::string extentionJSON = ".json";
	uint32_t extentionIndex = texName.find( extentionPNG, 0 );
	texName.replace( extentionIndex, extentionJSON.length(), extentionJSON );
	LoadTextureInfo( texName.c_str() );
}
