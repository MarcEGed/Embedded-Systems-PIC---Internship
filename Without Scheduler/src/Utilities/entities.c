/*
    Contains collision system 
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "entities.h"
#include "vec2.h"
#include "sprites.h"

int check_collisionBE(Bullet b, Enemy e){
    return (b.pos.x < e.pos.x + SPRITE_SIZE) &&
    (b.pos.x + SPRITE_SIZE > e.pos.x) &&
    (b.pos.y < e.pos.y + SPRITE_SIZE) &&
    (b.pos.y + SPRITE_SIZE > e.pos.y);
}

int check_collisionPE(Player p, Enemy e){
    return (p.pos.x < e.pos.x + SPRITE_SIZE) &&
    (p.pos.x + SPRITE_SIZE > e.pos.x) &&
    (p.pos.y < e.pos.y + SPRITE_SIZE) &&
    (p.pos.y + SPRITE_SIZE > e.pos.y);
}