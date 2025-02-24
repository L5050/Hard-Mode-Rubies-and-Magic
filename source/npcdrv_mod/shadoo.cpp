#include "shadoo.h"
#include "evtpatch.h"
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

EVT_BEGIN(shadooFight)
DO(0)
    USER_FUNC(spm::evt_npc::evt_npc_get_active_count, LW(0))
    IF_EQUAL(LW(0), 0)
        DO_BREAK()
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 183)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 183, 0, -100, 0, LW(10), EVT_NULLPTR)
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
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 187)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 187, 0, -100, 0, LW(10), EVT_NULLPTR)
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
    USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
    WAIT_FRM(1)
WHILE()
USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 226)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 226, 0, -100, 0, LW(10), EVT_NULLPTR)
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
EVT_END()

EVT_BEGIN(hookChunksDeathScript)
  USER_FUNC(returnChonkyFarts, LW(10))
  DELETE_EVT(LW(10))
RETURN_FROM_CALL()

EVT_BEGIN(mariounk3)
  SET(LW(1), 50)
RETURN_FROM_CALL()

static void hookShadooScripts()
{
  evtpatch::hookEvtReplaceBlock(spm::dan::dan_shadoo_fight_evt, 42, (spm::evtmgr::EvtScriptCode*)shadooFight, 79);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].onSpawnScript, 85, (spm::evtmgr::EvtScriptCode*)returnChunksDeathScript);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[183].unkScript6, 1, (spm::evtmgr::EvtScriptCode*)hookChunksDeathScript); //Fix for if O'Chunks is killed outside of boss rooms
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[422].unkScript3, 71, (spm::evtmgr::EvtScriptCode*)mariounk3);
}

void shadooMain() {
  hookShadooScripts();
}

}
