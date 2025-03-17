#include "patch_dan.h"
#include "evtpatch.h"
#include "patch.h"
#include "evt_cmd.h"
#include "mod.h"

#include <types.h>
#include <spm/npcdrv.h>
#include <spm/rel/dan.h>
#include <spm/mario.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/mario_motion.h>
#include <spm/evt_door.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_snd.h>
#include <spm/evt_sub.h>
#include <spm/evt_msg.h>
#include <spm/evt_eff.h>
#include <spm/evt_mobj.h>
#include <spm/seq_title.h>
#include <spm/spmario_snd.h>
#include <spm/evt_case.h>
#include <spm/evt_pouch.h>
#include <spm/map_data.h>
#include <spm/npc_ninja.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>

using namespace spm::npcdrv;
using namespace spm::mario_pouch;

int curPixlArray[17]; // I want this to be a u16 but for some fucking reason it causes the game to shit everywhere if I make it one so you get int instead

extern "C" {
  void* boomerRetLocation1 = &spm::mario_motion::boomerFuseMain + 0x9D8;
  void* boomerRetLocation2 = &spm::mario_motion::boomerFuseMain + 0x90C;
  void* fleepRetLocation = &spm::npcdrv::npcFleeped + 0x1C;
  void* throwRetLocation = &spm::npcdrv::throwDamage + 0x110;
  f32 floatValue = 0.5; // Damage Radius
  f32 floatValue2 = 1.0; // Visual Radius
  f32 fleepValue = 0.5;
  int throwDamageN = 0;
  s32 piccoloHealNum = 0;
  bool piccoloAlreadyHealed = false;

  void setBoomDamageRadiusFloat();
  asm
  (
    ".global setBoomDamageRadiusFloat\n"
    "setBoomDamageRadiusFloat:\n"
        "lis 12, floatValue@ha\n"
        "ori 12, 12, floatValue@l\n"
        "lfs 27, 0x0000 (12)\n"
        "lis 12, boomerRetLocation1@ha\n"
        "ori 12, 12, boomerRetLocation1@l\n"
        "lwz 12, 0 (12)\n"
        "mtctr 12\n"
        "bctr\n"
  );
  void setBoomVisualFloat();
  asm
  (
    ".global setBoomVisualFloat\n"
    "setBoomVisualFloat:\n"
        "lis 12, floatValue2@ha\n"
        "ori 12, 12, floatValue2@l\n"
        "lfs 4, 0x0000 (12)\n"
        "lis 12, boomerRetLocation2@ha\n"
        "ori 12, 12, boomerRetLocation2@l\n"
        "lwz 12, 0 (12)\n"
        "mtctr 12\n"
        "bctr\n"
  );

  void setFleepFloat();
  asm
  (
    ".global setFleepFloat\n"
    "setFleepFloat:\n"
        "lis 12, fleepValue@ha\n"
        "ori 12, 12, fleepValue@l\n"
        "lfs 0, 0x0000 (12)\n"
        "lis 12, fleepRetLocation@ha\n"
        "ori 12, 12, fleepRetLocation@l\n"
        "lwz 12, 0 (12)\n"
        "mtctr 12\n"
        "bctr\n"
  );

  void setThrowDamage();
  asm
  (
    ".global setThrowDamage\n"
    "setThrowDamage:\n"
        "or 7, 7, 0\n"
        "lis 12, throwDamageN@ha\n"
        "ori 12, 12, throwDamageN@l\n"
        "lwz 11, 0(12)\n"
        "add 6, 6, 11\n"
        "lis 12, throwRetLocation@ha\n"
        "ori 12, 12, throwRetLocation@l\n"
        "lwz 12, 0 (12)\n"
        "mtctr 12\n"
        "bctr\n"
  );

  void patchPiccoloSfx(char * name)
  {
    spm::spmario_snd::spsndSFXOn(name);
    if (piccoloAlreadyHealed == false) {
    spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
    pouch->hp += piccoloHealNum;
    piccoloAlreadyHealed = true;
    if (pouch->hp > pouch->maxHp) pouch->hp = pouch->maxHp;
    }
    return;
  }
}

