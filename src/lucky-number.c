#include <tizen.h>
#include <stdlib.h>
#include "lucky-number.h"

typedef struct widget_instance_data {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *label;
} widget_instance_data_s;

static int
get_lucky_number()
{
	int lucky_numbers[] = {
		1, 3, 7, 9, 13, 15, 21, 25, 31,
		33, 37, 43, 49, 51, 63, 67,
		69, 73, 75, 79, 87, 93, 99
	};
	time_t t;
	srand((unsigned) time(&t));
	int i = rand() % (sizeof(lucky_numbers) / sizeof(int));
	return lucky_numbers[i];
}

static int
widget_instance_create(widget_context_h context, bundle *content, int w, int h, void *user_data)
{
	widget_instance_data_s *wid = (widget_instance_data_s*) malloc(sizeof(widget_instance_data_s));
	int ret;

	if (content != NULL) {
		/* Recover the previous status with the bundle object. */

	}

	/* Window */
	ret = widget_app_get_elm_win(context, &wid->win);
	if (ret != WIDGET_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get window. err = %d", ret);
		return WIDGET_ERROR_FAULT;
	}

	evas_object_resize(wid->win, w, h);

	/* Conformant */
	wid->conform = elm_conformant_add(wid->win);
	evas_object_size_hint_weight_set(wid->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(wid->win, wid->conform);
	evas_object_show(wid->conform);

	/* Label*/
	wid->label = elm_label_add(wid->conform);
	evas_object_resize(wid->label, w, h / 3);
	evas_object_move(wid->label, w / 4, h / 3);
	evas_object_show(wid->label);
	int lucky_number_i = get_lucky_number();
	char lucky_number_s[12];
	sprintf(lucky_number_s, "%d", lucky_number_i);
	elm_object_text_set(wid->label, lucky_number_s);

	/* Show window after base gui is set up */
	evas_object_show(wid->win);

	widget_app_context_set_tag(context, wid);
	return WIDGET_ERROR_NONE;
}

static int
widget_instance_destroy(widget_context_h context, widget_app_destroy_type_e reason, bundle *content, void *user_data)
{
	widget_instance_data_s *wid = NULL;
	widget_app_context_get_tag(context,(void**)&wid);

	if (wid->win)
		evas_object_del(wid->win);

	free(wid);

	return WIDGET_ERROR_NONE;
}

static int
widget_instance_pause(widget_context_h context, void *user_data)
{
	/* Take necessary actions when widget instance becomes invisible. */
	return WIDGET_ERROR_NONE;

}

static int
widget_instance_resume(widget_context_h context, void *user_data)
{
	/* Take necessary actions when widget instance becomes visible. */
	return WIDGET_ERROR_NONE;
}

static int
widget_instance_update(widget_context_h context, bundle *content,
                             int force, void *user_data)
{
	/* Take necessary actions when widget instance should be updated. */
	return WIDGET_ERROR_NONE;
}

static int
widget_instance_resize(widget_context_h context, int w, int h, void *user_data)
{
	/* Take necessary actions when the size of widget instance was changed. */
	return WIDGET_ERROR_NONE;
}

static void
widget_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/* APP_EVENT_LANGUAGE_CHANGED */
	char *locale = NULL;
	app_event_get_language(event_info, &locale);
	elm_language_set(locale);
	free(locale);
}

static void
widget_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/* APP_EVENT_REGION_FORMAT_CHANGED */
}

static widget_class_h
widget_app_create(void *user_data)
{
	/* Hook to take necessary actions before main event loop starts.
	   Initialize UI resources.
	   Make a class for widget instance.
	*/
	app_event_handler_h handlers[5] = {NULL, };

	widget_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
		APP_EVENT_LANGUAGE_CHANGED, widget_app_lang_changed, user_data);
	widget_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
		APP_EVENT_REGION_FORMAT_CHANGED, widget_app_region_changed, user_data);

	widget_instance_lifecycle_callback_s ops = {
		.create = widget_instance_create,
		.destroy = widget_instance_destroy,
		.pause = widget_instance_pause,
		.resume = widget_instance_resume,
		.update = widget_instance_update,
		.resize = widget_instance_resize,
	};

	return widget_app_class_create(ops, user_data);
}

static void
widget_app_terminate(void *user_data)
{
	/* Release all resources. */
}

int
main(int argc, char *argv[])
{
	widget_app_lifecycle_callback_s ops = {0,};
	int ret;

	ops.create = widget_app_create;
	ops.terminate = widget_app_terminate;

	ret = widget_app_main(argc, argv, &ops, NULL);
	if (ret != WIDGET_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "widget_app_main() is failed. err = %d", ret);
	}

	return ret;
}
