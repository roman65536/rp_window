#include "mywin.h"

struct my_obj * my_create_button(struct my_win *,int ,int , int , int ,int , int ,char *);
struct my_obj * my_alloc_obj(struct my_win *);
void my_update_win(struct my_win *);
void draw_button(struct my_obj * );
void change_button( struct my_obj *,int  );


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
 tmp->u.button.locst=0;
 strcpy(tmp->u.button.label,text);
 tmp->update=draw_button;
 tmp->change=change_button;
 return tmp;
}

void change_button( struct my_obj * obj, int reason)
{
 int old;
 old=obj->u.button.state;
 if(reason == WIN_FOCUS ) 
  {
	obj->u.button.state=WIN_SUNKEN; 
     if(old != WIN_SUNKEN )
 	(obj->update)(obj);
	obj->u.button.locst=1;
   }
 else 
  {
	obj->u.button.state=0;
	obj->u.button.locst=0;
     if(old != 0 )
 	(obj->update)(obj);
  }

}


void draw_button(struct my_obj * obj)
{
int x,y;
int sx,sy;

if(obj->u.button.locst) return;
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

