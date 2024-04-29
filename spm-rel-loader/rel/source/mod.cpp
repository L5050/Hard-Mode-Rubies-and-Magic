#include "mod.h"
#include "cutscene_helpers.h"
#include "evt_cmd.h"
#include "evtpatch.h"
#include "exception.h"
#include "patch.h"

#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_cam.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/mario.h>
#include <spm/evt_sub.h>
#include <spm/fadedrv.h>
#include <spm/fontmgr.h>
#include <spm/map_data.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <wii/ipc.h>
#include <wii/os.h>

namespace mod {

bool gIsDolphin;

/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor green {0, 255, 0, 255};
    f32 scale = 0.8f;
    const char * msg = "SPM Rel Loader";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}

/*
    Platform detection to adjust certain settings
*/

static void checkForDolphin()
{
    // Thanks to TheLordScruffy for telling me about this
    gIsDolphin = wii::ipc::IOS_Open("/sys", 1) == -106;

    // If they ever fix that, it'll be in a version that's definitely new enough to have /dev/dolphin
    if (!gIsDolphin)
    {
        int ret = wii::ipc::IOS_Open("/dev/dolphin", 0);
        if (ret >= 0)
        {
            gIsDolphin = true;
            wii::ipc::IOS_Close(ret);
        }
    }
}

/*
    General mod functions
*/

spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  wii::os::OSReport("%x\n", link);
  spm::evtmgr::EvtScriptCode* arg = evtpatch::getInstructionArgv(link)[instruction];
  wii::os::OSReport("%x\n", arg);
  return arg;
}
/*
s32 checkForSpreadshot(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {

  if (firstRun == false) {}
  return 2;
}
*/
s32 mimiHittable = 1;
spm::evtmgr::EvtScriptCode* bleckMovementScript;

EVT_BEGIN(changeFlyingSpeedBleck)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, "me", LW(5), LW(6), LW(7), 0, FLOAT(180.0), 0, 11, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeFlyingSpeed)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, "me", LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 5, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeBoxAttack)
  USER_FUNC(spm::evt_npc::evt_npc_get_axis_movement_unit, "me", LW(1))
  ADD(LW(1), 1)
RETURN_FROM_CALL()

EVT_BEGIN(turnNull)
  SET(LW(0), LW(0))
RETURN_FROM_CALL()

EVT_BEGIN(changeSlowdown)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, "me", 10, 20000)
    WAIT_MSEC(4000)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, "me", 10, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeDashAttack)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, "me", LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 2, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeVoids)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, "me", 0, 2048)
RETURN_FROM_CALL()

EVT_BEGIN(changeVoids2)
  SET(LW(10), 0)
RETURN_FROM_CALL()

EVT_BEGIN(skipBigVoid)
  SET(LW(6), 0)
RETURN_FROM_CALL()

EVT_BEGIN(returnToAttackCalc1)
  SET(LW(0), 16)
RETURN_FROM_CALL()

EVT_BEGIN(returnToAttackCalc2)
IF_LARGE(LW(0), 29)
IF_SMALL(LW(0), 39)
  //RUN_CHILD_EVT(bleckMovementScript)
  SET(LW(0), 25)
END_IF()
IF_SMALL(LW(0), 45)
  SET(LW(0), 55)
END_IF()
END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiSpeed)
  SET(LW(7), 400)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiSpeed2)
  SET(LW(7), 600)
RETURN_FROM_CALL()

