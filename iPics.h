#include "stdio.h"
using namespace std;
#define endl "\n"

struct pic 
{
	int x,y;
	int pxl[800][500];
};

struct ENEMY
{
	struct pic die, arrowLoad, arrowUnload, knife, stand;
	int life, arrow;
	double x, y;
};

struct BOSS
{
	struct pic bossArrowLoad, bossArrowUnload, bossKnife, bossShield, bossStand;
	int life, arrow;
	double x, y;
};

struct HERO
{
	struct pic stand, move[4], crouch, arrowLoad, arrowUnload, die;
	int life, arrow;
	double x, y;
};

struct ARROW
{
	struct pic img;
	double x, y;
};

struct SHIELD
{
	struct pic heroShield, enemyShield;
	double x, y;
};