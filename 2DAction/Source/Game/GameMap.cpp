

#include "GameMap.h"
#include "GameRegister.h"
#include "System/picojson.h"
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"

#define MAP_TIP_SIZE	32		// �}�b�v�`�b�v��̃h�b�g�T�C�Y

GameMap *GameMap::CreateGameMap( const Common::GAME_FLOW &currentKind )
{
	return NEW GameMap( currentKind );
}

GameMap::GameMap( const Common::GAME_FLOW &currentKind )
: TaskUnit("GameMap")
{
	m_texInfo.Init();
	m_mapInfo.Init();
	m_vTileInfo.clear();

	switch( currentKind ){
	default:
		DEBUG_ASSERT( 0, "Map�����݂��Ȃ��͂��̑z��O�̎��" );
		break;
	case Common::FLOW_STAGE01:
		LoadMapInfo("MapStage01.json");
		break;
	case Common::FLOW_STAGE02:
		LoadMapInfo("MapStage02.json");
		break;
	case Common::FLOW_STAGE03:
		LoadMapInfo("MapStage03.json");
		break;
	case Common::FLOW_STAGE04:
		LoadMapInfo("MapStage04.json");
		break;
	case Common::FLOW_STAGE05:
		LoadMapInfo("MapStage05.json");
		break;
	case Common::FLOW_STAGE06:
		LoadMapInfo("MapStage06.json");
		break;
	case Common::FLOW_STAGE07:
		LoadMapInfo("MapStage07.json");
		break;
	case Common::FLOW_STAGE08:
		LoadMapInfo("MapStage08.json");
		break;
	case Common::FLOW_STAGE09:
		LoadMapInfo("MapStage09.json");
		break;
	case Common::FLOW_STAGE10:
		LoadMapInfo("MapStage10.json");
		break;
	}
}


GameMap::~GameMap(void)
{
	for( uint32_t i = 0; i < m_vTileInfo.size() ; ++i ){
		DeleteGraph( m_vTileInfo.at( i ).m_tileHandle );
	}
}

void GameMap::Update()
{
	EnemyManager *pManagerEnemy	= GameRegister::GetInstance()->UpdateManagerEnemy();
	ItemManager *pManagerItem	= GameRegister::GetInstance()->UpdateManagerItem();
}

