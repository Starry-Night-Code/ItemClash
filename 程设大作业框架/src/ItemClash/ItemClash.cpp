// ItemClash.cpp : 定义应用程序的入口点。
//

#include "ItemClash.h"
using namespace std;

void LoadGameBitmaps(HINSTANCE hInstance);                 //资源载入：背景、按钮、角色、道具等图片
void ShowTitleButtons();                                  // 标题界面-显示游戏标题和开始按钮
void ShowHelpButtons();                                   // 帮助界面-显示易于理解的游戏玩法/教程，有返回标题界面按钮
void OpenPauseMenu(HWND hWnd);                            // 暂停界面-暂停对局，停止音乐，显示继续、重开、回标题
void ShowResultButtons();                                 // 结算界面-显示玩家胜利或者失败的弹窗或界面
bool Collision_detection(HWND hWnd, Unit* unit);          // 游戏主界面-利用砖块设计基础地图、实现边界道具碰撞检测

void SpawnBattleUnits(int stageID);                       //   玩家初始化-主角/AI人物生成-初始化玩家和AI的人物属性
void DrawExtraDuoRoles(HDC hdc_memBuffer, HDC hdc_loadBmp); // 玩家初始化-双人第3、第4个角色的绘制
void BasicMove(HWND hWnd);                                //   基本移动-单人模式方向键移动，双人玩家1用 WASD、玩家2用方向键（可自己定义）

void PlayMenuBgm();                                       // 音乐交互-标题界面循环播放背景音乐
void PlayBattleBgm();                                     // 音乐交互-进入对局后循环播放对局音乐
void StopBattleBgm();                                     // 音乐交互-暂停时停止对局音乐
void ResumeBattleBgm();                                   // 音乐交互-继续游戏时接着播放
void CloseBattleBgm();                                    // 音乐交互-离开对局时关闭对局音乐

void PlaceBubbleByKey(HWND hWnd, WPARAM key);             // 玩家交互-双人模式空格放玩家1的泡泡，回车放玩家2的泡泡
void ApplyBlastHit(Unit* unit, bool inFlame);             // 玩家交互-玩家/AI被泡泡炸到则扣血，血量为 0 则死亡

void BasicAttack(HWND hWnd);                              // AI交互-电脑距离够近时放置泡泡
void ComputerMove(HWND hWnd);                             // AI交互-电脑躲开爆炸范围，并朝玩家走近


void ApplySpeedPotion(HWND hWnd, Unit* unit);             // 加速道具交互-拾取加速药水，移速提高约 3 秒
void EndSpeedPotion(Unit* unit);                          // 加速道具交互-加速结束，把提高的移速减回去
void ApplyHeart(Unit* unit);                              // 爱心道具交互-拾取爱心加 1 点血，上限 3
void ApplyAmmoBox(Unit* unit);                            // 泡泡道具交互-可同时放置的泡泡数加 1，上限 3
void ApplyPowerPotion(Unit* unit);                        // 威力道具交互-获得一次加强，下一颗泡泡范围为 2
void SetBubblePower(Unit* unit, Bubble* newBubble);       // 范围道具交互-决定这颗泡泡的爆炸范围，有加强次数就消耗一次


void StartCountdown(HWND hWnd);                           // 场景交互-开局 90 秒倒计时
void TickCountdown(HWND hWnd);                            // 场景交互-每秒减 1，减到 0 进入超时
void StartBubbleCooldown(HWND hWnd, Unit* unit);          // 场景交互-泡泡没到上限时，约 3 秒后补回一颗
void FinishBubbleCooldown(Unit* unit);                    // 场景交互-冷却结束，补回一颗泡泡

#define MAX_LOADSTRING 100
#define MAX_ROLE_NUM   4    //最大角色数量
#define MAX_HEALTH     3    //最大生命值
#define MAX_BOMB       3    //最大弹匣容量
#define TIMER          5001 //游戏内计时器ID

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

HBITMAP bmp_Grass;			//背景图像资源
HBITMAP bmp_Beginning;      //开始界面背景图像
HBITMAP bmp_Help_bg;        //帮助界面背景图片
HBITMAP bmp_Tittle;         //游戏标题
HBITMAP bmp_StartButton;	//开始按钮
HBITMAP bmp_HelpButton;     //帮助按钮
HBITMAP bmp_BackButton;     //返回按钮
HBITMAP bmp_SoloButton;     //单人游戏按钮
HBITMAP bmp_DuoButton;      //双人游戏按钮
HBITMAP bmp_ContinueButton; //继续游戏按钮
HBITMAP bmp_RestartButton;  //重新游戏按钮
HBITMAP bmp_HomepageButton; //返回主页按钮
HBITMAP bmp_PlayAgainButton;//再玩一次按钮
HBITMAP bmp_Choose;         //选角色界面背景图像
HBITMAP bmp_ChooseHelp1;    //单人选角帮助
HBITMAP bmp_ChooseHelp2;    //双人选角帮助
HBITMAP bmp_Win;            //胜利图标
HBITMAP bmp_Lose;           //失败图标
HBITMAP bmp_TimeOut;        //时间到图标
HBITMAP bmp_Unit_Rival;
HBITMAP bmp_Unit_Role1;     //角色1
HBITMAP bmp_Unit_Role2;     //角色2
HBITMAP bmp_Unit_Role3;     //角色3
HBITMAP bmp_Unit_Role4;     //角色4
HBITMAP bmp_Arrow;          //选角色箭头
HBITMAP bmp_Arrow2;         //选角色箭头2
HBITMAP bmp_Background;		//生成的背景图像
HBITMAP bmp_Bubble;			//泡泡图像资源
HBITMAP bmp_block;			//地图砖块图像
HBITMAP bmp_explosion;      //爆炸图像
HBITMAP bmp_solo_side_col;  //单人模式侧栏
HBITMAP bmp_duo_side_col;   //双人模式侧栏
HBITMAP bmp_heart;          //血量图像
HBITMAP bmp_bomb;           //炸弹图像
HBITMAP bmp_speedup;        //加速道具
HBITMAP bmp_bomb_enhance;   //加强炸弹道具
HBITMAP bmp_bomb_expansion; //炸弹扩容道具
//记时图像0->9
HBITMAP bmp_0;
HBITMAP bmp_1;
HBITMAP bmp_2;
HBITMAP bmp_3;
HBITMAP bmp_4;
HBITMAP bmp_5;
HBITMAP bmp_6;
HBITMAP bmp_7;
HBITMAP bmp_8;
HBITMAP bmp_9;


Stage* currentStage = NULL; //当前场景状态
vector<Unit*> units;		//单位
vector<HBITMAP> roles;      //四个角色
int pointer[] = {1,0,0,0};  //记录选角色时箭头指向的角色
int pointer2[] = {0,1,0,0};  //记录玩家2选角色时箭头指向的角色
int duoRoleNum = 2;          //双人选角页的可选角色数
vector<HBITMAP> items;      //四种道具（回血，加强炸弹，扩充弹匣，加速）
vector<HBITMAP> nums;       //九个数字
vector<Button*> buttons;	//按钮	
vector<Bubble*> bubbles;	//泡泡
vector<Explosion*> explosions;  //爆炸效果
int next_bubble_timer_id = 1001;		//下一个泡泡的计时器id
int next_explosion_timer_id = 2001;     //下一个爆炸效果的计时器id
int next_CD_timer_id = 3001;            //下一个炸弹冷却的计时器id
int next_speedup_timer_id = 4001;       //下一个加速计时器id

//计时器只通过 id 找回对应单位，避免用 id 奇偶猜测是玩家1还是玩家2
struct TimerBind
{
	int timerId;
	Unit* unit;
};
vector<TimerBind> cdTimers;
vector<TimerBind> speedTimers;
int matchWinner = -1;                   //双人胜者：0 玩家1，1 玩家2，-1 尚无胜者

bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
bool keyADown = false;
bool keyDDown = false;
bool keyWDown = false;
bool keySDown = false;
bool P1done = false;   //记录玩家1是否确认选定角色
bool P2done = false;   //记录玩家2是否确认选定角色
int count_down = 90;   //游戏内倒计时


//帧
int FRAMES_HOLD[] = { 0 };
int FRAMES_HOLD_COUNT = sizeof(FRAMES_HOLD) / sizeof(int);
int FRAMES_WALK[] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, };
int FRAMES_WALK_COUNT = sizeof(FRAMES_WALK) / sizeof(int);
int FRAMES_BUBBLE[] = { 0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5, };
int FRAMES_BUBBLE_COUNT = sizeof(FRAMES_BUBBLE) / sizeof(int);

