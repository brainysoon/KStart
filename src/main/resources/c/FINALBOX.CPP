#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <bios.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <dos.h>

#define ESC 0x11b
#define Up 0x4800
#define Down 0x5000
#define Left 0x4b00
#define Right 0x4d00
#define Ok 0x1c0d

#define Max 1000
#define Max_map 10

char Name[]="PUSH BOX";
int bkcolor=BLACK;
int step;
char sp[20], rp[100];

struct {
	int x, y, size;
	char name[20];
}menu[5];
typedef struct{
	int x, y, px, py, s;
}Node;
typedef struct{
	int x,y;
}node;
Node a[Max];
node b[Max];
int dir[5][2]={{0,0},{0,-1},{-1,0},{0,1},{1,0}};

int vis[Max_map][Max_map][Max_map][Max_map],map[Max_map][Max_map],visb[Max_map][Max_map];
int maph=5,mapw=5,mapup=5;

int inigraph(){
	int gdriver=DETECT,gmode,error;
	char path[]="C:\\TC3\\BGI";
	initgraph(&gdriver,&gmode,path);
	error=graphresult();
	if (error!=grOk)
	return 1;
	return 0;
}
void inimenu(){
	int Maxx=getmaxx();
	int Maxy=getmaxy();
	menu[0].x=Maxx/6;
	menu[0].y=Maxy/4;
	menu[0].size=3;
	strcpy(menu[0].name,"MENU");
	for (int i=1; i<5; ++i){
		menu[i].x=Maxx/8;
		menu[i].y=Maxy/4+i*60;
		menu[i].size=2;
	}
	strcpy(menu[1].name,"NEW GAME");
	strcpy(menu[3].name,"GAME SETING");
	strcpy(menu[2].name,"GAME RANK");
	strcpy(menu[4].name,"QUIT");
}
void changemenu(int count,int tmp){
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,menu[count].size);
	setcolor(12);
	outtextxy(menu[count].x,menu[count].y,menu[count].name);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,menu[tmp].size);
	setcolor(7);
	outtextxy(menu[tmp].x,menu[tmp].y,menu[tmp].name);	
}
void showmenu(int count){
	cleardevice();
	int Maxx=getmaxx();
	int Maxy=getmaxy();
	setbkcolor(bkcolor);
	setcolor(RED);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,5);
	outtextxy(Maxx/2,Maxy/2,Name);

	for (int i=0; i<5; ++i){
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,menu[i].size);
		if (!i) setcolor(BLUE);
		else if (i==count) setcolor(12);
		else setcolor(7);
		outtextxy(menu[i].x,menu[i].y,menu[i].name);
	}
}
int Getkey(){
		while (!bioskey(1));
		int key = bioskey(0);
		if (key==Up) return 1;
		if (key==Down) return 3;
		if (key==Left) return 2;
		if (key==Right) return 4;
		if (key==ESC) return 0;
		if (key==Ok) return 5;
		return -1;
}
void inigame(){
	setbkcolor(bkcolor);
	cleardevice();
	int maxx=getmaxx();
	int maxy=getmaxy();
	setfillstyle(1,7);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
	setcolor(BLUE);
	outtextxy(maxx/4+maxx/6,maxy/4-40,"INI GAME");
	bar(maxx/4,maxy/4,maxx/4*3,maxy/4*3);
	setcolor(RED);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
	outtextxy(maxx/4+maxx/10,maxy/4+maxy/7,"NAME: SUNXIAOCONG");
	int ke=10;
	setcolor(RED);
	outtextxy(maxx/4+maxx/10,maxy/5*4,"Laoding . . .");
	while (ke--){
		setcolor(WHITE);
		outtextxy(maxx/4+maxx/12+maxx/5,maxy/5*4,". . .");
			delay(50);
		setcolor(RED);
		outtextxy(maxx/4+maxx/12+maxx/5,maxy/5*4,". . .");
			delay(50);
	}
}
void inimap(){
	memset(vis,0,sizeof (vis));
	memset(map,0,sizeof (map));
	randomize();
	int x, y,k=0;
	for (int i=0; i<mapup+k; ++i){
		x=rand()%mapw;
		y=rand()%maph;
		if (map[y][x]){
			k++;
			continue ;
		}
		map[y][x]=1;
	}
	while (map[y][x]){
		x=rand()%mapw;
		y=rand()%maph;
	}
	map[y][x]=2;
	while (map[y][x]){
		x=rand()%mapw;
		y=rand()%maph;
	}
	map[y][x]=3;
	while (map[y][x]){
		x=rand()%mapw;
		y=rand()%maph;
	}
	map[y][x]=4;
}
int checkok(){
	int sx, sy, pex, pey, i, j;
	for (i=0; i<maph; ++i){
		for (j=0; j<mapw; ++j){
			if (map[i][j]==2){
				pex=j;
				pey=i;
			}
			else if (map[i][j]==3){
				sx=j;
				sy=i;
			}	
		}
	}
	int top=0,tp,k,t,flag,p;
	a[top].x=sx;
	a[top].y=sy;
	a[top].px=pex;
	a[top].py=pey;
	a[top++].s=0;
	for (i=0; i<top; ++i){
		if (top>=Max) return 0;
		for (j=1; j<5; ++j){
			a[top].x=a[i].x+dir[j][0];
			a[top].y=a[i].y+dir[j][1];
			if (a[top].x<0||a[top].y<0||a[top].x>=mapw||a[top].y>=maph) continue ;
			if (map[a[top].y][a[top].x]==1) continue ;
			if (vis[a[top].y][a[top].x][a[i].y][a[i].y]) continue ;
			t=j+2;
			if (t>4) t=t%5+1;
			tp=0;
			b[tp].x=a[i].x+dir[t][0];
			b[tp].y=a[i].y+dir[t][1];
			if (b[tp].x<0||b[tp].y<0||b[tp].x>=mapw||b[tp].y>=maph) continue ;
			if (map[b[tp].y][b[tp].x]==1) continue ;
			tp++;
			flag=0;
			memset(visb,0,sizeof (visb));
			for (k=0; k<tp; ++k){
				for (p=1; p<5; ++p){
					b[tp].x=b[k].x+dir[p][0];
					b[tp].y=b[k].y+dir[p][1];
					if (b[tp].x<0||b[tp].y<0||b[tp].x>=mapw||b[tp].y>=maph) continue ;
					if (map[b[tp].y][b[tp].x]==1) continue ;
					if (visb[b[tp].y][b[tp].x]) continue ;
					if (a[i].x==b[tp].x&&a[i].y==b[tp].y) continue ;
					if (b[tp].y==a[i].py&&b[tp].x==a[i].px){
						flag=1;
						break;
					}
					visb[b[tp].y][b[tp].x]=1;
					tp++;
				}
				if (flag) break;	
			}
			if (!flag) continue ;
			if (map[a[top].y][a[top].x]==4) return a[i].s+1;
			a[top].px=b[0].x;
			a[top].py=b[0].y;
			a[top].s=a[i].s+1;
			top++;
		}
	}
	return 0;
}
void drawbox(int i,int j){
	int ke=40, kel=20; 
	setfillstyle(EMPTY_FILL,WHITE);
	setcolor(WHITE);
	int maxx=getmaxx()/maph;
	int maxy=getmaxy()/mapw;
	bar3d(maxx+j*ke+kel, maxy+i*ke+kel, maxx+(j+1)*ke-kel, maxy+(i+1)*ke-kel, 4,4);
}
void showpeo(int i, int j){
	int ke=40, kel=1, maxx, maxy;
	maxx=getmaxx()/maph;
	maxy=getmaxy()/mapw;
	setfillstyle(1,6);
	bar(maxx+j*ke,maxy+i*ke,maxx+(j+1)*ke,maxy+(i+1)*ke);
	if (map[i][j]==2) setfillstyle(1,WHITE);
	else if (map[i][j]==3) setfillstyle(1,RED);			//drawbox(i,j);
	else if (map[i][j]==4) setfillstyle(1,BLUE);
	else if (map[i][j]==1) setfillstyle(5,8);
	else setfillstyle(10,6);
	bar(maxx+j*ke+kel,maxy+i*ke+kel,maxx+(j+1)*ke-kel,maxy+(i+1)*ke-kel);
}
void showmap(){
	int i, j;
	int maxx=getmaxx()/mapw;
	int maxy=getmaxy()/maph;
	cleardevice();
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(maxx/3, maxy/3, sp);
	for (i=0; i<maph; ++i){
		for (j=0; j<mapw; ++j){
			showpeo(i, j);
		}
	}
	return ;
}
void showstep(int s){
	int maxx=getmaxx()/mapw/3;
	int maxy=getmaxy()/maph/3+100;
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	sprintf(sp,"%d\n",s);
	setfillstyle(1,bkcolor);
	bar(maxx,maxy,maxx+20,maxy+20);
	outtextxy(maxx, maxy, sp);	
}
int newgame(){

	int k=1, sx, sy, i, j, nx, ny, px, py;
	int   temp=0,s=0;
	step=0;
	inigame();
	while (!step){
		inimap();
		step=checkok();
	}
	sprintf(sp,"%d\n",step);
	showmap();
	for (i=0; i<maph; ++i){
		for (j=0; j<maph; ++j){
			if (map[i][j]==2){
			sx=j;
			sy=i;
			break;
			}
		}
		if (j<maph) break ;
	}
	while (k){
		k=Getkey();
		nx=sx+dir[k][0];
		ny=sy+dir[k][1];
		if (nx<0||ny<0||nx>=mapw||ny>=maph) continue ;
		if (map[ny][nx]==1) continue ;

		if (map[ny][nx]==3){
			px=nx+dir[k][0];
			py=ny+dir[k][1];
			if (py<0||px<0||py>=maph||px>=mapw) continue ;
			if (map[py][px]==1) continue ;
			if (map[py][px]==4) return s+1;
			map[py][px]=3;
			showpeo(py,px);
			s++;
			showstep(s);
		}
		if (temp) map[sy][sx]=4;
		else map[sy][sx]=0;
		if (map[ny][nx]==4) temp=4;
		else temp=0;
		map[ny][nx]=2;
		showpeo(sy,sx);
		showpeo(ny,nx);
		sx=nx;
		sy=ny;
		if (!temp&&!checkok()){
			delay(1000);
			return 0;
		}
	}
	return 0;
}
void wingame(int s){
	cleardevice();
	int maxx, maxy;
	maxx=getmaxx()/4;
	maxy=getmaxy()/2;
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(maxx,maxy, "YOU HAVE WIN THE GAME");
	delay(1000);
}
void losegame(){
	cleardevice();
	int maxx, maxy;
	maxx=getmaxx()/4;
	maxy=getmaxy()/2;
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(maxx,maxy, "YOU HAVE LOSE THE GAME");
}
void setgame(){
	cleardevice();
	printf("please:");
	scanf("%d %d %d", &maph, &mapw, &mapup);
	return ;
}
void rank(){
	cleardevice();
	outtextxy(100,100,"NO SIGN");
}
void domenu(int count){
	int key=1;
	if (count==4) exit(1);
	else if (count==1){
		while (key){
			key=newgame();
			if (key) wingame(key);
			else losegame();
		}
	}
	else if (count==2){
		rank();
	}
	else if (count==3){
		setgame();
	}
	delay(1000);
	showmenu(count);
	return ;
}    
int main(){
	if (inigraph()){
	printf("Graphics Can Not Be Open, Please Try Again!\n");
	exit(1);
	}
	inimenu();
	int keybord=1, count=1, tmp;
	showmenu(count);
	while (keybord){
		keybord=Getkey();
		tmp=count;
		if (keybord==5) domenu(count);
		if (keybord==1) count--;
		else if (keybord==3) count++;
		if (count<1) count=4; 
		else if (count>4) count=1;
		if (count==tmp) continue ;
		else changemenu(count,tmp);
	}
	return 0;
}
