#include "support.h"

// 数据库是访问所有信息的入口
static DrawInfo _draw_info;
static MatrixInfo _matrix_info;

// 七种形状的起始位置 
const gint _initdat[MASS_TYPE_NUM][MASS_DATA_NUM] =
  {
    {0,0,0,1,1,0,1,1},
    {0,0,0,1,0,2,0,3},
    {0,0,0,1,1,1,1,2},
    {1,0,0,1,1,1,0,2},
    {0,0,0,1,0,2,1,2},
    {1,0,1,1,0,2,1,2},
    {1,0,1,1,0,1,2,1},
  };

// 单个形状变换数据
const gint _mp0[2*MASS_DATA_NUM] =
  {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,    
  };
const gint _mp1[2*MASS_DATA_NUM] =
  {
    -1,1,0,0,1,-1,2,-2,
    1,-1,0,0,-1,1,-2,2
  };
const gint _mp2[2*MASS_DATA_NUM] =
  {
    0,0,0,0,0,-1,-2,-1,
    0,0,0,0,0,1,2,1
  };
const gint _mp3[2*MASS_DATA_NUM] =
  {
    -1,0,0,0,0,0,-1,-2,
    1,0,0,0,0,0,1,2
  };
const gint _mp4[4*MASS_DATA_NUM] =
  {
    1,0,0,0,-1,-1,0,-1,
    -1,0,0,0,0,-1,-1,1,
    1,1,0,0,0,1,-1,0,
    -1,-1,0,0,1,1,2,0
  };
const gint _mp5[4*MASS_DATA_NUM] =
  {
    1,1,0,0,0,-1,1,0,
    0,-1,0,0,1,-1,-1,0,
    -2,0,0,0,-1,1,1,-1,
    1,0,0,0,0,1,-1,1
  };
const gint _mp6[4*MASS_DATA_NUM] =
  {
    0,0,0,0,0,0,-1,1,
    1,1,0,0,0,0,0,0,
    0,0,0,0,1,-1,0,0,
    -1,-1,0,0,-1,1,1,-1
  };

// 七种形状变形数目
const gint _fettledat[MASS_TYPE_NUM] = 
  {
    1,2,2,2,4,4,4
  };

// 七种形状变形数据
const gint *_muldat[MASS_TYPE_NUM]={_mp0,_mp1,_mp2,_mp3,_mp4,_mp5,_mp6};

// 游戏数据初始化
void russian_db_init();
// 取得绘图信息
DrawInfo *russian_get_db_draw_info();
// 取得游戏运行信息
MatrixInfo *russian_get_db_matrix_info();
// 根据位置设置容器标记
void russian_db_set_mark(GdkPoint, gboolean);
// 分析容器，清除已满的行
gboolean russian_db_repair_maxtrix();
// game over
void russian_db_draw_gameover();
// 是否可以向左移动
gboolean russian_db_is_leftmove(Mass*);
// 是否可以向右移动
gboolean russian_db_is_rightmove(Mass*);
// 是否可以向下移动
gboolean russian_db_is_downmove(Mass*);
// 是否可以转换
gboolean russian_db_is_translate(Mass*);
// 执行转换
void russian_db_do_tranlate(Mass*);
// 初始化mass
void russian_db_init_mass(Mass*);
// 根据位置取得容器数组的行/列
static void get_matrixpoint(GdkPoint, gint*, gint*);