//地图编辑 0-3：空地、可破坏砖块、墙、柱子
int map[][12] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
//初始地图
// 0 沙地  1 木箱（可炸毁）  2 椰子树（不可炸）  3 蓝箱子（不可炸）
// 中心对称：第 r 行 = 第 (11-r) 行倒过来，保证双方地形公平
// 玩家1出生在 [1][1]，玩家2/AI 出生在 [10][8]，周围都留了逃生空地
// 最外圈留空作为环形通道，双方不炸箱子也能互相走到（方便 AI 追人）
int map1[][12] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,2,1,1,2,1,1,1,0},	// [1][1] 玩家1出生点
	{0,0,2,1,0,1,1,0,1,2,1,0},
	{0,1,1,0,0,3,3,0,0,1,1,0},
	{0,2,0,0,2,1,1,2,0,0,2,0},
	{0,1,1,3,0,0,0,0,3,1,1,0},
	{0,1,1,3,0,0,0,0,3,1,1,0},
	{0,2,0,0,2,1,1,2,0,0,2,0},
	{0,1,1,0,0,3,3,0,0,1,1,0},
	{0,1,2,1,0,1,1,0,1,2,0,0},
	{0,1,1,1,2,1,1,2,0,0,0,0},	// [10][8] 玩家2/AI出生点
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
//初始道具掉落编辑0-3：加速、加血、加泡泡、加威力
int item[][12] = {
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,4,4,4,4,4,4,4,4},
};






// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ITEMCLASH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ITEMCLASH));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TITLEBAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ITEMCLASH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TITLEBAR));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
   RECT windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
   AdjustWindowRect(&windowRect, windowStyle, FALSE);
   hWnd = CreateWindowW(szWindowClass, szTitle, windowStyle,
	   CW_USEDEFAULT,
	   CW_USEDEFAULT,
	   windowRect.right - windowRect.left,
	   windowRect.bottom - windowRect.top,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // 初始化游戏窗体
        InitGame(hWnd, wParam, lParam);
        break;
    case WM_KEYDOWN:
        // 键盘按下事件
        KeyDown(hWnd, wParam, lParam);
        break;
    case WM_KEYUP:
        // 键盘松开事件
        KeyUp(hWnd, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        // 鼠标左键按下事件
        LButtonDown(hWnd, wParam, lParam);
        break;
    case WM_TIMER:
        // 定时器事件
        if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
        break;
    case WM_PAINT:
        // 绘图
        Paint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载图像资源
	LoadGameBitmaps(((LPCREATESTRUCT)lParam)->hInstance);

	roles.push_back(bmp_Unit_Role1);
	roles.push_back(bmp_Unit_Role2);
	roles.push_back(bmp_Unit_Role3);
	roles.push_back(bmp_Unit_Role4);
	items.push_back(bmp_speedup);
	items.push_back(bmp_heart);
	items.push_back(bmp_bomb_expansion);
	items.push_back(bmp_bomb_enhance);
	nums.push_back(bmp_0);
	nums.push_back(bmp_1);
	nums.push_back(bmp_2);
	nums.push_back(bmp_3);
	nums.push_back(bmp_4);
	nums.push_back(bmp_5);
	nums.push_back(bmp_6);
	nums.push_back(bmp_7);
	nums.push_back(bmp_8);
	nums.push_back(bmp_9);
	//添加按钮
	//开始游戏按钮
	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT,
		(WINDOW_WIDTH - BUTTON_STARTGAME_WIDTH) / 2, (WINDOW_WIDTH - BUTTON_STARTGAME_HEIGHT - 110) / 2);
	buttons.push_back(startButton);
	//帮助按钮
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT,
		(WINDOW_WIDTH - BUTTON_HELP_WIDTH) / 2, (WINDOW_WIDTH - BUTTON_HELP_HEIGHT + 110) / 2);
	buttons.push_back(helpButton);
	//返回按钮
	Button* backButton = CreateButton(BUTTON_BACK, bmp_BackButton, BUTTON_BACK_WIDTH, BUTTON_BACK_HEIGHT,
		WINDOW_WIDTH - BUTTON_BACK_WIDTH - 20, 0);
	buttons.push_back(backButton);
	//单人游戏按钮
	Button* soloButton = CreateButton(BUTTON_SOLO, bmp_SoloButton, BUTTON_SOLO_WIDTH, BUTTON_SOLO_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_SOLO_WIDTH)/2, (WINDOW_WIDTH - BUTTON_SOLO_HEIGHT - BUTTON_DUO_HEIGHT - 50)/2);
	buttons.push_back(soloButton);
	//双人游戏按钮
	Button* duoButton = CreateButton(BUTTON_DUO, bmp_DuoButton, BUTTON_DUO_WIDTH, BUTTON_DUO_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_DUO_WIDTH)/2, (WINDOW_WIDTH - BUTTON_DUO_HEIGHT + 50)/2);
	buttons.push_back(duoButton);
	//继续游戏按钮
	Button* continueButton = CreateButton(BUTTON_CONTINUE, bmp_ContinueButton, BUTTON_CONTINUE_WIDTH, BUTTON_CONTINUE_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_CONTINUE_WIDTH)/2, 150);
	buttons.push_back(continueButton);
	//重新开始按钮
	Button* restartButton = CreateButton(BUTTON_RESTART, bmp_RestartButton, BUTTON_RESTART_WIDTH, BUTTON_RESTART_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_CONTINUE_WIDTH)/2, 350);
	buttons.push_back(restartButton);
	//返回首页按钮
	Button* homepageButton = CreateButton(BUTTON_HOMEPAGE, bmp_HomepageButton, BUTTON_HOMEPAGE_WIDTH, BUTTON_HOMEPAGE_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_HOMEPAGE_WIDTH)/2, 550);
	buttons.push_back(homepageButton);
	//再玩一次按钮
	Button* playagainButton = CreateButton(BUTTON_PLAYAGAIN, bmp_PlayAgainButton, BUTTON_PLAYAGAIN_WIDTH, BUTTON_PLAYAGAIN_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_PLAYAGAIN_WIDTH)/2, (WINDOW_WIDTH - BUTTON_PLAYAGAIN_HEIGHT)/2);
	buttons.push_back(playagainButton);
	//初始化背景
	bmp_Background = InitBackGround(hWnd, bmp_Grass);


	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		if (currentStage->stageID == STAGE_CHOOSE1) UpdateArrow(hWnd);
		if (currentStage->stageID == STAGE_CHOOSE2 && P2done == false) UpdateArrow(hWnd);
		break;
	case VK_RIGHT:
		keyRightDown = true;
		if (currentStage->stageID == STAGE_CHOOSE1) UpdateArrow(hWnd);
		if (currentStage->stageID == STAGE_CHOOSE2 && P2done == false) UpdateArrow(hWnd);
		break;
	case 'A':
		keyADown = true;
		if (currentStage->stageID == STAGE_CHOOSE2 && P1done == false) UpdateArrow(hWnd);
		break;
	case 'D':
		keyDDown = true;
		if (currentStage->stageID == STAGE_CHOOSE2 && P1done == false) UpdateArrow(hWnd);
		break;
	case 'W':
		keyWDown = true;
		break;
	case 'S':
		keySDown = true;
		break;
	case VK_SPACE:
		if (currentStage->stageID == STAGE_CHOOSE1) {InitStage(hWnd, STAGE_1); break;}
		if (currentStage->stageID == STAGE_CHOOSE2 && P2done == true) {InitStage(hWnd, STAGE_2); break;}
		if (currentStage->stageID == STAGE_CHOOSE2 && P2done == false) P1done = true;
		PlaceBubbleByKey(hWnd, VK_SPACE);
		break;
	case VK_RETURN:
		if (currentStage->stageID == STAGE_CHOOSE2 && P1done == true) {InitStage(hWnd, STAGE_2); break;}
		if (currentStage->stageID == STAGE_CHOOSE2 && P1done == false) P2done = true;
		PlaceBubbleByKey(hWnd, VK_RETURN);
		break;
	case VK_ESCAPE:
		if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_2)
		{
			OpenPauseMenu(hWnd);
		}
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	case 'A':
		keyADown = false;
		break;
	case 'D':
		keyDDown = false;
		break;
	case 'W':
		keyWDown = false;
		break;
	case 'S':
		keySDown = false;
		break;
	default:
		break;
	}
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= x
				&& button->x + button->width >= x
				&& button->y <= y
				&& button->y + button->height >= y)
			{
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
				{
					//TODO：判断进入哪个关卡
					InitStage(hWnd, STAGE_GAMEMODE); break;
				}
				case BUTTON_HELP:
				{
					InitStage(hWnd, STAGE_HELP); break;
				}
				case BUTTON_BACK:
				{
					InitStage(hWnd, STAGE_STARTMENU); break;
				}
				case BUTTON_SOLO:
				{
					InitStage(hWnd, STAGE_1); break;
				}
				case BUTTON_DUO:
				{
					InitStage(hWnd, STAGE_CHOOSE2); break;
				}
				case BUTTON_CONTINUE:
				{
					// TODO: 处理按钮操作。
					break;
				}
				case BUTTON_RESTART:
				{
					// TODO: 处理按钮操作。
					break;
				}
				case BUTTON_HOMEPAGE:
				{
					// TODO: 处理按钮操作。
					break;
				}
				case BUTTON_PLAYAGAIN:
				{
					// TODO: 处理按钮操作。
					break;
				}
				default: break;
				}
				break;
			}
		}
	}

}

