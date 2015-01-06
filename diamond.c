#include "support.h"

// 新建方块对象
Diamond* russian_diamond_new();
// 向左移动
void russian_diamond_leftmove(Diamond*);
// 向右移动
void russian_diamond_rightmove(Diamond*);
// 向下移动
void russian_diamond_upmove(Diamond*);
// 向上移动
void russian_diamond_downmove(Diamond*);
// 清除
void russian_diamond_clear(Diamond*, int);
// 显示
void russian_diamond_show(Diamond*, int);
// 根据位置坐标得到真实坐标
static GdkPoint getCood(GdkPoint);

// 新建方块对象
Diamond *russian_diamond_new()
{
  Diamond *p = (Diamond*)g_malloc(sizeof(Diamond));
  // 默认位置
  p->seat.x = 0;
  p->seat.y = 0;
  return p;
}

// 根据位置坐标得到真实坐标
static GdkPoint getCood(GdkPoint seat)
{
  GdkPoint point;

  // 如果位置不合法，返回（0，0）坐标
  if (seat.x>RUSSIAN_SEAT_X_MAX || seat.x<RUSSIAN_SEAT_X_MIN ||
      seat.y>RUSSIAN_SEAT_Y_MAX || seat.y<0)
  {
    point.x = 0;
    point.y = 0;
    return point;
  }
  // 位置（0，0）对应坐标（<middle>，0）
  // 所以要在x上加上坐标的一半
  point.x = seat.x-RUSSIAN_SEAT_X_MIN;
  point.y = seat.y;
  point.x = point.x*DIAMOND_WIDTH+RUSSIAN_DRAW_PADDING;
  point.y = point.y*DIAMOND_WIDTH+RUSSIAN_DRAW_PADDING;
  return point;
}

// 显示
static void display(Diamond *diamond, int type, gboolean flag)
{
  GdkGC *drawing_gc;
  GdkPoint point;
  DrawInfo *info = russian_get_db_draw_info();
  GtkWidget *area;
  GdkDrawable *pixmap;
  GdkColor color;

  if (type==DIAMOND_TYPE_MASTER)
  {
    area = info->drawArea;
    pixmap = info->pixmap;
  }
  else
  {
    area = info->s_drawArea;
    pixmap = info->s_pixmap;
  }
  // 如果
  if (info->pixmap==NULL)
  {
    return;
  }
  if (diamond->seat.x>RUSSIAN_SEAT_X_MAX || diamond->seat.x<RUSSIAN_SEAT_X_MIN ||
      diamond->seat.y>RUSSIAN_SEAT_Y_MAX || diamond->seat.y<0)
  {
    return;
  }
  if (type==DIAMOND_TYPE_MASTER)
  {
    point = getCood(diamond->seat);    
  }
  else
  {
    point.x = diamond->seat.x+1;
    point.y = diamond->seat.y+1;  
    point.x = point.x*DIAMOND_WIDTH+RUSSIAN_DRAW_PADDING;
    point.y = point.y*DIAMOND_WIDTH+RUSSIAN_DRAW_PADDING;
  }
  if (flag==TRUE)
  {
    color.green = 10000;
    color.blue = 30000;  
    drawing_gc = area->style->fg_gc[GTK_STATE_INSENSITIVE];
    gdk_gc_set_rgb_fg_color(drawing_gc, &color);
  }
  else
  {
    drawing_gc = area->style->white_gc;
  }
  gdk_draw_rectangle(pixmap, drawing_gc, TRUE,
                     point.x, point.y,
                     DIAMOND_WIDTH-DIAMOND_SEAM,
                     DIAMOND_WIDTH-DIAMOND_SEAM);
  gtk_widget_queue_draw_area(area, point.x, point.y,
                             DIAMOND_WIDTH, DIAMOND_WIDTH);
}

// 清除
void russian_diamond_clear(Diamond *diamond, int type)
{
  display(diamond, type, FALSE);
}

// 显示
void russian_diamond_show(Diamond *diamond, int type)
{
  display(diamond, type, TRUE);
}

// 向左移动
void russian_diamond_leftmove(Diamond *diamond)
{
  if (diamond->seat.x>RUSSIAN_SEAT_X_MIN)
  {
    diamond->seat.x--;
  }
}

// 向右移动
void russian_diamond_rightmove(Diamond *diamond)
{
  if (diamond->seat.x<RUSSIAN_SEAT_X_MAX)
  {
    diamond->seat.x++;    
  }
}

// 向下移动
void russian_diamond_downmove(Diamond* diamond)
{
  if (diamond->seat.y<RUSSIAN_SEAT_Y_MAX)
  {
    diamond->seat.y++;    
  }
}

// 向下移动
void russian_diamond_upmove(Diamond* diamond)
{
  if (diamond->seat.y>0)
  {
    diamond->seat.y--;
  }
}
  
