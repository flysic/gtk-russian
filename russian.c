#include "support.h"

#define UI_FILE "russian.glade"

// 运行中形状的expose事件回调函数
gboolean on_drawArea_expose_event(GtkWidget *widget,
                          GdkEventExpose *event,
                          DrawInfo *info)
{
  if (info->pixmap==NULL) 
  { 
    return TRUE; 
  }
  // 把内存图形显示到绘图区域  
  gdk_draw_drawable(widget->window, widget->style->white_gc,
                    info->pixmap, event->area.x, event->area.y,
                    event->area.x, event->area.y, event->area.width, event->area.height);
  return TRUE;
}

// 后备形状的expose事件回调函数
gboolean on_s_drawArea_expose_event(GtkWidget *widget,
                            GdkEventExpose *event,
                            DrawInfo *info)
{
  if (info->s_pixmap==NULL)
  {
    return TRUE;
  }
  // 把内存图形显示到绘图区域
  gdk_draw_drawable(widget->window, widget->style->white_gc,
                    info->s_pixmap, event->area.x, event->area.y,
                    event->area.x, event->area.y, event->area.width, event->area.height);
  return TRUE;
}

// 定时事件
// 定时事件中维持两个形状，mass和s_mass，
// 当mass下落到容器的底部，释放mass
// 当mass为空时，将s_mass赋给mass，再重新生成s_mass
static gboolean timeout()
{
  gint type;
  gboolean flag;
  DrawInfo *draw_info = russian_get_db_draw_info();
  MatrixInfo *matrix_info = russian_get_db_matrix_info();
  
  // 程序刚运行时，s_mass为空，需要生成
  if (draw_info->s_mass==NULL)
  {
    type = g_rand_int_range(g_rand_new(), 0, MASS_TYPE_NUM);
    draw_info->s_mass = russian_mass_new(type);
  }
  // 当mass为空时，将s_mass赋给mass，再重新生成s_mass
  if (draw_info->mass==NULL)
  {
    draw_info->mass = draw_info->s_mass;
    russian_mass_show(draw_info->mass, DIAMOND_TYPE_MASTER);
    type = g_rand_int_range(g_rand_new(), 0, MASS_TYPE_NUM);
    russian_mass_clear(draw_info->s_mass, DIAMOND_TYPE_BACK);
    draw_info->s_mass = russian_mass_new(type);
    russian_mass_show(draw_info->s_mass, DIAMOND_TYPE_BACK);        
    return TRUE;
  }
  // 暂停，结束时，不执行下落
  if (matrix_info->pause==TRUE || matrix_info->end==TRUE)
  {
    return TRUE;
  }
  // 当mass下落到容器的底部，释放mass
  if (russian_mass_downMove(draw_info->mass)==FALSE)
  {
    // 释放mass
    g_free(draw_info->mass);
    draw_info->mass = NULL;
    // 清除容器中已成满的行
    flag = russian_db_repair_maxtrix();
    // 如果容器已经被填满，游戏结束
    if (flag==FALSE)
    {
      russian_db_draw_gameover();
      matrix_info->end = TRUE;
      return TRUE;
    }
  }
  return TRUE;
}

//按键事件，在事件中处理俄罗斯方块的移动，变化
gboolean on_main_window_key_press_event(GtkWidget *widget,
                                        GdkEventKey *event,
                                        DrawInfo *info)
{
  gboolean flag;
  DrawInfo *draw_info = russian_get_db_draw_info();
  MatrixInfo *matrix_info = russian_get_db_matrix_info();
  widget = NULL;
  info = NULL;
  
  if (draw_info->mass==NULL)
  {
    return TRUE;
  }
  // 如果是暂停、结束状态，不处理
  if (matrix_info->pause==TRUE || matrix_info->end==TRUE)
  {
    return TRUE;
  }
  // 响应左、右、下，上按键
  if (event->type == GDK_KEY_PRESS)
  {
    if (event->keyval==GDK_a && (event->state&GDK_CONTROL_MASK))
    {
      while (TRUE)
      {
        flag = russian_mass_leftMove(draw_info->mass);
        if (flag==FALSE)
        {
          break;
        }
      }
    }
    else if(event->keyval==GDK_e && (event->state&GDK_CONTROL_MASK))
    {
      while (TRUE)
      {
        flag = russian_mass_rightMove(draw_info->mass);
        if (flag==FALSE)
        {
          break;
        }
      }
    }
    else if(event->keyval==GDK_v && (event->state&GDK_CONTROL_MASK))
    {
      while (TRUE)
      {
        flag = russian_mass_downMove(draw_info->mass);
        if (flag==FALSE)
        {
          break;
        }
      }
    }
    else if (event->keyval==GDK_Left || (event->keyval==GDK_b && (event->state&GDK_CONTROL_MASK)))
    {
      russian_mass_leftMove(draw_info->mass);
    }
    else if (event->keyval==GDK_Right || (event->keyval==GDK_f && (event->state&GDK_CONTROL_MASK)))
    {
      russian_mass_rightMove(draw_info->mass);
    }
    else if (event->keyval==GDK_Down || (event->keyval==GDK_n && (event->state&GDK_CONTROL_MASK)))
    {
      russian_mass_downMove(draw_info->mass);
    }
    else if (event->keyval==GDK_Up || (event->keyval==GDK_p && (event->state&GDK_CONTROL_MASK)))
    {
      russian_mass_translate(draw_info->mass);
    }
  }
  return TRUE;  
}

// 重新开始游戏
void on_begin_button_clicked(GtkWidget *widget,
                             DrawInfo *info)
{
  widget = NULL;
  GtkButton *button = GTK_BUTTON(info->begin_button);
  // 重新开始游戏，相当于重新初始化程序
  russian_db_init();
  // 如果重新开始游戏时，处于暂停状态，要将暂停标记去除
  gtk_button_set_label(button, "pause");  
}

// 暂停事件
void on_pause_button_clicked(GtkWidget *widget,
                                    DrawInfo *info)
{
  widget = NULL;
  GtkButton *button = GTK_BUTTON(info->pause_button);
  MatrixInfo *matrix_info = russian_get_db_matrix_info();  
  
  // 暂停是开关形式的操作
  if (matrix_info->pause==TRUE)
  {
    gtk_button_set_label(button, "pause");
    matrix_info->pause = FALSE;
  }
  else
  {
    gtk_button_set_label(button, "run");    
    matrix_info->pause = TRUE;
  }
}


int main(int argc, char *argv[])
{
  GtkBuilder *builder;
  DrawInfo *info;
  GError *error = NULL;
  
  gtk_init(&argc, &argv);
  // 根据glade文件构建界面
  builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, UI_FILE, &error)==0)
  {
    g_warning("%s", error->message);
    g_free(error);
    return 1;
  }
  // 提取控件
  info = russian_get_db_draw_info();
  CH_GET_WIDGET(builder, main_window, info);
  CH_GET_WIDGET(builder, drawArea, info);
  CH_GET_WIDGET(builder, s_drawArea, info);
  CH_GET_WIDGET(builder, begin_button, info);
  CH_GET_WIDGET(builder, pause_button, info);  
  // 传递事件数据
  gtk_builder_connect_signals(builder, info);
  // 释放builder
  g_object_unref(G_OBJECT(builder));
  // 显示界面
  gtk_widget_show_all(info->main_window);

  // 数据库初始化
  russian_db_init();
  // 定时事件
  info->timeout_id = g_timeout_add(500, timeout, NULL);
  
  gtk_main();
  
  return 0;
}
