
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
	 char *data;	 	/* data for pixmap */
	 int state;		/* active / sunken */
	 int locst;		/* data for button */
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

struct my_obj * my_alloc_obj(struct my_win *);
