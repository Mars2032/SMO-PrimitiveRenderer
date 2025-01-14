#include "main.h"
#include "al/util.hpp"
#include "al/scene/Scene.h"
#include "game/StageScene/StageScene.h"
#include "game/GameData/GameDataFunction.h"

/*
 * ⨻WARNING!⨻ ->  This project is a mess!
 *                  I tried my best to clean it up but you might still find it confusing.
 *                  So, below is a map of the project! Hoo hoo hoo hee hee hee
 *                  -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 *                              [debugMenu.h]
 *                                    |
 *                                    |---------------                        [module.cpp]
 *                                    |              |
 *                                    |           [main.h]
 *                                    |              |
 *                              |debugMenu.cpp|   |main.cpp|
 *                                 ||     ||      ||   -YOU ARE HERE-
 *                                 ||     ||      ||
 *                       [helpers.cpp]   |drawer.cpp|
 *                  -----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
 *
 * Explanation of sead::PrimitiveRenderer
 *    There is an instance of sead::PrimitiveRenderer setup in RootTask::prepare, but it is never used.
 *
 *    In debugMenu.cpp, we get the instance and set the needed components to make it work.
 *    In any function in the game, we should be able to just do something like this:
 *
 *    sead::PrimitiveRenderer *renderer = sead::PrimitiveRenderer::instance();
 *    if (renderer->mProjection) {
 *        renderer->begin();
 *        < draw here >
 *        renderer->end();
 *    }
 */

// Toggles for showing the debug menu (TextWriter) and different areas (PrimitiveRenderer)
bool showMenu = true;
bool showAreas = false;
bool show2DMoveAreas = true;
bool showWaterAreas = true;
bool showDeathAreas = true;
bool showWarpAreas = true;
bool showMissRestartAreas = true;
bool showRaceCourseOutAreas = true;
bool showChangeStageAreas = true;
bool showCameraAreas = true;
bool showTransparentWalls = true;
bool enableFadeoff = true;
bool showHitSensors = true;
bool isShowEyes = true;
bool isShowAttacks = true;
bool isInScene = false;

bool isInGame = false;

int timer = 120;

void stageSceneControl(StageScene* stageScene)
{
    __asm("MOV X19, X0"); // Execute the instruction we just replaced

    isInGame = true;
    isInScene = true;

    /*
     * Inputs
     * D-UP     > Toggle menu
     * D-LEFT   > Toggle current rendering
     * D-RIGHT  > Toggle camera areas
     *
     * (L) D-UP     > Toggle change areas
     * (L) D-LEFT   > Toggle water areas
     * (L) D-RIGHT  > Toggle death areas
     *
     * (R) D-UP     > Toggle 2DMove areas
     * (R) D-LEFT   > Toggle TransparentWalls
     * (R) D-RIGHT  > Toggle race miss areas
     *
     * (L+R) D-LEFT > Toggle alpha fade-off
     */
    if (al::isPadHoldL(-1) && al::isPadHoldR(-1)) {
        if (al::isPadTriggerLeft(-1))
            enableFadeoff = !enableFadeoff;
    } else if (al::isPadHoldL(-1)) {
        if (al::isPadTriggerUp(-1)) {
            showChangeStageAreas = !showChangeStageAreas;
            showWarpAreas = !showWarpAreas;
        }
        if (al::isPadTriggerLeft(-1))
            showWaterAreas = !showWaterAreas;
        if (al::isPadTriggerRight(-1))
            showDeathAreas = !showDeathAreas;
    } else if (al::isPadHoldR(-1)) {
        if (al::isPadTriggerUp(-1))
            show2DMoveAreas = !show2DMoveAreas;
        if (al::isPadTriggerLeft(-1))
            showTransparentWalls = !showTransparentWalls;
        if (al::isPadTriggerRight(-1))
            showRaceCourseOutAreas = !showRaceCourseOutAreas;
    } else {
        if (al::isPadTriggerUp(-1))
            showMenu = !showMenu;
        if (al::isPadTriggerLeft(-1)) {
            showAreas = !showAreas;
            showHitSensors = !showHitSensors;
            isShowEyes = !isShowEyes;
            isShowAttacks = !isShowAttacks;
        }
        if (al::isPadTriggerRight(-1)) {
            showCameraAreas = !showCameraAreas;
            isShowAttacks = !isShowAttacks;
        }
    }
    if (timer > 0) timer--;
    
}

HOOK_ATTR
bool sceneKillHook(GameDataHolderAccessor value)
{
    timer = 120;
    isInScene = false;
    return GameDataFunction::isMissEndPrevStageForSceneDead(value);
}