namespace mod::patch_dan {

const char * (*getNextDanMapname)(s32 dungeonNo);
spm::evt_door::DokanDesc * mac_05_pipe;

const char * new_getNextDanMapname(s32 dungeonNo)
{
  switch (dungeonNo)
  {
    case 1:
    dungeonNo = 9;
    break;
    //case 101:
    //dungeonNo = 9;
    //break;
  }
  for (s32 i = 104; i < 198; i = i + 10)
  {
    if ((i + 10) >= 198) break;
    if (dungeonNo == i) return getNextDanMapname(9);
  }
  
  return getNextDanMapname(dungeonNo);
}

static void inline initPixlArray()
{
    MarioPouchWork * mario_pouch_wp = pouchGetPtr();
    u16 pixlArrayIndex = 0;

    for (u16 i = 0; i < POUCH_FAIRY_ITEM_MAX; i++)
    {
        if (mario_pouch_wp->pixls[i].selectable == true && i != (0x0E7 - 221))
        {
            curPixlArray[pixlArrayIndex] = mario_pouch_wp->pixls[i].itemType;
            pixlArrayIndex += 1;
        }
    }
    curPixlArray[pixlArrayIndex] = -1;
}

static void inline patchBoomer()
{
  writeBranch( & spm::mario_motion::boomerFuseMain, 0x9D4, setBoomDamageRadiusFloat);
  writeBranch( & spm::mario_motion::boomerFuseMain, 0x908, setBoomVisualFloat);
  writeWord(spm::mario_motion::boomerFuseMain, 0x904, 0x60000000);
}

static void inline patchFleep()
{
  writeBranch( & spm::npcdrv::npcFleeped, 0x18, setFleepFloat);
}

static void inline patchThoreau()
{
  writeBranch( & spm::npcdrv::throwDamage, 0x10C, setThrowDamage);
}

static void inline patchPiccolo()
{
  writeBranchLink( spm::mario_motion::marioMotTbl[0x29].mainFunc, 0x624, patchPiccoloSfx);
  evtpatch::hookEvt(spm::dan::dan_enemy_room_init_evt, 1, reset_piccolo);
}

static inline void increaseCoinGained() {
  for (int i = 0; i < 535; i++) {
    if (i != 0x7f && i != 0x81 && i != 0x83) {
    spm::npcdrv::npcTribes[i].coinDropBaseCount += 1;
    }
  }
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
"Pit and you will receive\n"
"my power...\n"
"<k>\n"
"<p>\n"
"In the meantime I shall\n"
"aid you.\n"
"<k>\n"
"<p>\n"
"...If you let me experiment\n"
"on your Pixls of course.<dkey><wait 500></dkey>\n"
"<o>\n";

const char * lore_system_intro = "<system><p>\n"
"The servant can upgrade any\n"
"pixl, feel free to experiment!\n"
"<k>\n"
"<p>\n"
"Remember, exiting the pit\n"
"will cause ALL pixl upgrades\n"
"to disappear.\n"
"<k>\n"
"<p>\n"
"All other effects, including\n"
"negative ones, are\n"
"<shake><scale 1.5><wait 500>permanent.\n"
"<k>\n";

const char introOptions[] =
  "<select 0 -1 160 20>\n"
  "Yes\n"
  "No";

const char * lore_2 = "<housou><p>\n"
"Welcome back.\n"
"Do you wish to take\n"
"some of my power?<dkey><wait 500></dkey>\n"
"<o>\n";

const char pixlOptions_1[] =
"<select 0 -1 540 20>\n"
"Upgrade random Pixl! (40 Coins)\n"
"Upgrade a specific Pixl! (100 Coins)";

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

const char * lore_upgrade_fleep = "<housou><p>\n"
"Fleep seems to stun\n"
"enemies longer now.\n"
"<k>\n"
"<p>\n"
"If you survive, heroes, I\n"
"shall see you again..."
"<k>\n";

const char * lore_upgrade_piccolo = "<housou><p>\n"
"Piccolo seems to have increased\n"
"healing capabilities...\n"
"<scale 0.7>(Limit one use per floor)\n"
"<k>\n"
"<p>\n"
"If you survive, heroes, I\n"
"shall see you again..."
"<k>\n";

const char * lore_upgrade_fail_coins = "<housou><p>\n"
"Can't do that I'm afraid.\n"
"<k>\n";

s32 reinit_pixl_array(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
    initPixlArray();
    return 2;
}

s32 upgrade_coins(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
    increaseCoinGained();
    return 2;
}

s32 reinit_pixl_upgrades(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
    spm::spmario::gp->gsw[1700] = 0;
    spm::spmario::gp->gsw[1701] = 0;
    spm::spmario::gp->gsw[1702] = 0;
    spm::spmario::gp->gsw[1703] = 0;
    floatValue = 0.5;
    floatValue2 = 1.1;
    fleepValue = 0.5;
    throwDamageN = 0;
    piccoloHealNum = 0;
    piccoloAlreadyHealed = false;
    for (int i = 0; i < 535; i++) {
      spm::npcdrv::npcTribes[i].coinDropBaseCount = 0;
    }

    // gives players any pixls they missed if they played through the main story
    if (spm::spmario::gp->gsw0 >= 420)  {
    MarioPouchWork * mario_pouch_wp = pouchGetPtr();
    for (int i = 0; i < 232 - 220 - 1; i++)
    {
      if (i != 0x0E7) // dont give away dashell
      {
      mario_pouch_wp->pixls[i].selectable = true;
      mario_pouch_wp->pixls[i].itemType = 220 + i + 1;
      }
    }
    }

    wii::os::OSReport("Pixl upgrades reset\n");
    return 2;
}

s32 upgrade_boomer_radius(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  floatValue += 0.2;
  floatValue2 += 0.2;
  return 2;
}

s32 upgrade_fleep_time(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  fleepValue += 1.0;
  return 2;
}

s32 upgrade_thoreau_damage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  throwDamageN += 2;
  return 2;
}

s32 upgrade_piccolo(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  piccoloHealNum += 1;
  return 2;
}

s32 resetPiccolo(spm::evtmgr::EvtEntry * evtEntry, bool firstRun)
{
  piccoloAlreadyHealed = false;
  return 2;
}

EVT_DECLARE_USER_FUNC(reinit_pixl_array, 0)
EVT_DECLARE_USER_FUNC(upgrade_coins, 0)
EVT_DECLARE_USER_FUNC(reinit_pixl_upgrades, 0)
EVT_DECLARE_USER_FUNC(upgrade_boomer_radius, 0)
EVT_DECLARE_USER_FUNC(upgrade_fleep_time, 0)
EVT_DECLARE_USER_FUNC(upgrade_thoreau_damage, 0)
EVT_DECLARE_USER_FUNC(upgrade_piccolo, 0)

EVT_BEGIN(reset_piccolo)
  USER_FUNC(resetPiccolo)
RETURN_FROM_CALL()

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
        USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thoreau"), 2)
        USER_FUNC(upgrade_thoreau_damage)
    END_SWITCH()
    SET(GSWF(1702), 1)
  END_IF()
  IF_SMALL(GSW(1), 99)
    SET(GSW(1), 99)
  END_IF()
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_system_intro), 0, 0)
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
EVT_END()

