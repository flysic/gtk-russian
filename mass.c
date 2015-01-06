#include "support.h"

// 向左移动
gboolean russian_mass_leftMove(Mass*);
// 向右移动
gboolean russian_mass_rightMove(Mass*);
// 向下移动
gboolean russian_mass_downMove(Mass*);
// 转换
void russian_mass_translate(Mass*);
// 清除
void russian_mass_clear(Mass*, int);
// 显示
void russian_mass_show(Mass*, int);

// 新建形状对象
Mass *russian_mass_new(gint type)
{
  gint i;
  Mass *p = (Mass*)g_malloc(sizeof(Mass));
  // 形状中包含MASS_DIAMOND_NUM个方块对象
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    p->diamonds[i]= russian_diamond_new();
  }
  // 形状类型
  p->type = type;
  // 形状变换类型
  p->tran_type = 0;
  // 初始化mass 
  russian_db_init_mass(p);
  return p;
}


// 向左移动
gboolean russian_mass_leftMove(Mass *mass)
{
  gint i; 

  // 先清除标记
  russian_mass_clear(mass, DIAMOND_TYPE_MASTER);
  // 判断是否可以左移
  if (russian_db_is_leftmove(mass)==FALSE)
  {
    russian_mass_show(mass, DIAMOND_TYPE_MASTER);    
    return FALSE;
  }
  // 左移
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    russian_diamond_leftmove(mass->diamonds[i]);
  }
  russian_mass_show(mass, DIAMOND_TYPE_MASTER);
  return TRUE;
}

// 向右移动
// 处理与左移类似
gboolean russian_mass_rightMove(Mass *mass)
{
  gint i;

  russian_mass_clear(mass, DIAMOND_TYPE_MASTER);  
  if (russian_db_is_rightmove(mass)==FALSE)
  {
    russian_mass_show(mass, DIAMOND_TYPE_MASTER);
    return FALSE;
  }
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    russian_diamond_rightmove(mass->diamonds[i]);
  }
  russian_mass_show(mass, DIAMOND_TYPE_MASTER);
  return TRUE;
}

// 向下移动
gboolean russian_mass_downMove(Mass *mass)
{
  gint i;

  russian_mass_clear(mass, DIAMOND_TYPE_MASTER);
  if (russian_db_is_downmove(mass)==FALSE)
  {
    russian_mass_show(mass, DIAMOND_TYPE_MASTER);    
    return FALSE;
  }
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    russian_diamond_downmove(mass->diamonds[i]);
  }
  russian_mass_show(mass, DIAMOND_TYPE_MASTER);
  return TRUE;
}

// 转换
void russian_mass_translate(Mass *mass)
{
  // 判断是否可以转换
  russian_mass_clear(mass, DIAMOND_TYPE_MASTER);
  if (russian_db_is_translate(mass)==FALSE)
  {
    russian_mass_show(mass, DIAMOND_TYPE_MASTER);    
    return;
  }
  // 转换
  russian_db_do_tranlate(mass);
  russian_mass_show(mass, DIAMOND_TYPE_MASTER);
}

// 清除
void russian_mass_clear(Mass *mass, int type)
{
  gint i;

  // 后备形状清除
  if (type==DIAMOND_TYPE_BACK)
  {
    for (i = 0; i < MASS_DIAMOND_NUM; ++i)
    {
      russian_diamond_clear(mass->diamonds[i], type);
    }
    return;
  }
  // 运行中形状清除
  // 清容器标记
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    russian_db_set_mark(mass->diamonds[i]->seat, FALSE);
    russian_diamond_clear(mass->diamonds[i], type);
  }  
}
// 显示
void russian_mass_show(Mass *mass, int type)
{
  gint i;
  
  // 后备形状显示
  if (type==DIAMOND_TYPE_BACK)
  {
    for (i = 0; i < MASS_DIAMOND_NUM; ++i)
    {
      russian_diamond_show(mass->diamonds[i], type);
    }
    return;
  }
  // 运行中形状显示
  // 置容器标记  
  for (i = 0; i < MASS_DIAMOND_NUM; ++i)
  {
    russian_db_set_mark(mass->diamonds[i]->seat, TRUE);
    russian_diamond_show(mass->diamonds[i], type);
  }
}
