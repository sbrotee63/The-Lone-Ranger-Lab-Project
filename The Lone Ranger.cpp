#define _CRT_SECURE_NO_WARNINGS
#define debug(x) printf("%d \n", &x)
#define TOTALENEMYL1 3
#define TOTALENEMYL2 10
#define TOTALENEMYL3 15
#define val(a,b,c,d) (a<=mx && mx<=a+c && b<=my && my<=b+d)
#include "iGraphics.h"
#include "iPics.h"
#include "iFunctions.h"

pic back[10], coin, gem;

HERO hero;
ENEMY enemy[75];
ARROW arrow[5];
SHIELD shield;
BOSS boss;

double width = 800, height = 500;
unsigned long long t;
long long money = 2000;
int i = 0, screen =  0, menuMusicOn = 0, gameMusicOn = 1, move = 0, dis = 2, shoot = 0, enemyShoot = 0, enemyShootInterval = 0, hShield = 0, eShield, crouch = 0;
int heroLives= 3, bossLives = 3, bossLifeInd = 0, hitShield = 20, bossShootInterval = 0, bossShieldInterval = 0, bossShoot = 0, bossShield = 0, takeCoin = 0;
bool loadDone,saved = 0,quitted = 0;
char coinStr[100], arrowStr[100];
int reScreen = 0,reI = 0,goodScreen = 0,targetTimer = 0;
char controllerText[10];
char MOVIE[][100] = {"BG/1.bmp","BG/2.bmp","BG/3.bmp","BG/4.bmp","BG/5.bmp","BG/6.bmp","BG/7.bmp"};
int movieScreen = 0;

void  init()
{
	t=0;
	money = 2000 ;
	i = 0, screen =  0, menuMusicOn = 0, gameMusicOn = 1, move = 0, dis = 2, shoot = 0, enemyShoot = 0, enemyShootInterval = 0, hShield = 0, eShield = 0, crouch = 0;
	heroLives= 3, bossLives = 3, bossLifeInd = 0, hitShield = 20, bossShootInterval = 0, bossShieldInterval = 0, bossShoot = 0, bossShield = 0, takeCoin = 0;
	loadDone = 0, saved = 0;
	reScreen = 0, reI = 0, goodScreen = 0;
	targetTimer = quitted = movieScreen = 0;
	for(int i=0; i<32; ++i)
	{
		enemy[i].life = 1;
		enemy[i].x = 600, enemy[i].y = 0;
	}
	hero.x = 80, hero.y = 10, hero.life = 1; boss.life = 1; hero.arrow = 20; 
	arrow[0].y = 140; arrow[1].y = 120; boss.x = 600; boss.y = 0;
}

void bar()
{
	iSetColor(255, 29, 29);
	iText(20, 455, "LIFE   : "); //heroLives;
	iFilledRectangle(90, 450, (150.0*heroLives)/3, 20);
	iSetColor(200, 200, 200);
	iText(20, 425, "SHIELD : "); //hitShield;
	iFilledRectangle(90, 420, (150.0*hitShield)/20, 20);
	iSetColor(255, 223, 0);
	sprintf(coinStr, "COIN   : %d", money);
	iText(20, 395, coinStr);
	sprintf(arrowStr, "ARROW  : %d", hero.arrow);
	iText(20, 370, arrowStr);
}

void bossBar()
{
	iSetColor(255, 29, 29);
	iText(520, 455, "BOSS LIFE : "); //heroLives;
	iFilledRectangle(620, 450, (150.0*bossLives)/3, 20);
}

