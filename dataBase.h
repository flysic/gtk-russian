#ifndef DATABASE_H_
#define DATABASE_H_

#include "mass.h"

// russian绘图区域边框宽度
#define RUSSIAN_DRAW_PADDING                    \
    2
// russian窗体大小
#define RUSSIAN_WINDOW_WIDTH                    \
    602
#define RUSSIAN_WINDOW_HEIGHT                   \
    554

// 方块位置最小横坐标
// 则生成方块位置是(0,0)，横坐标则好在中间
#define RUSSIAN_SEAT_X_MIN                      \
    -12
// 方块位置最大横坐标
#define RUSSIAN_SEAT_X_MAX                      \
    11
// 方块位置最大纵坐标
#define RUSSIAN_SEAT_Y_MAX                      \
    23
// 容器行数
// 对应方块位置的纵坐标
#define RUSSIAN_MATRIX_ROW                      \
    (RUSSIAN_SEAT_Y_MAX+1)
// 容器列数
// 对应方块位置的横坐标
#define RUSSIAN_MATRIX_COL                      \
    (RUSSIAN_SEAT_X_MAX-RUSSIAN_SEAT_X_MIN+1)

// 绘图信息
typedef struct _draw_info
{
    GtkWidget *main_window;
    // 主绘图区
    GtkWidget *drawArea;
    // 后备绘图区
    GtkWidget *s_drawArea;
    // begin button
    GtkWidget *begin_button;
    // begin pause
    GtkWidget *pause_button;
    // 定时事件编号
    gint timeout_id;
    // 主内存绘图区
    GdkDrawable *pixmap;
    // 后备内存绘图区
    GdkDrawable *s_pixmap;
    // 运行中形状
    Mass *mass;
    // 后备形状
    Mass *s_mass;
} DrawInfo;

// 游戏运行信息
typedef struct _matrixInfo
{
    // 游戏是否暂停
    gboolean pause;
    // 游戏是否结束
    gboolean end;
    // 运行容器-标记数组
    gboolean matrix[RUSSIAN_MATRIX_ROW][RUSSIAN_MATRIX_COL];
} MatrixInfo;

// 游戏数据初始化
extern void russian_db_init();
// 取得绘图信息
extern DrawInfo *russian_get_db_draw_info();
// 取得游戏运行信息
extern MatrixInfo *russian_get_db_matrix_info();
// 根据位置设置容器标记
extern void russian_db_set_mark(GdkPoint, gboolean);
// 分析容器，清除已满的行
extern gboolean russian_db_repair_maxtrix();
// game over
extern void russian_db_draw_gameover();
// 初始化mass
extern void russian_db_init_mass(Mass*);
// 是否可以向左移动
extern gboolean russian_db_is_leftmove(Mass*);
// 是否可以向右移动
extern gboolean russian_db_is_rightmove(Mass*);
// 是否可以向下移动
extern gboolean russian_db_is_downmove(Mass*);
// 是否可以转换
extern gboolean russian_db_is_translate(Mass*);
// 执行转换
extern void russian_db_do_tranlate(Mass*);

#endif