// 定时器事件处理函数
void DeleteUnits()
{
	for (int i = 0; i < (int)units.size(); i++) delete units[i];
	units.clear();
}

void DeleteBubbles()
{
	for (int i = 0; i < (int)bubbles.size(); i++) delete bubbles[i];
	bubbles.clear();
}

void DeleteExplosions()
{
	for (int i = 0; i < (int)explosions.size(); i++) delete explosions[i];
	explosions.clear();
}

//只关掉仍登记在对象上的计时器，id 继续递增，避免重开后旧消息撞上新泡泡
void KillMatchTimers(HWND hWnd)
{
	for (int i = 0; i < (int)bubbles.size(); i++) KillTimer(hWnd, bubbles[i]->timer_id);
	for (int i = 0; i < (int)explosions.size(); i++) KillTimer(hWnd, explosions[i]->timer_id);
	for (int i = 0; i < (int)cdTimers.size(); i++) KillTimer(hWnd, cdTimers[i].timerId);
	for (int i = 0; i < (int)speedTimers.size(); i++) KillTimer(hWnd, speedTimers[i].timerId);
	KillTimer(hWnd, TIMER);
	cdTimers.clear();
	speedTimers.clear();
}

void ClearMatch(HWND hWnd)
{
	KillMatchTimers(hWnd);
	DeleteUnits();
	DeleteBubbles();
	DeleteExplosions();
	matchWinner = -1;
}

void ReleaseProjectiles(HWND hWnd)
{
	KillMatchTimers(hWnd);
	DeleteBubbles();
	DeleteExplosions();
}

void ResetMapAndItems()
{
	for (int i = 0; i < (int)(sizeof(map) / sizeof(map[0])); i++)
	{
		for (int j = 0; j < (int)(sizeof(map[0]) / sizeof(map[0][0])); j++)
		{
			map[i][j] = map1[i][j];
		}
	}
	for (int i = 0; i < (int)(sizeof(item) / sizeof(item[0])); i++)
	{
		for (int j = 0; j < (int)(sizeof(item[0]) / sizeof(item[0][0])); j++)
		{
			item[i][j] = 4;
		}
	}
}

void PlayBgm(const wchar_t* alias, const wchar_t* openCmd)
{
	// TODO: 播放指定的背景音乐。
}

void BindTimer(vector<TimerBind>& binds, HWND hWnd, Unit* unit, int& nextId, int elapse)
{
	TimerBind bind;
	bind.timerId = nextId++;
	bind.unit = unit;
	binds.push_back(bind);
	SetTimer(hWnd, bind.timerId, elapse, NULL);
}

Bubble* DetachBubble(int timerId)
{
	for (int i = 0; i < (int)bubbles.size(); i++)
	{
		if (bubbles[i]->timer_id == timerId)
		{
			Bubble* bubble = bubbles[i];
			bubbles.erase(bubbles.begin() + i);
			return bubble;
		}
	}
	return NULL;
}

Explosion* DetachExplosion(int timerId)
{
	for (int i = 0; i < (int)explosions.size(); i++)
	{
		if (explosions[i]->timer_id == timerId)
		{
			Explosion* explosion = explosions[i];
			explosions.erase(explosions.begin() + i);
			return explosion;
		}
	}
	return NULL;
}

Unit* TakeTimerUnit(vector<TimerBind>& binds, int timerId)
{
	for (int i = 0; i < (int)binds.size(); i++)
	{
		if (binds[i].timerId == timerId)
		{
			Unit* unit = binds[i].unit;
			binds.erase(binds.begin() + i);
			return unit;
		}
	}
	return NULL;
}

bool InMap(int row, int col)
{
	int rows = (int)(sizeof(map) / sizeof(map[0]));
	int cols = (int)(sizeof(map[0]) / sizeof(map[0][0]));
	return row >= 0 && col >= 0 && row < rows && col < cols;
}

void FinishMatch(HWND hWnd, int stageId)
{
	CloseBattleBgm();
	InitStage(hWnd, stageId);
}

void SettleMatch(HWND hWnd)
{
	// TODO: 根据单位存活状态判定对局结果。
}

void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int timerId = (int)wParam;
	if (timerId == TIMER)
	{
		TickCountdown(hWnd);
	}
	else if (Bubble* bubble = DetachBubble(timerId))
	{
		explode(hWnd, bubble);
		delete bubble;
		KillTimer(hWnd, timerId);
	}
	else if (Explosion* explosion = DetachExplosion(timerId))
	{
		delete explosion;
		KillTimer(hWnd, timerId);
	}
	else if (Unit* unit = TakeTimerUnit(cdTimers, timerId))
	{
		FinishBubbleCooldown(unit);
		KillTimer(hWnd, timerId);
	}
	else if (Unit* unit = TakeTimerUnit(speedTimers, timerId))
	{
		EndSpeedPotion(unit);
		KillTimer(hWnd, timerId);
	}

	if (currentStage != NULL && currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_2 && currentStage->timerOn == true) UpdateUnits(hWnd);

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}



//TODO: 添加游戏需要的更多函数

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// 添加主角函数
Unit* CreateUnit(int side, int x, int y, int health, int max_bomb)
{
	Unit* unit = new Unit();
	unit->side = side;
	if (side == UNIT_SIDE_ME) {                //创建自己
		for (int i=0; i < MAX_ROLE_NUM; i++)   //选出被选中的角色
		{
			if (pointer[i] == 1)
			{
				unit->img = roles[i];
				break;
			}
		}
		unit->direction = UNIT_DIRECT_RIGHT;
		unit->vx = 6;
		unit->vy = 6;
	}
	else if (side == UNIT_SIDE_MATE) {              //创建队友
		for (int i=0; i < MAX_ROLE_NUM; i++)   //选出被选中的角色
		{
			if (pointer2[i] == 1)
			{
				unit->img = roles[i];
				break;
			}
		}
		unit->direction = UNIT_DIRECT_LEFT;
		unit->vx = 6;
		unit->vy = 6;
	}
	else if (side == UNIT_SIDE_RIVAL) {
		unit->img = bmp_Unit_Rival;
		unit->direction = UNIT_DIRECT_LEFT;
		unit->vx = 3;
		unit->vy = 3;
	}

	unit->state = UNIT_STATE_HOLD;


	unit->frame_row = unit->direction;
	unit->frame_column = 0;

	unit->frame_sequence = FRAMES_HOLD;
	unit->frame_count = FRAMES_HOLD_COUNT;
	unit->frame_id = 0;

	unit->x = x;
	unit->y = y;
	unit->alive = true;
	unit->health = health;
	unit->bubblesleft = max_bomb;
	unit->max_bomb = max_bomb;
	unit->enhance_bomb = 0;
	return unit;
}



// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;
	//创建开始界面
	if (stageID == STAGE_STARTMENU) {
		ClearMatch(hWnd);
		ResetMapAndItems();
		pointer[0] = 1; pointer[1] = 0; pointer[2] = 0; pointer[3] = 0;
		pointer2[0] = 0; pointer2[1] = 1; pointer2[2] = 0; pointer2[3] = 0;
		P1done = false;
		P2done = false;
		currentStage->timerOn = false;
		//播放开始界面背景音乐
		PlayMenuBgm();
		//显示开始界面的按钮
		ShowTitleButtons();
	}
	//创建帮助界面
	if (stageID == STAGE_HELP)
	{
		currentStage->timerOn = false;
		ShowHelpButtons();
	}
	//创建选择游戏模式界面
	if (stageID == STAGE_GAMEMODE)
	{
		currentStage->timerOn = false;
		//显示选择游戏模式界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_SOLO || button->buttonID == BUTTON_DUO)
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}
	}
	//创建单人选角色界面
	if (stageID == STAGE_CHOOSE1)
	{
		currentStage->timerOn = true;
		//显示单人选择角色模式界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			button->visible = false;
		}
	}
	//创建双人选角色界面
	if (stageID == STAGE_CHOOSE2)
	{
		currentStage->timerOn = true;
		//显示双人选择角色模式界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			button->visible = false;
		}
	}
	//创建游戏界面
	if (stageID >= STAGE_1 && stageID <= STAGE_2) //TODO：添加多个游戏场景
	{
		ClearMatch(hWnd);
		ResetMapAndItems();
		currentStage->timerOn = true;
		//切换bgm
		PlayBattleBgm();
		//显示游戏界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			button->visible = false;
		}
		StartCountdown(hWnd);
		// 按场景初始化单位
		SpawnBattleUnits(stageID);
	}
	//创建游戏结算界面
	if (stageID == STAGE_LOSE || stageID == STAGE_WIN || stageID == STAGE_TIMEOUT)
	{
		ReleaseProjectiles(hWnd);
		CloseBattleBgm();
		currentStage->timerOn = false;
		//显示选择游戏模式界面的按钮
		ShowResultButtons();
	}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// TODO: 角色选择 —— 补绘双人选角界面的其余角色。
void DrawExtraDuoRoles(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	// TODO: 补绘双人选角页的其余角色。
}

void KeepDuoPointerInRange(int* selected, int* other, int roleNum)
{
	for (int i = 0; i < roleNum && i < MAX_ROLE_NUM; i++)
		if (selected[i] == 1) return;
	for (int i = 0; i < MAX_ROLE_NUM; i++) selected[i] = 0;
	for (int i = 0; i < roleNum && i < MAX_ROLE_NUM; i++)
	{
		if (other[i] == 0)
		{
			selected[i] = 1;
			return;
		}
	}
}

//刷新箭头位置函数
void UpdateArrow(HWND hWnd)
{
	//单人选角色
	if (currentStage->stageID == STAGE_CHOOSE1)
	{
		if (keyRightDown)
		{
			for (int i=0; i<MAX_ROLE_NUM; i++)
			{
				if (pointer[i] == 1)
				{
					pointer[i] = 0;
					pointer[(i+1)%MAX_ROLE_NUM] = 1;
					break;
				}
			}
		}
		if (keyLeftDown)
		{
			for (int i=0; i<MAX_ROLE_NUM; i++)
			{
				if (pointer[i] == 1)
				{
					pointer[i] = 0;
					pointer[((i-1)%MAX_ROLE_NUM+MAX_ROLE_NUM)%MAX_ROLE_NUM] = 1;
					break;
				}
			}
		}
	}
	//双人选角色。光标范围是已经画出来的角色，对方已经站着的那个会跳过
	if (currentStage->stageID == STAGE_CHOOSE2)
	{
		int roleNum = duoRoleNum;
		KeepDuoPointerInRange(pointer, pointer2, roleNum);
		KeepDuoPointerInRange(pointer2, pointer, roleNum);
		//玩家1
		if (keyDDown)
		{
			for (int i=0; i<roleNum; i++)
			{
				if (pointer[i] == 1)                    //检查玩家2的角色是否为箭头指向的下一个角色
				{
					if (pointer2[(i+1)%roleNum] == 0)
					{
						pointer[i] = 0;
						pointer[(i+1)%roleNum] = 1;
						break;
					}
					else if (pointer2[(i+1)%roleNum] == 1)
					{
						pointer[i] = 0;
						pointer[(i+2)%roleNum] = 1;
						break;
					}
				}
			}
		}
		if (keyADown)
		{
			for (int i=0; i<roleNum; i++)
			{
				if (pointer[i] == 1)
				{
					if (pointer2[((i-1)%roleNum+roleNum)%roleNum] == 0)
					{
						pointer[i] = 0;
						pointer[((i-1)%roleNum+roleNum)%roleNum] = 1;
						break;
					}
					else if (pointer2[((i-1)%roleNum+roleNum)%roleNum] == 1)
					{
						pointer[i] = 0;
						pointer[((i-2)%roleNum+roleNum)%roleNum] = 1;
						break;
					}
				}
			}
		}
		//玩家2
		if (keyRightDown)
		{
			for (int i=0; i<roleNum; i++)
			{
				if (pointer2[i] == 1)                    //检查玩家1的角色是否为箭头指向的下一个角色
				{
					if (pointer[(i+1)%roleNum] == 0)
					{
						pointer2[i] = 0;
						pointer2[(i+1)%roleNum] = 1;
						break;
					}
					else if (pointer[(i+1)%roleNum] == 1)
					{
						pointer2[i] = 0;
						pointer2[(i+2)%roleNum] = 1;
						break;
					}
				}
			}
		}
		if (keyLeftDown)
		{
			for (int i=0; i<roleNum; i++)
			{
				if (pointer2[i] == 1)
				{
					if (pointer[((i-1)%roleNum+roleNum)%roleNum] == 0)
					{
						pointer2[i] = 0;
						pointer2[((i-1)%roleNum+roleNum)%roleNum] = 1;
						break;
					}
					else if (pointer[((i-1)%roleNum+roleNum)%roleNum] == 1)
					{
						pointer2[i] = 0;
						pointer2[((i-2)%roleNum+roleNum)%roleNum] = 1;
						break;
					}
				}
			}
		}
	}
}


// 刷新单位状态函数
void UpdateUnits(HWND hWnd)
{
	//先判断是否存活，再刷新行为
	BasicMove(hWnd);
	if (currentStage->stageID == STAGE_1 && units[1]->alive == true) UnitBehaviour_Rival(hWnd);
	UpdateBubbles(hWnd);


}

void MoveUnit(HWND hWnd, Unit* unit, bool keyLeft, bool keyUp, bool keyRight, bool keyDown)
{
	if (keyLeft || keyUp || keyRight || keyDown) {
		unit->state = UNIT_STATE_WALK;
		unit->frame_sequence = FRAMES_WALK;
		unit->frame_count = FRAMES_WALK_COUNT;
		if (keyLeft) {
			unit->direction = UNIT_DIRECT_LEFT;
		}
		else if (keyUp) {
			unit->direction = UNIT_DIRECT_UP;
		}
		else if (keyRight) {
			unit->direction = UNIT_DIRECT_RIGHT;
		}
		else if (keyDown) {
			unit->direction = UNIT_DIRECT_DOWN;
		}
		unit->frame_row = unit->direction;
	}
	else {
		unit->state = UNIT_STATE_HOLD;
		unit->frame_sequence = FRAMES_HOLD;
		unit->frame_count = FRAMES_HOLD_COUNT;
	}
	if (unit->state == UNIT_STATE_WALK && Collision_detection(hWnd, unit) == false) {
		switch (unit->direction) {
		case UNIT_DIRECT_LEFT:
			unit->x -= unit->vx;
			break;
		case UNIT_DIRECT_UP:
			unit->y -= unit->vy;
			break;
		case UNIT_DIRECT_RIGHT:
			unit->x += unit->vx;
			break;
		case UNIT_DIRECT_DOWN:
			unit->y += unit->vy;
			break;
		default:
			break;
		};
	}
	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;
	unit->frame_column = unit->frame_sequence[unit->frame_id];
}


//AI行为
void UnitBehaviour_Rival(HWND hWnd) {
	Unit* rival = units[1];
	//距离足够近时尝试攻击主角
	//动画运行到下一帧
	rival->frame_id++;
	rival->frame_id = rival->frame_id % rival->frame_count;
	rival->frame_column = rival->frame_sequence[rival->frame_id];
}

bool UnitHitsRect(Unit* unit, double left, double top, double right, double bottom, double sidePad, double vertPad, bool countBodyOverlap)
{
	int x = unit->x;
	int y = unit->y;
	bool yOverlap = (y - sidePad < top && y + sidePad > top) || (y - sidePad < bottom && y + sidePad > bottom);
	if (countBodyOverlap) {
		yOverlap = yOverlap || (y >= top && y <= bottom);
	}
	bool collision_from_left = x + 0.5 * UNIT_SIZE_X >= left && x - 0.5 * UNIT_SIZE_X <= left && yOverlap && unit->direction == UNIT_DIRECT_RIGHT;
	bool collision_from_right = (x - 0.5 * UNIT_SIZE_X) <= right && (x + 0.5 * UNIT_SIZE_X) >= right && yOverlap && unit->direction == UNIT_DIRECT_LEFT;
	bool collision_from_top = (y - vertPad) <= top && (y + vertPad) >= top && x >= left && x <= right && unit->direction == UNIT_DIRECT_DOWN;
	bool collision_from_bottom = (y + vertPad) >= bottom && (y - vertPad) <= bottom && x >= left && x <= right && unit->direction == UNIT_DIRECT_UP;
	return collision_from_left || collision_from_right || collision_from_top || collision_from_bottom;
}