EVT_BEGIN(addMimiAttack)
  ADD(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(removeMimiAnimation)
  SET(LW(0), FLOAT(0.0))
RETURN_FROM_CALL()

EVT_BEGIN(changeBehaviorToAttack)
  IF_EQUAL(LW(0), 0)
    SET(LW(0), 1)
  END_IF()
RETURN_FROM_CALL()

void hookDimentioScripts()
{
  spm::evtmgr::EvtScriptCode* dimentioOnSpawn = spm::npcdrv::npcEnemyTemplates[142].onSpawnScript;
  wii::os::OSReport("%x\n", dimentioOnSpawn);
  spm::evtmgr::EvtScriptCode* part1 = getInstructionEvtArg(dimentioOnSpawn, 4, 3);
  spm::evtmgr::EvtScriptCode* part2 = getInstructionEvtArg(part1, 7, 0);
  spm::evtmgr::EvtScriptCode* dimentioBattleLink = getInstructionEvtArg(part2, 3, 0);
  evtpatch::hookEvtReplace(dimentioBattleLink, 6, (spm::evtmgr::EvtScriptCode*)changeFlyingSpeed);
  spm::evtmgr::EvtScriptCode* mainLogic = getInstructionEvtArg(dimentioOnSpawn, 5, 3);
  spm::evtmgr::EvtScriptCode* shootScript = getInstructionEvtArg(mainLogic, 54, 0);
  spm::evtmgr::EvtScriptCode* boxScript = getInstructionEvtArg(mainLogic, 65, 0);
  //evtpatch::hookEvtReplace(boxScript, 17, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(shootScript, 32, (spm::evtmgr::EvtScriptCode*)turnNull);
}

void hookBleckScripts()
{
  spm::evtmgr::EvtScriptCode* bleckUnk2 = spm::npcdrv::npcEnemyTemplates[196].unkScript2;
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[196].unkScript7;
  spm::evtmgr::EvtScriptCode* bigVoidMainLogic = spm::npcdrv::npcEnemyTemplates[198].unkScript2;
  bleckMovementScript = getInstructionEvtArg(mainLogic, 34, 0);
  spm::evtmgr::EvtScriptCode* shootVoid = getInstructionEvtArg(mainLogic, 22, 0);
  //spm::evtmgr::EvtScriptCode* shootBigVoid = getInstructionEvtArg(mainLogic, 128, 0);
  spm::evtmgr::EvtScriptCode* dashAttack = getInstructionEvtArg(mainLogic, 42, 0);
  spm::evtmgr::EvtScriptCode* slowDown = getInstructionEvtArg(mainLogic, 69, 0);
  evtpatch::hookEvt(mainLogic, 13, (spm::evtmgr::EvtScriptCode*)returnToAttackCalc1);
  evtpatch::hookEvt(mainLogic, 118, (spm::evtmgr::EvtScriptCode*)returnToAttackCalc2);
  evtpatch::hookEvt(bigVoidMainLogic, 12, (spm::evtmgr::EvtScriptCode*)skipBigVoid);
  evtpatch::hookEvt(bigVoidMainLogic, 28, (spm::evtmgr::EvtScriptCode*)skipBigVoid);
  evtpatch::hookEvtReplace(shootVoid, 25, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(shootVoid, 52, (spm::evtmgr::EvtScriptCode*)changeVoids2);
  evtpatch::hookEvtReplaceBlock(shootVoid, 53, (spm::evtmgr::EvtScriptCode*)changeVoids, 61);
  evtpatch::hookEvtReplace(slowDown, 17, (spm::evtmgr::EvtScriptCode*)changeSlowdown);
  evtpatch::hookEvtReplace(dashAttack, 45, (spm::evtmgr::EvtScriptCode*)changeDashAttack);
  spm::evtmgr::EvtScriptCode* bleckMovement = getInstructionEvtArg(bleckUnk2, 8, 0);
  evtpatch::hookEvtReplace(bleckMovement, 52, (spm::evtmgr::EvtScriptCode*)changeFlyingSpeedBleck);
}

void hookMimiScripts()
{
  spm::evtmgr::EvtScriptCode* mimiUnk2 = spm::npcdrv::npcEnemyTemplates[130].unkScript2;
    spm::evtmgr::EvtScriptCode* mimiOnHitScript = spm::npcdrv::npcEnemyTemplates[130].unkScript3;
  spm::evtmgr::EvtScriptCode* mimiMovement = getInstructionEvtArg(mimiUnk2, 56, 0);
  spm::evtmgr::EvtScriptCode* mimiMainAttack = getInstructionEvtArg(mimiUnk2, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingMovement = getInstructionEvtArg(mimiUnk2, 60, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingAttack = getInstructionEvtArg(mimiCeilingMovement, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiMovementUnknown = getInstructionEvtArg(mimiUnk2, 63, 0);
  spm::evtmgr::EvtScriptCode* mimiMoneyWave = getInstructionEvtArg(mimiUnk2, 65, 0);
  spm::evtmgr::EvtScriptCode* mimiTrueHit = getInstructionEvtArg(mimiOnHitScript, 55, 0);
  evtpatch::hookEvtReplace(mimiTrueHit, 25, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 29, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 39, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 43, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 53, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvt(mimiUnk2, 33, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiUnk2, 54, (spm::evtmgr::EvtScriptCode*)changeBehaviorToAttack);
  evtpatch::hookEvt(mimiMovement, 41, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvtReplace(mimiMainAttack, 34, (spm::evtmgr::EvtScriptCode*)addMimiAttack);
  evtpatch::hookEvt(mimiMainAttack, 41, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovement, 52, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovement, 57, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMainAttack, 47, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiCeilingMovement, 49, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 53, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 71, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiCeilingMovement, 76, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  //evtpatch::hookEvtReplace(mimiCeilingMovement, 30, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiCeilingMovement, 60, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiCeilingAttack, 19, (spm::evtmgr::EvtScriptCode*)removeMimiAnimation);
  evtpatch::hookEvt(mimiMovementUnknown, 58, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovementUnknown, 60, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovementUnknown, 75, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovementUnknown, 77, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovementUnknown, 92, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMovementUnknown, 94, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiMoneyWave, 19, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
}

void patchVariables() {
    writeWord(&spm::mario::marioCalcDamageToEnemy, 0x16C, 0x57FF003E);
      writeWord(&spm::mario::marioCalcDamageToEnemy, 0xC8, 0x57FF003C);
       writeWord(&spm::mario::marioCalcDamageToEnemy, 0x94, 0x57FF003C);
}

void (*marioTakeDamage)(wii::mtx::Vec3 * position, u32 flags, s32 damage);

void patchMarioDamage(){
  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::mtx::Vec3 * position, u32 flags, s32 damage)
            {
              marioTakeDamage(position, flags, damage * 2);
            });
}
void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    //wii::os::OSReport("%x\n", spm::map_data::mapDataPtr("an1_04")->initScript);

    titleScreenCustomTextPatch();
    checkForDolphin();
    //exceptionPatch(); // Seeky's exception handler from Practice Codes
    //romfontExpand(); // Font expansion patch from Practice Codes
    evtpatch::evtmgrCmdExtensionInit(); // Initialize EVT scripting extension
    hookDimentioScripts();
    hookBleckScripts();
    hookMimiScripts();
    patchVariables();
    patchMarioDamage();
}

}
