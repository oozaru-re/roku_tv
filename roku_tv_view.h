#pragma once

#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <notification/notification.h>

typedef struct RokuTvView RokuTvView;

RokuTvView* roku_tv_view_alloc(
    ViewDispatcher* view_dispatcher,
    NotificationApp* notifications);

View* roku_tv_view_get_view(RokuTvView* remote_view);
void roku_tv_view_free(RokuTvView* remote_view);