// 游戏数据初始化
void russian_db_init()
{
  gint width,height;
  gint i, j;
  
  // 生成主内存绘图对象pixmap
  width = _draw_info.drawArea->allocation.width;
  height = _draw_info.drawArea->allocation.height;
  if (_draw_info.pixmap==NULL)
  {
    _draw_info.pixmap = gdk_pixmap_new(_draw_info.drawArea->window, width, height, -1);
  }
  gdk_draw_rectangle(_draw_info.pixmap, _draw_info.drawArea->style->white_gc,
                     TRUE, 0, 0, width, height);
  gdk_draw_rectangle(_draw_info.pixmap, _draw_info.drawArea->style->black_gc,
                     FALSE, 1, 1, width-2, height-2);
  gtk_widget_queue_draw_area(_draw_info.drawArea, 0, 0,
                             width, height);
  // 生成副内存绘图对象s_pixmap  
  width = _draw_info.s_drawArea->allocation.width;
  height = _draw_info.s_drawArea->allocation.height;
  if (_draw_info.s_pixmap==NULL)
  {
    _draw_info.s_pixmap = gdk_pixmap_new(_draw_info.s_drawArea->window, width, height, -1);    
  }
  gdk_draw_rectangle(_draw_info.s_pixmap, _draw_info.s_drawArea->style->white_gc,
                     TRUE, 0, 0, width, height);
  gdk_draw_rectangle(_draw_info.s_pixmap, _draw_info.s_drawArea->style->black_gc, 
                     FALSE, 1, 1, width-2, height-2); 
  gtk_widget_queue_draw_area(_draw_info.s_drawArea, 0, 0, 
                            width, height);
  // 清空形状容器
  for (i = 0; i < RUSSIAN_MATRIX_ROW; ++i)
  {
    for (j = 0; j < RUSSIAN_MATRIX_COL; ++j)
    {
      _matrix_info.matrix[i][j] = FALSE;
    }
  }
  // 初始数据
  _draw_info.mass = NULL;
  _draw_info.s_mass = NULL;
  _matrix_info.pause = FALSE;
  _matrix_info.end = FALSE;
}

// 取得绘图信息
DrawInfo *russian_get_db_draw_info()
{
  return &_draw_info;
}

// 取得游戏运行信息
MatrixInfo *russian_get_db_matrix_info()
{
  return &_matrix_info;
}

// 清除当前行
static void matrix_clear_line(gint row)
{
  gint i, j;
  gint sum;
  Diamond *diamond;
  
  // 把小于row的行向下移动，覆盖第row行
  memmove(_matrix_info.matrix[1], _matrix_info.matrix[0], row*sizeof(gboolean)*RUSSIAN_MATRIX_COL);
  // 刷新容器目前状态，只刷新小于row行的部分
  diamond = russian_diamond_new();
  for (i = row; i > 0; --i)
  {
    for (j = 0; j < RUSSIAN_MATRIX_COL; ++j)
    {
      diamond->seat.x = j+RUSSIAN_SEAT_X_MIN;
      diamond->seat.y = i;
      // 有标记，则显示方块
      if (_matrix_info.matrix[i][j]==TRUE)
      {
        russian_diamond_show(diamond, DIAMOND_TYPE_MASTER);
      }
      // 无标记，则清除方块
      else
      {
        russian_diamond_clear(diamond, DIAMOND_TYPE_MASTER);        
      }
    }
    // 如果当前行无标记，说明小于当前行也都无标记，可以退出了
    sum = 0;
    for (j = 0; j < RUSSIAN_MATRIX_COL; ++j)
    {
      sum += _matrix_info.matrix[i][j];
    }
    if (sum==0)
    {
      g_free(diamond);
      return;
    }
  }
  g_free(diamond);  
}

// 分析容器，清除已满的行
// 如容器未满，返回TRUE，已满，返回FALSE
gboolean russian_db_repair_maxtrix()
{
  gint i, j;
  gint sum;
  
  // 从最大列开始，逐步减小列号分析；
  // 当发现当前列已经没有标记了，可以肯定后面循环的列也没有标记，可以退出了；
  // 当发现循环到当前列号小于2，且当前列有标记，说明容器已满，返回FALSE；
  // 当发现当前列标记全满，清除当前列。
  for (i = RUSSIAN_MATRIX_ROW-1; i > 0; --i)
  {
    sum = 0;
    // 累加当前列标记
    for (j = 0; j < RUSSIAN_MATRIX_COL; ++j)
    {
      sum += _matrix_info.matrix[i][j];
    }
    // 当发现循环到当前列号小于2，且当前列有标记，说明容器已满，返回FALSE
    if (sum>0 && i<2)
    {
      return FALSE;
    }
    // 当发现当前列已经没有标记了，可以肯定后面循环的列也没有标记，可以退出了
    if (sum==0)
    {
      return TRUE;
    }
    // 当发现当前列标记全满，清除当前列
    // 同时，再从当前行开始分析
    if (sum==RUSSIAN_MATRIX_COL)
    {
      matrix_clear_line(i);
      i++;
    }
  }
  return TRUE;
}

