//entities.h

#ifndef ENTITIES_H
#define ENTITIES_H

#include "vec2.h"

#define PLAYER_MAX_X    100
#define PLAYER_MIN_X    80
#define PLAYER_TOP_Y    0
#define PLAYER_BOT_Y    50

#define ENEMY_TOP_Y     2
#define ENEMY_BOT_Y     50
#define ENEMY_STEP_X    13
#define ENEMY_STEP_Y    5

#define BULLET_SPEED    12
#define BULLET_MIN_X    5

typedef struct{
    Vec2 pos;
} Player;

typedef struct{
    Vec2 pos;
    int vdir;               //-1 = up, 1 = down
    int reached_y_edge;    //1 for top, 0 for not reached, -1 for bottom
    int is_active;
} Enemy;

typedef struct{
    Vec2 pos;
    int is_active;
} Bullet;

int check_collisionBE(Bullet b, Enemy e);

#endif