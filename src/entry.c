#include <zf4.h>
#include "scene_types.h"
#include "title_screen.h"
#include "world.h"

static void load_scene_type_info(ZF4SceneTypeInfo* const typeInfo, const int typeIndex) {
    switch (typeIndex) {
        case TITLE_SCREEN_SCENE:
            typeInfo->memArenaSize = ZF4_MEGABYTES(2);
            typeInfo->renderLayerCnt = TITLE_SCREEN_RENDER_LAYER_CNT;
            typeInfo->camRenderLayerCnt = 0;
            typeInfo->renderLayerPropsInitializer = init_title_screen_render_layer_props;
            typeInfo->init = init_title_screen;
            typeInfo->tick = title_screen_tick;
            break;

        case WORLD_SCENE:
            typeInfo->memArenaSize = ZF4_MEGABYTES(4);
            typeInfo->renderLayerCnt = WORLD_RENDER_LAYER_CNT;
            typeInfo->camRenderLayerCnt = UI_WORLD_RENDER_LAYER;
            typeInfo->renderLayerPropsInitializer = init_world_render_layer_props;
            typeInfo->init = init_world;
            typeInfo->tick = world_tick;
            break;
    }
}

int main() {
    ZF4SceneTypeInfo sceneTypeInfos[SCENE_TYPE_CNT];

    const ZF4UserGameInfo userGameInfo = {
        .windowInitWidth = 1280,
        .windowInitHeight = 720,
        .windowTitle = "Example ZF4 Game",
        .windowResizable = true,
        .windowHideCursor = true,

        .sceneTypeInfoLoader = load_scene_type_info,
        .sceneTypeCnt = SCENE_TYPE_CNT
    };

    zf4_start_game(&userGameInfo);
}
