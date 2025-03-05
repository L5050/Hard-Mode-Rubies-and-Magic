#include "patch_dan.h"
#include "evtpatch.h"
#include "patch.h"
#include "evt_cmd.h"

#include <types.h>
#include <spm/npcdrv.h>
#include <spm/rel/dan.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_snd.h>
#include <spm/evt_sub.h>
#include <spm/evt_msg.h>
#include <spm/evt_eff.h>
#include <spm/evt_mobj.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm::npcdrv;
using namespace spm::mario_pouch;

int curPixlArray[17]; // I want this to be a u16 but for some fucking reason it causes the game to shit everywhere if I make it one so you get int instead

namespace mod::patch_dan {

static void inline initPixlArray()
{
    MarioPouchWork * mario_pouch_wp = pouchGetPtr();
    u16 pixlArrayIndex = 0;
    for (u16 i = 0; i < POUCH_FAIRY_ITEM_MAX; i++)
    {
        if (mario_pouch_wp->pixls[i].selectable == true)
        {
            curPixlArray[pixlArrayIndex] = mario_pouch_wp->pixls[i].itemType;
            pixlArrayIndex += 1;
            wii::os::OSReport("itemType %d\n", curPixlArray[pixlArrayIndex]);
            wii::os::OSReport("itemType %d\n", mario_pouch_wp->pixls[i].itemType);
            wii::os::OSReport("pixlIndex %d\n", pixlArrayIndex);
        }
    }
    curPixlArray[pixlArrayIndex] = -1;
}

static NPCTribeAnimDef loreAnims[] = {
    {0, "S_4"}, // Standing (ID 0 is idle animation)
    {1, "W_1"}, // Slow walk
    {3, "T_4"}, // Talking (ID 3 is the ID to use when talking)
    {-1, nullptr}
  };

const char * lore_intro = "<housou><p>\n"
"I've waited for you...\n"
"<k>\n"
"<p>\n"
"I am a servant of the\n"
"Ancients, sent here to give\n"
"power to the true heroes.\n"
"<k>\n"
"<p>\n"
"But whether you are the true\n"
"heroes of legend or not is\n"
"yet to be seen...\n"
"<k>\n"
"<p>\n"
"I will give you a new\n"
"challenge!\n"
"<k>\n"
"<p>\n"
"Reach me in the Flopside\n"
"pit and you will receive\n"
"my power...\n"
"<k>\n"
"<p>\n"
"In the meantime I shall\n"
"aid you.\n"
"<k>\n"
"<p>\n"
"...If you let me experiment\n"
"on your Pixls of course.\n"
"<o>\n";

const char introOptions[] =
  "<select 0 -1 160 20>\n"
  "Yes\n"
  "No";

const char * lore_2 = "<housou><p>\n"
"Welcome back.\n"
"Do you wish to take\n"
"some of my power?\n"
"<o>\n";

const char pixlOptions_1[] =
"<select 0 -1 380 20>\n"
"Upgrade random Pixl!\n"
"Upgrade a specific Pixl!";

const char * lore_intro_2 = "<housou><p>\n"
"Delightful.\n"
"<k>\n";

const char * lore_upgrade_pixl_damage = "<housou><p>\n"
"%s seems to be much\n"
"stronger now. About\n"
"%d damage stronger or so.\n"
"<k>\n"
"<p>\n"
"If you survive, heroes, I\n"
"shall see you again..."
"<k>\n";

s32 reinit_pixl_array(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
    initPixlArray();
    return 2;
}

EVT_DECLARE_USER_FUNC(reinit_pixl_array, 0)

EVT_BEGIN(allow_shop)
  SET(LW(0), 0)
RETURN_FROM_CALL()

EVT_BEGIN(lore_speech_intro)
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
  IF_EQUAL(GSWF(1702), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_intro), 0, PTR("lore"))
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(introOptions))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    SWITCH(LW(0))
      CASE_EQUAL(0)
        USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
        USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_intro_2), 0, PTR("lore"))
        USER_FUNC(spm::evt_mario::evt_mario_pos_change, 75, 25, FLOAT(80.0))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
        USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(3))
        DIVF(LW(3), FLOAT(2.0))
        ADDF(LW(1), LW(3))
        USER_FUNC(spm::evt_eff::evt_eff, PTR("eff"), PTR("event_3dget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        USER_FUNC(spm::evt_snd::evt_snd_bgmon, 2, PTR("BGM_FF_ZIGENWAZA_GET1"))
        WAIT_MSEC(2000)
        INLINE_EVT()
            WAIT_MSEC(500)
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
            USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("lore"))
        END_INLINE()
        USER_FUNC(spm::evt_eff::evt_eff_softdelete, PTR("eff"))
        WAIT_MSEC(2000)
        INLINE_EVT()
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("M_1c"), 0)
            USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
            USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
        END_INLINE()
        USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
        USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Cudge"), 2)
        ADD(GSW(1700), 1)
    END_SWITCH()
    SET(GSWF(1702), 1)
  END_IF()
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
EVT_END()

