#ifndef DIAMOND_H_
#define DIAMOND_H_

// 方块寛度
#define DIAMOND_WIDTH                           \
    20
// 方块间距
#define DIAMOND_SEAM                            \
    1
// 方块类型
#define DIAMOND_TYPE_BACK                       \
    0
#define DIAMOND_TYPE_MASTER                     \
    1

// 方块结构
typedef struct _diamond
{
    // 位置
    GdkPoint seat;
} Diamond;

// 新建方块对象
extern Diamond* russian_diamond_new();
// 向左移动
extern void russian_diamond_leftmove(Diamond*);
// 向右移动
extern void russian_diamond_rightmove(Diamond*);
// 向下移动
extern void russian_diamond_upmove(Diamond*);
// 向上移动
extern void russian_diamond_downmove(Diamond*);
// 清除
extern void russian_diamond_clear(Diamond*, int);
// 显示
extern void russian_diamond_show(Diamond*, int);

#endif