EVT_BEGIN(lore_speech)
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
  LBL(0)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_2), 0, PTR("lore"))
  USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(pixlOptions_1))
  USER_FUNC(spm::evt_msg::evt_msg_continue)
  SWITCH(LW(0))
    CASE_EQUAL(0)
      USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(0))
      IF_LARGE_EQUAL(LW(0), 40)
        SUB(LW(0), 40)
      ELSE()
        USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_fail_coins), 0, PTR("lore"))
        USER_FUNC(spm::evt_mario::evt_mario_key_on)
        RETURN()
      END_IF()
      USER_FUNC(spm::evt_pouch::evt_pouch_set_coins, LW(0))
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
      USER_FUNC(spm::evt_sub::evt_sub_random, 6, LW(0))
      SWITCH(LW(0))
        CASE_EQUAL(0)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Cudge"), 2)
          ADD(GSW(1700), 1)
        CASE_EQUAL(1)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Boomer"), 2)
          USER_FUNC(upgrade_boomer_radius)
          ADD(GSW(1701), 1)
        CASE_EQUAL(2)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thudley"), 4)
          ADD(GSW(1702), 2)
        CASE_EQUAL(3)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Barry"), 1)
          ADD(GSW(1703), 1)
        CASE_EQUAL(4)
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_fleep), 0, 0)
          USER_FUNC(upgrade_fleep_time)
        CASE_EQUAL(5)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thoreau"), 2)
          USER_FUNC(upgrade_thoreau_damage)
        CASE_EQUAL(6)
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_piccolo), 0, 0)
          USER_FUNC(upgrade_piccolo)
      END_SWITCH()
    CASE_EQUAL(1)
      USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(0))
      IF_LARGE_EQUAL(LW(0), 100)
        SUB(LW(0), 100)
      ELSE()
        USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_fail_coins), 0, PTR("lore"))
        USER_FUNC(spm::evt_mario::evt_mario_key_on)
        RETURN()
      END_IF()
      USER_FUNC(spm::evt_sub::evt_sub_item_select_menu, 0, PTR(curPixlArray), LW(10), 0)
      IF_SMALL_EQUAL(LW(10), 0)
        GOTO(0)
      END_IF()
      USER_FUNC(spm::evt_pouch::evt_pouch_set_coins, LW(0))
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
          USER_FUNC(upgrade_thoreau_damage)
        CASE_EQUAL(1)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Boomer"), 2)
          USER_FUNC(upgrade_boomer_radius)
          ADD(GSW(1701), 1)
        CASE_EQUAL(3)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Thudley"), 4)
          ADD(GSW(1702), 2)
        CASE_EQUAL(5)
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_fleep), 0, 0)
          USER_FUNC(upgrade_fleep_time)
        CASE_EQUAL(6)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Cudge"), 2)
          ADD(GSW(1700), 1)
        CASE_EQUAL(8)
          USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(lore_upgrade_piccolo), 0, 0)
          USER_FUNC(upgrade_piccolo)
        CASE_EQUAL(9)
          USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(lore_upgrade_pixl_damage), 0, 0, PTR("Barry"), 1)
          ADD(GSW(1703), 1)
        END_SWITCH()
  END_SWITCH()
  USER_FUNC(spm::evt_mario::evt_mario_key_on)
  RETURN()
