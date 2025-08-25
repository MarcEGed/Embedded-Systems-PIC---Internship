/*
    Contains collision system 
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "entities.h"
#include "vec2.h"
#include "sprites.h"

int check_collisionBE(Bullet b, Enemy e){
    return (b.pos.x <= e.pos.x + SPRITE_SIZE - 1) &&
       (b.pos.x + SPRITE_SIZE - 1 >= e.pos.x) &&
       (b.pos.y <= e.pos.y + SPRITE_SIZE - 1) &&
       (b.pos.y + SPRITE_SIZE - 1 >= e.pos.y);
}