//碰撞检测函数
// TODO: 碰撞检测 —— 人物会被棋盘边界、砖块和已放下的泡泡挡住
bool Collision_detection(HWND hWnd, Unit* unit)
{
	// TODO: 检测边界、地形、道具和炸弹。
	// 边界 + 地形：把棋盘外面一圈（第 -1 行/列、第 12 行/列）也当作墙，统一处理
	for (int i = -1; i <= 12; i++) {
		for (int j = -1; j <= 12; j++) {
			if (InMap(i, j) && map[i][j] == 0) continue;  // 棋盘内的空地不挡路

			// 第 [i][j] 格的矩形范围
			double left = j * BG_CELL_WIDTH;
			double top = i * BG_CELL_HEIGHT;
			// 16：左右走时人物的上下判定半径；28：上下走时人物的判定半径
			if (UnitHitsRect(unit, left, top, left + BG_CELL_WIDTH, top + BG_CELL_HEIGHT, 16, 28, true))
				return true;
		}
	}

	// 泡泡：和障碍格一样会挡路
	int unitRow = unit->y / BG_CELL_HEIGHT;
	int unitCol = unit->x / BG_CELL_WIDTH;
	for (int i = 0; i < (int)bubbles.size(); i++) {
		int row = bubbles[i]->y / BG_CELL_HEIGHT;
		int col = bubbles[i]->x / BG_CELL_WIDTH;
		if (row == unitRow && col == unitCol) continue;  // 人还站在这颗泡泡上（刚放下），允许走出去

		double left = col * BG_CELL_WIDTH;
		double top = row * BG_CELL_HEIGHT;
		if (UnitHitsRect(unit, left, top, left + BG_CELL_WIDTH, top + BG_CELL_HEIGHT, 16, 28, true))
			return true;
	}
	return false;
}

// 放置泡泡
void PlaceBubble(HWND hWnd, Unit* unit)
{
	// TODO: 检查放置条件并创建炸弹。
	// 暂停中、已死亡或没有可用泡泡时不能放
	if (!currentStage->timerOn || !unit->alive || unit->bubblesleft <= 0) return;

	// 人物中心所在的格子
	int row = unit->y / BG_CELL_HEIGHT;
	int col = unit->x / BG_CELL_WIDTH;

	// 这一格已经有泡泡就不再放
	for (int i = 0; i < (int)bubbles.size(); i++)
		if (bubbles[i]->y / BG_CELL_HEIGHT == row && bubbles[i]->x / BG_CELL_WIDTH == col) return;

	// 创建泡泡，放在格子中心
	Bubble* bubble = new Bubble();
	bubble->img = bmp_Bubble;
	bubble->x = col * BG_CELL_WIDTH + BG_CELL_WIDTH / 2;
	bubble->y = row * BG_CELL_HEIGHT + BG_CELL_HEIGHT / 2;
	bubble->count_down_duration = 2500;          // 2.5 秒后爆炸
	bubble->timer_id = next_bubble_timer_id++;
	bubble->frame_id = 0;
	bubble->frame_colume = FRAMES_BUBBLE[0];
	SetBubblePower(unit, bubble);                // 决定爆炸范围
	bubbles.push_back(bubble);
	SetTimer(hWnd, bubble->timer_id, bubble->count_down_duration, NULL);  // 到时由 TimerUpdate 调用 explode

	// 扣掉一颗，并开始补充冷却
	unit->bubblesleft--;
	StartBubbleCooldown(hWnd, unit);
}

//更新泡泡的帧动画
void UpdateBubbles(HWND hWnd)
{
	for (int i = 0; i < bubbles.size(); i++) {
		Bubble* bubble = bubbles[i];
		bubble->frame_id = (bubble->frame_id + 1) % FRAMES_BUBBLE_COUNT;
		bubble->frame_colume = FRAMES_BUBBLE[bubble->frame_id];
	}
}

//爆炸效果
// TODO: 炸弹爆炸 —— 十字爆炸，可炸砖变成空地，并可能掉落道具
void explode(HWND hWnd, Bubble* bubble)
{
	// TODO: 计算爆炸范围，处理阻挡、毁砖、掉落和命中。
	// 在泡泡中心显示爆炸图片，200ms 后由 TimerUpdate 自动删除
	Explosion* explosion = new Explosion();
	explosion->img = bmp_explosion;
	explosion->x = bubble->x;
	explosion->y = bubble->y;
	explosion->duration = 200;
	explosion->timer_id = next_explosion_timer_id++;
	explosions.push_back(explosion);
	SetTimer(hWnd, explosion->timer_id, explosion->duration, NULL);

	// flamed 记录哪些格子着火，泡泡所在格一定着火
	bool flamed[12][12] = { false };
	int row = bubble->y / BG_CELL_HEIGHT;
	int col = bubble->x / BG_CELL_WIDTH;
	flamed[row][col] = true;

	// 向上、下、左、右四个方向传播
	int dr[4] = { -1, 1, 0, 0 };
	int dc[4] = { 0, 0, -1, 1 };
	for (int dir = 0; dir < 4; dir++)
	{
		for (int k = 1; k <= bubble->range; k++)
		{
			int r = row + dr[dir] * k;
			int c = col + dc[dir] * k;
			if (!InMap(r, c)) break;                        // 出了棋盘
			if (map[r][c] == 2 || map[r][c] == 3) break;    // 树和蓝箱子挡住火焰
			flamed[r][c] = true;
			if (map[r][c] == 1)                             // 木箱：炸毁后火焰停下
			{
				map[r][c] = 0;
				if (rand() % 2 == 0) item[r][c] = rand() % 4;  // 一半概率掉落道具
				break;
			}
		}
	}

	// 站在着火格子上的人受到伤害
	for (int i = 0; i < (int)units.size(); i++)
	{
		Unit* unit = units[i];
		ApplyBlastHit(unit, flamed[unit->y / BG_CELL_HEIGHT][unit->x / BG_CELL_WIDTH]);
	}

	// 统一判断胜负
	SettleMatch(hWnd);
}


// 绘图函数

void DrawResultScreen(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	// TODO: 绘制胜利、失败和超时结算画面。
}

