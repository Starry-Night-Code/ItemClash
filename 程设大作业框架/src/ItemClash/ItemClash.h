#pragma once

#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#pragma comment(lib,"Winmm.lib")
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <mmsystem.h>


// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//图象处理的函数接口，例如：透明色的位图的绘制TransparentBlt函数


#pragma endregion


#pragma region 宏定义

#define WINDOW_WIDTH			1024		//游戏窗口宽度
#define WINDOW_HEIGHT			768			//游戏窗口高度
#define TITTLEBARHEIGHT         256         //游戏标题高度
#define TITTLEBARWIDTH          472         //游戏标题宽度


#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_HELP              1       //帮助界面的ID
#define STAGE_GAMEMODE          2       //选择游戏模式界面的ID
#define STAGE_CHOOSE1           3       //单人选角色界面的ID
#define STAGE_CHOOSE2           4       //双人选角色界面的ID
#define STAGE_1					5		//单人游戏场景的ID
#define STAGE_2                 6       //双人游戏场景的ID
#define STAGE_LOSE              7       //失败界面场景的ID
#define STAGE_WIN               8       //胜利界面场景的ID
#define STAGE_TIMEOUT           9       //时间到界面场景的ID


#define UNIT_SIZE_X				67.5	//单位的宽度
#define UNIT_SIZE_Y				90		//单位的高度
#define UNIT_BITMAP_SIZE_X		45		//单位在位图上的宽度
#define UNIT_BITMAP_SIZE_Y		60
#define INTERVAL_X              55      //单位在位图上的水平间隔
#define INTERVAL_Y              40      //单位在位图上的竖直间隔
#define BUBBLE_SIZE_X			60		//泡泡
#define BUBBLE_SIZE_Y			60		
#define BUBBLE_BITMAP_SIZE_X	46		
#define BUBBLE_BITMAP_SIZE_Y	46
#define EXPLOSION_SIZE_X        60      //爆炸效果
#define EXPLOSION_SIZE_Y        60
#define EXPLOSION_BITMAP_SIZE_X 92
#define EXPLOSION_BITMAP_SIZE_Y 92
#define BLOCK_BITMAP_SIZE_X		40		//砖块在位图上的宽度
#define BLOCK_BITMAP_SIZE_Y		40
#define CHOOSEHELP1_SIZE_X      443     //单人选角帮助宽度
#define CHOOSEHELP1_SIZE_Y      206     //单人选角帮助高度
#define CHOOSEHELP2_SIZE_X      729     //双人选角帮助宽度
#define CHOOSEHELP2_SIZE_Y      214     //双人选角帮助高度
#define ARROW_SIZE_X            64      //选角色箭头宽度
#define ARROW_SIZE_Y            80      //选角色箭头高度
#define WIN_SIZE_X              282     //胜利图片宽度
#define WIN_SIZE_Y              90      //胜利图片高度
#define LOSE_SIZE_X             685     //失败图片宽度
#define LOSE_SIZE_Y             103     //失败图片高度
#define TIMEOUT_SIZE_X          685     //时间到图片宽度
#define TIMEOUT_SIZE_Y          103     //时间到图片高度
#define SIDE_COL_WIDTH          256     //侧栏宽度
#define SIDE_COL_HEIGHT         768     //侧栏高度
#define HEART_WIDTH             64      //血量宽度
#define HEART_HEIGHT            64      //血量高度
#define BOMB_WIDTH              64      //炸弹宽度
#define BOMB_HEIGHT             64      //炸弹高度
#define ITEM_WIDTH              64      //道具宽度
#define ITEM_HEIGHT             64      //道具高度
#define NUM_WIDTH               64      //数字宽度
#define NUM_HEIGHT              64      //数字高度

//单位阵营定义
#define UNIT_SIDE_ME			10000	//我方
#define UNIT_SIDE_MATE			10001   //队友
#define UNIT_SIDE_RIVAL			10002	//对手


//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走

//单位方向定义
#define UNIT_DIRECT_RIGHT		2		//向右
#define UNIT_DIRECT_LEFT		1		//向左
#define UNIT_DIRECT_UP			3		//向上
#define UNIT_DIRECT_DOWN		0		//向下


