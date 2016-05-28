#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>

#define Up 0x4800
#define Down 0x5000
#define Left 0x4b00
#define Right 0x4d00
#define Esc 0x11b
#define Ok 0x1c0d
#define Max 100
#define Space 0x3920
#define W 0x1177
#define S 0x1f73
#define A 0x1e61
#define D 0x2064


int maxx, maxy, num, poph=10, popw=10;
int i, j, k, p, sx, sy, flag;
int dir[5][2]={{0,0},{0,-1},{0,1},{-1,0},{1,0}};

char pop[Max][Max], vis[Max][Max];

struct{
	char name[20];
}menu[6];

struct{
	int  x, y;
}a[Max*10];

//  Initialize the graphics.
void inigraph(){
	int gdriver=DETECT,gmode;
	char path[]="C:\\TC3\\BGI";
	initgraph(&gdriver, &gmode, path);
	if (graphresult()!=grOk){
		printf("Can not open the graphics, Please try again!\n");
		exit(1);
	}
	return ;
}

// Initialize the menu.
void inimenu(){
	strcpy(menu[1].name,"NEW GAME");
	strcpy(menu[2].name,"GAME RANK");
	strcpy(menu[3].name,"GAME SETING");
	strcpy(menu[4].name,"INSTRUCTION");
	strcpy(menu[5].name,"QUIT");
	return ;	
}

//  Show the menu.
void showmenu(int count){
	cleardevice();
	setbkcolor(BLACK);
	setcolor(RED);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,8);
	outtextxy(maxx/2+maxx/10, maxy/3+maxy/10, "POP");
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	for (i=1; i<=5; ++i){
		if (i==count) setcolor(12);
		else setcolor(7);
		outtextxy(maxx/6,maxy/4+maxy/10*i,menu[i].name);
	}
	return ;	
}

//   To get the key that you have press.
int Getkey(){
	int key;
	while (!bioskey(1));
	key=bioskey(0);
	if (key==Esc) return 0;
	else if (key==Up||key==W) return 1;
	else if (key==Down||key==S) return 2;
	else if (key==Left||key==A) return 3;
	else if (key==Right||key==D) return 4;
	else if (key==Ok||key==Space) return 5;
	else return -1;
}

//  To initailize the pop.
void inipop(){
	int maxcolor, x, y,tmp,temp;
	int b[20];
	memset(pop,0,sizeof(pop));
	memset(b,0,sizeof (b));
	tmp=0;
	randomize();
	maxcolor=getmaxcolor()-1;
	temp=num/maxcolor;
	for (i=1; i<=maxcolor; ++i){
		if (temp&&temp%2){
			tmp++;
			b[i]+=temp-1;
			
		}
                else b[i]+=temp;
		if (tmp==2){
			b[rand()%maxcolor+1]+=tmp;
			tmp=0;
		}
	}
	if (tmp) b[rand()%maxcolor+1]+=tmp;
	b[rand()%maxcolor+1]+=num%maxcolor;
	tmp=0;
	tmp=num/maxcolor-2;
	while (tmp>=0&&tmp--){
		for (i=1; i<=maxcolor; ++i){
			x=rand()%popw;
			y=rand()%poph;
			while (pop[y][x]){
				x=rand()%popw;
				y=rand()%poph;
			}
			pop[y][x]=i;
			b[i]--; 
		}
	}
	for (i=0; i<poph; ++i){
		for (j=0; j<popw; ++j){
			if (pop[i][j]) continue ;
			for (k=1; k<=maxcolor; ++k){
				if (b[k]<=0) continue ;
				pop[i][j]=k;
				b[k]--;
				break;
			}
		}
	}
	return ;
}

//  To show the pop.
void showpop(){
	cleardevice();
	for (i=0; i<poph; ++i){
		for (j=0; j<popw; ++j){
			setfillstyle(SOLID_FILL,pop[i][j]);
			bar(maxx/popw+j*20+5,maxy/poph+i*20+5,maxx/popw+(j+1)*20-5,maxy/poph+(i+1)*20-5);
		}
	}
	return ;
}
//  To draw a bar.
void showbar(int x, int y){
	setfillstyle(SOLID_FILL,WHITE);	
	bar(maxx/popw+x*20+2,maxy/poph+y*20+2, maxx/popw+(x+1)*20-2, maxy/poph+(y+1)*20-2);
	setfillstyle(SOLID_FILL,pop[y][x]);
	bar(maxx/popw+x*20+5,maxy/poph+y*20+5, maxx/popw+(x+1)*20-5, maxy/poph+(y+1)*20-5);
}

//  To change the bar's place.
void changebar(int x, int y){
	setfillstyle(SOLID_FILL,BLACK);
	bar(maxx/popw+x*20+2, maxy/poph+y*20+2, maxx/popw+(x+1)*20-2, maxy/poph+(y+1)*20-2);
	setfillstyle(SOLID_FILL,pop[y][x]);
	bar(maxx/popw+x*20+5, maxy/poph+y*20+5, maxx/popw+(x+1)*20-5, maxy/poph+(y+1)*20-5);								       
}

