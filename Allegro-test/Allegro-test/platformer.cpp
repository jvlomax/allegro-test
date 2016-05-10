#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <list>
#include <time.h>

int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 800;
int const TEXT_OFFSET = 55;

enum key_map {
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_LEFT
};

class GameObject {
	
	public:
		ALLEGRO_BITMAP *image;
		int x, y;
		int w, h;
		bool draw_sprite;
		int speed;
		int scale;
		void update();
		void draw();
		int get_w();
		int get_h();
};

void GameObject::update() {
	y += speed;
	if (y > SCREEN_HEIGHT ||
		x > SCREEN_WIDTH ||
		x < 0 ||
		y < 0) {
		draw_sprite = false;
	}
	else {
		draw_sprite = true;
	}

}
int GameObject::get_w() {
	return w;
}

int GameObject::get_h() {
	return h;
}


void GameObject::draw() {
		al_draw_scaled_bitmap(image, 0, 0, w, h, x, y, w * scale, h *scale, 0);
	
}

class Sheep: public GameObject {
	public:	
		Sheep(int, int);
	
		
};



Sheep::Sheep(int start_x, int start_y) {
	// http://orig12.deviantart.net/f668/f/2012/135/0/b/rpg_maker_vx_gulliver_eureka_seven_by_dfox20-d4zwajj.png
	image = al_load_bitmap("sheep.png");
	w = al_get_bitmap_width(image);
	h = al_get_bitmap_height(image);
	x = start_x;
	y = start_y;
	draw_sprite = true;
	scale = 1;
	speed = (al_get_time() / 50) + 1;
}






class Man: public GameObject {
	public:
		int score;
		Man();
		int frame;
		ALLEGRO_BITMAP *image2;
		void draw();
		bool collision(GameObject);
};

Man::Man() {
	// https://embed.gyazo.com/e02b13490950d36b9d0aba0fac4fd274.png
	image = al_load_bitmap("man_1.png");
	image2 = al_load_bitmap("man_2.png");
	w = al_get_bitmap_width(image);
	h = al_get_bitmap_height(image);
	speed = 2;
	frame = 0;
	scale = 1;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - TEXT_OFFSET - h;
	score = 0;
}


void Man::draw() {
	
	if (frame == 1) {
		
		al_draw_scaled_bitmap(image2, 0, 0, w, h, x, y, w * scale, h *scale, 0);
	}
	else {
		al_draw_scaled_bitmap(image, 0, 0, w, h, x, y, w * scale, h *scale, 0);
	}
	
	
}

bool Man::collision(GameObject other) {
	
	if ((x > other.x + other.w) ||
	   (y > other.y + other.w) ||
	   (other.x > x + w) ||
	    (other.y > y + h)) {
		return false;
	}
	return true;
}

int sheep_game(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font) {
	
	bool keyboard_map[4] = { false, false, false, false };
	

	// create timers
	ALLEGRO_TIMER *FPS = al_create_timer(1.0 / 60);
	ALLEGRO_TIMER *sheep_spawner = al_create_timer(3);
	ALLEGRO_TIMER *animation_timer = al_create_timer(0.5);
	al_register_event_source(event_queue, al_get_timer_event_source(animation_timer));
	al_register_event_source(event_queue, al_get_timer_event_source(FPS));
	al_register_event_source(event_queue, al_get_timer_event_source(sheep_spawner));
	al_start_timer(sheep_spawner);
	al_start_timer(FPS);
	al_start_timer(animation_timer);
	
	// Create the inital sheep. Keep it around as a measuring tool for later sheep;
	std::list<Sheep> sheep_list;
	Sheep initial_sheep((rand() % SCREEN_WIDTH), 0 - 41); // TODO: fix magic number
	sheep_list.push_front(initial_sheep);


	// Create the main character
	Man man;

	bool paused = true;

	// Draw on some higher power to ensure randomness-ness
	srand(time(NULL));

	// GO-GO main loop
	while (1) {
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
				paused = false;
			}
		}
		
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keyboard_map[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keyboard_map[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keyboard_map[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keyboard_map[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				if (paused) {
					paused = false;
				}
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {

			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keyboard_map[KEY_UP] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keyboard_map[KEY_RIGHT] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keyboard_map[KEY_DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keyboard_map[KEY_LEFT] = false;
				break;
			}
		}
		
		
		if (ev.timer.source == sheep_spawner) {
			Sheep sheep(rand() % 640 - initial_sheep.get_w(), 0 - initial_sheep.get_h());
			sheep_list.push_back(sheep);

		}
		if (ev.timer.source == animation_timer) {
			
			man.frame += 1;
			man.frame %= 2;
		}

		if (ev.timer.source == FPS && al_event_queue_is_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(255, 255, 255));

			if (!paused) {
				
			

				for (std::list<Sheep>::iterator i = sheep_list.begin(); i != sheep_list.end(); i++) {
					i->update();
					i->draw();
					if (man.collision(*i)) {
						man.score += 1;
						try {
							sheep_list.erase(i++);
						}
						catch (int e){
							sheep_list.clear();
						}		
					}
				}
				if (keyboard_map[KEY_LEFT]) {
					if (man.x > 0) {
						man.x -= man.speed;
					}

				}
				if (keyboard_map[KEY_RIGHT]) {
					if (man.x < SCREEN_WIDTH) {
						man.x += man.speed;
					}
				}

				man.draw();
			}
			if (paused) {
				al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT - TEXT_OFFSET, ALLEGRO_ALIGN_CENTER, "Press space to start game");
				
			}
			al_draw_textf(font, al_map_rgb(0, 0, 0), 10, 20, ALLEGRO_ALIGN_LEFT, "Score: %d", man.score);
			al_flip_display();

		}

	}
	
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(FPS);
	al_destroy_timer(sheep_spawner);
	al_destroy_timer(animation_timer);
	al_destroy_font(font);
	
	return 0;
}
