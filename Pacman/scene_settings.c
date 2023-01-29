// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include <allegro5/allegro_primitives.h>
#include "scene_settings.h"
#include "map.h"
#include "game.h"
#include "shared.h"
#include "scene_menu.h"
#include "scene_menu_object.h"
extern ALLEGRO_SAMPLE* gameMusic;
//static ALLEGRO_BITMAP* one_ghost = NULL;
//static ALLEGRO_BITMAP* one_pacman1 = NULL;
//static ALLEGRO_BITMAP* one_pacman2 = NULL;
//static ALLEGRO_BITMAP* one_pacman3 = NULL;
static ALLEGRO_BITMAP* BACK = NULL;
int x_stay = 400;
bool volume = false;
bool charactor_one = true;
bool charactor_two = false;
bool charactor_three = false;
static Button back;
static Button vol;
static Button vol_plus;
static Button vol_minus;
static Button background;
static Button character;
static Button p;
static Button t;
static Button j;
static Button temp;
bool choose_pacman = true;
bool choose_twinky = false;
bool choose_jelly = false;

static ALLEGRO_SAMPLE_ID settingBGM;
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init(void) {
    background = button_create(0, 0, 1200, 1200, "Assets/setting_background.jpg", "Assets/setting_background.jpg");
    vol = button_create(315, 0, 600, 600, "Assets/volume_title.png", "Assets/volume_title.png");
    vol_plus = button_create(805, 220, 120, 120, "Assets/v_plus.png", "Assets/v_plus_clicked.png");
    vol_minus = button_create(290, 235, 95, 95, "Assets/v_minus.png", "Assets/v_minus_clicked.png");
    character = button_create(250, 95, 700, 700, "Assets/character.png", "Assets/character.png");
    p = button_create(385, 520, 404, 80, "Assets/choose_p.png", "Assets/chosen_p.png");
    t = button_create(385, 630, 323, 80, "Assets/choose_t.png", "Assets/chosen_t.png");
    j = button_create(385, 735, 246, 80, "Assets/choose_j.png", "Assets/chosen_j.png");
    back = button_create(520, 930, 150, 150, "Assets/back_1.png", "Assets/back_2.png");
    volume = false;
    //stop_bgm(settingBGM);
    //settingBGM = play_bgm(gameMusic, music_volume);
}
static void init_success(void) {
    vol = button_create(0, 0, 1200, 1200, "Assets/win_pic.png", "Assets/win_pic.png");
    back = button_create(300, 905, 620, 100, "Assets/backtomenu.png", "Assets/backtomenu2.png");
}
static void init_failed(void) {
    vol = button_create(0, 0, 1200, 1200, "Assets/lose_pic.png", "Assets/lose_pic.png");
    back = button_create(300, 905, 620, 100, "Assets/backtomenu.png", "Assets/backtomenu2.png");
}

static void draw_failed(void ) {
    al_clear_to_color(al_map_rgb(60, 60, 60));
    drawButton(vol);
    drawButton(back);
}

static void draw_success(void ) {
    al_clear_to_color(al_map_rgb(60, 60, 60));
    drawButton(vol);
    drawButton(back);
}

static void draw(void ) {
    al_clear_to_color(al_map_rgb(60, 60, 60));
    drawButton(background);
    drawButton(back);
    drawButton(vol);
    drawButton(vol_plus);
    drawButton(vol_minus);
    drawButton(character);
    drawButton(p);
    drawButton(t);
    drawButton(j);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    back.hovered = pnt_in_rect(mouse_x, mouse_y, back.body);
    back.hovered = buttonHover(back, mouse_x, mouse_y);
    
    vol_plus.hovered = pnt_in_rect(mouse_x, mouse_y, vol_plus.body);
    vol_plus.hovered = buttonHover(vol_plus, mouse_x, mouse_y);
    
    vol_minus.hovered = pnt_in_rect(mouse_x, mouse_y, vol_minus.body);
    vol_minus.hovered = buttonHover(vol_minus, mouse_x, mouse_y);
    
    p.hovered = pnt_in_rect(mouse_x, mouse_y, p.body);
    p.hovered = buttonHover(p, mouse_x, mouse_y);
    
    t.hovered = pnt_in_rect(mouse_x, mouse_y, t.body);
    t.hovered = buttonHover(t, mouse_x, mouse_y);
    
    j.hovered = pnt_in_rect(mouse_x, mouse_y, j.body);
    j.hovered = buttonHover(j, mouse_x, mouse_y);
    
}

