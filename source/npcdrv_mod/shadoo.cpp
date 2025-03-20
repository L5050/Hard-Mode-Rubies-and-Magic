#include "shadoo.h"
#include "evtpatch.h"
#include "king_sammer.h"
#include "patch.h"
#include "mod.h"
#include "evt_cmd.h"

#include <types.h>
#include <spm/rel/dan.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_item.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_pouch.h>
#include <spm/evt_sub.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/memory.h>
#include <spm/evt_env.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/pausewin.h>
#include <spm/animdrv.h>
#include <spm/wpadmgr.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/mario.h>
#include <spm/map_data.h>
#include <spm/evtmgr_cmd.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm;
using namespace npcdrv;
using namespace evt_npc;

namespace mod {


EVT_BEGIN(ninja_pls_dont_crash)
DO(0)
    USER_FUNC(check_for_ninja, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    WAIT_FRM(1)
WHILE()
RETURN()
EVT_END()

EVT_BEGIN(shadooFight)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DODONTAS1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 183)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 183, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_MONEYLA1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 187)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 187, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_MONEYLA1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 397)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 397, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, LW(10), 14, LW(0))
    IF_EQUAL(LW(0), 1)
        DO_BREAK()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DIMEN1"), 1000)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 226)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 226, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    SUB(LW(0), 1)
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
IF_EQUAL(GSWF(1102), 0)
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 500)
WAIT_MSEC(500)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 32)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 32, 0, -100, 0, LW(10), EVT_NULLPTR)
USER_FUNC(spm::evt_npc::evt_npc_set_rgba, LW(10), 0, 0, 0, 0xff)
USER_FUNC(spm::evt_npc::evt_npc_set_anim, LW(10), 0, 1)
USER_FUNC(spm::evt_npc::func_80107c38, LW(10), 0)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 0, 8)
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 65536)
USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
IF_SMALL(LW(11), -480)
    SET(LW(11), -480)
END_IF()
IF_LARGE(LW(11), 480)
    SET(LW(11), 480)
END_IF()
IF_SMALL(LW(12), 0)
    SET(LW(12), 0)
END_IF()
IF_SMALL(LW(13), -140)
    SET(LW(13), -140)
END_IF()
IF_LARGE(LW(13), 140)
    SET(LW(13), 140)
END_IF()
USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(11), FLOAT(0.0), LW(13))
USER_FUNC(spm::evt_npc::func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_wait_flip_finished, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_sammer_display_count, 101)
WAIT_MSEC(2500)
USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_STG6_2"))
USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 8)
USER_FUNC(spm::evt_npc::evt_npc_restart_evt_id, LW(10))
USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 10, 0)
USER_FUNC(spm::evt_mario::evt_mario_key_on)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    SUB(LW(0), 1)
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    IF_LARGE_EQUAL(LW(0), 1)
        USER_FUNC(check_for_ninja, LW(0))
        IF_LARGE_EQUAL(LW(0), 1)
            RUN_CHILD_EVT(ninja_pls_dont_crash)
            USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
            IF_EQUAL(LW(0), 0)
                DO_BREAK()
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
END_IF()
RETURN_FROM_CALL()

static s32 chunkFartId;

s32 grabChonkyFarts(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  chunkFartId = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
  return 2;
}

s32 returnChonkyFarts(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
  chunkFartId = spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], chunkFartId);
  return 2;
}

EVT_DECLARE_USER_FUNC(grabChonkyFarts, 1)
EVT_DECLARE_USER_FUNC(returnChonkyFarts, 1)

EVT_BEGIN(returnChunksDeathScript)
  USER_FUNC(grabChonkyFarts, LW(10))
RETURN_FROM_CALL()

EVT_BEGIN(hookChunksDeathScript)
  USER_FUNC(returnChonkyFarts, LW(10))
  DELETE_EVT(LW(10))
RETURN_FROM_CALL()

EVT_BEGIN(mariounk3)
  SET(LW(1), 50)
RETURN_FROM_CALL()

EVT_BEGIN(luigiReset)
  WAIT_MSEC(1000)
  USER_FUNC(evt_npc_set_unitwork, PTR("me"), 12, 0)
RETURN_FROM_CALL()

EVT_BEGIN(luigiUnk7_2)
  USER_FUNC(evt_npc_get_unitwork, PTR("me"), 12, LW(15))
  IF_NOT_EQUAL(LW(15), 1)
    SET(LW(0), 1)
    USER_FUNC(evt_npc_set_unitwork, PTR("me"), 12, 1)
  ELSE()
    SET(LW(0), 0)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(luigiUnk7_1)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(1), 75)
RETURN_FROM_CALL()

spm::evtmgr::EvtScriptCode luigiPatch[] = { IF_NOT_EQUAL(LW(0), 1) };
spm::evtmgr::EvtScriptCode luigiPatch2[] = { USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 500, FLOAT(0.0), FLOAT(55.0), 0, 0, 0) };

static void hookShadooScripts()
{
  spm::evtmgr::EvtScriptCode* luigi_attack_script = spm::npcdrv::npcEnemyTemplates[286].unkScript7;
  spm::evtmgr::EvtScriptCode* luigi_idle_script = spm::npcdrv::npcEnemyTemplates[286].unkScript2;
  evtpatch::hookEvt(luigi_idle_script, 6, (spm::evtmgr::EvtScriptCode*)luigiReset);
  evtpatch::hookEvtReplace(luigi_attack_script, 9, luigiUnk7_1);
  evtpatch::hookEvtReplace(luigi_attack_script, 2, luigiUnk7_2);
  evtpatch::patchEvtInstruction(luigi_attack_script, 3, luigiPatch);
  evtpatch::patchEvtInstruction(luigi_attack_script, 3, luigiPatch);
  
  evtpatch::hookEvtReplaceBlock(spm::dan::dan_shadoo_fight_evt, 42, (spm::evtmgr::EvtScriptCode*)shadooFight, 91);
  evtpatch::patchEvtInstruction(spm::dan::dan_shadoo_main_evt, 126, EVT_CAST(USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_STG4"))));
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].onSpawnScript, 85, (spm::evtmgr::EvtScriptCode*)returnChunksDeathScript);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].unkScript6, 1, (spm::evtmgr::EvtScriptCode*)hookChunksDeathScript); //Fix for if O'Chunks is killed outside of boss rooms
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[422].unkScript3, 71, (spm::evtmgr::EvtScriptCode*)mariounk3);
}

void shadooMain() {
  hookShadooScripts();
}

}