EVT_BEGIN(lore_speech)
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
  USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
  LBL(0)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_2), 0, PTR("lore"))
  USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(pixlOptions_1))
  USER_FUNC(spm::evt_msg::evt_msg_continue)
  SWITCH(LW(0))
    CASE_EQUAL(0)
      USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_intro_2), 0, PTR("lore"))
      USER_FUNC(spm::evt_mario::evt_mario_pos_change, 75, 25, FLOAT(80.0))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(3))
      DIVF(LW(3), FLOAT(2.0))
      ADDF(LW(1), LW(3))
      USER_FUNC(spm::evt_eff::evt_eff, PTR("eff"), PTR("event_3dget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_snd::evt_snd_bgmon, 2, PTR("BGM_FF_ZIGENWAZA_GET1"))
      WAIT_MSEC(2000)
      INLINE_EVT()
          WAIT_MSEC(500)
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
          USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("lore"))
      END_INLINE()
      USER_FUNC(spm::evt_eff::evt_eff_softdelete, PTR("eff"))
      WAIT_MSEC(2000)
      INLINE_EVT()
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("M_1c"), 0)
          USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
      END_INLINE()
      USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
      USER_FUNC(spm::evt_sub::evt_sub_random, 2, LW(0))
      SWITCH(LW(0))
        CASE_EQUAL(0)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Cudge"), 2)
          ADD(GSW(1700), 1)
        CASE_EQUAL(1)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Boomer"), 2)
          ADD(GSW(1701), 1)
        CASE_EQUAL(2)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thudley"), 4)
          ADD(GSW(1702), 2)
      END_SWITCH()
    CASE_EQUAL(1)
      USER_FUNC(spm::evt_sub::evt_sub_item_select_menu, 0, PTR(curPixlArray), LW(10), 0)
      IF_SMALL_EQUAL(LW(10), 0)
        GOTO(0)
      END_IF()
      USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 1000)
      USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_intro_2), 0, PTR("lore"))
      USER_FUNC(spm::evt_mario::evt_mario_pos_change, 75, 25, FLOAT(80.0))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
      USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(3))
      DIVF(LW(3), FLOAT(2.0))
      ADDF(LW(1), LW(3))
      USER_FUNC(spm::evt_eff::evt_eff, PTR("eff"), PTR("event_3dget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
      USER_FUNC(spm::evt_snd::evt_snd_bgmon, 2, PTR("BGM_FF_ZIGENWAZA_GET1"))
      WAIT_MSEC(2000)
      INLINE_EVT()
          WAIT_MSEC(500)
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
          USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("lore"))
      END_INLINE()
      USER_FUNC(spm::evt_eff::evt_eff_softdelete, PTR("eff"))
      WAIT_MSEC(2000)
      INLINE_EVT()
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("M_1c"), 0)
          USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
          USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
      END_INLINE()
      USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
      SUB(LW(10), 221)
      SWITCH(LW(10))
        CASE_EQUAL(0)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thoreau"), 2)
          ADD(GSW(1700), 1)
        CASE_EQUAL(1)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Boomer"), 2)
          ADD(GSW(1701), 1)
        CASE_EQUAL(3)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thudley"), 4)
          ADD(GSW(1702), 2)
        CASE_EQUAL(6)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Cudge"), 2)
          ADD(GSW(1700), 1)
        END_SWITCH()
  END_SWITCH()
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
EVT_END()


EVT_BEGIN(fwd_lore_speech)
    SET(GSWF(1702), 1)
    IF_EQUAL(GSWF(1702), 0)
        RUN_EVT(lore_speech_intro)
    ELSE()
        RUN_EVT(lore_speech)
    END_IF()
RETURN()
EVT_END()

EVT_BEGIN(lore_setup)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    USER_FUNC(reinit_pixl_array)
    USER_FUNC(spm::evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("lore"), PTR("n_machi_dearle"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("lore"), 75, 25, FLOAT(-87.5))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("lore"), 14, PTR(loreAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_rgba, PTR("lore"), 0, 0, 0, 0xff)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("lore"), 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("lore"), 9, PTR(fwd_lore_speech))
RETURN_FROM_CALL()

void patch_dan_main()
{
    evtpatch::hookEvtReplace(spm::dan::dan_chest_room_init_evt, 65, allow_shop);
    evtpatch::hookEvtReplace(spm::dan::dan_chest_room_init_evt, 83, lore_setup);
}

}