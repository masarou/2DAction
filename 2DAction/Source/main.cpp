
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Message/SystemMessageManager.h"
#include "System/Collision/SystemCollisionManager.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/SystemFPSManager.h"
#include "System/Draw2D/SystemDraw2DManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/SystemFadeManager.h"
#include "System/SystemPauseFilter.h"
#include "System/SystemBgManager.h"
#include "Game/GameRecorder.h"
#include "Common/CmnGameAccesser.h"
#include "Flow/FlowManager.h"

void GameMain();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	//! リークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//! ＤＸライブラリ初期化
	SetAlwaysRunFlag(true);

	// ウインドウタイトル設定
	SetWindowText( "MonsterBuster" );

	// ウインドウサイズ設定
	SetGraphMode( WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	if( ChangeWindowMode(true) != 0 ){
		return 0;
	}
	if( DxLib_Init() != 0 ){
		return 0;
	}

	// デフォルトフォント設定
	SetDefaultFontState( "メイリオ", FONT_DEFAULT_SIZE, 6 );

	// アプリログ出力なし
	//SetOutApplicationLogValidFlag( false );

	//! TaskUnit非継承常駐物
	GameAccesser::Create();
	Draw2DManager::Create();
	TextureResourceManager::Create();
	SystemMessageManager::Create();
	CollisionManager::Create();

	//! TaskUnit継承常駐物
	FlowManager::Create();
	SoundManager::Create();
	FadeManager::Create();
	PauseFilter::Create();
	GameRecorder::Create();
	BgManager::Create();

	//! 乱数用意
	srand( static_cast<uint32_t>(time(NULL)) );

	while(1){

		//! FPSによる更新待ち
		if( !FpsManager::GetInstance()->IsUpdateTiming() ){
			Sleep(1);
			continue;
		}

		GameMain();

		if( ProcessMessage() != 0 ){ //!< メッセージ処理
			break;//ウィンドウの×ボタンが押されたらループを抜ける
		}
	};

	//! タスクマネージャ削除
	for(;;){
		if(TaskManager::GetInstance()->ReleaseAllEnd()){
			TaskManager *pTask = TaskManager::GetInstance();
			SAFE_DELETE(pTask);
			break;
		}
	}

	//! 常駐物削除
	CollisionManager::GetInstance()->DeleteCollisionManager();
	SystemMessageManager::GetInstance()->DeleteMessageManager();
	TextureResourceManager::GetInstance()->DeleteResourceManager();
	Draw2DManager::GetInstance()->DeleteDraw2DManager();
	GameAccesser::GetInstance()->DeleteGameAccesser();

	// ＤＸライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

void GameMain()
{
	//! 描画クリア
	ClearDrawScreen();

	//! 描画先を裏画面にする
	SetDrawScreen( DX_SCREEN_BACK ) ;

	TaskManager::GetInstance()->ExecApp();

	//! すべての2D描画はここで行う
	Draw2DManager::GetInstance()->Action();

	//! 裏画面の内容を表画面に反映させる
	ScreenFlip() ;
}
