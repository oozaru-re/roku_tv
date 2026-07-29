#include "roku_tv_view.h"
#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#define REMOTE_VIEW_ID 0U

int32_t roku_tv_app(void* p) {
    UNUSED(p);

    ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
    furi_check(view_dispatcher);

    Gui* gui = furi_record_open(RECORD_GUI);
    NotificationApp* notifications =
        furi_record_open(RECORD_NOTIFICATION);

    RokuTvView* remote_view =
        roku_tv_view_alloc(view_dispatcher, notifications);

    view_dispatcher_add_view(
        view_dispatcher,
        REMOTE_VIEW_ID,
        roku_tv_view_get_view(remote_view));

    view_dispatcher_attach_to_gui(
        view_dispatcher,
        gui,
        ViewDispatcherTypeFullscreen);

    notification_message(
        notifications,
        &sequence_display_backlight_enforce_on);

    view_dispatcher_switch_to_view(
        view_dispatcher,
        REMOTE_VIEW_ID);

    view_dispatcher_run(view_dispatcher);

    notification_message(
        notifications,
        &sequence_display_backlight_enforce_auto);

    view_dispatcher_remove_view(
        view_dispatcher,
        REMOTE_VIEW_ID);

    roku_tv_view_free(remote_view);
    view_dispatcher_free(view_dispatcher);

    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);

    return 0;
}