// game over
// 在游戏中间区域，写文字消息GAME OVER
void russian_db_draw_gameover()
{
  GdkGC *drawing_gc;
  PangoLayout *layout;
  GdkPoint point;

  if (_draw_info.pixmap==NULL)
  {
    return;
  }
  point.x = _draw_info.drawArea->allocation.width*0.4;
  point.y = _draw_info.drawArea->allocation.height/2;
  // 用黑色画笔
  drawing_gc = _draw_info.drawArea->style->black_gc;
  // 用pango函数写字符串
  layout = gtk_widget_create_pango_layout(_draw_info.drawArea, "GAME OVER");
  PangoFontDescription *fontdesc = pango_font_description_from_string("Sans 18");
  pango_layout_set_font_description (layout, fontdesc);
  gdk_draw_layout(_draw_info.pixmap, drawing_gc, point.x, point.y, layout);
  // 请求更新写了字符串区域，发出expose事件
  gtk_widget_queue_draw_area(_draw_info.drawArea, point.x, point.y,
                             point.x, point.y);
}

// 根据位置取得容器数组的行/列
void get_matrixpoint(GdkPoint point, gint *row, gint *col)
{
  point.x -= RUSSIAN_SEAT_X_MIN;
  *row = point.y;
  *col = point.x;
}

// 是否可以向左移动
gboolean russian_db_is_leftmove(Mass *mass)
{
  gint i, row, col;

  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    // 根据位置取得容器数组的行/列
    get_matrixpoint(mass->diamonds[i]->seat, &row, &col);
    col -= 1;
    // 判断左移后的列是否法，且左移后容器位置已有标记；
    // 如条件成立，说明不能左移
    if (col>=RUSSIAN_MATRIX_COL || col<0 ||
        _matrix_info.matrix[row][col]==TRUE)
    {
      return FALSE;
    }
  }
  return TRUE;
  
}

// 是否可以向右移动
gboolean russian_db_is_rightmove(Mass *mass)
{
  gint i, row, col;

  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    get_matrixpoint(mass->diamonds[i]->seat, &row, &col);    
    col += 1;
    // 判断右移后的列是否法，且左移后容器位置已有标记；
    // 如条件成立，说明不能右移    
    if (col>=RUSSIAN_MATRIX_COL || col<0 ||
        _matrix_info.matrix[row][col]==TRUE)
    {
      return FALSE;
    }
  }
  return TRUE;
  
}

// 是否可以向下移动
gboolean russian_db_is_downmove(Mass *mass)
{
  gint i, row, col;
  
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    get_matrixpoint(mass->diamonds[i]->seat, &row, &col);    
    row += 1;
    if (row>=RUSSIAN_MATRIX_ROW ||
        _matrix_info.matrix[row][col]==TRUE)
    {
      return FALSE;
    }
  }
  return TRUE;
}

// 是否可以转换
gboolean russian_db_is_translate(Mass *mass)
{
  gint i, k, row, col;
  const gint *dat;
  
  k = MASS_DATA_NUM*mass->tran_type;
  dat = _muldat[mass->type];
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    get_matrixpoint(mass->diamonds[i]->seat, &row, &col);
    col += dat[k++];
    row += dat[k++];
    // 判断转换后的行和列是否非法，且变换后容器位置已有标记；
    // 如条件成立，说明不能转换
    if (col>=RUSSIAN_MATRIX_COL || col<0 ||
        row>=RUSSIAN_MATRIX_ROW || _matrix_info.matrix[row][col]==TRUE)
    {
      return FALSE;
    }
  }
  return TRUE;
}

// 执行转换
void russian_db_do_tranlate(Mass *mass)
{
  gint i, k;
  const gint *dat;  
  
  // 取得转换数据
  k = MASS_DATA_NUM*mass->tran_type;
  dat = _muldat[mass->type];
  // 转换
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    mass->diamonds[i]->seat.x += dat[k++];
    mass->diamonds[i]->seat.y += dat[k++];    
  }
  // 记录转换类型
  mass->tran_type++;
  mass->tran_type %= _fettledat[mass->type];  
}

// 初始化mass
void russian_db_init_mass(Mass *mass)
{
  gint i, k;
  if (mass->type>MASS_TYPE_NUM)
  {
    mass->type = 0;
  }
  k = 0;
  // diamonds以类型编号存储形状初始数据
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    mass->diamonds[i]->seat.x = _initdat[mass->type][k++];
    mass->diamonds[i]->seat.y = _initdat[mass->type][k++];    
  }
}

// 根据位置设置容器标记
void russian_db_set_mark(GdkPoint point, gboolean flag)
{
  gint row, col;
  
  get_matrixpoint(point, &row, &col);
  _matrix_info.matrix[row][col] = flag;  
}
