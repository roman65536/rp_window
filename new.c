#include "vogl.h"

#define WIN_TITLE	1
#define WIN_CLOSE	2
#define WIN_SUNKEN	3
#define WIN_BUTTON	4
#define WIN_SLIDE	5
#define WIN_ICON	8
#define WIN_FOCUS	1
#define WIN_LOOSF	2

#define VERT 2

#define WIN_EXIT	(-1)

struct my_obj {
	int x,y;	/* x,y of the object */
	int sx,sy;	/* size of x,y of the object */
	int type;	/* type of the object */
	int needupdate; /* need update on object */
	void ( * init) ( struct my_obj *);
	void ( * update) (struct my_obj *);
	void ( * change) (struct my_obj *,int );
	void ( * destroy) (struct my_obj *);
	void ( * inobj) (struct my_obj *, int );
	struct my_win * win;	/* we are in this window */
	struct my_obj * next;

	union {

	struct button {
	 int type;		/* do we have pixmap or text */
	 int val;		/* for callback */	
	 int size;		/* size of border */
	 char label[256];	/* label for the button */
	 char *px_data;	 	/* data for pixmap */
	 int state;		/* active / sunken */
	 int data;		/* data for button */
	 } button; 

	struct slider {
	 int type;		/* vert. or horiz.*/
	 int pos;		/* current position */
	 float max;		/* max value */	
	 float cur;		/* current value */
	 } slider;

         } u;
 };
 
	
struct my_win {
	int x;
	int y;
	int sx;
	int sy;
	int focus;
	int type;
 	int len;
	int opt;
        int flag;
	char text[80];
	struct my_win *next;
	struct my_obj *obj;
	struct my_obj *objfocus;
	void (* update)( struct my_win *);

	};

struct my_win * first;
struct my_win * focus;

struct my_obj * my_create_button(struct my_win *,int ,int , int , int ,int , int ,char *);
void my_update_win(struct my_win *);
void draw_button(struct my_obj * );
void change_button( struct my_obj *,int  );

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
 obj->next=win->obj;
 obj->win=win;
 win->obj=obj;
return obj;
}
 

struct my_win *my_create_win(int x,int y, int sx, int sy, int flag, char *name)
{
 struct my_win *tmp;
 tmp=my_alloc_win();
 tmp->x=x;  tmp->y=y;
 tmp->sx=sx; tmp->sy=sy;
 tmp->flag=flag;
 strcpy(tmp->text,name);
 tmp->update=my_update_win;
if(flag & WIN_CLOSE)
   my_create_button(tmp,x+sx-19,y+sy-16,13,12,1,WIN_EXIT,"");
if(flag & WIN_ICON)
   my_create_button(tmp,x+sx-33,y+sy-16,13,12,1,WIN_ICON,"");
 return tmp;
} 
 
update_all(struct my_win * win)
{
 (win->update)(win);
 update_obj(win);
}

update_obj(struct my_win * win)
{
 struct my_obj *obj;
 obj = win->obj;
 if(obj == 0) return;
 while( obj != 0 )
 {
  (obj->update)(obj);
   obj=obj->next;
 }
 X11_sync();
}


change_obj(struct my_win * win)
{
 struct my_obj *obj;
 obj = win->obj;
 if(obj == 0) return;
 while( obj != 0 )
 {
  (obj->change)(obj,WIN_LOOSF);
   obj=obj->next;
 }
 X11_sync();
}

