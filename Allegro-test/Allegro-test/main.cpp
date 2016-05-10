#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "test.h"
#include "platformer.h"

int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 800;
int const MENU_BUTTON_SIDE_MARGIN = 50;

int main(int argc, char **argv) {



	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	// INIT ALL THE THINGS!
	if (!al_init()) {
		fprintf(stderr, "Could not initiliza allegro\n");
		return -1;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "Could not capture keyboard device\n");
		return -1;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "Could not capture mouse device\n");
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "Could not initilize allegro image addon");
		return -1;
	}
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display) {
		fprintf(stderr, "Could not initialize display\n");
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Could not create event queue\n");
	}


	// TODO: Tese should also be tested for null
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_native_dialog_addon();
	al_init_primitives_addon();

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 22, 0);
	if (!font) {
		al_show_native_message_box(display, "Error", "Error", "Fail to load font pirulen", NULL,
			ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}


	// Create event queue and register stuff

	ALLEGRO_EVENT event;
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	

	while (1) {
		al_wait_for_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			printf("mouse up");
			if ((event.mouse.x > MENU_BUTTON_SIDE_MARGIN) &&
				(event.mouse.x < SCREEN_WIDTH - MENU_BUTTON_SIDE_MARGIN) &&
				(event.mouse.y < SCREEN_HEIGHT / 3) &&
				(event.mouse.y > SCREEN_HEIGHT / 6)) {
				return beaker_game(display, event_queue, font);
			}

			if ((event.mouse.x > MENU_BUTTON_SIDE_MARGIN) &&
				(event.mouse.x < SCREEN_WIDTH - MENU_BUTTON_SIDE_MARGIN) &&
				(event.mouse.y < SCREEN_HEIGHT / 1.5) &&
				(event.mouse.y > SCREEN_HEIGHT / 2)) {
				return sheep_game(display, event_queue, font);
			}
		}
	
		al_draw_filled_rectangle(MENU_BUTTON_SIDE_MARGIN, SCREEN_HEIGHT / 3, SCREEN_WIDTH - MENU_BUTTON_SIDE_MARGIN, SCREEN_HEIGHT / 6, al_map_rgb(184, 22, 22));
		al_draw_filled_rectangle(MENU_BUTTON_SIDE_MARGIN, SCREEN_HEIGHT / 1.5, SCREEN_WIDTH - MENU_BUTTON_SIDE_MARGIN, SCREEN_HEIGHT / 2, al_map_rgb(184, 22, 22));

		al_draw_text(font, al_map_rgb(255, 255, 255), MENU_BUTTON_SIDE_MARGIN + 70, SCREEN_HEIGHT / 4.5, ALLEGRO_ALIGN_LEFT, "BEAKER_BALL");
		al_draw_text(font, al_map_rgb(255, 255, 255), MENU_BUTTON_SIDE_MARGIN + 70, SCREEN_HEIGHT / 1.75, ALLEGRO_ALIGN_LEFT, "SHEEP CATCHER");
		al_flip_display();
	
	}
	


	


	
			//beaker_game(display, event_queue, font);

			//sheep_game(display, event_queue, font);

	
	
	
		
	
		


}
/*
bool check_collision(block_t block1, block_t block2) {
	//printf("block1 x %d, block2 x %d\n", block1.x, block2.x);
	if ((block1.x > block2.x + 25) ||
		(block1.y > block2.y + 25) ||
		(block2.x > block1.x + 75) ||
		(block2.y > block1.y + 75)) {
		return false;
	}
	return true;
	*/