void DrawGameHud(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	if (currentStage->stageID == STAGE_1)
	{
		// TODO: 游戏主界面 —— 右侧栏显示双方血量、剩余炸弹和倒计时；双人侧栏见下方 STAGE_2
		SelectObject(hdc_loadBmp, bmp_solo_side_col);
			TransparentBlt(
				hdc_memBuffer,
				WINDOW_WIDTH - SIDE_COL_WIDTH,0,  //界面上起始绘制点
				SIDE_COL_WIDTH, SIDE_COL_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				SIDE_COL_WIDTH, SIDE_COL_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		//绘制血量和剩余炸弹数量
		Unit *unit = units[0];
		for (int i=0; i<unit->health; i++)
		{
			SelectObject(hdc_loadBmp, bmp_heart);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,303,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		for (int i=0; i<unit->bubblesleft; i++)
		{
			SelectObject(hdc_loadBmp, bmp_bomb);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + i * BOMB_WIDTH,367,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				BOMB_WIDTH, BOMB_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		unit = units[1];
		for (int i=0; i<unit->health; i++)
		{
			SelectObject(hdc_loadBmp, bmp_heart);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,560,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		for (int i=0; i<unit->bubblesleft; i++)
		{
			SelectObject(hdc_loadBmp, bmp_bomb);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + i * BOMB_WIDTH,624,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				BOMB_WIDTH, BOMB_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		//绘制倒计时
		//十位数
		SelectObject(hdc_loadBmp, nums[count_down/10]);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH, 120,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				NUM_WIDTH, NUM_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		//个位数
			SelectObject(hdc_loadBmp, nums[count_down%10]);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + NUM_WIDTH, 120,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				NUM_WIDTH, NUM_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
	}
	//绘制双人模式侧栏
	if (currentStage->stageID == STAGE_2)
	{
		SelectObject(hdc_loadBmp, bmp_duo_side_col);
			TransparentBlt(
				hdc_memBuffer,
				WINDOW_WIDTH - SIDE_COL_WIDTH,0,  //界面上起始绘制点
				SIDE_COL_WIDTH, SIDE_COL_HEIGHT,                                            //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                                                                                 //位图上起始绘制点
				SIDE_COL_WIDTH, SIDE_COL_HEIGHT,                                     //位图上绘制宽度高度
				RGB(255, 255, 255));
		//绘制血量和剩余炸弹数量
		Unit *unit = units[0];
		for (int i=0; i<unit->health; i++)
		{
			SelectObject(hdc_loadBmp, bmp_heart);
			TransparentBlt(
				hdc_memBuffer,
				24 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,303,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		for (int i=0; i<unit->bubblesleft; i++)
		{
			SelectObject(hdc_loadBmp, bmp_bomb);
			TransparentBlt(
				hdc_memBuffer,
				24 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,367,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		unit = units[1];
		for (int i=0; i<unit->health; i++)
		{
			SelectObject(hdc_loadBmp, bmp_heart);
			TransparentBlt(
				hdc_memBuffer,
				24 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,560,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		for (int i=0; i<unit->bubblesleft; i++)
		{
			SelectObject(hdc_loadBmp, bmp_bomb);
			TransparentBlt(
				hdc_memBuffer,
				24 + 12 * BG_CELL_WIDTH + i * HEART_WIDTH,624,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		//十位数
		SelectObject(hdc_loadBmp, nums[count_down/10]);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH, 120,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				NUM_WIDTH, NUM_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
		//个位数
			SelectObject(hdc_loadBmp, nums[count_down%10]);
			TransparentBlt(
				hdc_memBuffer,
				22 + 12 * BG_CELL_WIDTH + NUM_WIDTH, 120,  //界面上起始绘制点
				HEART_WIDTH, HEART_HEIGHT,  //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                             //位图上起始绘制点
				NUM_WIDTH, NUM_HEIGHT,  //位图上绘制宽度高度
				RGB(255, 255, 255));
	}
}

void DrawHelpScreen(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	// TODO: 绘制游戏帮助页。
	if (currentStage->stageID != STAGE_HELP) return;  // 只在帮助页绘制

	// 背景图（1024x768）已包含操作按键和道具介绍，直接铺满
	SelectObject(hdc_loadBmp, bmp_Help_bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 在底部空白处补充背景图里没有的规则说明
	HFONT font = CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		0, 0, 0, 0, L"微软雅黑");
	HGDIOBJ oldFont = SelectObject(hdc_memBuffer, font);
	SetBkMode(hdc_memBuffer, TRANSPARENT);         // 文字背景透明
	SetTextColor(hdc_memBuffer, RGB(60, 60, 60));  // 深灰色文字
	RECT rect = { 0, 722, WINDOW_WIDTH, 752 };     // 文字区域：底部一整行
	DrawText(hdc_memBuffer,
		L"Esc 暂停 | 泡泡放下后会十字爆炸并炸毁砖块 | 炸倒对手即获胜，90 秒内未分胜负则超时",
		-1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// 恢复原字体并释放新建的字体
	SelectObject(hdc_memBuffer, oldFont);
	DeleteObject(font);
}

void DrawTitleScreen(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	// TODO: 标题界面 —— 绘制开始背景和游戏标题
	if (currentStage->stageID == STAGE_STARTMENU || currentStage->stageID == STAGE_GAMEMODE) {
		//绘制背景图片：原图与窗口同为 1024x768，直接铺满
		SelectObject(hdc_loadBmp, bmp_Beginning);
		BitBlt(hdc_memBuffer, 0, 0, BEGINNING_WIDTH, BEGINNING_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

		//绘制游戏标题：原图 260x260，水平居中，位于按钮上方，白色视为透明
		SelectObject(hdc_loadBmp, bmp_Tittle);
		TransparentBlt(
			hdc_memBuffer,
			(WINDOW_WIDTH - 260) / 2, 120,  //界面上起始绘制点
			260, 260,                       //界面上绘制宽度高度
			hdc_loadBmp,
			0, 0,                           //位图上起始绘制点
			260, 260,                       //位图上绘制宽度高度
			RGB(255, 255, 255));
	}
}

void DrawGrassBackground(HDC hdc_memBuffer, HDC hdc_loadBmp)
{
	// TODO: 背景图片 —— 对局先铺草地；开始、选角、帮助、侧栏在各自界面里绘制
	SelectObject(hdc_loadBmp, bmp_Background);

	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
}

void LoadGameBitmaps(HINSTANCE hInstance)
{
	// 加载背景、按钮、角色和道具等位图资源。
	bmp_Grass = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GRASS));
	bmp_Beginning = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BEGINNING));
	bmp_Tittle = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_TITTLE));
	bmp_Help_bg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_HELP_BG));
	bmp_StartButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_START));
	bmp_HelpButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_HELP));
	bmp_BackButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BACK));
	bmp_SoloButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SOLO));
	bmp_DuoButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DUO));
	bmp_ContinueButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CONTINUE));
	bmp_RestartButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RESTART));
	bmp_HomepageButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_HOMEPAGE));
	bmp_PlayAgainButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PLAYAGAIN));
	bmp_Choose = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CHOOSE));
	bmp_ChooseHelp1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CHOOSEHELP1));
	bmp_ChooseHelp2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CHOOSEHELP2));
	bmp_Win = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_WIN));
	bmp_Lose = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOSE));
	bmp_TimeOut = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_TIMEOUT));
	bmp_Unit_Rival = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIVAL));
	bmp_Unit_Role1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ROLE1));
	bmp_Unit_Role2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ROLE2));
	bmp_Unit_Role3 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ROLE3));
	bmp_Unit_Role4 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ROLE4));
	bmp_Arrow = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ARROW));
	bmp_Arrow2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ARROW2));
	bmp_Bubble = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BUBBLE));
	bmp_block = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BLOCK));
	bmp_explosion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_EXPLOSION));
	bmp_solo_side_col = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SOLO_SIDE_COL));
	bmp_duo_side_col = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DUO_SIDE_COL));
	bmp_heart = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_HEART));
	bmp_bomb = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BOMB));
	bmp_speedup = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPEEDUP));
	bmp_bomb_expansion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BOMB_EXPANSION));
	bmp_bomb_enhance = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BOMB_ENHANCE));
	bmp_0 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_0));
	bmp_1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_1));
	bmp_2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_2));
	bmp_3 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_3));
	bmp_4 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_4));
	bmp_5 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_5));
	bmp_6 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_6));
	bmp_7 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_7));
	bmp_8 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_8));
	bmp_9 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_9));
}
// TODO: 标题界面 —— 开始画面显示开始按钮
void ShowTitleButtons()
{
	for (int i = 0; i < (int)buttons.size(); i++)
	{
		Button* button = buttons[i];
		// 标题页只显示"开始游戏"和"帮助"两个按钮，其余全部隐藏
		button->visible = (button->buttonID == BUTTON_STARTGAME || button->buttonID == BUTTON_HELP);
	}
}

// TODO: 帮助界面 —— 独立帮助页，只显示右上角返回标题的按钮
void ShowHelpButtons()
{
	// TODO: 设置帮助页按钮的显示状态。
	// 帮助页只显示右上角的"返回"按钮，其余全部隐藏
	for (int i = 0; i < (int)buttons.size(); i++)
	{
		Button* button = buttons[i];
		button->visible = (button->buttonID == BUTTON_BACK);
	}
}

// TODO: 暂停界面 —— Esc 暂停对局，背景音乐停止，并显示继续、重开、回标题
void OpenPauseMenu(HWND hWnd)
{
	// TODO: 暂停对局并显示暂停菜单。
}

// TODO: 结束画面 —— 结算界面显示“再玩一次”，点击后回到标题
void ShowResultButtons()
{
	// TODO: 设置结算页按钮的显示状态。
}

// TODO: 死亡检测 —— 开局血量传入 1，被炸到即死
void SpawnBattleUnits(int stageID)
{
	switch (stageID) {
	case STAGE_1:
		pointer[0] = 1; pointer[1] = 0; pointer[2] = 0; pointer[3] = 0;
		units.push_back(CreateUnit(UNIT_SIDE_ME, 97.75, 100, 1, 1));
		units.push_back(CreateUnit(UNIT_SIDE_RIVAL, 545.75, 685, 1, 1));
		break;
	case STAGE_2:
		units.push_back(CreateUnit(UNIT_SIDE_ME, 97.75, 100, 1, 1));
		units.push_back(CreateUnit(UNIT_SIDE_MATE, 545.75, 685, 1, 1));
		break;
	default:
		break;
	}
}

// TODO: 基本移动 —— 四方向移动。单人用方向键；双人玩家1用 WASD，玩家2用方向键
void BasicMove(HWND hWnd)
{
	if (units.size() >= 1 && units[0]->alive == true) {
		MoveUnit(hWnd, units[0], keyLeftDown, keyUpDown, keyRightDown, keyDownDown);
	}

}

