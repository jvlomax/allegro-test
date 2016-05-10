#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int const SCREEN_WIDTH = 800;
int const SCREEN_HEIGHT = 640;

enum key_map {
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT
};
struct Block {
    int x;
    int y;
    int w;
    int h;
    int dirty;
    int vx;
    int vy;
    bool redraw;
    ALLEGRO_BITMAP *image;
};
typedef struct  Block block_t;

block_t create_block(char *file){
    ALLEGRO_BITMAP *image = al_load_bitmap(file);
    Block block;

    block.image = image;
    block.x = 0;
    block.y = 0;
    block.w = al_get_bitmap_width(image);
    block.h = al_get_bitmap_height(image);
    block.dirty = 0;
    block.redraw = true;
    block.vx = 0;
    block.vy = 0;
    return block;
}


bool check_collision(block_t block1, block_t block2){
    //printf("block1 x %d, block2 x %d\n", block1.x, block2.x);
    if((block1.x > block2.x + 25) ||
       (block1.y > block2.y + 25) ||
       (block2.x > block1.x + 75) ||
       (block2.y > block1.y + 75)){
        return false;
    }
    return true;
}

int beaker_game(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font){
	bool keyboard_map[4] = { false, false, false, false };
	ALLEGRO_TIMER *FPS;
    block_t block = create_block((char *)"image.jpg");
    if(!block.image){
        al_show_native_message_box(display, "Error", "Error", "Failed to load image.jpg", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }
	al_resize_display(display, SCREEN_WIDTH, SCREEN_HEIGHT);

    ALLEGRO_BITMAP *ball_image = al_load_bitmap("ball.png");
    block_t ball;
    ball.x = al_get_display_height(display) / 2;
    ball.y = al_get_display_width(display) / 2;
    ball.image = ball_image;
    ball.dirty = 0;
    ball.redraw = true;
    ball.h = al_get_bitmap_height(ball.image);
    ball.w = al_get_bitmap_width(ball.image);
    int ball_speed = 1;
    ball.vx = rand() % 3;
    ball.vy = rand() % 3;
    if (ball.vy == 0)
        ball.vy += 1;
    if (ball.vx == 0)
        ball.vx += 1;

    if(!ball.image) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }
    bool collision = false;
    int speed = 3;
    int time = 0;
    
    FPS = al_create_timer(1.0 / 60);
    ALLEGRO_TIMER *ball_timer = al_create_timer(10);
    ALLEGRO_TIMER *score_timer = al_create_timer(1);
    al_register_event_source(event_queue, al_get_timer_event_source(ball_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(FPS));
    al_register_event_source(event_queue, al_get_timer_event_source(score_timer));
    al_start_timer(ball_timer);
    al_start_timer(FPS);
    al_start_timer(score_timer);
    bool flipped = false;

    short res = al_show_native_message_box(display, "Ready?", "Ready to play?", "Do you want to start the game?",
    NULL, ALLEGRO_MESSAGEBOX_YES_NO);
    if(res == 0 || res == 2){
        return 0;
    }
    while(1) {

        ALLEGRO_EVENT ev;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 1.0/60);
        al_wait_for_event_until(event_queue, &ev, &timeout);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
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
            }
        }
        if (ev.type == ALLEGRO_EVENT_KEY_UP){

            switch(ev.keyboard.keycode){
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
        if(ev.timer.source == ball_timer){
            ball_speed += 1;
        }
        if(ev.timer.source == score_timer){
            time += 1;
			al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 20, ALLEGRO_ALIGN_LEFT, "%d", time);
			al_flip_display();
        }
        if(ev.timer.source == FPS && al_event_queue_is_empty(event_queue)){

           if(keyboard_map[KEY_UP]){
               if(!(block.y < 0)) {

                   block.y -= 1 * speed;
                   block.redraw = true;
               }
           }


            if(keyboard_map[KEY_DOWN]){
                if(!(block.y + 75 > al_get_display_height(display))) {
                    block.y += 1 * speed;
                    block.redraw = true;
                }
            }

            if(keyboard_map[KEY_LEFT]){
                if(!(block.x < 0)) {
                    block.x -= 1 * speed;
                    if (flipped) {
                        flipped = false;
                    }

                    block.redraw = true;
                }
            }

            if(keyboard_map[KEY_RIGHT]){
                if(!(block.x + 75 > al_get_display_width(display))){
                    block.x += 1 * speed;
                    if (!flipped) {
                        flipped = true;
                    }
                    block.redraw = true;
                }
            }



            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            if (block.redraw) {

                if (flipped) {

                    al_draw_scaled_bitmap(block.image, 0, 0, block.w, block.h, block.x, block.y,
                                          75, 75, ALLEGRO_FLIP_HORIZONTAL);
                } else {

                    al_draw_scaled_bitmap(block.image, 0, 0, block.w, block.h, block.x, block.y, 75, 75, 0);
                }
                //block.redraw = false;

            }
                if(ball.x > al_get_display_width(display) - 25){
                    ball.vx *= -1;
                }
                if(ball.x < 0){
                    ball.vx *= -1;
                }



            if(ball.y > al_get_display_height(display) - 25){
                    ball.vy *= -1;
                }
                if(ball.y < 0){
                    ball.vy *= -1;
                }

                ball.x += ball.vx * ball_speed;
                ball.y += ball.vy * ball_speed;

                al_draw_scaled_bitmap(ball.image, 0, 0, ball.w, ball.h, ball.x, ball.y, 25, 25, 0);

            collision = check_collision(block, ball);
            if(collision){
                short res = al_show_native_message_box(display, "Dead", "You are dead", "You died. Do you want to play again?",
                NULL, ALLEGRO_MESSAGEBOX_YES_NO);
                if(res == 0 || res == 2){
                    break;
                }else{
                    block.x = 0;
                    block.y = 0;
                    block.vx = 0;
                    block.vy = 0;
                    keyboard_map[KEY_UP] = false;
                    keyboard_map[KEY_DOWN] = false;
                    keyboard_map[KEY_RIGHT] = false;
                    keyboard_map[KEY_LEFT] = false;
                    ball.vx = rand() % ball.vx +1;
                    ball.vy = rand() % ball.vy +1;
                    ball.x = al_get_display_width(display) / 2;
                    ball.y = al_get_display_height(display) / 2;
                    time = 0;
                }
            }

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 20, ALLEGRO_ALIGN_LEFT, "%d", time);
            al_flip_display();
        }
    }

    al_destroy_display(display);
    al_destroy_bitmap(block.image);
    al_destroy_bitmap(ball.image);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(ball_timer);
    al_destroy_timer(score_timer);
    al_destroy_timer(FPS);
    al_destroy_font(font);
    return 0;
    
}

