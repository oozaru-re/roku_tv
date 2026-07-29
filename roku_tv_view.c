#include "roku_tv_view.h"
#include "roku_tv_ir.h"
#include "roku_tv_layout.h"
#include <furi.h>
#include <input/input.h>
#include <notification/notification_messages.h>

typedef struct {
    TvButton current_button;
} RemoteViewModel;

struct RokuTvView {
    ViewDispatcher* view_dispatcher;
    NotificationApp* notifications;
    View* view;
};

static void draw_text_button(
    Canvas* canvas,
    const RemoteButtonDef* button,
    bool selected) {

    canvas_set_font(canvas, FontSecondary);

    if(selected) {
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_rbox(
            canvas,
            button->x,
            button->y,
            button->width,
            button->height,
            2);
        canvas_set_color(canvas, ColorWhite);
    } else {
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_rframe(
            canvas,
            button->x,
            button->y,
            button->width,
            button->height,
            2);
    }

    if(button->label) {
        canvas_draw_str_aligned(
            canvas,
            button->x + (button->width / 2),
            button->y + (button->height / 2),
            AlignCenter,
            AlignCenter,
            button->label);
    }
}

static void draw_icon_button(
    Canvas* canvas,
    const RemoteButtonDef* button,
    bool selected) {

    if(selected && button->selected_icon) {
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_icon(
            canvas,
            button->x,
            button->y,
            button->selected_icon);
        return;
    }

    if(selected) {
        const int32_t frame_x = (int32_t)button->x - 3;
        const int32_t frame_y = (int32_t)button->y - 3;
        const uint8_t frame_width = button->width + 6;
        const uint8_t frame_height = button->height + 6;

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_rbox(
            canvas,
            frame_x,
            frame_y,
            frame_width,
            frame_height,
            2);

        canvas_set_color(canvas, ColorWhite);
        canvas_draw_icon(
            canvas,
            button->x,
            button->y,
            button->icon);
    } else {
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_icon(
            canvas,
            button->x,
            button->y,
            button->icon);
    }
}

static void roku_tv_draw_callback(Canvas* canvas, void* model) {
    RemoteViewModel* view_model = model;

    canvas_clear(canvas);

    for(size_t i = 0; i < ButtonMax; i++) {
        const RemoteButtonDef* button = &roku_tv_button_defs[i];
        const bool selected = (i == view_model->current_button);

        if(button->icon) {
            draw_icon_button(canvas, button, selected);
        } else {
            draw_text_button(canvas, button, selected);
        }
    }
}

static void roku_tv_notify_send(NotificationApp* notifications) {
    notification_message(notifications, &sequence_single_vibro);
    notification_message(notifications, &sequence_blink_magenta_10);
    furi_delay_ms(20);
    notification_message(notifications, &sequence_reset_rgb);
}

static bool roku_tv_input_callback(InputEvent* event, void* context) {
    RokuTvView* remote_view = context;
    furi_assert(remote_view);

    if((event->type != InputTypeShort) &&
       (event->type != InputTypeRepeat)) {
        return false;
    }

    if(event->key == InputKeyBack) {
        if(event->type == InputTypeShort) {
            view_dispatcher_stop(remote_view->view_dispatcher);
        }

        return true;
    }

    if(event->key == InputKeyOk) {
        TvButton selected = ButtonPower;

        with_view_model(
            remote_view->view,
            RemoteViewModel * model,
            { selected = model->current_button; },
            false);

        roku_tv_ir_send(selected);
        roku_tv_notify_send(remote_view->notifications);

        return true;
    }

    with_view_model(
        remote_view->view,
        RemoteViewModel * model,
        {
            const RemoteButtonDef* current =
                &roku_tv_button_defs[model->current_button];

            switch(event->key) {
            case InputKeyUp:
                model->current_button = current->nav_up;
                break;

            case InputKeyDown:
                model->current_button = current->nav_down;
                break;

            case InputKeyLeft:
                model->current_button = current->nav_left;
                break;

            case InputKeyRight:
                model->current_button = current->nav_right;
                break;

            default:
                break;
            }
        },
        true);

    return true;
}

RokuTvView* roku_tv_view_alloc(
    ViewDispatcher* view_dispatcher,
    NotificationApp* notifications) {

    furi_assert(view_dispatcher);
    furi_assert(notifications);

    RokuTvView* remote_view = malloc(sizeof(RokuTvView));
    furi_check(remote_view);

    remote_view->view_dispatcher = view_dispatcher;
    remote_view->notifications = notifications;
    remote_view->view = view_alloc();

    view_set_orientation(
        remote_view->view,
        ViewOrientationVertical);

    view_set_context(
        remote_view->view,
        remote_view);

    view_allocate_model(
        remote_view->view,
        ViewModelTypeLockFree,
        sizeof(RemoteViewModel));

    with_view_model(
        remote_view->view,
        RemoteViewModel * model,
        { model->current_button = ButtonPower; },
        false);

    view_set_draw_callback(
        remote_view->view,
        roku_tv_draw_callback);

    view_set_input_callback(
        remote_view->view,
        roku_tv_input_callback);

    return remote_view;
}

View* roku_tv_view_get_view(RokuTvView* remote_view) {
    furi_assert(remote_view);
    return remote_view->view;
}

void roku_tv_view_free(RokuTvView* remote_view) {
    furi_assert(remote_view);

    view_free(remote_view->view);
    free(remote_view);
}