//背景
#define BG_SRC_COUNT			6		//背景资源数量
#define BG_COLUMNS				16		//背景列数
#define BG_ROWS					12		//背景行数
#define BG_ROWS_SKY				3		//背景天空行数
#define BG_CELL_WIDTH			64		//背景单格宽度
#define BG_CELL_HEIGHT			64		//背景单格高度
#define BEGINNING_WIDTH         1024    //开始界面背景宽度
#define BEGINNING_HEIGHT        768     //开始界面背景高度
#define CHOOSE_BITMAP_WIDTH     40      //选角色界面背景位图上宽度
#define CHOOSE_BITMAP_HEIGHT    40      //选角色界面背景位图上高度

//其它定义
#define TITTLE_WIDTH                251     //标题位图上宽度
#define TITTLE_HEIGHT               136     //标题位图上高度
#define BUTTON_STARTGAME			1001	//开始游戏按钮ID
#define BUTTON_STARTGAME_WIDTH		374		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		118		//开始游戏按钮高度
#define BUTTON_HELP                 1002    //帮助按钮ID
#define BUTTON_HELP_WIDTH           374     //帮助按钮宽度
#define BUTTON_HELP_HEIGHT          118     //帮助按钮高度
#define BUTTON_BACK                 1003    //返回按钮ID
#define BUTTON_BACK_WIDTH           100     //返回按钮宽度
#define BUTTON_BACK_HEIGHT          100     //返回按钮高度
#define BUTTON_SOLO                 1004    //单人游戏按钮ID
#define BUTTON_SOLO_WIDTH		    352		//单人游戏按钮宽度
#define BUTTON_SOLO_HEIGHT		    123		//单人游戏按钮高度
#define BUTTON_DUO                  1005    //双人游戏按钮ID
#define BUTTON_DUO_WIDTH		    351		//双人游戏按钮宽度
#define BUTTON_DUO_HEIGHT		    138		//双人游戏按钮高度
#define BUTTON_CONTINUE             1006    //继续游戏按钮ID
#define BUTTON_CONTINUE_WIDTH       442     //继续游戏按钮宽度
#define BUTTON_CONTINUE_HEIGHT      73      //继续游戏按钮高度
#define BUTTON_RESTART              1007    //重新开始按钮ID
#define BUTTON_RESTART_WIDTH        442     //重新开始按钮宽度
#define BUTTON_RESTART_HEIGHT       73      //重新开始按钮高度
#define BUTTON_HOMEPAGE             1008    //返回首页按钮ID
#define BUTTON_HOMEPAGE_WIDTH       442     //返回首页按钮宽度
#define BUTTON_HOMEPAGE_HEIGHT      73      //返回首页按钮高度
#define BUTTON_PLAYAGAIN            1009    //再玩一次按钮ID
#define BUTTON_PLAYAGAIN_WIDTH      372     //再玩一次按钮宽度
#define BUTTON_PLAYAGAIN_HEIGHT     109     //再玩一次按钮高度

#define TIMER_GAMETIMER				1		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 单位结构体
struct Unit
{
	HBITMAP img;	//图片

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;		//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几个


	int side;		//单位阵营
	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	bool alive;     //是否存活
	int health;		//生命值
	int bubblesleft;//剩余炸弹数量
	int max_bomb;   //弹夹容量
	int enhance_bomb; //增强炸弹的数量
};

struct Bubble
{
	HBITMAP img;						//图片

	int x;								//坐标x
	int y;								//坐标y
	int count_down_duration;			//爆炸时长
	int range;							//爆炸半径
	int timer_id;						//定时器id
	int frame_id;
	int frame_colume;
};

struct Explosion
{
	HBITMAP img;                        //图片

	int x;                              //坐标x
	int y;                              //坐标y
	int duration;                       //爆炸效果持续时长
	int timer_id;                       //定时器id
};



//TODO: 添加游戏需要的更多种数据（地物、砖块等）


#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 其它游戏状态处理函数声明

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加单位函数
Unit* CreateUnit(int side, int x, int y, int health, int max_bomb);

//碰撞检测函数
bool Collision_detection(HWND hWnd, Unit* unit);


// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

//刷新选角色界面箭头位置函数
void UpdateArrow(HWND hWnd);

// 刷新单位状态函数
void UpdateUnits(HWND hWnd);



//单位行为函数
void UnitBehaviour_Rival(HWND hWnd);
void PlaceBubble(HWND hWnd, Unit* unit);
void UpdateBubbles(HWND hWnd);

//TODO: 添加游戏需要的更多函数
//爆炸效果函数
void explode(HWND hWnd, Bubble* bubble);

#pragma endregion 


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

#pragma endregion