EVT_END()


EVT_BEGIN(fwd_lore_speech)
    IF_EQUAL(GSWF(1702), 0)
        RUN_EVT(lore_speech_intro)
    ELSE()
        RUN_EVT(lore_speech)
    END_IF()
RETURN()
EVT_END()

EVT_BEGIN(lore_setup)
    USER_FUNC(reinit_pixl_array)
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("lore"), PTR("n_machi_dearle"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("lore"), 75, 25, FLOAT(-87.5))
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("lore"), 14, PTR(loreAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_rgba, PTR("lore"), 0, 0, 0, 0xff)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("lore"), 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("lore"), 9, PTR(fwd_lore_speech))
RETURN_FROM_CALL()

EVT_BEGIN(resetPixls)
  USER_FUNC(reinit_pixl_upgrades)
RETURN_FROM_CALL()

EVT_BEGIN(flipsideStart)
  IF_SMALL(GSW(1), 9)
    SET(GSW(1), 98)
    //USER_FUNC(spm::evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, resetPixls, 0, GSWF(433))
  END_IF()
RETURN_FROM_CALL()

static void inline patch_mac()
{
  spm::map_data::MapData * mac_05_md = spm::map_data::mapDataPtr("mac_05");
  spm::map_data::MapData * mac_15_md = spm::map_data::mapDataPtr("mac_15");
  spm::evt_door::DokanDesc* mac_05_pipe = (spm::evt_door::DokanDesc*)getInstructionEvtArg(mac_05_md->initScript, 11, 1);
  evtpatch::hookEvt(mac_05_md->initScript, 1, resetPixls);
  evtpatch::hookEvt(mac_15_md->initScript, 1, resetPixls);
  mac_05_pipe->destMapName = "dan_21";
}

void patch_dan_main()
{
    evtpatch::hookEvt(spm::dan::dan_chest_room_init_evt, 1, flipsideStart);
    evtpatch::hookEvtReplace(spm::dan::dan_chest_room_init_evt, 65, allow_shop);
    evtpatch::hookEvt(spm::dan::dan_chest_room_init_evt, 6, lore_setup);
    evtpatch::hookEvtReplaceBlock(spm::dan::dan_chest_room_init_evt, 7, turnNull, 47);
    patchBoomer();
    patchFleep();
    patchThoreau();
    patchPiccolo();
    patch_mac();
    patch_wracktail_main();
    getNextDanMapname = patch::hookFunction(spm::seq_title::getNextDanMapname, new_getNextDanMapname);
}

}