#ifndef WORLD_H
#define WORLD_H

#include <zf4.h>
#include "tilemap.h"
#include "ents.h"

typedef enum {
    TILEMAP_WORLD_RENDER_LAYER,
    ENTS_WORLD_RENDER_LAYER,
    UI_WORLD_RENDER_LAYER,

    WORLD_RENDER_LAYER_CNT
} WorldRenderLayer;

typedef struct {
    Tilemap tilemap;
    Player player;
} World;

void init_world_render_layer_props(ZF4RenderLayerProps* const props, const int layerIndex);
bool init_world(ZF4Scene* const scene, const ZF4GamePtrs* const gamePtrs);
bool world_tick(ZF4Scene* const scene, int* const sceneChangeIndex, const ZF4GamePtrs* const gamePtrs);

#endif
