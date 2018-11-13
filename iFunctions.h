#include "stdio.h"
using namespace std;
#define endl "\n"

void load(char *filename, struct pic *aa)
{
	FILE *fp = fopen(filename,"r");
	int xx,yy,temp;
	fscanf(fp,"%d %d",&xx,&yy);
	aa->x = xx, aa->y = yy;
	for(int i = 0; i<xx;i++)
	{
		for(int j = 0; j<yy;j++)
		{
			if(!feof(fp))
			{
				fscanf(fp,"%d",&temp);
				aa->pxl[i][j] = temp;
			}
		}
	}
	fclose(fp);
}

void iShowBMP2(int x, int y, struct pic *a, int ignr0 = -1, int ignr1=-1, int ignr2=-1)
{
	int xx,yy;
	xx = a->x,yy=a->y;
	int arr[4],temp;
	for(int i = 0; i<xx;i++)
	{
		for(int j = 0; j<yy;j++)
		{
			temp = a->pxl[i][j];
			arr[0] = temp / 1000000;
			temp%=1000000;
			arr[1] = temp / 1000;
			temp %= 1000;
			arr[2] = temp;
			if(arr[0]==ignr0 && arr[1]==ignr1 && arr[2]==ignr2) continue;
			iSetColor(arr[0],arr[1],arr[2]);
			iPoint(i+x,j+y);
		}
	}
}

void buildingDraw(double x, double y, double w, double h)
{
	int i, j; double hor=x+w-5, ver=y+h-5;
	iSetColor(192, 192, 192);
	iFilledRectangle(x, y, w, h);
	iSetColor(255, 0, 0);
	for(i=x+5; i<=x+w-7; i+=8)
	{
		for(j=y+5; j<=y+h-12; j+=13)
			iFilledRectangle(i, j, 5, 10);
	}
}

