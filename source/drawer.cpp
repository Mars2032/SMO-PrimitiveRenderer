#include "drawer.h"

bool tryDrawAreaGroup(al::Scene* curScene, char const* area, bool isDrawSolid = false, sead::Color4f wire = sead::Color4f(0, 255, 0, .1), sead::Color4f solid = sead::Color4f(0, 255, 0, .1), sead::Color4f cyl = sead::Color4f(0, 255, 0, .1))
{
    if (!curScene)
        return false;
    sead::PrimitiveRenderer* renderer = sead::PrimitiveRenderer::instance();
    if (!renderer)
        return false;

    al::AreaObjGroup* group = al::getSceneAreaObjDirector(curScene)->getAreaObjGroup(area);
    if (!group)
        return false;
    for (int i = 0; i < group->mMaxCount; i++) {
        renderer->begin();
        al::AreaObj* area = group->mAreas[i];
        const char* shapeType;
        al::tryGetAreaObjStringArg(&shapeType, area, "ModelName");

        sead::Vector3f scale = area->mAreaShape->mScale;
        sead::Vector3f pos;
        pos.x = area->mAreaMtx.m[0][3];
        pos.y = area->mAreaMtx.m[1][3];
        pos.z = area->mAreaMtx.m[2][3];
        renderer->setModelMatrix(area->mAreaMtx);

        al::PlayerHolder* pHolder = al::getScenePlayerHolder(curScene);
        PlayerActorHakoniwa* player = al::tryGetPlayerActor(pHolder, 0);

        if (isDrawSolid)
            wire = solid;
        // negatives mess this up. fix this
        if (player) {
            wire.a = calcFadeoff(player, pos);
            cyl.a = calcFadeoff(player, pos);
        }
        if (al::isEqualString(shapeType, "AreaCubeBase")) { // origin is at the bottom

            sead::PrimitiveDrawer::CubeArg shapeArea(sead::Vector3f(0, (scale.y / 2 * 1000), 0), scale * 1000.0f, wire);
            if (isDrawSolid)
                renderer->drawCube(shapeArea);
            renderer->drawWireCube(shapeArea);
        }
        if (al::isEqualString(shapeType, "AreaCubeCenter")) {
            sead::PrimitiveDrawer::CubeArg shapeArea(sead::Vector3f(0, 0, 0), scale * 1000.0f, wire);
            if (isDrawSolid)
                renderer->drawCube(shapeArea);
            renderer->drawWireCube(shapeArea);
        }
        if (al::isEqualString(shapeType, "AreaCubeTop")) {
            sead::PrimitiveDrawer::CubeArg shapeArea(sead::Vector3f(0, -(scale.y / 2 * 1000), 0), scale * 1000.0f, wire);
            if (isDrawSolid)
                renderer->drawCube(shapeArea);
            renderer->drawWireCube(shapeArea);
        }

        // if (player) cyl.a = .7-(5000/al::calcDistance(player, pos));
        if (al::isEqualString(shapeType, "AreaSphere")) {
            renderer->drawSphere8x16(sead::Vector3f(0, 0, 0), scale.x * 1000, cyl);
        }
        if (al::isEqualString(shapeType, "AreaCylinder")) { // origin is at the bottom
            renderer->drawCylinder32(sead::Vector3f(0, (scale.y / 2 * 1000), 0), scale.x * 1000, scale.y * 1000, cyl);
        }
        if (al::isEqualString(shapeType, "AreaCylinderCenter")) {
            renderer->drawCylinder32(sead::Vector3f(0, 0, 0), scale.x * 1000, scale.y * 1000, cyl);
        }
        if (al::isEqualString(shapeType, "AreaCylinderTop")) {
            renderer->drawCylinder32(sead::Vector3f(0, -(scale.y / 2 * 1000), 0), scale.x * 1000, scale.y * 1000, cyl);
        }
        renderer->end();
    }
    return true;
}

bool tryDrawActorHitsensors(al::LiveActor* actor, 
    sead::Color4f body = sead::Color4f(0, 1, 0, .4), sead::Color4f eye = sead::Color4f(0, 1, 0, .4), sead::Color4f attack = sead::Color4f(0,1,1,0.25)) {
    sead::PrimitiveRenderer* renderer = sead::PrimitiveRenderer::instance();
    if (!renderer) return false;

    if (actor->mHitSensorKeeper) {
        PlayerActorHakoniwa* player = al::getPlayerActor(actor, 0);
        if (al::calcDistance(player, actor) < 5000.0f) {
            al::HitSensor** sensors = actor->mHitSensorKeeper->mSensors;
            int sensorNum = actor->mHitSensorKeeper->mSensorNum;
            for (int i = 0; i < sensorNum; i++) {
                renderer->begin();
                sead::Vector3f* pos = al::getSensorPos(sensors[i]);
                
                float radius = al::getSensorRadius(sensors[i]);
                const char* sensorName = sensors[i]->mName;
                renderer->setModelMatrix(sead::Matrix34f::ident);
                if (al::isSensorValid(sensors[i])) {
                    if (al::isSensorPlayerAttack(sensors[i]) || al::isSensorEnemyAttack(sensors[i]) || al::isEqualString(sensorName, "Attack")) {
                        if (isShowAttacks) {
                            renderer->drawSphere4x8(*pos, radius, attack);
                            renderer->end();
                        } else {
                            renderer->end();
                        }
                    } else if (al::isSensorEye(sensors[i]) || al::isSensorPlayerEye(sensors[i])) {
                        if (isShowEyes) {
                            renderer->drawCircle32(*pos, radius, eye);

                            renderer->end();
                        } else {
                            renderer->end();
                        }
                    } else if (al::isEqualString(sensorName, "Trample")) {
                            renderer->end();
                    } else {
                        renderer->drawSphere4x8(*pos, radius, body);
                        renderer->end();
                    }
                }
            }
        }
    }
    return true;
}