void LOAD()
{
	iShowBMP(0, 0, "BG/loading.bmp");
	load("Sprites/hero/walk1.rs", &hero.move[0]);
	load("Sprites/hero/walk2.rs", &hero.move[1]);
	load("Sprites/hero/walk3.rs", &hero.move[2]);
	load("Sprites/hero/walk4.rs", &hero.move[3]);
	load("Sprites/hero/stand.rs", &hero.stand);
	load("Sprites/hero/crouch.rs", &hero.crouch);
	load ("BG/back1.rs", &back[0]);
	load("Sprites/hero/arrow_1_loaded.rs", &hero.arrowLoad);
	load("Sprites/hero/arrow_1_unloaded.rs", &hero.arrowUnload);
	load("Sprites/hero/die_hero.rs", &hero.die);
	load("Sprites/shield_hero.rs", &shield.heroShield);
	load("Sprites/shield_load_enemy.rs", &shield.enemyShield);
	load("Sprites/boss/boss_arrow_load.rs", &boss.bossArrowLoad);
	load("Sprites/boss/boss_arrow_unload.rs", &boss.bossArrowUnload);
	load("Sprites/boss/boss_knife.rs", &boss.bossKnife);
	load("Sprites/boss/boss_shield.rs", &boss.bossShield);
	load("Sprites/boss/boss_stand.rs", &boss.bossStand);
	load("Sprites/arrow.rs", &arrow[0].img);
	load("Sprites/enemyArrow.rs", &arrow[1].img);
	load("Sprites/coin1.rs", &coin);
	load("Sprites/coin2.rs", &gem);
	for(int i=0; i<32; ++i)
	{
		load("Sprites/enemy/enemy.rs", &enemy[i].stand);
		load("Sprites/enemy/enemy_knife.rs", &enemy[i].knife);
		load("Sprites/enemy/s1_arrow_loaded.rs", &enemy[i].arrowLoad);
		load("Sprites/enemy/s1_arrow_unloaded.rs", &enemy[i].arrowUnload);
		load("Sprites/enemy/die_enemy.rs", &enemy[i].die);
		enemy[i].life = 1;
		enemy[i].x = 600, enemy[i].y = 0;
	}
	hero.x = 80, hero.y = 10, hero.life = 1; boss.life = 1; hero.arrow = 20; 
	arrow[0].y = 140; arrow[1].y = 120; boss.x = 600; boss.y = 0; // bug found (solved?) here
	
}

void shootArrow(int i)
{ 
	if(hero.life == 1)
	{
		if(shoot == 0 && crouch == 0 && hShield == 0)
		{
			if(move == 0)
				iShowBMP2(hero.x, hero.y, &hero.arrowLoad, 235, 235, 235);
			arrow[0].x = hero.x+80;
		}
		if(shoot == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.arrowUnload, 235, 235, 235);
			if(hero.arrow>0)
			{
				iShowBMP2(arrow[0].x, arrow[0].y, &arrow[0].img, 255, 255, 255);
				arrow[0].x+=10;
			}
		}
		if(!(screen % 2))
		{
			if(arrow[0].x >= enemy[i].x-80 && move!=1 && crouch == 0 && hShield == 0 && hero.arrow>0)
			{
				iShowBMP2(hero.x, hero.y, &hero.arrowUnload, 235, 235, 235);
				if(enemy[i].life == 1)
					enemy[i].life = 0;
				arrow[0].x = hero.x+70;
				shoot=0;
			}
		}
		else if(screen % 2)
		{
			bossLifeInd = 0;
			if(arrow[0].x >= boss.x-80 && move!=1 && crouch == 0 && hShield == 0 && hero.arrow>0)
			{
				iShowBMP2(hero.x, hero.y, &hero.arrowUnload, 235, 235, 235);
				if(boss.life == 1 && bossShield == 0 && bossLifeInd == 0)
				{
					if(bossLives > 0)
						bossLives--;
					bossLifeInd = 1;
					if(bossLives == 0)
						boss.life = 0;
				}
				arrow[0].x = hero.x+70;
				shoot=0;
			}
		}
	}
}  