static void on_mouse_down() {
    //stop_bgm(settingBGM);
    if (back.hovered)
        game_change_scene(scene_menu_create());
    if (vol_plus.hovered) {
        music_volume += 0.15;
        effect_volume += 0.15;
        if(music_volume > 1.5) music_volume = 1.5;
        if(effect_volume > 1.5) effect_volume = 1.5;
    }
    if (vol_minus.hovered) {
        music_volume -= 0.15;
        effect_volume -= 0.15;
        if(music_volume < 0) music_volume = 0;
        if(effect_volume < 0) effect_volume = 0;
    }
    if(p.hovered) {
        //p.hovered = pnt_in_rect(mouse_x, mouse_y, p.body);
        /*if(temp != NULL)
            free(temp);*/
        al_destroy_bitmap(p.default_img);
        al_destroy_bitmap(p.hovered_img);
        al_destroy_bitmap(t.default_img);
        al_destroy_bitmap(t.hovered_img);
        al_destroy_bitmap(j.default_img);
        al_destroy_bitmap(j.hovered_img);
        choose_pacman = true;
        choose_jelly = false;
        choose_twinky = false;
        p = button_create(385, 520, 404, 80, "Assets/chosen_p.png", "Assets/chosen_p.png");
        t = button_create(385, 630, 323, 80, "Assets/choose_t.png", "Assets/choose_t.png");
        j = button_create(385, 735, 246, 80, "Assets/choose_j.png", "Assets/choose_j.png");
        drawButton(p);
        drawButton(t);
        drawButton(j);
    }
    if(t.hovered) {
        //t.hovered = pnt_in_rect(mouse_x, mouse_y, t.body);
        /*if(temp != NULL)
            free(temp);*/
        al_destroy_bitmap(p.default_img);
        al_destroy_bitmap(p.hovered_img);
        al_destroy_bitmap(t.default_img);
        al_destroy_bitmap(t.hovered_img);
        al_destroy_bitmap(j.default_img);
        al_destroy_bitmap(j.hovered_img);
        choose_pacman = false;
        choose_jelly = false;
        choose_twinky = true;
        p = button_create(385, 520, 404, 80, "Assets/choose_p.png", "Assets/choose_p.png");
        t = button_create(385, 630, 323, 80, "Assets/chosen_t.png", "Assets/chosen_t.png");
        j = button_create(385, 735, 246, 80, "Assets/choose_j.png", "Assets/choose_j.png");
        drawButton(p);
        drawButton(t);
        drawButton(j);
    }
    if(j.hovered) {
        //j.hovered = pnt_in_rect(mouse_x, mouse_y, t.body);
        /*if(temp != NULL)
            free(temp);**/
        al_destroy_bitmap(p.default_img);
        al_destroy_bitmap(p.hovered_img);
        al_destroy_bitmap(t.default_img);
        al_destroy_bitmap(t.hovered_img);
        al_destroy_bitmap(j.default_img);
        al_destroy_bitmap(j.hovered_img);
        choose_pacman = false;
        choose_jelly = true;
        choose_twinky = false;
        p = button_create(385, 520, 404, 80, "Assets/choose_p.png", "Assets/choose_p.png");
        t = button_create(385, 630, 323, 80, "Assets/choose_t.png", "Assets/choose_t.png");
        j = button_create(385, 735, 246, 80, "Assets/chosen_j.png", "Assets/chosen_j.png");
        drawButton(p);
        drawButton(t);
        drawButton(j);
    }
    //stop_bgm(settingBGM);
    //settingBGM = play_bgm(gameMusic, music_volume);//causing error
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init;
    scene.name = "Settings";
    scene.draw = &draw;
    stop_bgm(settingBGM);
    // TODO: Register more event callback functions such as keyboard, mouse, ...
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_move = &on_mouse_move;
    game_log("Settings scene created");
    return scene;
}

Scene scene_failed_create(void) {

    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init_failed;
    scene.draw = &draw_failed;
    scene.name = "failed";
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_move = &on_mouse_move;
    game_log("failed scene created");
    return scene;
}

Scene scene_success_create(void) {

    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init_success;
    scene.draw = &draw_success;
    scene.name = "success";
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_move = &on_mouse_move;
    game_log("success scene created");
    return scene;
}