// TODO: 基本移动 —— 单人按空格放泡泡；双人玩家1按空格，玩家2按回车
void PlaceBubbleByKey(HWND hWnd, WPARAM key)
{
	if (key == VK_SPACE && currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_2 && units[0]->alive == true)
		PlaceBubble(hWnd, units[0]);
	if (key == VK_RETURN && currentStage->stageID == STAGE_2 && units[1]->alive == true)
		PlaceBubble(hWnd, units[1]);
}


// TODO: 背景音乐 —— 标题界面循环播放 bgm1，并先关掉对局音乐
void PlayMenuBgm()
{
	// TODO: 播放菜单背景音乐。
}

// TODO: 背景音乐 —— 进入对局后循环播放 bgm2
void PlayBattleBgm()
{
	// TODO: 播放对局背景音乐。
}

// TODO: 背景音乐 —— 暂停时停止对局音乐，继续时再接着播
void StopBattleBgm()
{
	// TODO: 暂停对局背景音乐。
}

// TODO: 背景音乐 —— 继续游戏时接着播放对局音乐
void ResumeBattleBgm()
{
	// TODO: 继续播放对局背景音乐。
}

// TODO: 背景音乐 —— 离开对局或进入结算时关闭对局音乐
void CloseBattleBgm()
{
	// TODO: 关闭对局背景音乐。
}

// TODO: 倒计时 —— 开局 90 秒，每秒减 1
void StartCountdown(HWND hWnd)
{
	count_down = 10;
	SetTimer(hWnd, TIMER, 1000, NULL);
}

// TODO: 倒计时 —— 每秒减 1，减到 0 进入超时；减到 30 时全体移速提高
void TickCountdown(HWND hWnd)
{
	if (count_down > 0)
	{
		count_down--;
	}

}

// TODO: 泡泡冷却 —— 放下后若还没到上限，约 3 秒补回一颗
void StartBubbleCooldown(HWND hWnd, Unit* unit)
{
	// TODO: 登记炸弹数量的补充计时。
	// 3 秒后由 TimerUpdate 调用 FinishBubbleCooldown 补回一颗
	BindTimer(cdTimers, hWnd, unit, next_CD_timer_id, 3000);
}

// TODO: 泡泡冷却 —— 冷却结束，补回一颗泡泡
void FinishBubbleCooldown(Unit* unit)
{
	// 补回一颗，但不超过容量上限
	if (unit->bubblesleft < unit->max_bomb)
		unit->bubblesleft++;
}

// TODO: 电脑走位 —— 躲开泡泡的爆炸范围，并朝玩家走近
void ComputerMove(HWND hWnd)
{
	// TODO: 实现 AI 的移动策略。
}

// TODO: 基本攻击 —— 电脑在距离足够近时放置泡泡
void BasicAttack(HWND hWnd)
{
	// TODO: 判断 AI 放置炸弹的时机。
}

// TODO: 加速药水 —— 拾取后移速提高，持续约 3 秒
void ApplySpeedPotion(HWND hWnd, Unit* unit)
{
	// TODO: 处理加速道具效果。
}

// TODO: 加速药水 —— 约 3 秒后把这次提高的移速减回
void EndSpeedPotion(Unit* unit)
{
	// TODO: 结束加速效果。
}

// TODO: 死亡检测 —— 拾取爱心后加 1 点血，上限 3，因此要多挨几下
void ApplyHeart(Unit* unit)
{
	// TODO: 处理回血道具效果。
}

// TODO: 弹药匣 —— 拾取后可同时放置的泡泡数加 1，上限 3
void ApplyAmmoBox(Unit* unit)
{
	// TODO: 处理弹药匣效果。
}

// TODO: 威力药水 —— 拾取后获得加强次数，下一颗泡泡爆炸范围变为 2 格
void ApplyPowerPotion(Unit* unit)
{
	// TODO: 处理威力药水效果。
}

// TODO: 威力药水 —— 有加强次数时本颗泡泡范围为 2，并消耗一次
void SetBubblePower(Unit* unit, Bubble* newBubble)
{
	// TODO: 设置本次炸弹的爆炸范围。
	// 默认范围 1 格；有加强次数时范围 2 格，并消耗一次
	newBubble->range = 1;
	if (unit->enhance_bomb > 0) {
		newBubble->range = 2;
		unit->enhance_bomb--;
	}
}

