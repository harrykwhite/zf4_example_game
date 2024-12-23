#ifndef ENTS_H
#define ENTS_H

#include <zf4.h>
#include "tilemap.h"

typedef struct {
    ZF4Vec2D pos;
    ZF4Vec2D vel;
} EntCore;

typedef struct {
    EntCore core;
} Player;

typedef struct {
    EntCore core;
} Enemy;

typedef struct {
    float grav;
} PhysicsProps;

void proc_player_movement(Player* const player, const float grav, const Tilemap* const tilemap, const ZF4Textures* const textures);
void write_player_to_sprite_batch(ZF4Renderer* const renderer, const int layerIndex, const Player* const player, const ZF4Textures* const textures);
ZF4RectF create_player_collider(const Player* const player, const ZF4Textures* const textures);

#endif
