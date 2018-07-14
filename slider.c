#include "mywin.h"

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
  
  my_draw_separator(x+6,y+obj->u.slider.pos+34,7); 
  my_draw_separator(x+6,y+obj->u.slider.pos+40,7); 
  my_draw_separator(x+6,y+obj->u.slider.pos+46,7); 
 }
else
 {
  my_draw_shadow(x,y,obj->sx,20,3,WIN_SUNKEN);
  my_draw_shadow(x+3,y+3,14,14,2,0);
  my_draw_shadow(x+(obj->sx-18),y+3,14,14,2,0);
  my_draw_shadow(x+17+obj->u.slider.pos,y+3,50,14,2,0); 

  my_draw_separatory(x+obj->u.slider.pos+36,y+6,7);
  my_draw_separatory(x+obj->u.slider.pos+42,y+6,7);
  my_draw_separatory(x+obj->u.slider.pos+48,y+6,7);

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
 min=0;
 max=(min+obj->sx)-(50+17+19);
if((mt=X11_locator(&mx,&my)) == 1)  
{
 if (my_check_pointer(mx,my,x,y,14,14) == 1) /* is left button ? */
   {
    if(obj->u.slider.pos > min)
     obj->u.slider.pos --;
    obj->needupdate=1;
printf("\nmin: %d max:%d pos: %d",min,max,obj->u.slider.pos);
     slide_update(obj);
   }
 else
 if(my_check_pointer(mx,my,(x+obj->sx)-14,y,14,14)==1)
					    /* is right button ? */
  {
    if(obj->u.slider.pos < max)
    obj->u.slider.pos++;
    obj->needupdate=1;
printf("\nmin: %d max:%d pos: %d",min,max,obj->u.slider.pos);
    slide_update(obj);
  }
 else
 if  ( my_check_pointer(mx,my,(x+17+obj->u.slider.pos),y+3,50,14) == 1)
					    /* is pointer ? */
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
  my_draw_separatory(nx+18,y+6,7);
  my_draw_separatory(nx+24,y+6,7);
  my_draw_separatory(nx+30,y+6,7);
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

  my_draw_separator(x+6,nx+18,7); 
  my_draw_separator(x+6,nx+24,7); 
  my_draw_separator(x+6,nx+30,7); 
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
   obj->sx=10;
	}
 else
   {
   obj->sx=size;
   obj->sy=10;
  }

 obj->type=WIN_SLIDE;
 obj->update=slide_update;
 obj->change=slide_change;
 return obj;
} 