void enemyShootArrow(int i)
{
	if(enemy[i].life == 1)
	{
		enemyShootInterval++;
		if(enemyShootInterval>=150 && enemyShoot == 0 && hero.life == 1)
		{
			enemyShootInterval = 0;
			enemyShoot = 1;
			arrow[1].x=enemy[i].x-80;
			iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].arrowLoad, 239, 239, 239);
		}
		if(enemyShoot == 1 && hero.life == 1)
		{
			iShowBMP2(arrow[1].x, arrow[1].y, &arrow[1].img, 255, 255, 255);
			arrow[1].x-=10;
		}
		if(enemyShoot == 1)
		{
			if(arrow[1].x <= hero.x+70)
			{
				if(crouch == 1 || hShield == 1)
				{
					if(hShield == 1 && hitShield > 0)
						hitShield--;
					if(hitShield == 0)
						hShield = 0;
					arrow[1].x=enemy[i].x-80;
					printf("%d\n", hitShield);
				}
				else
				{
					if(heroLives > 0)
						heroLives--;
					if(heroLives == 0)
						hero.life = 0;
				}
				enemyShoot = 0;
				enemyShootInterval = 0;
			}
		}
		if(hero.life == 0)
		{
			iShowBMP2(hero.x, hero.y-20, &hero.die, 239, 239, 239);
		}
	}
}

void bossShootArrow()
{
	if(boss.life == 1)
	{
		bossShootInterval++;
		if(bossShootInterval>=130 && bossShoot == 0 && hero.life == 1)
		{
			bossShootInterval = 0;
			bossShoot = 1;
			arrow[1].x=boss.x-80;
			iShowBMP2(boss.x, boss.y, &boss.bossArrowLoad, 239, 239, 239);
		}
		if(bossShootInterval>=30 && bossShootInterval <= 100 && bossShoot == 0 && hero.life == 1)
		{
			bossShield = 1;
			iShowBMP2(boss.x, boss.y, &boss.bossShield, 239, 239, 239);
		}
		if(bossShootInterval >= 100)
			bossShield = 0;
		if(bossShoot == 1 && boss.life == 1)
		{
			iShowBMP2(arrow[1].x, arrow[1].y, &arrow[1].img, 255, 255, 255);
			arrow[1].x-=12;
		}
		if(bossShoot == 1)
		{
			if(arrow[1].x <= hero.x+70)
			{
				if(crouch == 1 || hShield == 1)
				{
					if(hShield == 1 && hitShield > 0)
						hitShield--;
					if(hitShield == 0)
						hShield = 0;
					arrow[1].x=boss.x-80;
				}
				else
				{
					if(heroLives > 0)
						heroLives--;
					if(heroLives == 0)
						hero.life = 0;
				}
				bossShoot = 0;
				bossShootInterval = 0;
			}
		}
		if(hero.life == 0)
		{
			iShowBMP2(hero.x, hero.y-20, &hero.die, 239, 239, 239);
		}
	}
}

