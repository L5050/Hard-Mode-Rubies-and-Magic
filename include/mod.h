#pragma once
#include "evt_cmd.h"
namespace mod {

#define MOD_VERSION "Rubies and Magic"

extern bool gIsDolphin;

extern "C" {

EVT_DECLARE(evt_cutscene_pure_heart)
EVT_DECLARE(evt_memcard_save)

}
spm::evtmgr::EvtScriptCode* getStandardDeathScript();

spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction);
EVT_DECLARE_USER_FUNC(displayBossHealth, 0)
EVT_DECLARE_USER_FUNC(checkBboxChonkyBoi, 1)
void main();

}
