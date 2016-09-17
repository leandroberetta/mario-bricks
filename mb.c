/*
 * Mario Bricks
 * ------------
 *
 * Allegro 5 minimal example
 *
 * Author: Leandro Beretta <lea.beretta@gmail.com>
 * Informatica I - UTN.BA
 * 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define BRICKS 10
#define APP_TITLE "Mario Bricks"
#define FPS 10
#define LEFT 0
#define RIGHT 1

#define APP_WINDOW_W 800
#define APP_WINDOW_H 600
#define APP_WINDOW_X 100
#define APP_WINDOW_Y 100

void destroy(ALLEGRO_DISPLAY *display,
	           ALLEGRO_EVENT_QUEUE *event_queue,
					   ALLEGRO_TIMER *timer,
					   ALLEGRO_SAMPLE *sample,
					   ALLEGRO_SAMPLE *sample_win,
					   ALLEGRO_BITMAP *image,
					   ALLEGRO_BITMAP *image_bg,
					   ALLEGRO_FONT *font_title,
					   ALLEGRO_FONT *font_info) {
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
  al_destroy_timer(timer);
	al_destroy_sample(sample);
	al_destroy_sample(sample_win);
  al_destroy_bitmap(image);
	al_destroy_bitmap(image_bg);
	al_destroy_font(font_title);
	al_destroy_font(font_info);
}

int main(int argc, char **argv) {
	// Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *sample = NULL;
  ALLEGRO_SAMPLE *sample_win = NULL;
	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image_bg = NULL;
	ALLEGRO_FONT *font_title = NULL;
	ALLEGRO_FONT *font_info = NULL;
	ALLEGRO_EVENT events;

	int brick = 0;
	int tries = 0;
	int i = 0;
	int x = 0;
	int y = 550;
	int vec_brick[10];
	int dir = RIGHT;
	char progress[50];

	// Init and configuration
	if(!al_init())
  	al_show_native_message_box(NULL, NULL, "Error", "No se ha podido inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
  al_init_ttf_addon();
  al_init_acodec_addon();

	al_install_keyboard();
	al_install_audio();

	display = al_create_display(APP_WINDOW_W, APP_WINDOW_H);

  al_set_new_display_flags(ALLEGRO_WINDOWED);
  al_set_window_position(display, APP_WINDOW_X, APP_WINDOW_Y);
	al_set_window_title(display, APP_TITLE);

	// Configures the timer to create an event every 1/10 seconds
	timer = al_create_timer(1.0 / FPS);

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	image_bg = al_load_bitmap("images/background.png");
	al_draw_bitmap(image_bg, 0, 0, 0);

	image = al_load_bitmap("images/brick.png");
	al_draw_bitmap(image, 0, 550, 0);

 	font_title = al_load_ttf_font("fonts/mario_font.ttf", 30, 0);
	font_info = al_load_ttf_font("fonts/mario_font.ttf", 20, 0);

	al_reserve_samples(3);

	sample = al_load_sample("music/coin.wav");
	sample_win = al_load_sample("music/level.wav");

 	al_start_timer(timer);

	al_flip_display();

	while (1) {
		// Waits for a new event
		al_wait_for_event(event_queue, &events);

		switch (events.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				destroy(display, event_queue, timer, sample, sample_win, image, image_bg, font_title, font_info);

				return 0;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				switch (events.keyboard.keycode) {
					case ALLEGRO_KEY_ENTER:
						if (brick < BRICKS)
							tries++;

						if (brick < BRICKS && ((brick == 0) || (brick > 0 && vec_brick[brick - 1] == x))) {
							vec_brick[brick] = x;
	        		brick++;
	        		y = y - 50;

							al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}

						if (brick >= 3 && brick <= 9) {
							al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
							al_destroy_timer(timer);

							if (brick >= 3 && brick < 6)
								timer = al_create_timer(1.0 / (FPS * 2));
						  else if (brick >= 6 && brick < 9)
								timer = al_create_timer(1.0 / (FPS * 3));
						  else if (brick == 9)
								timer = al_create_timer(1.0 / (FPS * 4));

							al_register_event_source(event_queue, al_get_timer_event_source(timer));
							al_start_timer(timer);
						}

            if (brick == 10) {
              al_play_sample(sample_win, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}

						break;
					case ALLEGRO_KEY_ESCAPE:
						destroy(display, event_queue, timer, sample, sample_win, image, image_bg, font_title, font_info);

						return 0;
	          break;
				}

				break;
			case ALLEGRO_EVENT_TIMER:
				// Uses the timer event to redraw the screen (FPS)
				if (x == 750)
					dir = LEFT;

				if (x == 0)
					dir = RIGHT;

				if (dir)
					x = x + 50;
				else
					x = x - 50;

				al_draw_bitmap(image_bg, 0, 0, 0);
				al_draw_text(font_title, al_map_rgb(0, 0, 0), 800, 10, ALLEGRO_ALIGN_RIGHT, APP_TITLE);
				sprintf(progress, "tries: %d - progress: %d/%d ", tries, brick, BRICKS);
				al_draw_text(font_info, al_map_rgb(0, 0, 0), 800, 50, ALLEGRO_ALIGN_RIGHT, progress);

				if (brick < BRICKS)
					al_draw_bitmap(image, x, y, 0);

				for (i = 0; i < brick; i++)
					al_draw_bitmap(image, vec_brick[i], 550 - i*50, 0);

				al_flip_display();

				break;
		}
	}

	destroy(display, event_queue, timer, sample, sample_win, image, image_bg, font_title, font_info);

	return 0;
}
