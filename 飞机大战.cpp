#include <time.h>
#include<math.h>
#include <conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<easyx.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")


//基本数据类型
typedef struct coord
{
	float x;
	float y;
	bool flag;//判断是否存在
	int init_time;//创建时的时间
	coord() :x(0), y(0), flag(false), init_time(0) {};
}c;
c plane, bullet[20], enemy[50], bomb[80];//坐标
IMAGE p1[2], bk1, bk2, bullet1[2], enemy1[2], bomb1[2];//图片
int start_time, last_time;//控制子弹发射间隔
int play_time = -1000;
int bullet_num = 20, enemy_num = 0, score = 0;//记录
ExMessage m;//获取鼠标信息


//函数实现
void load_picture()
{
	//加载图片
	loadimage(&bullet1[0], "飞机大战_/bullet_bk.png", 30, 30);
	loadimage(&bullet1[1], "飞机大战_/bullet_blue.png", 30, 30);
	loadimage(&bk1, "飞机大战_/bk1.jpg", 800, 900);
	loadimage(&bk2, "飞机大战_/bk2.jpg", 800, 900);
	loadimage(&p1[0], "飞机大战_/plane2bk.png", 100, 100);
	loadimage(&p1[1], "飞机大战_/plane2.png", 100, 100);
	loadimage(&bomb1[0], "飞机大战_/bomb_bk.png", 80, 80);
	loadimage(&bomb1[1], "飞机大战_/bomb.png", 80, 80);
	loadimage(&enemy1[0], "飞机大战_/enemy1_bk.png", 100, 100);
	loadimage(&enemy1[1], "飞机大战_/enemy1.png", 100, 100);
	mciSendString("open 飞机大战_/bk_music.mp3", 0, 0, 0);
	mciSendString("play 飞机大战_/bk_music.mp3 repeat", 0, 0, 0);
}
void init_plane()
{
	plane.x = 400, plane.y = 850, plane.flag = true, plane.init_time = clock();
	for (int i = 0; i < rand() % 7 + 1; i++)
	{
		if (!enemy[i].flag)
		{
			enemy_num++;
			enemy[i].flag = true;
			enemy[i].init_time = clock();
			enemy[i].x = rand() % 600 + 100;
			enemy[i].y = rand() % 300 + 100;
		}
	}
	start_time = last_time = clock();
}
void show()
{
	BeginBatchDraw();
	putimage(0, 0, &bk1);
	putimage(plane.x - 50, plane.y - 50, &p1[0], SRCAND);
	putimage(plane.x - 50, plane.y - 50, &p1[1], SRCPAINT);
	for (int i = 0; i < 50; i++)
	{
		if (enemy[i].flag)
		{
			putimage(enemy[i].x - 50, enemy[i].y - 50, &enemy1[0], SRCAND);
			putimage(enemy[i].x - 50, enemy[i].y - 50, &enemy1[1], SRCPAINT);
		}
	}
	for (int i = 0; i < 20; i++)
	{
		if (bullet[i].flag)
		{
			putimage(bullet[i].x - 15, bullet[i].y - 15, &bullet1[0], SRCAND);
			putimage(bullet[i].x - 15, bullet[i].y - 15, &bullet1[1], SRCPAINT);
		}
	}
	for (int i = 0; i < 80; i++)
	{
		if (bomb[i].flag)
		{
			putimage(bomb[i].x - 40, bomb[i].y - 40, &bomb1[0], SRCAND);
			putimage(bomb[i].x - 40, bomb[i].y - 40, &bomb1[1], SRCPAINT);
		}
	}
}
void plane_move()
{

	peekmessage(&m, EX_MOUSE | EX_KEY);
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (plane.y < 900)
			plane.y += 0.5;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		if (plane.y > 0)
			plane.y -= 0.5;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (plane.x < 800)
			plane.x += 0.5;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (plane.x > 0)
			plane.x -= 0.5;
	}
	if (GetAsyncKeyState(VK_SPACE) && (last_time - start_time) > 100)
	{
		for (int i = 0; i < 20; i++)
		{
			if (!bullet[i].flag)
			{
				bullet[i].flag = true;
				bullet_num--;
				bullet[i].x = plane.x;
				bullet[i].y = plane.y - 60;
				start_time = last_time;
				break;
			}
		}
	}
}
void enemy_move()
{
	for (int i = 0; i < 50; i++)
	{
		if (enemy[i].flag && last_time % 80 == 0)
		{
			if (enemy[i].x < 200)
			{
				if (rand() % 10 > 6)
					enemy[i].x -= 10;
				else enemy[i].x += 10;
			}
			else if (enemy[i].x > 600)
			{
				if (rand() % 10 > 6)
					enemy[i].x += 10;
				else enemy[i].x -= 10;
			}
			else
			{
				if (rand() % 10 < 5)
					enemy[i].x -= 10;
				else enemy[i].x += 10;
			}
			if (enemy[i].y < 600)
			{
				if (rand() % 10 > 6)
					enemy[i].y -= 15;
				else enemy[i].y += 15;
			}
			else
			{
				if (rand() % 10 < 5)
					enemy[i].y -= 15;
				else enemy[i].y += 15;
			}
			if (fabs(enemy[i].x - plane.x) > fabs(enemy[i].y - plane.y))
			{
				if (enemy[i].x - plane.x > 0) enemy[i].x -= 10;
				else enemy[i].x += 10;
			}
			else
			{
				if (enemy[i].y - plane.y > 0) enemy[i].y -= 10;
				else enemy[i].y += 10;
			}
			if ((enemy[i].x > 800) || (enemy[i].x < 0) || (enemy[i].y > 900) || (enemy[i].y < 0))
			{
				enemy[i].flag = false;
				enemy_num--;
			}
		}
	}
	last_time = clock();
}
void bullet_move()
{
	for (int i = 0; i < 20; i++)
	{
		if (bullet[i].flag)
		{
			if (bullet[i].y < -1)
			{
				bullet[i].flag = false;
				bullet_num++;
				continue;
			}
			if (last_time % 2 == 0)
				bullet[i].y--;
		}
	}
}
void add_enemy()
{
	if (last_time % 1000 != 0)
		return;
	int num = rand() % 3 + 1;
	if (last_time % 10000 == 0) num += 10;
	while (num)
	{
		for (int i = 0; i < 50; i++)
		{
			if (enemy[i].flag)
				continue;
			if (!enemy[i].flag)
			{
				num--;
				enemy_num++;
				enemy[i].flag = true;
				enemy[i].init_time = clock();
				enemy[i].x = rand() % 600 + 100;
				enemy[i].y = rand() % 300 + 100;
				if (!num) break;
			}
		}
	}
}
void collision_check()
{
	for (int i = 0; i < 50; i++)
	{
		if (!enemy[i].flag)
			continue;
		if (clock() - plane.init_time > 3000 && fabs(plane.y - enemy[i].y) < 80 && fabs(plane.x - enemy[i].x) < 80)//检测敌机与本机相撞
		{
			enemy[i].flag = false;
			enemy_num--;
			score++;
			plane.flag = false;
		}
		for (int j = 0; j < 20; j++)//检测子弹与敌机相撞
		{
			if (!bullet[j].flag)
				continue;
			if (clock() - bullet[j].init_time > 1000 && fabs(bullet[j].y - enemy[i].y) < 60 && fabs(bullet[j].x - enemy[i].x) < 50)
			{
				bullet[j].flag = false;
				bullet_num++;
				enemy[i].flag = false;
				enemy_num--;
				score++;
				for (int i = 0; i < 80; i++)//爆炸效果
				{
					if (!bomb[i].flag)
					{
						bomb[i].flag = true;
						bomb[i].init_time = clock();
						bomb[i].x = enemy[i].x;
						bomb[i].y = enemy[i].y;
						break;
					}
				}
				if (clock() - play_time > 2000)
				{
					play_time = clock();
					mciSendString("close 飞机大战_/bomb.mp3", 0, 0, 0);
					mciSendString("open 飞机大战_/bomb.mp3", 0, 0, 0);
					mciSendString("play 飞机大战_/bomb.mp3", 0, 0, 0);
				}
				break;
			}
		}
	}



}
void bomb_effect()
{
	for (int i = 0; i < 80; i++)
	{
		if (!bomb[i].flag)
			continue;
		else if (clock() - bomb[i].init_time > 300 && rand() % 40 == 7)
			bomb[i].flag = false;
	}
}
void end()
{
	mciSendString("close 飞机大战_/bk_music.mp3", 0, 0, 0);
	mciSendString("close 飞机大战_/bomb.mp3", 0, 0, 0);
	BeginBatchDraw();
	putimage(0, 0, &bk2);
	setbkmode(TRANSPARENT);

	settextcolor(RED);
	settextstyle(100, 0, "楷体");
	outtextxy(150, 200, "GAME OVER!");

	settextcolor(BLUE);
	settextstyle(50, 0, "楷体");
	char s[20];
	sprintf_s(s, "您最后的得分是:%d", score);
	outtextxy(180, 400, s);

	FlushBatchDraw();
}
int main()
{
	//获取随机种子
	srand(time(NULL));
	initgraph(800, 900, NULL);// 创建绘图窗口


	//设置样式
	setbkcolor(RGB(120, 255, 255));
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	settextstyle(20, 0, "楷体");
	cleardevice();


	//初始化
	load_picture();
	init_plane();


	while (true)
	{
		//主要函数
		show();
		plane_move();
		enemy_move();
		bullet_move();
		add_enemy();
		collision_check();
		bomb_effect();
		last_time = clock();
		if (!plane.flag)
			break;

		//文字内容
		char print1[20], print2[20], print3[20];
		sprintf_s(print1, "剩余子弹数:%d", bullet_num);
		sprintf_s(print2, "剩余敌机数:%d", enemy_num);
		sprintf_s(print3, "我的得分:%d", score);
		outtextxy(0, 0, print1);
		outtextxy(0, 20, print2);
		outtextxy(0, 40, print3);


		FlushBatchDraw();
		cleardevice();
	}
	end();
	getchar();
	return 0;
}



/*
	1.存在爆炸声效无法及时放出问题
	2.改进将设置我机，敌机和爆炸的无敌时间，即最短存在时间
*/