//  To comfir clearpop  is ok.
int comfirok(int x, int y){
	if (pop[y][x]!=pop[sy][sx]||(sx==x&&sy==y)) return 0;
	if ((sx==0||sy==0||sx==popw-1||sy==poph-1)&&(x==0||y==0||x==popw-1||y==poph-1)) return 1;
	memset(vis,0,sizeof (vis));
	int top=0;
	a[top].x=sx;
	a[top++].y=sy;
	for (i=0; i<top; ++i){
		if (top>=1000) return 0;
		for (j=1; j<5; ++j){
			a[top].x=a[i].x+dir[j][0];
			a[top].y=a[i].y+dir[j][1];
			if (a[top].x<0||a[top].x>=popw||a[top].y<0||a[top].y>=poph) continue ;
			if (a[top].x==x&&a[top].y==y) return 1;
			if (pop[a[top].y][a[top].x]!=BLACK) continue ;
			if (vis[a[top].y][a[top].x]) continue ;
			vis[a[top].y][a[top].x]=1;
			top++;
		}
	}
	return 0;
}


// To clear pop that comfired.
void clearpop(int x, int y){
	pop[y][x]=BLACK;
}

//  To comfir the tagert.
void comfir(int x, int y){
	if (pop[y][x]==BLACK) return ;
	if (!flag){
		sx=x;
		sy=y;
		flag=1;
	}
	else{
		flag=0;
		if (comfirok(x, y)){
			clearpop(x, y);
			x=sx;
			y=sy;
			clearpop(x, y);
			changebar(x, y);
			sx=-1;
			sy=-1;
			num-=2;
		}
		else{
			x=sx;
			y=sy;
			changebar(x, y);
			sx=-1;
			sy=-1;
			flag=0;
		}
	}
	return ;		
}

//  To show who  creat the game.
void showauthor(){
	cleardevice();
	settextstyle(TRIPLEX_FONT, HORIZ_DIR,3);
	setcolor(RED);
	outtextxy(maxx/4, maxy/3, "AUTHOR: SUNXIAOCONG");
	delay(2000);
}

//  To show you have won the game.
void wingame(){
	cleardevice();
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
	setcolor(RED);
	outtextxy(maxx/4, maxy/3, "YOU HAVE WON THE GAME");
	delay(1000);
	return ;
}

//  To show you have lose the game.
void losegame(){
	cleardevice();
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
	setcolor(RED);
	outtextxy(maxx/4, maxy/3, "YOU HAVE LOSE THE GAME");
	delay(1000);
	return ;
}

//  To begain a new game.
int newgame(){
	int key=1, x=0, y=0;
	sx=-1;
	sy=-1;
	num=poph*popw;
	flag=0;
	showauthor();
	inipop();
	showpop();
	showbar(x,y);
	while (key){
		key=Getkey();
		if (key==5) comfir(x, y);
		else if (key>0&&key<5){
			if (!(x==sx&&y==sy)) changebar(x, y);
			x+=dir[key][0];
			y+=dir[key][1];
			if (x<0) x=popw-1;
			if (y<0) y=poph-1;
			if (x>=popw) x=0;
			if (y>=poph) y=0;
			showbar(x,y);
		}
		if (!num){
			wingame();
			return 1;
		} 
	}
	losegame();
	return 0;
}

//  To  seting the game.
void gameseting(){
	cleardevice();
	printf("Please input the poph&popw:\n");
	scanf("%d %d", &poph, &popw);
	getchar();
	return ;
}

//   Instruction of the game.
void instruction(){
	cleardevice();
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
	setcolor(RED);
	outtextxy(maxx/4, maxy/3, "HAVE NO MORE INSTRUCTION");
	delay(1000);
}

//  To show the rank list of  the game.
void gamerank(){
	cleardevice();
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
	setcolor(RED);
	outtextxy(maxx/6, maxy/3, "THIS FUNCTION CAN NOT BE USE");
	delay(1000);
}

//  To do the comand.
void domenu(int count){
       if (count==1) while (newgame()) ;
       else if (count==2) gamerank();
       else if (count==3) gameseting();
       else if (count==4) instruction();
       else exit(1);
       showmenu(count);
	return ;
}

//   To change the key.
int changekey(int count){
	int key;
	key=Getkey();
	if (!key) exit(1);
	else if(key==1) return -1;
	else if(key==2) return  1;
	else if(key==5) domenu(count);
	else return 0;
	return 0;
}
void changemenu(int count, int key){
	setcolor(12);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(maxx/6, maxy/4+maxy/10*key,menu[key].name);
	setcolor(7);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(maxx/6, maxy/4+maxy/10*count, menu[count].name);
	return ;
}
int main(){
	int count=1, key=1;
	inigraph();
	maxx=getmaxx();
	maxy=getmaxy();
	inimenu();
	showmenu(count);
	while (key){
		key+=changekey(count);
		if (key<1) key=5;
		if (key>5) key=1;
		if (count!=key){
			changemenu(count, key);
			count=key;
		}
	}
	return 0;           	
}