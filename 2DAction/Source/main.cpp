
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/SystemFPSManager.h"
#include "System/Draw2D/SystemDraw2DManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/SystemFadeManager.h"
#include "Game/GameScoreRecorder.h"
#include "Common/CmnGameAccesser.h"
#include "Flow/FlowManager.h"

void GameMain();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	//! リークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//! ＤＸライブラリ初期化
	SetAlwaysRunFlag(true);

	if( ChangeWindowMode(true) != 0 ){
		return 0;
	}
	if( DxLib_Init() != 0 ){
		return 0;
	}
	if(!AddFontResourceEx("Data/Font/misaki_gothic.ttf", FR_PRIVATE, NULL)) {
		return 0;
	}

	//! TaskUnit非継承常駐物
	GameAccesser::Create();
	Draw2DManager::Create();
	TextureResourceManager::Create();

	//! TaskUnit継承常駐物
	FlowManager::Create();
	SoundManager::Create();
	FadeManager::Create();
	ScoreRecorder::Create();

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

	//! フォント削除
	RemoveFontResourceEx("Data/Font/misaki_gothic.ttf", FR_PRIVATE, NULL);

	//! 常駐物削除
	GameAccesser *pStats = GameAccesser::GetInstance();
	SAFE_DELETE(pStats);
	Draw2DManager *p2DManager = Draw2DManager::GetInstance();
	SAFE_DELETE(p2DManager);
	TextureResourceManager *pResMan = TextureResourceManager::GetInstance();
	SAFE_DELETE(pResMan);

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

	//! 裏画面の内容を表画面に反映させる
	ScreenFlip() ;
}
