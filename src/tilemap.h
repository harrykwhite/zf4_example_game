#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdbool.h>
#include <zf4.h>

#define TILE_SIZE 16

typedef struct {
    ZF4Byte* activity; // Each bit represents whether the tile of that index is active.
    ZF4Pt2D size;
} Tilemap;

bool push_tilemap(Tilemap* const tilemap, ZF4MemArena* const memArena, const ZF4Pt2D size);
void write_tilemap_to_sprite_batch(ZF4Renderer* const renderer, const int layerIndex, const Tilemap* const tilemap, const ZF4Textures* const textures);
bool get_tile_collision(const ZF4RectF* const collider, const Tilemap* const tilemap);
void proc_tilemap_collisions(ZF4Vec2D* const vel, const ZF4RectF* const collider, const Tilemap* const tilemap);

inline int tile_pos_to_index(const Tilemap* const tilemap, const ZF4Pt2D pos) {
    assert(pos.x >= 0 && pos.y >= 0 && pos.x < tilemap->size.x && pos.y < tilemap->size.y);
    return (pos.y * tilemap->size.x) + pos.x;
}

inline bool is_tile_present(const Tilemap* const tilemap, const ZF4Pt2D pos) {
    assert(pos.x >= 0 && pos.y >= 0 && pos.x < tilemap->size.x && pos.y < tilemap->size.y);
    const int tileIndex = tile_pos_to_index(tilemap, pos);
    return zf4_is_bit_active(tilemap->activity, tileIndex);
}

inline void place_tile(Tilemap* const tilemap, const ZF4Pt2D pos) {
    assert(pos.x >= 0 && pos.y >= 0 && pos.x < tilemap->size.x && pos.y < tilemap->size.y);
    assert(!is_tile_present(tilemap, pos));
    const int tileIndex = tile_pos_to_index(tilemap, pos);
    zf4_activate_bit(tilemap->activity, tileIndex);
}

inline void remove_tile(Tilemap* const tilemap, const ZF4Pt2D pos) {
    assert(pos.x >= 0 && pos.y >= 0 && pos.x < tilemap->size.x && pos.y < tilemap->size.y);
    assert(is_tile_present(tilemap, pos));
    const int tileIndex = tile_pos_to_index(tilemap, pos);
    zf4_deactivate_bit(tilemap->activity, tileIndex);
}

inline ZF4RectF get_tile_collider(const ZF4Pt2D pos) {
    return zf4_create_rect_f(pos.x * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

#endif