void iDraw()
{
	iClear();
	if(!loadDone)
	{
		LOAD();
		loadDone = 1;
	}
	iShowBMP(0, 0, "BG/back1.bmp");
	iShowBMP(0,0,"GROUND/ground.bmp");
	if(screen == 12345){
		iShowBMP(0,0,"BG/manual.bmp");
	}
	else if(screen == 123456){
		iShowBMP(0,0,"BG/credit.bmp");
	}
	else if(screen == -1)
	{
		iShowBMP(0,0,MOVIE[movieScreen]);
	}
	else if(screen == 0)
	{
		iShowBMP(0, 0, "BG/menu.bmp");
	}
	else if(screen == 1)
	{
		bar();
		goodScreen = screen;
		if(move == 1)
		{
			hero.x += dis;
			iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			if(hero.x >= width - 130)
				screen = 2, t=0, hero.x = 50;	
		}
		else//(crouch == 0)
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
	}
	else if(screen==2)
	{
		bar();
		goodScreen = screen;
		if(enemy[i].life == 0)
		{
			iShowBMP2(enemy[i].x, enemy[i].y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < enemy[i].x)
				iShowBMP2(enemy[i].x, enemy[i].y+75, &coin, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 50;
				takeCoin = 1;
			}
		}
		else if(enemy[i].life == 1)
		{
			if(hero.x + 100 >= enemy[i].x) 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].knife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].arrowUnload, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 2; t = 0, hero.x = 50; enemyShootInterval = 0; takeCoin = 0; i++;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		if(i<TOTALENEMYL1)
		{
			shootArrow(i);
			enemyShootArrow(i);
		}
		if(i == TOTALENEMYL1)
			screen = 3, t=0, hero.x = 50, bossShootInterval = 0, takeCoin = 0, i = 0;
	}
	else if(screen == 3)
	{
		bar(); bossBar();
		goodScreen = screen;
		if(boss.life == 0)
		{
			iShowBMP2(boss.x, boss.y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < boss.x)
				iShowBMP2(boss.x, boss.y+75, &gem, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 250;
				takeCoin = 1;
			}
		}
		else if(boss.life == 1)
		{
			if(hero.x + 100 >= boss.x) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossKnife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else if(bossShield == 0) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossArrowLoad, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 4; t = 0, hero.x = 50; enemyShootInterval = 0; bossShootInterval = 0; takeCoin = 0; boss.life = 1; bossLives = 3; i = TOTALENEMYL1;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		shootArrow(i);
		bossShootArrow();
	}
	else if(screen == 4)
	{
		bar();
		goodScreen = screen;
		if(enemy[i].life == 0)
		{
			iShowBMP2(enemy[i].x, enemy[i].y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < enemy[i].x)
				iShowBMP2(enemy[i].x, enemy[i].y+75, &coin, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 50;
				takeCoin = 1;
			}
		}
		else if(enemy[i].life == 1)
		{
			if(hero.x + 100 >= enemy[i].x) 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].knife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].arrowUnload, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 4; t = 0, hero.x = 50; enemyShootInterval = 0; bossShootInterval = 0; takeCoin = 0; i++;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		if(i < TOTALENEMYL1 + TOTALENEMYL2)
		{
			shootArrow(i);
			enemyShootArrow(i);
		}
		if(i == TOTALENEMYL1 + TOTALENEMYL2)
			screen = 5, t=0, hero.x = 50, bossShootInterval = 0, takeCoin = 0, i = 0;
	}
	else if(screen == 5)
	{
		bar(); bossBar();
		goodScreen = screen;
		if(boss.life == 0)
		{
			iShowBMP2(boss.x, boss.y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < boss.x)
				iShowBMP2(boss.x, boss.y+75, &gem, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 500;
				takeCoin = 1;
			}
		}
		else if(boss.life == 1)
		{
			if(hero.x + 100 >= boss.x) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossKnife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else if(bossShield == 0) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossArrowLoad, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 6; t = 0, hero.x = 50; enemyShootInterval = 0; bossShootInterval = 0; takeCoin = 0; boss.life = 1; bossLives = 3; i = TOTALENEMYL1 + TOTALENEMYL2;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		shootArrow(i);
		bossShootArrow();
	}
	else if(screen == 6)
	{
		bar();
		goodScreen = screen;
		if(enemy[i].life == 0)
		{
			iShowBMP2(enemy[i].x, enemy[i].y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < enemy[i].x)
				iShowBMP2(enemy[i].x, enemy[i].y+75, &coin, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 50;
				takeCoin = 1;
			}
		}
		else if(enemy[i].life == 1)
		{
			if(hero.x + 100 >= enemy[i].x) 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].knife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else 
			{
				iShowBMP2(enemy[i].x, enemy[i].y, &enemy[i].arrowUnload, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 6; t = 0, hero.x = 50; enemyShootInterval = 0; takeCoin = 0; i++;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		if(i < TOTALENEMYL1 + TOTALENEMYL2 + TOTALENEMYL3)
		{
			shootArrow(i);
			enemyShootArrow(i);
		}
		if(i == TOTALENEMYL1 + TOTALENEMYL2 + TOTALENEMYL3)
			screen = 7, t=0, hero.x = 50, bossShootInterval = 0, takeCoin = 0, i = 0;
	}
	else if(screen == 7)
	{
		bar(); bossBar();
		goodScreen = screen;
		if(boss.life == 0)
		{
			iShowBMP2(boss.x, boss.y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < boss.x)
				iShowBMP2(boss.x, boss.y+75, &gem, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 500;
				takeCoin = 1;
			}
		}
		else if(boss.life == 1)
		{
			if(hero.x + 100 >= boss.x) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossKnife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else if(bossShield == 0) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossArrowLoad, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 8; t = 0, hero.x = 50; enemyShootInterval = 0; bossShootInterval = 0; boss.life = 1; takeCoin = 0; bossLives = 3; i = 0;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		shootArrow(i);
		bossShootArrow();
	}
	else if(screen == 8)
	{
		bar(); bossBar();
		goodScreen = screen;
		if(boss.life == 0)
		{
			iShowBMP2(boss.x, boss.y-20, &enemy[i].die, 239, 239, 239);
			if(hero.x + 100 < boss.x)
				iShowBMP2(boss.x, boss.y+75, &gem, 255, 255, 255);
			else if(takeCoin == 0)
			{
				money += 500;
				takeCoin = 1;
			}
		}
		else if(boss.life == 1)
		{
			if(hero.x + 100 >= boss.x) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossKnife, 239, 239, 239);
				//iShowBMP2(hero.x-50, hero.y-20, &enemy[i].die, 239, 239, 239);
				hero.life = 0;
			}
			else if(bossShield == 0) 
			{
				iShowBMP2(boss.x, boss.y, &boss.bossArrowLoad, 239, 239, 239);
			}
		}
		if(move == 1)
		{
			if(hero.life == 1)
			{
				hero.x += dis;
				iShowBMP2(hero.x, hero.y, &hero.move[(t/13) % 4], 235, 235, 235);
			}
			if(hero.x >= width - 130)
			{
				screen = 9;//; t = 0, hero.x = 50; enemyShootInterval = 0; bossShootInterval = 0; takeCoin = 0; boss.life = 1; bossLives = 3; i = 0;
			}
		}
		//if(crouch == 1 && hero.life == 1)
			//iShowBMP2(hero.x, hero.y-10, &hero.crouch, 239, 239, 239);
		if(hShield == 1 && hero.life == 1)
		{
			iShowBMP2(hero.x, hero.y, &hero.stand, 239, 239, 239);
			iShowBMP2(hero.x+70, hero.y+50, &shield.heroShield, 239, 239, 239);
		}
		shootArrow(i);
		bossShootArrow();
	}
	else if(screen == 9){
		iShowBMP(0,0,"BG/last.bmp");
	}
	else if(screen == 999999)
	{
		iShowBMP(0,0,"BG/menu.bmp");
		iSetColor(29,29,29);
		iFilledRectangle(200,175,400,150);
		iSetColor(255,255,255);
		iText(220,290,"Are you sure you want to quit?");
		iSetColor(255,255,255);
		iFilledRectangle(540,185,50,20);
		iFilledRectangle(480,185,50,20);
		iSetColor(29,29,29);
		iText(550,190,"NO");
		iText(490,190,"YES");
	}
	else if(screen == 101010)
	{
		iShowBMP(0, 0,"BG/myshop.bmp");
		bar();
	}
	else if(screen == 1010)
	{
		iShowBMP(0,0,"BG/pause.bmp");
		iSetColor(255,255,255);
		if(saved == 1) iText(10,10,"SAVED");
	}
	if(hero.life == 0){
		if(targetTimer == 0)targetTimer = t;
		if(t >= targetTimer + 100) iShowBMP(0,0,"BG/gg.bmp");
	}
} 

void iMouseMove(int mx, int my)
{

}
int kk;
void saveGame()
{
	FILE *fp = fopen("SAVED/GAME.log","w");
	fprintf(fp,"%d\n%d\n%d\n%d\n%d\n%d\n%d", goodScreen, money, kk, hero.arrow, heroLives, hitShield, i);
	printf("%d %d %d %d %d %d\n", goodScreen, money, hero.arrow, heroLives, hitShield, i);
	fclose(fp);
}

void loadGame()
{
	FILE *fp = fopen("SAVED/GAME.log","r");
	fscanf(fp,"%d %d %d %d %d %d %d %d",&screen, &money, &kk, &kk, &hero.arrow,&heroLives,&hitShield,&i);
	printf("%d %d %d %d %d %d\n", goodScreen, money, hero.arrow, heroLives, hitShield, i);
	fclose(fp);
}

void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(screen == 12345){
			screen = 0;
		}
		if(screen == 123456){
			screen = 0;
		}
		if(screen == 9){
			init();
			screen = 0;
		}
		if(screen == 0)
		{
			if(val(20, 345, 160, 50))
			{
			//play
				screen = -1;
				menuMusicOn = 1;
				PlaySound(NULL, NULL, 0);
				PlaySound("Music/Ard Skellig.wav", NULL, SND_LOOP | SND_ASYNC);
			}
			if(val(20, 272, 160, 50))
			{
			//load
				loadGame();
			}
			if(val(20, 200, 160, 50))
			{
			//credit
				screen = 123456;
			}
			if(val(20, 120, 160, 50))
			{
			//manual
				screen = 12345;
			}
			if(val(20, 50, 160, 50))
			{
				//exit
				screen = 999999;
			}
		}
		if(screen == 999999)
		{
			if(val(540, 185, 50, 20))
				screen = 0;
			if(val(480, 185, 50, 20)) 
				exit(0);
		}
		if(screen == 101010){
			if(val(50,230,220,35)){
				//bow
			}
			if(val(290,230,220,35)){
				//arrow
				if(money >= 300 && hero.arrow != 20) hero.arrow=20,money -=300;
			}
			if(val(530,230,220,35)){
				//health
				if(money >= 2000 && heroLives != 3) heroLives=20,money -=2000;
			}
			if(val(290,35,220,35)){
				//shield
				if(money >= 500 && hitShield != 20) hitShield=20,money -=500;
			}
			if(val(700,400,100,100)){
				//exit
				screen = 1010;
			}
		}
		if(screen == 1010){
			saved = 0;
			if(val(280,280,210,50)){
				//resume
				screen = reScreen;
				i = reI;
				iResumeTimer(0);
			}if(val(280,220,210,50)){
				//store
				screen = 101010;
			}if(val(280,160,210,50)){
				//save
				saveGame();
				saved = 1;
			}if(val(280,90,210,50)){
				//quit
				init();
				iResumeTimer(0);
				screen = 0;
			}
		}
		if(hero.life == 0){
			init();
			screen = 0;
		}
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		
	}
}


