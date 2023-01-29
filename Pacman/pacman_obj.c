#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "scene_menu.h"
#include "map.h"
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 40;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;
int pac_death_count = -1;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

    int pman_probe_x = pacman->objData.Coord.x, pman_probe_y = pacman->objData.Coord.y;
	
	switch (targetDirec)
	{
	case UP:
        pman_probe_y--;
        break;
	case DOWN:
        pman_probe_y++;
        break;
	case LEFT:
        pman_probe_x--;
        break;
	case RIGHT:
        pman_probe_x++;
        break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
        break;
	}
	if (is_wall_block(M, pman_probe_x, pman_probe_y) || is_room_block(M, pman_probe_x, pman_probe_y))
		return false;
	
	return true;
}

Pacman* pacman_create(void) {
    game_log("still_running!\n");
	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
    /*
		Pacman* pman = ...
		if(!pman)
			return NULL;
	*/
	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
    game_log("still_running!\n");
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;
    game_log("still_running!\n");
	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;
    game_log("still_running!\n");
	pman->death_anim_counter = al_create_timer(1.0f / 64);
	pman->powerUp = false;
	/* load sprites */
	pman->move_sprite = load_bitmap("Assets/pacman_move.png");
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	return pman;
}
Pacman* twinky_create(void) {

    Pacman* twinky = (Pacman*)malloc(sizeof(Pacman));
    twinky->objData.Coord.x = 24;
    twinky->objData.Coord.y = 24;
    twinky->objData.Size.x = block_width;
    twinky->objData.Size.y = block_height;

    twinky->objData.preMove = NONE;
    twinky->objData.nextTryMove = NONE;
    twinky->speed = basic_speed;

    twinky->death_anim_counter = al_create_timer(1.0f / 64);
    twinky->powerUp = false;
    /* load sprites */
    twinky->move_sprite = load_bitmap("Assets/twinky.png");
    twinky->die_sprite = load_bitmap("Assets/twinky_die.png");
    return twinky;
}

Pacman* jelly_create(void) {

    Pacman* jelly = (Pacman*)malloc(sizeof(Pacman));
    jelly->objData.Coord.x = 24;
    jelly->objData.Coord.y = 24;
    jelly->objData.Size.x = block_width;
    jelly->objData.Size.y = block_height;

    jelly->objData.preMove = NONE;
    jelly->objData.nextTryMove = NONE;
    jelly->speed = basic_speed;

    jelly->death_anim_counter = al_create_timer(1.0f / 64);
    jelly->powerUp = false;
    /* load sprites */
    jelly->move_sprite = load_bitmap("Assets/jelly.png");
    jelly->die_sprite = load_bitmap("Assets/jelly_die.png");
    return jelly;
}

void pacman_destroy(Pacman* pman) {
    al_destroy_bitmap(pman->move_sprite);
    al_destroy_bitmap(pman->die_sprite);
    al_destroy_timer(pman->death_anim_counter);
    free(pman);
}


void pacman_draw(Pacman* pman) {
    /*
        [HW-TODO ]
        Draw Pacman and animations
        hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
    */
    RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);
    int offset = 0;
    
    /*al_draw_scaled_bitmap(pman->move_sprite, 0, 0,
        16, 16,
        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
        draw_region, draw_region, 0
    );*/
    
    if (game_over) {
        al_start_timer(pman->death_anim_counter);
        long long int time_count = al_get_timer_count(pman->death_anim_counter);
        al_draw_scaled_bitmap(pman->die_sprite, (time_count/16)*16, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,draw_region, draw_region, 0);
    }
    else {
        switch(pman->objData.facing) {
        case DOWN:
            offset += 32;
        case UP:
            offset += 32;
        case LEFT:
            offset += 32;
        default:
            break;
        }
        //Draw default image
        int i = (pman->objData.moveCD / 32) % 2;
        al_draw_scaled_bitmap(pman->move_sprite, offset + 16 * i, 0,
            16, 16,
            drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
            draw_region, draw_region, 0
        );
    }
}

void pacman_move(Pacman* pacman, Map* M) {
    if (!movetime(pacman->speed))
        return;
    if (game_over)
        return;

    int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
    if (pacman_movable(pacman, M, pacman->objData.nextTryMove))
        pacman->objData.preMove = pacman->objData.nextTryMove;
    else if (!pacman_movable(pacman, M, pacman->objData.preMove))
        return;

    switch (pacman->objData.preMove)
    {
        case UP:
            pacman->objData.Coord.y -= 1;
            pacman->objData.preMove = UP;
            break;
        case DOWN:
            pacman->objData.Coord.y += 1;
            pacman->objData.preMove = DOWN;
            break;
        case LEFT:
            pacman->objData.Coord.x -= 1;
            pacman->objData.preMove = LEFT;
            break;
        case RIGHT:
            pacman->objData.Coord.x += 1;
            pacman->objData.preMove = RIGHT;
            break;
        default:
            break;
    }
    pacman->objData.facing = pacman->objData.preMove;
    pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item) {
    switch (Item)
    {
    case '.':
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
        break;
    case 'P':
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
        //al_rest(1);
        break;
    case 'S':
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
        //al_rest(1);
        break;
    case 'D':
        stop_bgm(PACMAN_MOVESOUND_ID);
        PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
        //al_rest(1);
        break;
    default:
        break;
    }
}

void pacman_NextMove(Pacman* pacman, Directions next) {
    pacman->objData.nextTryMove = next;
}

void pacman_die() {
    stop_bgm(PACMAN_MOVESOUND_ID);
    PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}









