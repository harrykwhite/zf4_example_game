#include "title_screen.h"

#include <stdalign.h>
#include "asset_indexes.h"
#include "scene_types.h"

static ZF4Vec2D get_title_pos() {
    return (ZF4Vec2D) { zf4_get_window_size().x / 2.0f, zf4_get_window_size().y / 2.0f };
}

void init_title_screen_render_layer_props(ZF4RenderLayerProps* const props, const int layerIndex) {
    switch (layerIndex) {
        case GENERAL_TITLE_SCREEN_RENDER_LAYER:
            props->spriteBatchCnt = 1;
            props->charBatchCnt = 1;
            break;
    }
}

bool init_title_screen(ZF4Scene* const scene, const ZF4GamePtrs* const gamePtrs) {
    TitleScreen* const titleScreen = zf4_push_to_mem_arena(&scene->memArena, sizeof(*titleScreen), alignof(TitleScreen));

    if (!titleScreen) {
        return false;
    }

    scene->miscPtr = titleScreen;

    titleScreen->titleCBID = zf4_activate_any_char_batch(&scene->renderer, GENERAL_TITLE_SCREEN_RENDER_LAYER, 32, EB_GARAMOND_72_FONT, get_title_pos());
    zf4_write_to_char_batch(&scene->renderer, titleScreen->titleCBID, "ZF4 Example Game", ZF4_FONT_HOR_ALIGN_CENTER, ZF4_FONT_VER_ALIGN_CENTER, &gamePtrs->assets->fonts);

    return true;
}

bool title_screen_tick(ZF4Scene* const scene, int* const sceneChangeIndex, const ZF4GamePtrs* const gamePtrs) {
    TitleScreen* const titleScreen = scene->miscPtr;

    if (zf4_is_key_down(ZF4_KEY_ENTER)) {
        *sceneChangeIndex = WORLD_SCENE;
    }

    return true;
}