void iKeyboard(unsigned char key)
{
	if(screen == 12345){
		screen = 0;
	}
	if(screen == 123456){
		screen = 0;
	}
	if(screen == 9){
		init();
		screen = 0;
	}
	if(hero.life == 0){
		init();
		screen = 0;
	}
	move=0;
	if(key == controllerText[0] && shoot == 0)
	{
		move = 1;
		shoot = 0;
		//crouch = 0;
		hShield = 0;
	}
	if(key == controllerText[1])
	{
		if(shoot == 0)
		{
			shoot = 1;
			//crouch = 0;
			hShield = 0;
			if(hero.arrow>0)
				hero.arrow--;
		}
	}
	if(key == controllerText[3])
	{
		//crouch = 1;
		move = 0;
		shoot = 0;
		hShield = 0;
	}
	if(key == controllerText[2])
	{
		if(hitShield > 0)
		{
			hShield = 1;
			move = 0;
			shoot = 0;
			//crouch = 0;
		}
		else
			hShield = 0;
	}
	else if(key == 32 && screen > 0){
		reScreen = goodScreen;
		reI = i;
		screen = 1010;
		iPauseTimer(0);
	}
}

void iSpecialKeyboard(unsigned char key)
{

	if(key == GLUT_KEY_RIGHT){
		if(screen == -1 && movieScreen != 6) movieScreen += 1;
		else if(screen == -1 && movieScreen == 6) screen = 1;
	}
	if(key == GLUT_KEY_LEFT){
		if(screen == -1 && movieScreen !=0) movieScreen -= 1;
	}
}

void change()
{
	t++;
	if(t == 1000000000000000007)
		t=0;
}

int main()
{
	iSetTimer(10, change);
	FILE *fp = fopen("SAVED/CONTROLLER.log","r");
	fscanf(fp,"%s",controllerText);
	fclose(fp);
	if(menuMusicOn == 0)
		PlaySound("Music/Rogue.wav", NULL, SND_LOOP | SND_ASYNC);
	iInitialize(width, height, "The Lone Ranger");
	return 0;
}