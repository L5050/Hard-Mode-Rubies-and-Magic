#include "mod.h"
#include "cutscene_helpers.h"
#include "evt_cmd.h"
#include "evtpatch.h"
#include "exception.h"
#include "patch.h"
#include "romfontexpand.h"

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
#include <spm/fadedrv.h>
#include <spm/fontmgr.h>
#include <spm/map_data.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/rel/an1_04.h>
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
EVT_BEGIN(modTest)
  SETF(LW(5), FLOAT(5.0))
RETURN_FROM_CALL()

void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    wii::os::OSReport("%x\n", spm::map_data::mapDataPtr("an1_04")->initScript);

    titleScreenCustomTextPatch();
    checkForDolphin();
    exceptionPatch(); // Seeky's exception handler from Practice Codes
    romfontExpand(); // Font expansion patch from Practice Codes
    evtpatch::evtmgrCmdExtensionInit(); // Initialize EVT scripting extension

    spm::evtmgr::EvtScriptCode* dimentioBattleLink = spm::npcdrv::npcEnemyTemplates[142].unkScript3;
    wii::os::OSReport("%x\n", dimentioBattleLink);
    evtpatch::hookEvtReplace(dimentioBattleLink, 122, (spm::evtmgr::EvtScriptCode*)modTest);
}

}
