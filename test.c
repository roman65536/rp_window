#include "vogl.h"

#define WIN_TITLE	1
#define WIN_CLOSE	2
#define WIN_SUNKEN	3
#define WIN_BUTTON	4
#define WIN_ICON	8

#define WIN_EXIT	(-1)

struct my_obj {
	int x,y;	/* x,y of the object */
	int sx,sy;	/* size of x,y of the object */
	int type;	/* type of the object */
	void ( * init) ( struct my_obj *);
	void ( * update) (struct my_obj *);
	void ( * destroy) (struct my_obj *);
	struct my_win * win;	/* we are in this window */
	struct my_obj * obj;
	union {
	struct button {
	 int type;		/* do we have pixmap or text */
	 int val;		/* for callback */	
	 char label[256];	/* label for the button */
	 char *data;	 	/* data for pixmap */
	 int state;		/* active / sunken */
	 }; 
	struct slider {
	 int type;		/* vert. or horiz.*/
	 int pos;		/* current position */
	 float max;		/* max value */	
	 };
         } u;
 };
 
	
struct my_win {
	int x;
	int y;
	int sx;
	int sy;
	int val;
	int type;
 	int len;
	int opt;
	char text[80];
	struct my_win *next;
	struct my_obj *obj;
	void (* update)();

	};

struct my_win * first;

struct my_win * my_create_button(int ,int , int , int ,int , int,char * );

my_init()
{
 first = (struct my_win *)0;
 X11_init();
}

struct my_win * my_alloc_win()
{
 struct my_win * tmp;
 
 tmp=(struct my_win *) malloc(sizeof( struct my_win));
 tmp->next=first;
 first = tmp;
 return tmp;
} 

struct my_obj * my_alloc_obj(struct my_win * win)
{

 struct my_obj * obj;
 obj=(struct my_obj *) malloc( sizeof( struct my_obj));
 obj->obj=win->obj;
 win->obj=obj;
return obj;
}
 
 

my_create(int x,int y, int sx,int sy,char *text,int flag)
{

my_draw_shadow( x,y,sx,sy,2,0);


X11_color(0);

if(flag & WIN_TITLE)
{
my_draw_separator(x+2, (y+sy)-20, (x+sx)-4);
 X11_font("small");
 X11_move(x+20,y+sy-15);
 X11_string(text);
 }

if(flag & WIN_CLOSE)
   my_create_button(x+sx-19,y+sy-16,13,12,1,WIN_EXIT,"");
if(flag & WIN_ICON)
   my_create_button(x+sx-33,y+sy-16,13,12,1,WIN_ICON,"");
  
X11_sync();
}

my_draw_text(int x,int y, char *text)
{
 
 X11_move(x,y);
 X11_string(text);
}

my_draw_shadow(int x, int y, int sx, int sy,int len, int flag)
{
 int a,b;
 int idx;

 X11_mapcolor(20,0xe0,0xe0,0xe0);
 X11_mapcolor(21,0xa0,0xa0,0xa0);
 X11_mapcolor(22,0xc0,0xc0,0xc0);
 if(flag==WIN_SUNKEN)
  idx=22;
 else
  idx=21;
 
 X11_color(idx);
 for(a=x;a<=(x+sx);a++)
   {
    X11_move(a,y);
    X11_draw(a,y+sy); 
   }

 for(a=0;a<len;a++)
 {
if(flag == WIN_SUNKEN)
 X11_color(7);
else
 X11_color(0);
 X11_move(x+a,y+a);
 X11_draw(x+sx-a,y+a);
 X11_move(x+sx-a,y+a);
 X11_draw(x+sx-a,y+sy-a);

if(flag == WIN_SUNKEN)
 X11_color(0);
else
 X11_color(7);

 X11_move(x+sx-a,y+sy-a);
 X11_draw(x+a,y+sy-a);
 X11_move(x+a,y+a);
 X11_draw(x+a,y+sy-a);
}
/*
 X11_sync();
*/
} 


my_draw_separator(int x, int y, int len)
{

  
 X11_mapcolor(20,0xe0,0xe0,0xe0);

 X11_color(0);
 X11_move(x,y);
 X11_draw(x+len,y);
 X11_color(20);
 X11_move(x,y+1);
 X11_draw(x+len,y+1);

}

my_check_pointer(int x, int y , int wx,int wy, int wsx, int wsy)
{
 
 if(  (x >= wx ) && ( x <= (wx +wsx)) &&
   ((y >= wy ) && ( y <= (wy +wsy))) )
	return 1;
 else 
    return 0;
}

struct my_win * my_create_button(int x ,int y , int sx, int sy,int len, int val,char *text)
{
 
 struct my_win * tmp = my_alloc_win();
 if (tmp == 0) return 0;

 tmp->x=x;
 tmp->y=y;
 tmp->sx=sx;
 tmp->sy=sy;
 tmp->val=val;
 tmp->len=len;
 tmp->type=WIN_BUTTON;
 tmp->opt=0; 
 my_draw_shadow(x,y,sx,sy,len,0);
 strcpy(tmp->text,text);
 if(strlen(tmp->text) > 0 )
 my_draw_text((x+10),y +(sy/2) - 4,tmp->text);
 return tmp;

}

