#include "vogl.h"
#include "mywin.h"


struct my_win * first;
struct my_win * focus;

void my_update_win(struct my_win *);


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
 }
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
  idx=21;
 else
  idx=22;
 
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

my_draw_separatory(int x, int y, int len)
{

  
 X11_mapcolor(20,0xe0,0xe0,0xe0);

 X11_color(0);
 X11_move(x,y);
 X11_draw(x,y+len);
 X11_color(20);
 X11_move(x+1,y);
 X11_draw(x+1,y+len);
 X11_move(x+2,y);
 X11_draw(x+2,y+len);

}

my_check_pointer(int x, int y , int wx,int wy, int wsx, int wsy)
{
 
 if(  (x >= wx ) && ( x <= (wx +wsx)) &&
   ((y >= wy ) && ( y <= (wy +wsy))) )
	return 1;
 else 
    return 0;
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