void GameMap::DrawUpdate()
{
	for(uint32_t i = 0; i < m_mapInfo.m_height; ++i){
		for(uint32_t j = 0; j < m_mapInfo.m_width; ++j){

			//!�`�悵�悤�Ƃ��Ă���ʒu
			int32_t posY = i*m_texInfo.m_sizeHeight;
			int32_t posX = j*m_texInfo.m_sizeWidth;

			//!��ʓ����ǂ�������
			if( Utility::IsPositionInWindowArea( posX, posY ) ){
				
				uint32_t index = m_mapInfo.m_vTileKind[i*m_mapInfo.m_width + j];
				TILE_INFO tmpInfo = m_vTileInfo.at(index-1);
				//!�v���C���[���擾
				float offsetx = 0.0f;
				float offsety = 0.0f;
				GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);
				posY += - static_cast<int32_t>(offsety);
				posX += - static_cast<int32_t>(offsetx);

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

const math::Vector2 GameMap::GetPlayerStartPos() const
{
	math::Vector2 pos = math::Vector2( GameRegister::GetInstance()->GetGameMap()->GetMapWidth() / 2.0f
		, GameRegister::GetInstance()->GetGameMap()->GetMapHeight() / 2.0f );
	return pos;
}

const int32_t GameMap::GetTileHeight( const math::Vector2 &pos ) const
{
	if( m_vTileInfo.size() == 0 || m_mapInfo.m_vTileKind.size() == 0 ){
		return INVALID_VALUE;
	}
	if( pos.x < 0.0f || pos.y < 0.0f ){
		return INVALID_VALUE;
	}

	int32_t retValue = INVALID_VALUE;
	uint32_t row	= ( static_cast<uint32_t>(pos.y+m_texInfo.m_sizeHeight/2) / m_texInfo.m_sizeHeight);// �}�b�v�S�̂ŏc���Ԗڂ�
	uint32_t column	= ( static_cast<uint32_t>(pos.x+m_texInfo.m_sizeWidth/2) / m_texInfo.m_sizeWidth);	// �����Ԗڂ�
	uint32_t index	= row*m_mapInfo.m_width + column;	// map��ł̔z��ԍ������߂�

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

// ��i���\���̑S�Ẵr�b�g�̑O��0������
// ��) ���� 5(101) �� 17(010001)
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

	// ���ׂ�G���A�ŏ��̈�Ђ̒���
	double splitValue = pow(static_cast<double>(2), static_cast<double>(HIT_AREA_SPLIT_MAX));
	float width = (columnNum*MAP_TIP_SIZE) / static_cast<float>( splitValue );
	float height = (rowNum*MAP_TIP_SIZE) / static_cast<float>( splitValue );

	uint32_t numberC = static_cast<uint32_t>(pos.x / height);
	uint32_t numberR = static_cast<uint32_t>(pos.y / width);

	retValue = Get2DMortonNumber( math::Vector2(static_cast<float>(numberC), static_cast<float>(numberR)) );
	return retValue;
}

// �}�b�v�T�C�Y�擾(����)
const uint32_t GameMap::GetMapWidth() const
{
	uint32_t retVal = 0;
	retVal = m_mapInfo.m_width * MAP_TIP_SIZE;
	return retVal;
}

// �}�b�v�T�C�Y�擾(�c��)
const uint32_t GameMap::GetMapHeight() const
{
	uint32_t retVal = 0;
	retVal = m_mapInfo.m_height * MAP_TIP_SIZE;
	return retVal;
}

void GameMap::LoadMapInfo(const char *jsonFile)
{
	std::string path = JSON_GAME2D_MAP_PATH;
	path += jsonFile;
	idxfstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	//!��{�ƂȂ���擾
	picojson::value mapData = root.get("layers");

	// �}�b�v�̏c���̃`�b�v��
	m_mapInfo.m_height	= static_cast<uint32_t>(mapData.get(0).get("height").get<double>());
	m_mapInfo.m_width	= static_cast<uint32_t>(mapData.get(0).get("width").get<double>());

	// �}�b�v�̃`�b�v�z��f�[�^�擾
	picojson::value null;
	for( uint32_t i = 0;;++i ){
		if( mapData.get(0).get("data").get(i) == null ){
			break;
		}
		MAP_INFO mapsTile;
		uint32_t tileKind = static_cast<uint32_t>(mapData.get(0).get("data").get(i).get<double>());
		m_mapInfo.m_vTileKind.push_back(tileKind);
	}

	// MapTip�e�N�X�`���̊�{���擾
	// ���O�ƕR�t����json�t�@�C�����擾(m_texInfo�ɏ��i�[)
	picojson::value tileData = root.get("tilesets");
	std::string texName	= tileData.get(0).get("image").get<std::string>();

	// �s�v�ȃp�X������
	std::istringstream iss( texName );
	std::vector< std::string > strArray;
	std::string tmpStr = "";
	while( std::getline( iss, tmpStr, '/' ) ){
		strArray.push_back( tmpStr );
	};
	texName = strArray.back();

	std::string extentionPNG = ".png";
	std::string extentionJSON = ".json";
	uint32_t extentionIndex = texName.find( extentionPNG, 0 );
	texName.replace( extentionIndex, extentionJSON.length(), extentionJSON );
	LoadTextureInfo( texName.c_str() );

	// �}�b�v�`�b�v�ꖇ���̏����擾
	int32_t	texHandle[SPLIT_MAX];
	path = TEXTURE_PATH;
	path += m_texInfo.m_fileName;
	LoadDivGraph( path.c_str(), m_texInfo.m_splitNum
	, m_texInfo.m_splitWidth, m_texInfo.m_splitHeight
	, m_texInfo.m_sizeWidth, m_texInfo.m_sizeHeight, texHandle);

	picojson::value tileproperties = tileData.get(0).get("tileproperties");
	for(uint32_t i = 0;; ++i){
		std::string indexStr = std::to_string( static_cast<long double>(i) );
		picojson::value null;
		if( tileproperties == null || tileproperties.get(indexStr) == null){
			break;
		}
		TILE_INFO tileInfo; 
		tileInfo.Init();
		tileInfo.m_tileTileKind = i;
		tileInfo.m_tileHandle = texHandle[i];
		tileInfo.m_tileHeight = atoi( tileproperties.get(indexStr).get("height").get<std::string>().c_str() );
		m_vTileInfo.push_back(tileInfo);
	}
}

void GameMap::LoadTextureInfo(const char *jsonFile)
{
	std::string path = JSON_GAME2D_MAP_PATH;
	path += jsonFile;
	idxfstream ifs(path.c_str());

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
}