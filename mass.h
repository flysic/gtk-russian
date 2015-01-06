#ifndef MASS_H_
#define MASS_H_

#include "diamond.h"

// 形状包含的方块数目
#define MASS_DIAMOND_NUM                        \
    4
// 形状包含的方块数目
#define MASS_DATA_NUM                           \
    8
#define MASS_TYPE_NUM                           \
    7

typedef struct _mass
{
    Diamond *diamonds[MASS_DIAMOND_NUM];
    // 形状类型
    gint type;
    // 转换类型
    gint tran_type;
} Mass;

// 新建形状对象
extern Mass* russian_mass_new();
// 向左移动
extern gboolean russian_mass_leftMove(Mass*);
// 向右移动
extern gboolean russian_mass_rightMove(Mass*);
// 向下移动
extern gboolean russian_mass_downMove(Mass*);
// 转换
extern void russian_mass_translate(Mass*);
// 清除
extern void russian_mass_clear(Mass*, int);
// 显示
extern void russian_mass_show(Mass*, int);

#endif
