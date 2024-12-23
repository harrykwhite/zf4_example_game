#include "tilemap.h"

#include <stdalign.h>
#include "asset_indexes.h"

bool push_tilemap(Tilemap* const tilemap, ZF4MemArena* const memArena, const ZF4Pt2D size) {
    assert(zf4_is_zero(tilemap, sizeof(*tilemap)));
    assert(size.x > 0 && size.y > 0);

    tilemap->size = size;

    tilemap->activity = zf4_push_to_mem_arena(memArena, ZF4_BITS_TO_BYTES(tilemap->size.x * tilemap->size.y), alignof(ZF4Byte));

    if (!tilemap->activity) {
        return false;
    }

    return true;
}

void write_tilemap_to_sprite_batch(ZF4Renderer* const renderer, const int layerIndex, const Tilemap* const tilemap, const ZF4Textures* const textures) {
    const ZF4Vec2D camTopLeft = zf4_get_cam_top_left(&renderer->cam);
    const ZF4Vec2D camBottomRight = zf4_get_cam_bottom_right(&renderer->cam);

    const ZF4Pt2D writeBeginPos = {
        ZF4_CLAMP(camTopLeft.x / TILE_SIZE, 0, tilemap->size.x - 1),
        ZF4_CLAMP(camTopLeft.y / TILE_SIZE, 0, tilemap->size.y - 1)
    };

    const ZF4Pt2D writeEndPos = {
        ZF4_CLAMP(ceilf((float)camBottomRight.x / TILE_SIZE), 0, tilemap->size.x),
        ZF4_CLAMP(ceilf((float)camBottomRight.y / TILE_SIZE), 0, tilemap->size.y)
    };

    ZF4SpriteBatchWriteInfo writeInfo = {
        .texIndex = TILES_TEX,
        .srcRect = {0, 0, textures->sizes[TILES_TEX].x, textures->sizes[TILES_TEX].y},
        .origin = {0.0f, 0.0f},
        .rot = 0.0f,
        .scale = {1.0f, 1.0f},
        .alpha = 1.0f
    };

    for (int y = writeBeginPos.y; y < writeEndPos.y; ++y) {
        for (int x = writeBeginPos.x; x < writeEndPos.x; ++x) {
            const int tileIndex = tile_pos_to_index(tilemap, (ZF4Pt2D) { x, y });

            if (zf4_is_bit_active(tilemap->activity, tileIndex)) {
                writeInfo.pos.x = x * TILE_SIZE;
                writeInfo.pos.y = y * TILE_SIZE;
                zf4_write_to_sprite_batch(renderer, layerIndex, &writeInfo, textures);
            }
        }
    }
}

bool get_tile_collision(const ZF4RectF* const collider, const Tilemap* const tilemap) {
    const ZF4Pt2D tileCheckBeginPos = {
        ZF4_CLAMP(collider->x / TILE_SIZE, 0, tilemap->size.x - 1),
        ZF4_CLAMP(collider->y / TILE_SIZE, 0, tilemap->size.y - 1)
    };

    const ZF4Pt2D tileCheckEndPos = {
        ZF4_CLAMP(ceilf(zf4_get_rect_f_right(collider) / TILE_SIZE), 0, tilemap->size.x),
        ZF4_CLAMP(ceilf(zf4_get_rect_f_bottom(collider) / TILE_SIZE), 0, tilemap->size.y)
    };

    for (int y = tileCheckBeginPos.y; y < tileCheckEndPos.y; ++y) {
        for (int x = tileCheckBeginPos.x; x < tileCheckEndPos.x; ++x) {
            if (!is_tile_present(tilemap, (ZF4Pt2D) { x, y })) {
                continue;
            }

            const ZF4RectF tileCollider = get_tile_collider((ZF4Pt2D) { x, y });

            if (zf4_do_rect_fs_intersect(collider, &tileCollider)) {
                return true;
            }
        }
    }

    return false;
}

void proc_tilemap_collisions(ZF4Vec2D* const vel, const ZF4RectF* const collider, const Tilemap* const tilemap) {
    const ZF4RectF horCollider = zf4_create_rect_f(collider->x + vel->x, collider->y, collider->width, collider->height);

    if (get_tile_collision(&horCollider, tilemap)) {
        vel->x = 0.0f;
    }

    const ZF4RectF verCollider = zf4_create_rect_f(collider->x, collider->y + vel->y, collider->width, collider->height);

    if (get_tile_collision(&verCollider, tilemap)) {
        vel->y = 0.0f;
    }

    if (vel->x && vel->y) {
        const ZF4RectF diagCollider = zf4_create_rect_f(collider->x + vel->x, collider->y + vel->y, collider->width, collider->height);

        if (get_tile_collision(&diagCollider, tilemap)) {
            vel->x = 0.0f;
        }
    }
}
