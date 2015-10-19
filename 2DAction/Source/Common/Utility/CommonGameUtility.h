/* ====================================================================== */
/**
 * @brief  ゲーム全体utilityクラス
 *
 * @note
 *
 */
/* ====================================================================== */
#ifndef __CMN_GAME_UTILITY__
#define __CMN_GAME_UTILITY__

#include "Common/CommonDefine.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Input/SystemInputWatcher.h"

class EnemyAIBase;

namespace Utility
{

// ゲームポーズのセットと確認
void StartGamePause();
void EndGamePause();
bool IsGamePause();

// ゲームストップ
void StartGameStop( bool withFileter = false );	// 引数は黒板(BlackFilter)の有無
void EndGameStop();
bool IsGameStop();
bool IsShowPauseFilter();

// ゲームが一周して終わるとき、タイトルに戻るときなどにプレイ記録リセット
void GameInitALL();

// プレイヤーオフセットの変更と取得
void AddPlayerOffsetPos( math::Vector2 &pos );
const math::Vector2 &GetPlayerOffsetPos();

// 二つの所属空間が当たり判定が必要かどうか判定
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// 描画位置が重なっているかどうか(当たり判定)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// 指定の円と四角形の当たり判定
const bool CheckCollisionCircleSquare( const CollisionCircle &circle, const math::Vector2 &squarePosUpperLeft, const math::Vector2 &squarePosUnderRight );

// 第一引数から第二引数までのベクトルを求める(大きさは1)
math::Vector2 GetDirAToB( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// マップ上での所属空間を求める
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// マップ上での高さを取得
const int32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY );
const int32_t GetMapHeight( const math::Vector2 &pos );

// マップ上で適当な数値を返す
math::Vector2 GetMapRandamPos( bool allowInWindow = true, int32_t mapHeight = 0 );

// 画面上に表示しているオブジェクトかどうか
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo );
bool IsPositionInWindowArea( const uint32_t &xx, const uint32_t &yy );

// プレイヤーに重ならない位置かどうか
bool IsPositionInPlayerPos( const float &xx, const float &yy );

// 指定のAIクラスの生成
EnemyAIBase *CreateEnemyAI( Common::ENEMY_AI nextAI );

// プレイヤーの位置情報を取得
math::Vector2 GetPlayerPos();

// Window上の特定の位置に常にいる実体の位置をゲーム上の位置に変換
math::Vector2 ConvertWindowPosToGamePos( const math::Vector2 &windowPos );

// 向く方向を上下左右から選択
InputWatcher::BUTTON_KIND GetDirection( const float dirX, const float dirY );

// 指定の範囲内のランダムな数字を返す(maxに3, minに0を入れたら「0,1,2」のいずれか)
int32_t GetRandamValue( const int32_t &max, const int32_t &min);
float GetRandamValueFloat( const int32_t &max, const int32_t &min);

// セーブデータの情報を取得
bool GetSaveData( Common::SAVE_DATA &saveData );
bool OverWriteSaveData( Common::SAVE_DATA &saveData );

// プレイヤーのレベルから実際にセットする値へ変換
uint32_t ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level );

// 画面のjsonからパーツ情報を取得("partsInfo")
void GetPartsInfoFromJson( const std::string &jsonStr, std::map< std::string, Common::PARTS_INFO > &vParts );

// 文字列の表示
void DrawStringOnWindow( const std::string &str, const math::Vector2 &pos, uint32_t color = GetColor( 255, 255, 255 ) );

// 指定の場所は移動できる場所かどうか
bool IsMovable( const std::string &resourceJson, const math::Vector2 &pos );

// 対象の当たり判定物からイベントタイプを取得
Common::EVENT_MESSAGE GetEventMessageFromObjType( const Common::TYPE_OBJECT &objType );

// イベントタイプからもととなる当たり判定物のTYPEを取得
Common::TYPE_OBJECT GetObjTypeFromEventMessage( const Common::EVENT_MESSAGE &eventMessage );

// デバッグ用、指定位置に図形を描く
void DrawDebugCircle( const math::Vector2 &pos, const float &radius );
void DrawDebugBox( const math::Vector2 &pos, const float &radius );

} // Utility

#endif