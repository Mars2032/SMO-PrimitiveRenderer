#include "debugMenu.h"
#include "drawer.h"
#include "gfx/seadColor.h"
#include "main.h"

#include "rs/util.hpp"
#include "actorHooks.h"

sead::PtrArray<TransparentWall> transparentWalls;
sead::PtrArray<Shine> shines;
sead::PtrArray<CheckpointFlag> checkpoints;
sead::PtrArray<CapSwitch> capSwitches;
sead::PtrArray<Frog> frogs;
sead::PtrArray<ElectricWire> wires;
sead::PtrArray<KuriboWing> kuriboWings;
sead::PtrArray<Wanwan> chomps;

void destroyArrays() {
    transparentWalls.clear();
    shines.clear();
    checkpoints.clear();
    capSwitches.clear();
    frogs.clear();
    wires.clear();
    kuriboWings.clear();
    chomps.clear();
    __asm("MOV X0, X19");
}

void createWallHook(TransparentWall *wall) {
    al::trySyncStageSwitchAppearAndKill(wall);
    transparentWalls.pushBack(wall);
}

bool createShineHook(Shine *shine) {
    bool isInvalid = rs::isInvalidChangeStage(shine);
    shines.pushBack(shine);
    return isInvalid;
}

void createCheckpointFlagHook(CheckpointFlag* checkpoint) {
    rs::registerCheckpointFlagToWatcher(checkpoint);
    checkpoints.pushBack(checkpoint);
}

CapSwitch* createCapSwitchHook() {
    CapSwitch* capSwitch;
    __asm("MOV %[result], X19" : [result] "=r" (capSwitch));
    capSwitches.pushBack(capSwitch);
    return capSwitch;
}

void createFrogHook(Frog* frog) {
    al::tryListenStageSwitchKill(frog);
    frogs.pushBack(frog);
}

ElectricWire* createElectricWireHook() {
    ElectricWire* wire;
    __asm("MOV %[result], X19" : [result] "=r" (wire));
    wires.pushBack(wire);
    return wire;
}

Wanwan* createChompHook() {
    Wanwan* chomp;
    __asm("MOV %[result], X19" : [result] "=r" (chomp));
    chomps.pushBack(chomp);
    return chomp;
}

void createKuriboWingHook(KuriboWing* kuriboWing) {
    al::setSyncRailToNearestPos(kuriboWing);
    kuriboWings.pushBack(kuriboWing);
}