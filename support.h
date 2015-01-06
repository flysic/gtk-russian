#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <string.h>
#include "dataBase.h"
#include "diamond.h"
#include "mass.h"

// 从builder中提取控件
#define CH_GET_OBJECT(builder, name, type, data)              \
    (data)->name = type(gtk_builder_get_object(builder, #name))
#define CH_GET_WIDGET(builder, name, data)          \
    CH_GET_OBJECT(builder, name, GTK_WIDGET, data)

#endif