// TODO: 死亡检测 —— 血量减到 0 即死亡
void ApplyBlastHit(Unit* unit, bool inFlame)
{
	// TODO: 处理单位受击和死亡状态。
	// 已死亡或没被火焰命中则不处理
	if (!unit->alive || !inFlame) return;

	// 扣 1 点血，血量归零即死亡
	unit->health--;
	if (unit->health <= 0)
		unit->alive = false;
}
void Paint(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	HGDIOBJ oldMemBmp = SelectObject(hdc_memBuffer, blankBmp);            // 保存内存DC的原位图
	HGDIOBJ oldLoadBmp = GetCurrentObject(hdc_loadBmp, OBJ_BITMAP);       // 保存素材DC的原位图（各绘制函数会往里换素材）

	// 绘制背景到缓存
	DrawGrassBackground(hdc_memBuffer, hdc_loadBmp);

	// 按场景分类绘制内容到缓存
	DrawTitleScreen(hdc_memBuffer, hdc_loadBmp);
	//帮助界面
	DrawHelpScreen(hdc_memBuffer, hdc_loadBmp);
	//单人选角色界面
	if (currentStage->stageID == STAGE_CHOOSE1)
	{
		//绘制选角色背景图片
		SelectObject(hdc_loadBmp, bmp_Choose);
		for (int i=0; i < BG_ROWS; i++)
		{
			for (int j=0; j < BG_COLUMNS; j++)
			{
				TransparentBlt(
				hdc_memBuffer, j * BG_CELL_WIDTH, i * BG_CELL_HEIGHT,
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				hdc_loadBmp, 0,0,
				CHOOSE_BITMAP_WIDTH, CHOOSE_BITMAP_HEIGHT,
				RGB(255, 255, 255));
			}
		}
		//绘制单人选角帮助
		SelectObject(hdc_loadBmp, bmp_ChooseHelp1);
		TransparentBlt(
			hdc_memBuffer,
			300,500,                    //界面上起始绘制点
			CHOOSEHELP1_SIZE_X, CHOOSEHELP1_SIZE_Y,    //界面上绘制宽度高度
			hdc_loadBmp,
			0,0,                    //位图上绘制起点
			CHOOSEHELP1_SIZE_X, CHOOSEHELP1_SIZE_Y,   //位图上绘制宽度高度
			RGB(255,255,255));
		//绘制四个角色
		for (int i=0; i<roles.size(); i++)
		{
			SelectObject(hdc_loadBmp, roles[i]);
			TransparentBlt(
			hdc_memBuffer,
			50 + i * 225,200,                    //界面上起始绘制点
			3 * UNIT_SIZE_X, 3 * UNIT_SIZE_Y,    //界面上绘制宽度高度
			hdc_loadBmp,
			0,0,                    //位图上绘制起点
			UNIT_BITMAP_SIZE_X, UNIT_BITMAP_SIZE_Y,   //位图上绘制宽度高度
			RGB(255,255,255));
		}
		//绘制箭头
		for (int i=0; i < MAX_ROLE_NUM; i++)
		{
			if (pointer[i] == 1)
			{
				SelectObject(hdc_loadBmp, bmp_Arrow);
				TransparentBlt(
					hdc_memBuffer,
					125 + i * 225 ,100,                    //界面上起始绘制点
					ARROW_SIZE_X, ARROW_SIZE_Y,    //界面上绘制宽度高度
					hdc_loadBmp,
					0,0,                    //位图上绘制起点
					ARROW_SIZE_X, ARROW_SIZE_Y,   //位图上绘制宽度高度
					RGB(255,255,255));
				break;
			}
		}
	}
	//双人选角色界面
	if (currentStage->stageID == STAGE_CHOOSE2)
	{
		//绘制选角色背景图片
		SelectObject(hdc_loadBmp, bmp_Choose);
		for (int i=0; i < BG_ROWS; i++)
		{
			for (int j=0; j < BG_COLUMNS; j++)
			{
				TransparentBlt(
				hdc_memBuffer, j * BG_CELL_WIDTH, i * BG_CELL_HEIGHT,
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				hdc_loadBmp, 0,0,
				CHOOSE_BITMAP_WIDTH, CHOOSE_BITMAP_HEIGHT,
				RGB(255, 255, 255));
			}
		}
		//绘制双人选角帮助
		SelectObject(hdc_loadBmp, bmp_ChooseHelp2);
		TransparentBlt(
			hdc_memBuffer,
			100,500,                    //界面上起始绘制点
			CHOOSEHELP2_SIZE_X, CHOOSEHELP2_SIZE_Y,    //界面上绘制宽度高度
			hdc_loadBmp,
			0,0,                    //位图上绘制起点
			CHOOSEHELP2_SIZE_X, CHOOSEHELP2_SIZE_Y,   //位图上绘制宽度高度
			RGB(255,255,255));
		//先画前两个角色。第 3、第 4 个在 DrawExtraDuoRoles 里，由学生实现
		for (int i=0; i<2; i++)
		{
			SelectObject(hdc_loadBmp, roles[i]);
			TransparentBlt(
			hdc_memBuffer,
			50 + i * 225,200,                    //界面上起始绘制点
			3 * UNIT_SIZE_X, 3 * UNIT_SIZE_Y,    //界面上绘制宽度高度
			hdc_loadBmp,
			0,0,                    //位图上绘制起点
			UNIT_BITMAP_SIZE_X, UNIT_BITMAP_SIZE_Y,   //位图上绘制宽度高度
			RGB(255,255,255));
		}
		duoRoleNum = 2;
		DrawExtraDuoRoles(hdc_memBuffer, hdc_loadBmp);
		int roleNum = duoRoleNum;
		KeepDuoPointerInRange(pointer, pointer2, roleNum);
		KeepDuoPointerInRange(pointer2, pointer, roleNum);
		//绘制箭头1
		for (int i=0; i < roleNum; i++)
		{
			if (pointer[i] == 1)
			{
				SelectObject(hdc_loadBmp, bmp_Arrow);
				TransparentBlt(
					hdc_memBuffer,
					125 + i * 225 ,100,                    //界面上起始绘制点
					ARROW_SIZE_X, ARROW_SIZE_Y,    //界面上绘制宽度高度
					hdc_loadBmp,
					0,0,                    //位图上绘制起点
					ARROW_SIZE_X, ARROW_SIZE_Y,   //位图上绘制宽度高度
					RGB(255,255,255));
				break;
			}
		}
		//绘制箭头2
		for (int i=0; i < roleNum; i++)
		{
			if (pointer2[i] == 1)
			{
				SelectObject(hdc_loadBmp, bmp_Arrow2);
				TransparentBlt(
					hdc_memBuffer,
					125 + i * 225 ,100,                    //界面上起始绘制点
					ARROW_SIZE_X, ARROW_SIZE_Y,    //界面上绘制宽度高度
					hdc_loadBmp,
					0,0,                    //位图上绘制起点
					ARROW_SIZE_X, ARROW_SIZE_Y,   //位图上绘制宽度高度
					RGB(255,255,255));
				break;
			}
		}
	}
	if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_2) //TODO：添加多个游戏场景
	{
		//绘制地图
		for (int i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
			for (int j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++) {
				SelectObject(hdc_loadBmp, bmp_block);
				TransparentBlt(
					hdc_memBuffer,
					j * BG_CELL_WIDTH, i * BG_CELL_HEIGHT,					// 界面上起始绘制点
					BG_CELL_WIDTH, BG_CELL_HEIGHT,											// 界面上绘制宽度高度
					hdc_loadBmp,
					map[i][j] * BLOCK_BITMAP_SIZE_X, 0,										// 位图上起始绘制点
					BLOCK_BITMAP_SIZE_X, BLOCK_BITMAP_SIZE_Y,								// 位图上绘制宽度高度
					RGB(255, 255, 255));
			}
		}
		//绘制掉落的道具
		for (int i = 0; i < sizeof(item) / sizeof(item[0]); i++)
		{
			for (int j = 0; j < sizeof(item[0]) / sizeof(item[0][0]); j++)
			{
				if (item[i][j] >= 0 && item[i][j] <= 3)
				{
					SelectObject(hdc_loadBmp, items[item[i][j]]);
					TransparentBlt(
						hdc_memBuffer,
						j * BG_CELL_WIDTH , i * BG_CELL_HEIGHT ,					// 界面上起始绘制点
						ITEM_WIDTH, ITEM_HEIGHT,											// 界面上绘制宽度高度
						hdc_loadBmp,
						0, 0,										// 位图上起始绘制点
						ITEM_WIDTH, ITEM_HEIGHT,	    // 位图上绘制宽度高度
						RGB(255, 255, 255));
				}
			}
		}
		//绘制泡泡
		for (int i = 0; i < bubbles.size(); i++) {
			Bubble* bubble = bubbles[i];
			SelectObject(hdc_loadBmp, bubble->img);
			TransparentBlt(
				hdc_memBuffer,
				bubble->x - 0.5 * BUBBLE_SIZE_X, bubble->y - 0.5 * BUBBLE_SIZE_Y,		// 界面上起始绘制点
				BUBBLE_SIZE_X, BUBBLE_SIZE_Y,											// 界面上绘制宽度高度
				hdc_loadBmp,
				bubble->frame_colume * BUBBLE_BITMAP_SIZE_X, 0,							// 位图上起始绘制点
				BUBBLE_BITMAP_SIZE_X, BUBBLE_BITMAP_SIZE_Y,								// 位图上绘制宽度高度
				RGB(255, 255, 255));
		}
		//绘制爆炸效果
		for (int i=0; i<explosions.size(); i++){
			Explosion* explosion = explosions[i];
			SelectObject(hdc_loadBmp, explosion->img);
			TransparentBlt(
				hdc_memBuffer,
				explosion->x - 0.5 * EXPLOSION_SIZE_X, explosion->y - 0.5 * EXPLOSION_BITMAP_SIZE_Y,  //界面上起始绘制点
				EXPLOSION_SIZE_X, EXPLOSION_BITMAP_SIZE_Y,                                            //界面上绘制宽度高度
				hdc_loadBmp,
			    0, 0,                                                                                 //位图上起始绘制点
				EXPLOSION_BITMAP_SIZE_X, EXPLOSION_BITMAP_SIZE_Y,                                     //位图上绘制宽度高度
				RGB(255, 255, 255));
		}


		// 绘制存活的人物
		for (int i = 0; i < units.size(); i++) {
			Unit* unit = units[i];
			if (unit->alive == true)
			{
				SelectObject(hdc_loadBmp, unit->img);
				TransparentBlt(
					hdc_memBuffer,
					unit->x - 0.5 * UNIT_SIZE_X, unit->y - 0.5 * UNIT_SIZE_Y,			// 界面上起始绘制点
					UNIT_SIZE_X, UNIT_SIZE_Y,											// 界面上绘制宽度高度
					hdc_loadBmp,
					UNIT_BITMAP_SIZE_X * unit->frame_column + INTERVAL_X * unit->frame_column, UNIT_BITMAP_SIZE_Y * unit->frame_row + INTERVAL_Y * unit->frame_row,	// 位图上起始绘制点
					UNIT_BITMAP_SIZE_X, UNIT_BITMAP_SIZE_Y,											// 位图上绘制宽度高度
					RGB(255, 255, 255)
				);
			}
		}
	}
	//绘制单人模式侧栏
	DrawGameHud(hdc_memBuffer, hdc_loadBmp);
	//绘制游戏胜利界面
	DrawResultScreen(hdc_memBuffer, hdc_loadBmp);


	// 绘制按钮到缓存

	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}


	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	SelectObject(hdc_memBuffer, oldMemBmp);   // 先选回原位图，blankBmp 才能被删除
	SelectObject(hdc_loadBmp, oldLoadBmp);    // 素材位图换出，避免删除DC时仍被占用
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}



// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	HDC hdc_window = GetDC(hWnd);
	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	HBITMAP oldBuffer = (HBITMAP)SelectObject(hdc_memBuffer, bmp_output);
	HBITMAP oldSrc = (HBITMAP)SelectObject(hdc_loadBmp, bmp_src);

	for (int i = 0; i < BG_ROWS; i++) {

		int src_row = 1;
		if (i >= BG_ROWS_SKY)
			src_row = 0;

		for (int j = 0; j < BG_COLUMNS; j++) {
			int src_column = rand() % BG_SRC_COUNT;

			TransparentBlt(
				hdc_memBuffer, j * BG_CELL_WIDTH, i * BG_CELL_HEIGHT,
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				hdc_loadBmp, src_column * BG_CELL_WIDTH, src_row * BG_CELL_HEIGHT,
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				RGB(255, 255, 255)
			);

		}
	}

	SelectObject(hdc_memBuffer, oldBuffer);
	SelectObject(hdc_loadBmp, oldSrc);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);
	ReleaseDC(hWnd, hdc_window);

	return bmp_output;
}
