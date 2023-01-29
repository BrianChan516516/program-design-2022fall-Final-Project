#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"
#include "game.h"
#include "scene_game.h"
extern int score;
/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;
/* shared variables. */
extern ALLEGRO_TIMER* game_tick_timer;
extern ALLEGRO_TIMER* power_up_timer;
extern const int power_up_duration;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;
/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 40;
// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;

Ghost* ghost_create(int flag) {

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case Blinky:
		ghost->objData.Coord.x = cage_grid_x + 1;
		ghost->objData.Coord.y = cage_grid_y + 1;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
    case Pinky:
        ghost->objData.Coord.x = cage_grid_x + 1;
        ghost->objData.Coord.y = cage_grid_y - 1;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case Inky:
        ghost->objData.Coord.x = cage_grid_x - 1;
        ghost->objData.Coord.y = cage_grid_y - 1;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_orange.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case Clyde:
        ghost->objData.Coord.x = cage_grid_x - 1;
        ghost->objData.Coord.y = cage_grid_y + 1;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    case Babb:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/babb_move.png");
        ghost->move_script = &ghost_red_move_script;
        break;
    default:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}

void ghost_destory(Ghost* ghost) {
    al_destroy_bitmap(ghost->move_sprite);
    al_destroy_bitmap(ghost->dead_sprite);
    al_destroy_bitmap(ghost->flee_sprite);
    free(ghost);
}
//ALLEGRO_TIMER* ghost_timer = al_create_timer(1.0f / GAME_TICK_CD / 2);
void ghost_draw(Ghost* ghost) {
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
    
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);
	//Draw default image
	/*al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
		16, 16,
		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
		draw_region, draw_region, 0
	);*/
    
    long long int counter = al_get_timer_count(game_tick_timer)/16;
	// [TODO] below is for animation usage, change the sprite you want to use.
    if (ghost->status == FLEE) {
        if(al_get_timer_count(power_up_timer)*100 < 65*power_up_duration ) {
            al_draw_scaled_bitmap(ghost->flee_sprite,0, 0,
                16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                draw_region, draw_region, 0);
        } else {
            printf("power up timer : %lld\n",al_get_timer_count(power_up_timer));
            al_draw_scaled_bitmap(ghost->flee_sprite, counter%2*32, 0,
                16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                draw_region, draw_region, 0);
        }
    }
    else if (ghost->status == GO_IN) {
    
        int offset_ghost_death = 0;
        switch (ghost->objData.facing) {
        case DOWN:
            offset_ghost_death += 16;
        case UP:
            offset_ghost_death += 16;
        case LEFT:
            offset_ghost_death += 16;
        default:
            break;
        }
        al_draw_scaled_bitmap(ghost->dead_sprite, offset_ghost_death , 0,
            16, 16,
            drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
            draw_region, draw_region, 0
        );
    }
    else {
        int offset_ghost = 0;
        switch (ghost->objData.facing)
        {
        case DOWN:
            offset_ghost += 32;
        case UP:
            offset_ghost += 32;
        case LEFT:
            offset_ghost += 32;
        default:
            break;
        }
        //Draw default image
        al_draw_scaled_bitmap(ghost->move_sprite, offset_ghost + 16 * (ghost->objData.moveCD / 32) % 2, 0,
            16, 16,
            drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
            draw_region, draw_region, 0
        );
    }
}

void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}

void printGhostStatus(GhostStatus S) {
    
	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.
	
	int ghost_probe_x = ghost->objData.Coord.x, ghost_probe_y = ghost->objData.Coord.y;
    
	switch (targetDirec)
	{
	case UP:
        ghost_probe_y--;
        break;
	case DOWN:
        ghost_probe_y++;
        break;
	case LEFT:
        ghost_probe_x--;
        break;
	case RIGHT:
        ghost_probe_x++;
        break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}

	if (is_wall_block(M, ghost_probe_x, ghost_probe_y) || (room && is_room_block(M, ghost_probe_x, ghost_probe_y)))
		return false;

	return true;

}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
	// [TODO]
	// TODO: Here is reserved for power bean implementation.
	// The concept is "When pacman eats the power bean, only
	// ghosts who are in state FREEDOM will change to state FLEE.
	// For those who are not (BLOCK, GO_IN, etc.), they won't change state."
	// This implementation is based on the classic PACMAN game.
	// You are allowed to do your own implementation of power bean system.
    if(setFLEE){
        if(ghost->status == FLEE){
            return;
        }
        ghost->status = FLEE;
        ghost->speed = basic_speed/2;
    } else{
        if(ghost->status == FREEDOM){
            return;
        }
        ghost->status = FREEDOM;
        if(ghost->typeFlag == Babb) {
            ghost->speed = basic_speed/2;
        } else
            ghost->speed = basic_speed;
    }
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE) {
		ghost->status = GO_IN;
		ghost->speed = 4;
        score += 200;
	}
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
    static Directions proba[4]; // possible movement
    int cntflee = 0;
    int movable_dirrection = 0;
    for (Directions j = 1; j <= 4; j++) {
        int unmovable = 0;
        if (unmovable == 3&&j==4)ghost_NextMove(ghost, movable_dirrection);
        if (!ghost_movable(ghost, M, j, 1)) unmovable++;
        else movable_dirrection = j;
    }
    
        for (Directions i = 1; i <= 4; i++)
            if (ghost_movable(ghost, M, i, 1) && i != shortestDirection && ghost->objData.facing + i != 5)    proba[cntflee++] = i;
        ghost_NextMove(ghost, proba[generateRandomNumber(0, cntflee )]);
}

