#include "world.h"

#include <stdalign.h>

const ZF4Vec3D i_bgColor = {0.59f, 0.79f, 0.93f};
const ZF4Pt2D i_tilemapSize = {128, 72};
const float i_camPosLerp = 0.3f;
const float i_grav = 0.3f;

static inline const int get_world_width() {
    return i_tilemapSize.x * TILE_SIZE;
}

static inline const int get_world_height() {
    return i_tilemapSize.y * TILE_SIZE;
}

void init_world_render_layer_props(ZF4RenderLayerProps* const props, const int layerIndex) {
    switch (layerIndex) {
        case TILEMAP_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 2;
            props->charBatchCnt = 0;
            break;

        case ENTS_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 0;
            break;

        case UI_WORLD_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 1;
            break;
    }
}

bool init_world(ZF4Scene* const scene, const ZF4GamePtrs* const gamePtrs) {
    scene->renderer.cam.scale = 2.0f;
    scene->renderer.bgColor = i_bgColor;

    World* const world = zf4_push_to_mem_arena(&scene->memArena, sizeof(*world), alignof(World));

    if (!world) {
        return false;
    }

    scene->miscPtr = world;

    if (!push_tilemap(&world->tilemap, &scene->memArena, i_tilemapSize)) {
        return false;
    }

    for (int y = world->tilemap.size.y / 2; y < world->tilemap.size.y; ++y) {
        for (int x = 0; x < world->tilemap.size.x; ++x) {
            place_tile(&world->tilemap, (ZF4Pt2D) { x, y });
        }
    }

    world->player.core.pos.x = get_world_width() / 2.0f;
    scene->renderer.cam.pos = world->player.core.pos;

    return true;
}

bool world_tick(ZF4Scene* const scene, int* const sceneChangeIndex, const ZF4GamePtrs* const gamePtrs) {
    World* const world = scene->miscPtr;

    proc_player_movement(&world->player, i_grav, &world->tilemap, &gamePtrs->assets->textures);
    write_player_to_sprite_batch(&scene->renderer, ENTS_WORLD_RENDER_LAYER, &world->player, &gamePtrs->assets->textures);

    scene->renderer.cam.pos.x = zf4_lerp(scene->renderer.cam.pos.x, world->player.core.pos.x, i_camPosLerp);
    scene->renderer.cam.pos.y = zf4_lerp(scene->renderer.cam.pos.y, world->player.core.pos.y, i_camPosLerp);

    write_tilemap_to_sprite_batch(&scene->renderer, TILEMAP_WORLD_RENDER_LAYER, &world->tilemap, &gamePtrs->assets->textures);

    return true;
}