void my_update_win(struct my_win *win)
{
int x,y,sx,sy;
int a;
x=win->x;
y=win->y;
sx=win->sx;
sy=win->sy;

my_draw_shadow(x,y,sx,sy,2,0);


if(win == focus)
   X11_color(4);
 for(a=0;a<16;a++)
  {
   X11_move(x+4,y+sy-4-a);
   X11_draw(x+sx-4,y+sy-4-a);
  }
 
X11_color(0);

if(win->flag & WIN_TITLE)
{
my_draw_separator(x+2, (y+sy)-20, (x+sx)-4);
 X11_font("small");
 X11_move(x+20,y+sy-15);
 X11_string(win->text);
my_draw_shadow(x+5,y+5,sx-10,sy-30,2,WIN_SUNKEN);
 }
else  
my_draw_shadow(x+5,y+5,sx-10,sy-10,2,WIN_SUNKEN);
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

struct my_obj * my_create_button(struct my_win *win,int x ,int y , int sx, int sy,int len, int val,char *text)
{
 
 struct my_obj * tmp ;

 tmp=my_alloc_obj(win);
 if (tmp == 0) return 0;

 tmp->x=x;
 tmp->y=y;
 tmp->sx=sx;
 tmp->sy=sy;
 tmp->type=WIN_BUTTON;
 tmp->u.button.val=val;
 tmp->u.button.size=len;
 tmp->u.button.state=0;
 tmp->u.button.data=0;
 strcpy(tmp->u.button.label,text);
 tmp->update=draw_button;
 tmp->change=change_button;
 return tmp;
}

void change_button( struct my_obj * obj, int reason)
{
 if(reason == WIN_FOCUS ) 
  {
	obj->u.button.state=WIN_SUNKEN; 
	obj->u.button.data=1;
   }
 else 
  {
	obj->u.button.state=0;
	obj->u.button.data=0;
  }

 (obj->update)(obj);
}


void draw_button(struct my_obj * obj)
{
int x,y;
int sx,sy;
x=obj->x + obj->win->x;
y=obj->y + obj->win->y;
sx=obj->sx;
sy=obj->sy;

 my_draw_shadow(x,y,sx,sy,obj->u.button.size,obj->u.button.state);
 if(strlen(obj->u.button.label) > 0 )
 if(obj->u.button.state == WIN_SUNKEN)
 my_draw_text((x+10),y +(sy/2) - 6,obj->u.button.label);
else
 my_draw_text((x+10),y +(sy/2) - 4,obj->u.button.label);
}

slide_update(struct my_obj * obj)
{
 int x,y,sx,sy;
 int mx,my,mt;
 int tmp;
 int min,max;
 int nx,ny;

 x=obj->x + obj->win->x;
 y=obj->y + obj->win->y;
if(obj->u.slider.type == VERT)
 {
  my_draw_shadow(x,y,20,obj->sy,3,WIN_SUNKEN);
  my_draw_shadow(x+3,y+3,14,14,2,0);
  my_draw_shadow(x+3,y+(obj->sy-18),14,14,2,0);
  my_draw_shadow(x+3,y+17+obj->u.slider.pos,14,50,2,0); 
  
 }
else
 {
  my_draw_shadow(x,y,obj->sx,20,3,WIN_SUNKEN);
  my_draw_shadow(x+3,y+3,14,14,2,0);
  my_draw_shadow(x+(obj->sx-18),y+3,14,14,2,0);
  my_draw_shadow(x+17+obj->u.slider.pos,y+3,50,14,2,0); 
 }
}

slide_change(struct my_obj * obj,int flag)
{
int x,y,mx,my,mt;
int max,min;
int esz;
int tmp;
int nx;


 x=obj->x + obj->win->x;
 y=obj->y + obj->win->y;

if(obj->u.slider.type == 0)
{
if((mt=X11_locator(&mx,&my)) == 1)  
{
 if (my_check_pointer(mx,my,x,y,14,14) == 1)
   {
     obj->u.slider.pos --;
    obj->needupdate=1;
     slide_update(obj);
   }
 else
 if(my_check_pointer(mx,my,(x+obj->sx)-14,y,14,14)==1)
  {
    obj->u.slider.pos++;
    obj->needupdate=1;
    slide_update(obj);
  }
 else
 if  ( my_check_pointer(mx,my,(x+17+obj->u.slider.pos),y+3,50,14) == 1)
  {
    printf("\n in the slider x: %d y:%d xx:%d yy:%d",mx,my,x,y);
    tmp=mx- (x+17+obj->u.slider.pos);
    min= x+17;
    max=(x+obj->sx)-(50+19);
    esz=max-min;
    printf("\n in the slider max: %d min:%d esz:%d ",max,min,esz);
  while( X11_locator_change(&mx, &my) == 1)
   {
    nx=mx-tmp;
    if(nx > max) nx=max;
    if(nx < min) nx=min; 
  my_draw_shadow(nx,y+3,50,14,2,WIN_SUNKEN); 
   }
  obj->u.slider.pos=nx-x-17;
 slide_update(obj);
  }
   printf("\n Position: %f",(float)((nx-min)*obj->u.slider.max/(max-min)) );
  printf("\nmouse: %x",mt);
 }
}
else
 {
if((mt=X11_locator(&mx,&my)) == 1)  
 if  ( my_check_pointer(mx,my,x+3,(y+17+obj->u.slider.pos),14,50) == 1)
  {
    printf("\n in the slider x: %d y:%d xx:%d yy:%d",mx,my,x,y);
    tmp=my- (y+17+obj->u.slider.pos);
    min= y+17;
    max=(y+obj->sy)-(50+19);
    printf("\n in the slider max: %d min:%d ",max,min);
  while( X11_locator_change(&mx, &my) == 1)
   {
    nx=my-tmp;
    if(nx > max) nx=max;
    if(nx < min) nx=min; 
  my_draw_shadow(x+3,nx,14,50,2,WIN_SUNKEN); 
   }
  obj->u.slider.pos=nx-y-17;
 slide_update(obj);
   printf("\n Position: %f",(float)((nx-min)*obj->u.slider.max/(max-min)) );
   printf("\n Position: %d",(max-min) );
   printf("\n Position: %d",(nx-min)  );
  }
} 
}

struct my_obj * create_slide(struct my_win *win, int x, int y,int size,int orient)
{
 struct my_obj *obj;

 obj=my_alloc_obj(win);
 if(obj==0) return 0;
 
 obj->x=x;
 obj->y=y;
 obj->sx=obj->sy=0;
 obj->u.slider.type=orient;
 obj->u.slider.max=1000.0;
 obj->u.slider.cur=0.0;
 obj->needupdate=0;

 if(obj->u.slider.type == VERT )
   {
   obj->sy=size;
   obj->sx=20;
	}
 else
   {
   obj->sx=size;
   obj->sy=20;
  }

 obj->type=WIN_SLIDE;
 obj->update=slide_update;
 obj->change=slide_change;
 return obj;
} 

static need_refresh;

struct my_obj * find_obj(int x, int y)
{
struct my_win * tmp;
struct my_obj * tmpo;
struct my_obj * fc;

need_refresh=0;
tmp=first;
fc=0;

while ( tmp != 0)
{
  if(tmp->objfocus != 0) {
			fc = tmp->objfocus;
			tmp->objfocus=0;
			}
  if( my_check_pointer( x,y,tmp->x,tmp->y,tmp->sx,tmp->sy) )
  {
   tmpo=tmp->obj;
   if(tmpo == 0){
		tmp->objfocus=0;
		if(fc ) (fc->change)(fc,WIN_LOOSF);
		 return 0; 
		}
    while( tmpo != 0)
	{
      if(my_check_pointer(x,y,tmpo->x,tmpo->y,tmpo->sx,tmpo->sy))
	{
	tmp->objfocus=tmpo;
	if((tmp->objfocus != fc) && (fc != 0)) 
		(fc->change)(fc,WIN_LOOSF);
	return tmpo;
	}
      tmpo=tmpo->next;
    }
    if(tmpo == 0) {
		tmp->objfocus=0;
		if(fc ) (fc->change)(fc,WIN_LOOSF);
		return 0;
		}
  }
 tmp=tmp->next;
}
	if(fc != 0 ) (fc->change)(fc,WIN_LOOSF);
return 0;
}






main()
{
int a;
int b;
int x,y;
int xo,yo;
int ok,oko;
int tmpy;
int tmpy1;
int need;

struct my_win *bt;
struct my_obj *obj;
struct my_obj *obj1;


my_init();
bt=my_create_win(0,0,400,400,WIN_TITLE|WIN_CLOSE|WIN_ICON,"my Win");
my_create_button(bt,10,10, 50,30,3,1,"test1"  );
my_create_button(bt,10,43, 50,30,3,2,"test2"  );
my_create_button(bt,10,76, 50,30,3,3,"test3"  );
obj=my_create_button(bt,10,109, 50,30,3,4,"test4"  );
create_slide(bt,375,30,300,VERT);
create_slide(bt,70,10,300,0);

focus=bt;
obj1=0;

oko=X11_locator(&xo,&yo);

update_all(bt);
while(1 )
{
here:
ok=X11_locator(&x,&y);

if((ok == oko) && (x == xo) && (y == yo) &&(need==0) ) goto here;

 {
 obj=obj1;
obj1=find_obj(x,y);
if(obj1 )
{

 if((obj1->type == WIN_BUTTON) && (ok == 1))
      {
      if(obj1 != 0)
	  {
	    (obj1->change)(obj1,WIN_FOCUS);
	    need=1;
	   }
	goto here2; 
       }

 if( ( obj1->type == WIN_SLIDE))
	{

if(obj1 != 0)
   {
    (obj1->change)(obj1,WIN_FOCUS); 
if(obj)
    need=obj->needupdate;
	}

	}
}
 printf("\n %x %x ",obj1,bt->objfocus);
  }
here2:
oko=ok;
xo=x; yo=y;
 }

sleep(60);
 
}
