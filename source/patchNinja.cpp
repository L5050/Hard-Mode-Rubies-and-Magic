#include "patchNinja.h"
#include "evtpatch.h"
#include "patch.h"

#include <types.h>
#include <spm/npcdrv.h>
#include <spm/evt_npc.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm::npcdrv;

namespace mod {
  extern "C" {

    s32 ninjaBombLogic(NPCEntry *npcEntry, NPCPart *npcPart, s32 lastAttackedDefenseType, s32 defenseType, s32 power, u8 param_6) {
      if (defenseType < 0x1d) {
        if ((defenseType < 0x1b) && (-1 < defenseType)) {
      if (npcEntry->moveMode != 1) {
        return 4;
      }
      if (npcEntry->unitWork[7] != 0) {
        return 3;
      }
      if (npcEntry->unitWork[15] > 0 && (u32)power >= npcEntry->hp) {
        npcEntry->unitWork[15] -= 1;
        npcEntry->unitWork[7] = 1;
        return 1;
      }
    }}
      return 4;
    }

  }

  EVT_BEGIN(patch_ninjoe) // Set the maximum number of feigns in one life
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(0))
    SWITCH(LW(0))
      CASE_EQUAL(211)
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 1)
      SWITCH_BREAK()
      CASE_EQUAL(214)
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 1)
      SWITCH_BREAK()
      CASE_EQUAL(217) //Ninjerry and Dark Ninjoe get one extra feign
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 2)
      SWITCH_BREAK()
      CASE_EQUAL(220) //Ninjerry and Dark Ninjoe get one extra feign
        USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 15, 2)
      SWITCH_BREAK()
    END_SWITCH()
  RETURN_FROM_CALL()

void ninjaPatch()
{
    //writeWord(&spm::npc_ninja::ninjoe_bomb_calc_chance, 0x16C, 0x57FF003E);
    patch::hookFunction(spm::npc_ninja::ninjoe_bomb_calc_chance, ninjaBombLogic);
    //writeBranch(&spm::npc_ninja::ninjoe_bomb_calc_chance, 0x50, branchToNewLogic);
    evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[259].onSpawnScript, 1, (spm::evtmgr::EvtScriptCode*)patch_ninjoe);
}

}
