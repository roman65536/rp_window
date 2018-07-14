#include "mywin.h"






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
bt=my_create_win(0,0,400,200,WIN_TITLE|WIN_CLOSE|WIN_ICON,"my Win");
my_create_button(bt,10,10, 50,20,2,1,"test1"  );
my_create_button(bt,10,43, 50,20,3,2,"test2"  );
my_create_button(bt,10,76, 50,20,3,3,"test3"  );
obj=my_create_button(bt,10,109, 50,30,3,4,"test4"  );
create_slide(bt,275,30,100,VERT);
create_slide(bt,70,10,200,0);
focus=bt;
obj1=0;

oko=X11_locator(&xo,&yo) & 1;

update_all(bt);
my_draw_separatory(80,50,80);
my_draw_separator(80,50,80);
my_draw_separatory(160,50,80);
my_draw_separator(120,130,40);
while(1 )
{
here:
ok=X11_locator(&x,&y) & 1;

if((ok == oko) && (x == xo) && (y == yo) &&(need==0) ) goto here;

 {
 obj=obj1;
obj1=find_obj(x,y);
printf("%s: %x \n",__FUNCTION__,obj1);
if(obj1 )
{

 if((obj1->type == WIN_BUTTON) )
      {
      if(obj1 != 0)
	  {
	  if(ok==1)
	   {
	    (obj1->change)(obj1,WIN_FOCUS);
	    need=1;
	    }
	   else {
		(obj1->change)(obj1,WIN_LOOSF);
		need=0;
		}
	   }
	goto here2; 
       }

 if( ( obj1->type == WIN_SLIDE))
	{

if(obj1 != 0)
   {
    (obj1->change)(obj1,WIN_FOCUS); 
    need=obj1->needupdate;
	}

	}
}
  }
here2:
oko=ok;
xo=x; yo=y;
 }

sleep(60);
 
}
