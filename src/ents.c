#include "ents.h"

#include "asset_indexes.h"

const ZF4Vec2D i_playerOrigin = {0.5f, 0.5f};
const float i_playerMoveSpd = 2.0f;
const float i_playerHorVelLerp = 0.3f;

void proc_player_movement(Player* const player, const float grav, const Tilemap* const tilemap, const ZF4Textures* const textures) {
    const int moveAxis = zf4_is_key_down(ZF4_KEY_D) - zf4_is_key_down(ZF4_KEY_A);
    const float horVelTarg = moveAxis * i_playerMoveSpd;
    player->core.vel.x = zf4_lerp(player->core.vel.x, horVelTarg, i_playerHorVelLerp);

    player->core.vel.y += grav;

    const ZF4RectF collider = create_player_collider(player, textures);

    if (zf4_is_key_down(ZF4_KEY_SPACE)) {
        const ZF4RectF colliderDown = {
            collider.x,
            collider.y + 1.0f,
            collider.width,
            collider.height
        };

        if (get_tile_collision(&colliderDown, tilemap)) {
            player->core.vel.y = -7.0f;
        }
    } else {
        player->core.vel.y = ZF4_MAX(0.0f, player->core.vel.y);
    }

    proc_tilemap_collisions(&player->core.vel, &collider, tilemap);

    player->core.pos.x += player->core.vel.x;
    player->core.pos.y += player->core.vel.y;
}

void write_player_to_sprite_batch(ZF4Renderer* const renderer, const int layerIndex, const Player* const player, const ZF4Textures* const textures) {
    const ZF4SpriteBatchWriteInfo writeInfo = {
        .texIndex = 0,
        .pos = player->core.pos,
        .srcRect = {0, 0, textures->sizes[PLAYER_TEX].x, textures->sizes[PLAYER_TEX].y},
        .origin = i_playerOrigin,
        .rot = 0.0f,
        .scale = {1.0f, 1.0f},
        .alpha = 1.0f
    };

    zf4_write_to_sprite_batch(renderer, layerIndex, &writeInfo, textures);
}

ZF4RectF create_player_collider(const Player* const player, const ZF4Textures* const textures) {
    const ZF4Pt2D texSize = textures->sizes[PLAYER_TEX];
    return zf4_create_rect_f(
        player->core.pos.x - (texSize.x * i_playerOrigin.x),
        player->core.pos.y - (texSize.y * i_playerOrigin.y),
        texSize.x,
        texSize.y
    );
}