my_change_property(struct my_win * win, int x,int y, int sx, int sy, int what)
{

 if(what & 1) win->x=x;
 if(what & 2) win->y=y;
 if(what & 4) win->sx=sx;
 if(what & 8) win->sy=sy;

  my_draw_shadow(win->x,win->y,win->sx,win->sy,win->len,win->opt );
}

static int ots ;


my_check_event()
{
 int x,y,ok;
 int ts;
 struct my_win * tmp;

ts=X11_locator(&x,&y);
if(ts == 1)
{
 for(tmp=first; tmp != 0; tmp=tmp->next)
 {

   if(tmp->type != WIN_BUTTON)  
	continue;
   ok = my_check_pointer(x,y,tmp->x,tmp->y,tmp->sx,tmp->sy); 
  
  if((tmp->type == WIN_BUTTON ) && ((ok == 1) && (ots != ts)))
  {
   my_draw_shadow(tmp->x,tmp->y,tmp->sx,tmp->sy,tmp->len,ok == 1? WIN_SUNKEN :0 );
   tmp->opt=(ok ==1 ?WIN_SUNKEN:0); 
 if(strlen(tmp->text) > 0 )
 my_draw_text((tmp->x+10),tmp->y +(tmp->sy/2) - 4 -(ok*2) ,tmp->text);
  }
  if(ok == 1) {
		ots=ts;
		return tmp->val;
		}
 }
}

for(tmp=first; tmp != 0; tmp=tmp->next)
   {
 if((ots != ts )) 
  {
  my_draw_shadow(tmp->x,tmp->y,tmp->sx,tmp->sy,tmp->len,0 );
 if(strlen(tmp->text) > 0 )
 my_draw_text((tmp->x+10),tmp->y +(tmp->sy/2) - 4  ,tmp->text);
  tmp->opt=0;
   }
  } 
ots=ts;
return 0;
}

my_update_else( struct my_win *my)
{
 struct my_win * tmp;
for(tmp=first; tmp != 0; tmp=tmp->next)
   {
 if((tmp != my )) 
  {
  my_draw_shadow(tmp->x,tmp->y,tmp->sx,tmp->sy,tmp->len,0 );
 if(strlen(tmp->text) > 0 )
 my_draw_text((tmp->x+10),tmp->y +(tmp->sy/2) - 4  ,tmp->text);
  tmp->opt=0;
   }
  } 
}

main()
{
int a;
int b;
int x,y;
int ok;
int tmpy;
int tmpy1;

struct my_win *bt1;
struct my_win *bt2;
struct my_win *bt3;
struct my_win *bt4;
struct my_win *tmp;


my_init();
my_create(0,0,400,400,"my win",WIN_TITLE|WIN_CLOSE|WIN_ICON);
my_create_button(10,10, 50,30,3,1,"test1"  );
my_create_button(10,43, 50,30,3,2,"test2"  );
my_create_button(10,76, 50,30,3,3,"test2"  );
my_create_button(10,109, 50,30,3,4,"test4"  );
y=35;
bt1=my_create_button(380,10,12,12,2,5,"");
bt2=my_create_button(380,y,12,80,2,6,"");
bt3=my_create_button(380,350,12,12,2,7,"");
bt4=my_create_button(375,6,20,360,2,8,"");
bt4->type=0;

while(1)
{
ok=my_check_event();
if(ok != 0)
{
printf("\n God some thing %d ",ok);
switch(ok)
 {

 case (-1):
	     exit();
	     break;

 case 6:
	{
	int xo,yo;
	 X11_locator(&x,&y);
	 xo=x;y=y;
	 tmpy=y - bt2->y ;
       while(	 X11_locator(&x,&y) == 1)
	{
	if((xo != x) || (yo != y))
	{
	 printf("\n changing to %d %d %d ",y,tmpy,bt2->y);
	 tmpy1=y-tmpy;
	 if(tmpy1 > (348-bt2->sy)) tmpy1=348-bt2->sy;
	 if(tmpy1 < (24) ) tmpy1=24;
	 my_update_else(bt2);
	 my_change_property(bt2,0, tmpy1,0,0,2);
	 }
	xo=x; yo=y;
	}
	y=tmpy1;
	}
	break;
 case 7: y++;
	 goto here;
 case 5: y--;
	here:
	 tmpy1=y;
	 if(tmpy1 > (348-bt2->sy)) tmpy1=348-bt2->sy;
	 if(tmpy1 < (24) ) tmpy1=24;
	 my_update_else(bt2);
  	my_change_property(bt2,0,tmpy1,0,0,2);
  	y=tmpy1;
  	printf("\nY: %d",y);
	break;
 case 1: 
	{
 	int mx,my;
	char text[80];
	
	    mx=30;
	    my=360;
	     for(tmp=first;tmp != 0; tmp=tmp->next)
		{
		 sprintf(text,"Type: %2x x: %4d y: %4d",tmp->type,tmp->x,tmp->y);
		my_draw_text(mx,my,text);
		my-=10;
		 sprintf(text,"val:  %x",tmp->val);
		my_draw_text(mx,my,text);
		my-=10;
		}
	     }
	 break;
 }

 }
} 
 
}
