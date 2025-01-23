#include "mod.h"
#include "patch.h"
#include "cutscene_helpers.h"
#include "evt_cmd.h"
#include "evtpatch.h"
#include "exception.h"
#include "scripting.cpp"
#include "exception.h"
#include "romfontexpand.h"
#include "patchNinja.h"

#include <common.h>
#include <spm/rel/dan.h>
#include <spm/evt_cam.h>
#include <spm/evt_sub.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
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
#include <spm/map_data.h>
#include <spm/evtmgr_cmd.h>
#include <spm/seq_game.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>
#include <spm/spmario.h>
#include <spm/mario_pouch.h>
#include <spm/mario_motion.h>
#include <spm/mario_status.h>
#include <spm/seqdef.h>
#include <spm/npc_ninja.h>
#include <spm/npc_dimeen_l.h>
#include <spm/item_data_ids.h>
#include <spm/item_data.h>
//#include <spm/item_event_data.h>
#include <spm/evt_shop.h>
#include <msl/string.h>
#include <wii/os/OSError.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/ipc.h>
#include <string>
#include <cstdio>
#include <limits>
#include <functional>
using namespace std;
namespace mod {
spm::mario::MarioWork * marioWork = spm::mario::marioGetPtr();
spm::spmario::SpmarioGlobals * globals = spm::spmario::gp;
spm::evtmgr::EvtEntry * eventEntry;
int bossSequence = 1;
int holee = 0;
/*
    Title Screen Custom Text
    Prints "SPM Hard Mode" at the top of the title screen
*/
static spm::seqdef::SeqFunc *seq_titleMainReal;
static spm::seqdef::SeqFunc *seq_gameMainReal;

static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
  wii::gx::GXColor green = {
    0,
    255,
    0,
    255
  };
    f32 scale = 0.8f;
    const char * msg = "SPM Hard Mode";
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
int checkCharmStats()
{
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  int charms = pouch->killsBeforeNextCharm;
  if (charms > 0) {
    return charms;
  } else {
    return 0;
  }
}
int checkCharmNum()
{
  if (spm::mario::marioKeyOffChk() == 1) {
    return 0;
  }
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  int charms = pouch->charmsRemaining;
  if (charms > 0) {
    return charms;
  } else {
    return 0;
  }
}
int checkBossHealth() {
  if (spm::mario::marioKeyOffChk() == 1) {
    return 0;
  }
  spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
  int health = 0;
  s32 plotValue = globals->gsw0;
  if (globals->gsw[24] == 0) globals->gsw[24] = 1;
    if (plotValue == 0x21){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 270) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x66){
            bossSequence = 1;
              }
    if (plotValue == 0x67){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 315) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xb8){
      globals->gsw0 = 0xb9;
    }
    if (plotValue == 0x73){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 286) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x7c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 318) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xaa){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 295) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xab){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 296) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xbe){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 271) {
        health = NPCWork->entries[i].hp;
      }
      if (NPCWork->entries[i].tribeId == 289) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xd4){
        bossSequence = 1;
    }
    if (plotValue == 0xd5){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 272) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0xda){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 319) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x112){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 282) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x11d){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 300) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x139){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 316) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x160){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 327) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x16c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 273) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x191){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 292) {
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x19a){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 305) {
        wii::os::OSReport("%d\n", NPCWork->entries[i].m_Anim.m_nPoseId);
        health = NPCWork->entries[i].hp;
      }
    }}
    if (plotValue == 0x19c){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        bossSequence = 3;
        health = 99999;
      }
    }}
    if (plotValue == 0x19f){
    for (int i = 0; i < 535; i++) {
      if (NPCWork->entries[i].tribeId == 309) {
        health = NPCWork->entries[i].hp;
      }
    }}
  return health;
}
static void bossHealth(spm::seqdrv::SeqWork *wp)
{
    if (checkBossHealth() > 0){
      wii::gx::GXColor green = {
        0,
        255,
        0,
        255
      };
    f32 scale = 0.8f;
    const char * msg = "Boss Health:";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-300, 70.0f, msg);}
    seq_gameMainReal(wp);
}
static void bossActualHealth(spm::seqdrv::SeqWork *wp)
{
    if (checkBossHealth() > 0){
      wii::gx::GXColor green = {
        0,
        255,
        0,
        255
      };
    f32 scale = 0.8f;
    char buffer [50];
    int health = checkBossHealth();
    sprintf(buffer, "%d", health);
    if (health < 99999) {
    const char * msg = buffer;
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColor();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-320, 50.0f, msg);
  } else if (health == 99999) {
    const char * msg = "Infinite";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColor();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x-320, 50.0f, msg);
  }
  }
    seq_gameMainReal(wp);
}
void charmTextGenerator(spm::seqdrv::SeqWork *wp)
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  const char * msg = "Enemies until next charm:";
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColorOff();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+265, 70.0f, msg);}
  seq_gameMainReal(wp);
}
void charmKillsTextGenerator(spm::seqdrv::SeqWork *wp)
{
  int charmNum = checkCharmNum();
  if (charmNum > 0){
  char buffer [50];
  int charmStats = checkCharmStats();
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  sprintf(buffer, "%d", charmStats);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColor();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+350, 55.0f, msg);}
  seq_gameMainReal(wp);
}
void charmNumText(spm::seqdrv::SeqWork *wp)
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  wii::gx::GXColor red = {
        255,
        0,
        0,
        255
  };
  f32 scale = 0.6f;
  const char * msg = "Charms left:";
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColorOff();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+322, 100.0f, msg);}
  seq_gameMainReal(wp);
}
void charmNumLeftText(spm::seqdrv::SeqWork *wp)
{
  int charmStats = checkCharmNum();
  if (charmStats > 0){
  char buffer [50];
  wii::gx::GXColor red = {
      255,
      0,
      0,
      255
  };
  f32 scale = 0.6f;
  sprintf(buffer, "%d", charmStats);
  const char * msg = buffer;
  spm::fontmgr::FontDrawStart();
  spm::fontmgr::FontDrawEdge();
  spm::fontmgr::FontDrawColor(&red);
  spm::fontmgr::FontDrawScale(scale);
  spm::fontmgr::FontDrawNoiseOff();
  spm::fontmgr::FontDrawRainbowColor();
  f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
  spm::fontmgr::FontDrawString(x+350, 85.0f, msg);}
  seq_gameMainReal(wp);
}
void customUI(spm::seqdrv::SeqWork *wp)
{
  charmTextGenerator(wp);
  charmKillsTextGenerator(wp);
  charmNumText(wp);
  charmNumLeftText(wp);
  bossHealth(wp);
  bossActualHealth(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &customUI;
}
static void setBossHP() {
  spm::npcdrv::npcTribes[270].maxHp = 15; //O'Chunks 1
  spm::npcdrv::npcTribes[315].maxHp = 10; //Bowser 1
  spm::npcdrv::npcTribes[286].maxHp = 12; //Dimentio 1
  spm::npcdrv::npcTribes[318].maxHp = 25; //Francis
  spm::npcdrv::npcTribes[295].maxHp = 16; //Mr. L
  spm::npcdrv::npcTribes[271].maxHp = 20; //O'Chunks 2
  spm::npcdrv::npcTribes[289].maxHp = 10; //Dimentio 2
  spm::npcdrv::npcTribes[272].maxHp = 20; //O'Cabbage
  //spm::npcdrv::npcTribes[319].maxHp = 20; //King Croacus
  spm::npcdrv::npcTribes[282].maxHp = 15; //Mimi
  spm::npcdrv::npcTribes[300].maxHp = 36; //Brobot L-Type
  spm::npcdrv::npcTribes[316].maxHp = 12; //Bowser 2
  //spm::npcdrv::npcTribes[327].maxHp = 30; //Bonechill
  spm::npcdrv::npcTribes[273].maxHp = 100; //O'Chunks 3
  spm::npcdrv::npcTribes[292].maxHp = 5; //Dimentio 3
  spm::npcdrv::npcTribes[305].maxHp = 25; //Count Bleck
  spm::npcdrv::npcTribes[309].maxHp = 170; //Super Dimentio
  spm::npcdrv::npcTribes[330].maxHp = 12; //Dark Mario
  spm::npcdrv::npcTribes[331].maxHp = 12; //Dark Luigi
  spm::npcdrv::npcTribes[332].maxHp = 12; //Dark Peach
  spm::npcdrv::npcTribes[333].maxHp = 12; //Dark Bowser
}
static void setBossXp() {
  /*for (int i = 0; i < 535; i++) {
    if (spm::npcdrv::npcTribes[i].killXp >= 3) {
     int newXp = spm::npcdrv::npcTribes[i].killXp / 3;
     spm::npcdrv::npcTribes[i].killXp = newXp;
    }
  }*/
}
/*
  Gives all bosses the megabite defense stat
*/
static void setBossDef() {
  spm::npcdrv::NPCDefense def;
  def.type = 0x0;
  def.defense = 0x2;
  def.flags = 0x2;
  spm::npcdrv::NPCDefense fireDef;
  fireDef.type = 0xA;
  fireDef.defense = 0x64;
  fireDef.flags = 0x0;
  spm::npcdrv::NPCDefense holeDef1;
  holeDef1.type = 0;
  holeDef1.defense = 99;
  holeDef1.flags = 0x4;
  spm::npcdrv::NPCDefense holeDef2;
  holeDef2.type = 41;
  holeDef2.defense = 0;
  holeDef2.flags = 0x4;
  /*for (int i = 0; i < 7; i++) {//O'chunks 1 defense
    if (spm::npcdrv::npcTribes[270].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[270].partsList[i].defenses[0] = chunkDef;
    }
  }*/
  for (int i = 0; i < 2; i++) {//Bowser 1 defense
    if (spm::npcdrv::npcTribes[315].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[315].partsList[i].defenses[0] = def;
    }
  }
  for (int i = 0; i < 3; i++) {//Dimentio 1 defense
    if (spm::npcdrv::npcTribes[286].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[286].partsList[i].defenses[0] = fireDef;
    }
    if (spm::npcdrv::npcTribes[289].partsList[i].id == 1) {
     spm::npcdrv::npcTribes[289].partsList[i].defenses[0] = fireDef;
    }
  }
   for (int i = 0; i < 7; i++) {//Brobot defense
      spm::npcdrv::npcTribes[296].partsList[i].defenses[0] = def;
   }
   for (int i = 0; i < 7; i++) {//O'chunks 2 defense
     if (spm::npcdrv::npcTribes[271].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[271].partsList[i].defenses[0] = fireDef;
      spm::npcdrv::npcTribes[271].partsList[i].defenses[5] = def;
     }
   }
   //spm::npcdrv::npcTribes[282].partsList[0].defenses[0] = def;//mimi defense
   for (int i = 0; i < 15; i++) {//Brobot L-Type defense
      spm::npcdrv::npcTribes[300].partsList[i].defenses[0] = def;
   }
   for (int i = 0; i < 2; i++) {//Bowser 2 defense
     if (spm::npcdrv::npcTribes[316].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[316].partsList[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 9; i++) {//Underchomp defense
     if (spm::npcdrv::npcTribes[316].partsList[i].id == 1) {
      spm::npcdrv::npcTribes[316].partsList[i].defenses[0] = def;
     }
   }
   for (int i = 0; i < 21; i++) {//Bonechill defense
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 2) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 3) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
     if (spm::npcdrv::npcTribes[327].partsList[i].id == 4) {
      spm::npcdrv::npcTribes[327].partsList[i].defenses[2] = def;
     }
   }
   /*for (int i = 0; i < 3; i++) {//Dimentio 2 defense
      spm::npcdrv::npcTribes[292].partsList[i].defenses[0] = def;
      spm::npcdrv::npcTribes[293].partsList[i].defenses[0] = def;
   }*/
      spm::npcdrv::npcTribes[306].partsList[0].defenses[0] = holeDef1;
      spm::npcdrv::npcTribes[306].partsList[0].defenses[1] = holeDef2;

   }
/*
    Function patching
*/
void (*marioTakeDamage)(wii::mtx::Vec3 * position, u32 flags, s32 damage);

s32 scriptTakeDamage(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
  pouch->hp = pouch->hp - 6;
  return firstRun;
}

int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
void (*seq_gameMain)(spm::seqdrv::SeqWork *param_1);
/*spm::evtmgr::EvtEntry * newEntry(const spm::evtmgr::EvtScriptCode * script, u8 priority, u8 flags, s32 type)
        {
          spm::mario_pouch::MarioPouchWork * pouch = spm::mario_pouch::pouchGetPtr();
          //turtley leaf
            if (spm::item_event_data::getItemUseEvt(104) == script){
              pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
            }
            //bone in cut and dayzee syrup
            if (spm::item_event_data::getItemUseEvt(113) == script || spm::item_event_data::getItemUseEvt(124) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //hot sauce and spit roast
            if (spm::item_event_data::getItemUseEvt(109) == script || spm::item_event_data::getItemUseEvt(162) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //spicy dinner and spicy pasta dish
            if (spm::item_event_data::getItemUseEvt(183) == script || spm::item_event_data::getItemUseEvt(155) == script){
              pouch->charmsRemaining += 5;
              pouch->killsBeforeNextCharm += 1;
            }
            //dangerous delight
            if (spm::item_event_data::getItemUseEvt(173) == script){
              pouch->killsBeforeNextCharm += 2;
              pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
              pouch->charmsRemaining += 6;
            }*
            return evtEntryType(script, priority, flags, type);
        }*/

void patchGameMain() {
seq_gameMain = patch::hookFunction(spm::seq_game::seq_gameMain,
[](spm::seqdrv::SeqWork *param_1)
    {
        seq_gameMain(param_1);
    }
);
}

void reduceXpGained() {
  for (int i = 0; i < 535; i++) {
    if (spm::npcdrv::npcTribes[i].killXp >= 2) {
     int newXp = spm::npcdrv::npcTribes[i].killXp / 2;
     spm::npcdrv::npcTribes[i].killXp = newXp;
    }
    if (spm::npcdrv::npcTribes[i].stylishXp >= 2) {
       int newXp = spm::npcdrv::npcTribes[i].stylishXp / 2;
       spm::npcdrv::npcTribes[i].stylishXp = newXp;
    }
  }
  for (int i = 0; i < ITEM_ID_MAX; i++) {
    if (spm::item_data::itemDataTable[i].xpGain >= 2) {
      spm::item_data::itemDataTable[i].xpGain = spm::item_data::itemDataTable[i].xpGain / 2;
    }
  }
}

s32 itemCharm(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  if (pouch->killsBeforeNextCharm > 5) {
    pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm - 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 5;
  } else if (pouch->killsBeforeNextCharm < 6) {
    pouch->killsBeforeNextCharm = 5;
    pouch->charmsRemaining = pouch->charmsRemaining + 5;
  }
  return 2;
}

s32 reduceEnemyRequirements(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_pouch::MarioPouchWork* pouch = spm::mario_pouch::pouchGetPtr();
  if (pouch->killsBeforeNextCharm > 1) {
    pouch->killsBeforeNextCharm = pouch->killsBeforeNextCharm / 2;
  }
  return 2;
}

s32 removeAbilities(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::mario_status::marioStatusApplyStatuses(4, 2);
  return 2;
}

s32 unPauseGame(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
  spm::pausewin::pausewinUnpauseGame();
  spm::spmario::spmarioSystemLevel(0);
  return 2;
}

s32 giveHampterCheese(spm::evtmgr::EvtEntry * evtEntry, bool firstRun) {
spm::mapdrv::mapGrpFlagOn(0, "A2_pas_01", 1);
spm::mapdrv::mapGrpFlagOn(0, "A3_pas_01", 1);
spm::mapdrv::mapGrpFlagOn(0, "A2_doa_05", 1);
spm::mapdrv::mapGrpFlagOn(0, "A3_doa_05", 1);
return 2;
}

EVT_DECLARE_USER_FUNC(itemCharm, 0)
EVT_DECLARE_USER_FUNC(giveHampterCheese, 0)
EVT_DECLARE_USER_FUNC(reduceEnemyRequirements, 0)
EVT_DECLARE_USER_FUNC(removeAbilities, 0)

EVT_BEGIN(insertNop)
  SET(LW(0), 0)
RETURN_FROM_CALL()

EVT_BEGIN(summonMario)
  SET(LW(0), 0)
RETURN_FROM_CALL()

EVT_BEGIN(charmAdd)
  USER_FUNC(itemCharm)
RETURN_FROM_CALL()

EVT_BEGIN(reduceCharmEnemies)
  USER_FUNC(reduceEnemyRequirements)
RETURN_FROM_CALL()

EVT_BEGIN(increaseEggDamage)
  SET(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(increaseSnowBunnyDamage1)
  SET(LW(10), 7)
RETURN_FROM_CALL()

EVT_BEGIN(increaseSnowBunnyDamage2)
  SET(LW(10), 8)
RETURN_FROM_CALL()

EVT_BEGIN(increaseDynamiteDamage)
  SET(LW(10), 13)
RETURN_FROM_CALL()

EVT_BEGIN(changeThunderDamage)
  IF_LARGE(LW(5), 4)
    SET(LW(5), 4)
  END_IF()
RETURN_FROM_CALL()

void patchItems() {
  spm::evtmgr::EvtScriptCode* mightyTonicUseScript = spm::item_event_data::getItemUseEvt(75);
  evtpatch::hookEvtReplace(mightyTonicUseScript, 17, (spm::evtmgr::EvtScriptCode*)charmAdd);
  spm::evtmgr::EvtScriptCode* default_item_script = spm::item_event_data::default_item_use_evt;
  evtpatch::hookEvt(default_item_script, 67, (spm::evtmgr::EvtScriptCode*)reduceCharmEnemies);
  evtpatch::hookEvt(default_item_script, 45, (spm::evtmgr::EvtScriptCode*)reduceCharmEnemies);
  evtpatch::hookEvt(default_item_script, 58, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 78, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 129, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 70, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 109, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 113, (spm::evtmgr::EvtScriptCode*)charmAdd);
  evtpatch::hookEvt(default_item_script, 44, (spm::evtmgr::EvtScriptCode*)charmAdd);
  spm::evtmgr::EvtScriptCode* eggBomb = spm::item_event_data::getItemUseEvt(160);
  evtpatch::hookEvtReplace(eggBomb, 18, (spm::evtmgr::EvtScriptCode*)increaseEggDamage);
  evtpatch::hookEvtReplace(eggBomb, 20, (spm::evtmgr::EvtScriptCode*)increaseDynamiteDamage);
  spm::evtmgr::EvtScriptCode* snowBunny = spm::item_event_data::getItemUseEvt(148);
  evtpatch::hookEvtReplace(snowBunny, 16, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage1);
  evtpatch::hookEvtReplace(snowBunny, 18, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage2);
  evtpatch::hookEvtReplace(snowBunny, 12, (spm::evtmgr::EvtScriptCode*)increaseSnowBunnyDamage2);
  spm::evtmgr::EvtScriptCode* thunderRage = spm::item_event_data::getItemUseEvt(67);
  evtpatch::hookEvt(thunderRage, 46, (spm::evtmgr::EvtScriptCode*)changeThunderDamage);
}

void patchCooking() {
  spm::evtmgr::EvtScriptCode* cookScript = spm::evt_shop::saffronCookingScript;
  evtpatch::hookEvtReplaceBlock(cookScript, 130, (spm::evtmgr::EvtScriptCode*)insertNop, 189);
}

void patchMarioDamage(){
  marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
    [](wii::mtx::Vec3 * position, u32 flags, s32 damage)
            {
              if (marioWork->character == 2) {
                damage = damage - 1;
                if (damage <= 0) {
                  damage = 1;
                }
              }
              //adds the rpg elements to boss fights
              int health = checkBossHealth();
              s32 plotValue = globals->gsw0;
              if (plotValue == 0x67 && bossSequence > 0){
                if (health > 0 && health <= 3 && bossSequence == 1){
                  //Bowser
              bossSequence -= 1;
              damage = 0;
              flags = 0x4;
              marioTakeDamage(position, flags, damage);
              spm::pausewin::pausewinPauseGame();
              spm::spmario::spmarioSystemLevel(1);
              for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
            eventEntry = spm::evtmgr::evtEntryType(iceStorm, 0, 0, 0);
          }}} else {
            marioTakeDamage(position, flags, damage * 2);
          }} else if (plotValue == 0xd5 && bossSequence > 0){
            if (health <= 10 && bossSequence == 1){
              //O'Cabbage
          bossSequence -= 1;
          damage = 0;
          flags = 0x4;
          marioTakeDamage(position, flags, damage);
          spm::pausewin::pausewinPauseGame();
          spm::spmario::spmarioSystemLevel(1);
          for (int i = 0; i < 33; i++) {
        if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
        eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
      }}} else {
        marioTakeDamage(position, flags, damage * 2);
      }} else if (plotValue == 0xd5 && bossSequence > 0){
        if (health <= 6 && bossSequence == 1){
          //King Croacus
      bossSequence -= 1;
      damage = 0;
      flags = 0x4;
      marioTakeDamage(position, flags, damage);
      spm::pausewin::pausewinPauseGame();
      spm::spmario::spmarioSystemLevel(1);
      for (int i = 0; i < 33; i++) {
    if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
    eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
  }}} else {
    marioTakeDamage(position, flags, damage * 2);
  }} else if (plotValue == 0x19f && bossSequence > 0){
                if (health <= 150 && bossSequence == 3){
                  //Super Dimentio
              bossSequence -= 1;
              damage = 0;
              flags = 0x4;
              marioTakeDamage(position, flags, damage);
              spm::pausewin::pausewinPauseGame();
              spm::spmario::spmarioSystemLevel(1);
              for (int i = 0; i < 33; i++) {
            if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
            eventEntry = spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
          }}} else if (health <= 100 && bossSequence == 2){
        bossSequence -= 1;
        damage = 0;
        flags = 0x4;
        marioTakeDamage(position, flags, damage);
        spm::pausewin::pausewinPauseGame();
        spm::spmario::spmarioSystemLevel(1);
        for (int i = 0; i < 33; i++) {
      if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
      eventEntry = spm::evtmgr::evtEntryType(fireBurst, 0, 0, 0);
    }}} else if (health <= 50 && bossSequence == 1){
    bossSequence -= 1;
    damage = 0;
    flags = 0x4;
    marioTakeDamage(position, flags, damage);
    spm::pausewin::pausewinPauseGame();
    spm::spmario::spmarioSystemLevel(1);
    for (int i = 0; i < 33; i++) {
    if (spm::item_event_data::itemEventDataTable[i].itemId == 68) {
    eventEntry = spm::evtmgr::evtEntryType(shootingStar, 0, 0, 0);
    }}} else {
      marioTakeDamage(position, flags, damage * 2);
    }
        }
            else {
              marioTakeDamage(position, flags, damage * 2);
            }
            }
        );
  marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
    [](s32 damageType, s32 tribeId)
            {
              //spm::npcdrv::NPCWork * NPCWork = spm::npcdrv::npcGetWorkPtr();
              //wii::os::OSReport("%d\n", damageType);
              /*if (damageType == 1) {
                spm::setup_data::MiscSetupDataV6 miscSetupData;
                s32 test1 = 0x80a7cfc0;
                s32 test2 = 0x33c00000;
                miscSetupData.instanceId = 0;
                miscSetupData.gravityRotation = 0;
                for (int i = 0; i < 16; i++) {
                  miscSetupData.unitWork[i] = 0;
                }
                wii::mtx::Vec3 pos = marioWork->position;
                spm::npcdrv::NPCEntry * voidEntry = spm::npcdrv::npcEntryFromSetupEnemy(0, &pos, 198, &miscSetupData);
                voidEntry->partsList = spm::npcdrv::npcTribes[307].partsList;
              }*/
              int damage = 0;
              if (damageType == 20) return 2; //Shell Shock Damage Nerf
              switch(tribeId) {
                case 211:
                damage = 1;
                break;
                case 214:
                damage = 1;
                break;
                case 217:
                damage = 1;
                break;
                case 270:
                damage = 100;
                break;
                case 272:
                damage = 1;
                break;
                case 273:
                damage = 2;
                break;
                case 282:
                damage = 1;
                break;
                case 286:
                damage = 1;
                break;
                case 289:
                damage = 1;
                break;
                case 292:
                return 1;
                break;
                case 295:
                damage = 1;
                break;
                case 305:
                damage = 1;
                break;
                case 306:
                return 0;
                break;
                case 309:
                /*if (damageType == 1) {
                  for (int i = 0; i < 535; i++) {
                    if (NPCWork->entries[i].tribeId == 309) {
                      NPCWork->entries[i].hp = NPCWork->entries[i].hp - 1;
                    }
                  }
                  damage = 1;
                } else {return 1;}*/
                break;
                case 316:
                damage = 3;
                break;
                case 318:
                damage = 1;
                break;
                case 319:
                damage = 2;
                break;
                case 330:
                damage = 1;
                break;
                case 331:
                damage = 1;
                break;
                case 332:
                damage = 1;
                break;
                case 333:
                damage = 5;
                break;
              }
              if (damageType == 0xf) {
                if (damage == 0) {
                  damage = marioCalcDamageToEnemy(0, tribeId) + 2;
                }
                if (damage > 0) {
                  damage = damage + 1;
                }
              }
              if (damage > 0) return damage;
              if (tribeId > -1){
              if (spm::npcdrv::npcTribes[tribeId].catchCardItemId != 0){
              int cards = spm::mario_pouch::pouchGetCardCount(spm::npcdrv::npcTribes[tribeId].catchCardItemId);
              cards = cards + 1;
              return marioCalcDamageToEnemy(damageType, tribeId) / cards;
            }
          }
          return marioCalcDamageToEnemy(damageType, tribeId);
            }
        );
}

void patchBarry()
{
  //check if the version is eu0, if not then change the assembly to point to the correct float which is 0x8 off between versions
  #ifdef SPM_EU0
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x2BC, 0xC002BA98); //Barry patch 1
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x298, 0xC022BA98); //Barry patch 2
  #else
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x2BC, 0xC002BA90); //Barry patch 1
    writeWord(spm::mario_motion::marioMotTbl[0x2A].mainFunc, 0x298, 0xC022BA90); //Barry patch 2
  #endif
}

void patchVariables() {
    writeWord(&spm::mario::marioCalcDamageToEnemy, 0x16C, 0x57FF003E);
      writeWord(&spm::mario::marioCalcDamageToEnemy, 0xC8, 0x57FF003C);
       writeWord(&spm::mario::marioCalcDamageToEnemy, 0x94, 0x57FF003C);
        writeWord(&spm::npc_dimeen_l::superDimentioHeadDefenses, 0x0, 0x00000002);
          writeWord(&spm::npc_dimeen_l::superDimentioBodyDefenses, 0x0, 0x00000002);
            patchBarry();
}

/*
    General mod functions
*/

spm::evtmgr::EvtScriptCode* getInstructionEvtArg(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  wii::os::OSReport("%p\n", link);
  spm::evtmgr::EvtScriptCode* arg = evtpatch::getInstructionArgv(link)[instruction];
  wii::os::OSReport("%p\n", arg);
  return arg;
}

spm::npcdrv::NPCDefense * getInstructionEvtDefense(spm::evtmgr::EvtScriptCode* script, s32 line, int instruction)
{
  spm::evtmgr::EvtScriptCode* link = evtpatch::getEvtInstruction(script, line);
  wii::os::OSReport("%p\n", link);
  spm::npcdrv::NPCDefense * arg = evtpatch::getInstructionArgv(link)[instruction];
  wii::os::OSReport("%p\n", arg);
  return arg;
}

s32 mimiHittable = 1;
spm::evtmgr::EvtScriptCode* bleckMovementScript;
spm::evtmgr::EvtScriptCode* standard_death_script;

EVT_BEGIN(rollForNinjoe)
  USER_FUNC(spm::evt_sub::evt_sub_random, 90, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_sub::evt_sub_random, 9, LW(0))
    IF_LARGE(LW(0), 6)
      USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 178)
      USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 178, 0, -100, 0, LW(10), EVT_NULLPTR)
      USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
      USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
      USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
      USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
      ADD(LW(2), 100)
      USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
      USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
    ELSE()
      IF_LARGE(LW(0), 3)
        USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 259)
        USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 259, 0, -100, 0, LW(10), EVT_NULLPTR)
        USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
        USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
        USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
        ADD(LW(2), 100)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
      ELSE()
        USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 262)
        USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 262, 0, -100, 0, LW(10), EVT_NULLPTR)
        USER_FUNC(spm::evt_npc::evt_npc_flip_to, LW(10), 1)
        USER_FUNC(spm::evt_npc::evt_npc_finish_flip_instant, LW(10))
        USER_FUNC(spm::evt_npc::evt_npc_flip, LW(10))
        USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
        ADD(LW(2), 100)
        USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), LW(1), LW(2), LW(3))
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_NINJA_APPEAR1"), LW(10))
      END_IF()
    END_IF()
  END_IF()
RETURN_FROM_CALL()

void patchStandardDeathScript(){
  evtpatch::hookEvt(standard_death_script, 13, (spm::evtmgr::EvtScriptCode*)rollForNinjoe);
}

EVT_BEGIN(changeFlyingSpeedBleck)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(180.0), 0, 11, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeFlyingSpeed)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 5, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeBoxAttack)
  USER_FUNC(spm::evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(1))
  ADD(LW(1), 1)
RETURN_FROM_CALL()

EVT_BEGIN(turnNull)
  NEXT()
RETURN_FROM_CALL()

EVT_BEGIN(changeSlowdown)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 10, 0)
  USER_FUNC(spm::evt_npc::evt_npc_get_hp, PTR("me"), LW(0))
  IF_LARGE(LW(0), 20)
    ADD(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_hp, PTR("me"), LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 10, 0)
    SET(LW(0), 16)
    WAIT_MSEC(1000)
  ELSE()
    WAIT_MSEC(200)
  END_IF()
RETURN_FROM_CALL()

EVT_BEGIN(changeDashAttack)
  USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(5), LW(6), LW(7), 0, FLOAT(800.0), 0, 2, 0, 0)
RETURN_FROM_CALL()

EVT_BEGIN(changeVoids)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 0, 2048)
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
  SET(LW(0), 25)
END_IF()
IF_SMALL(LW(0), 45)
  SET(LW(0), 55)
END_IF()
END_IF()
RETURN_FROM_CALL()

spm::evtmgr::EvtScriptCode* mimiUnk2 = spm::npcdrv::npcEnemyTemplates[130].unkScript2;
spm::evtmgr::EvtScriptCode* mimiRollAttack = getInstructionEvtArg(mimiUnk2, 63, 0);
spm::npcdrv::NPCDefense * chaosHeartDefense = getInstructionEvtDefense(mimiRollAttack, 20, 3);

EVT_BEGIN(removeSkills)
  USER_FUNC(removeAbilities)
RETURN_FROM_CALL()

EVT_BEGIN(mimiChangeDefense)
  USER_FUNC(spm::evt_npc::evt_npc_modify_part, PTR("me"), 1, 15, PTR(chaosHeartDefense))
RETURN_FROM_CALL()

EVT_BEGIN(mimiFlag8_2048)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 2048)
RETURN_FROM_CALL()

EVT_BEGIN(mimiFlag8_turn_both_on)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 2048)
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("me"), 1, 32768)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiAi1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 4)
RETURN_FROM_CALL()

EVT_BEGIN(changeMimiAi2)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 1)
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

EVT_BEGIN(increaseLuigiShits)
  SET(LW(0), 60000)
RETURN_FROM_CALL()

EVT_BEGIN(makePhase1LastLessTime)
  SET(LW(1), 250)
RETURN_FROM_CALL()

EVT_BEGIN(makeBowserJump)
  ADD(LW(1), 100)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(4), LW(5))
  USER_FUNC(spm::evt_npc::evt_npc_jump_to, PTR("me"), LW(0), LW(1), LW(2), FLOAT(0.0), 180)
RETURN_FROM_CALL()

EVT_BEGIN(changeBowserScript)
  USER_FUNC(spm::evt_sub::evt_sub_random, 3, LW(0))
  IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 0, 4)
  END_IF()
RETURN_FROM_CALL()

/*EVT_BEGIN(shadooFight)
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
USER_FUNC(spm::evt_npc::func_80102bf8, LW(10))
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
USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 200)
USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 200, 0, -100, 0, LW(10), EVT_NULLPTR)
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
USER_FUNC(spm::evt_npc::func_80102bf8, LW(10))
RETURN_FROM_CALL()*/

EVT_BEGIN(oChunksFix)
IF_LARGE(GSW(0), 35)
  SET(LW(0), 0)
END_IF()
RETURN_FROM_CALL()

const char * dimentio_stg5_1 = "<p>\n"
"Ah ha ha ha, I'm surprised you\n"
"defeated him so easily...\n"
"<k>\n";

const char * dimentio_stg5_2 = "<p>\n"
"But I'm not going to leave\n"
"quite yet...\n"
"<k>\n";

const char * dimentio_stg5_3 = "<p>\n"
"<wave>En garde!</wave>\n"
"<k>\n";

const char * dimentio_stg5_4 = "<p>\n"
"<wave>Look how much effort it takes\n"
"for you to land 10 blows on me.</wave>\n"
"<k>\n";

const char * dimentio_stg5_5 = "<p>\n"
"Alas, I would let you entertain\n"
"me further...\n"
"<k>\n"
"<p>\n"
"But I have an appointment\n"
"with an old acquaintance\n"
"I must keep.\n"
"<k>\n";

const char * dimentio_stg5_6 = "<p>\n"
"I have one last surprise\n"
"for you.\n"
"<k>\n"
"<p>\n"
"Ah ha ha ha.\n"
"<wave>Ciao!</wave>\n"
"<k>\n";

spm::npcdrv::NPCTribeAnimDef animsDimen[] = {
  {0, "S_2"}, // Flying (ID 0 is idle animation)
  {1, "S_1"}, // Standing
  {20, "W_3"}, // Finger click
  {21, "A_1a"}, // Magic start
  {22, "W_2c"}, // Land
  {23, "A_1c"}, // Magic cast
  {24, "A_2a"}, // Raise hands
  {25, "A_2b"}, // Wave hands
  {26, "A_2c"}, // Raise hands cast
  {27, "W_2a"}, // Fly
  {3, "T_2"}, // Talking (ID 3 is the ID to use when talking)
  {-1, nullptr}
};

EVT_BEGIN(spawnDimiGn2)
  USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("dimi"), PTR("e_dmen"), 0)
  USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("dimi"), 0xe, PTR(animsDimen))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 0, 1)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), 450, 25, 0)
RETURN_FROM_CALL()

EVT_BEGIN(dimentioMusic1)
  USER_FUNC(spm::evt_snd::evt_snd_bgmon_f_d, 0, PTR("BGM_BTL_DIMEN1"), 2000)
RETURN_FROM_CALL()

EVT_BEGIN(dimiDeath)
  SET(LW(10), PTR("me"))
  USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
  USER_FUNC(spm::evt_mario::evt_mario_fairy_reset)
  USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, LW(10), 0)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, LW(10), 1, 1073741824)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("dmen_warp"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_BS_DMN_GOOUT1"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, LW(10), 0, -1000, 0)
  WAIT_MSEC(700)
  USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
  ADD(LW(0), 100)
  ADD(LW(1), 75)
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, PTR("dimi"))
  WAIT_MSEC(300)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_4), 0, PTR("dimi"))
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, 484, LW(0), EVT_NULLPTR, -16, 1000, 11)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_5), 0, PTR("dimi"))
  USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 2000)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_6), 0, PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 24, 1)
  USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("dimi"), 1)
  USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_MAGIC_HAND_UP1"), PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 25, 1)
  WAIT_MSEC(1500)
  USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("dimi"), 26, 1)
  USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("dimi"), 1)
  USER_FUNC(spm::evt_fade::evt_set_transition, 26, 25)
  INLINE_EVT()
    USER_FUNC(spm::evt_env::evt_env_blur_on, 0, 1000)
  END_INLINE()
  SET(GSW(0), 191)
  USER_FUNC(spm::evt_seq::evt_seq_mapchange, PTR("gn2_04"), 0)
EVT_END()

EVT_BEGIN(dimentioDialogue1)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_1), 0, PTR("dimi"))
  USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, -1, 380, 60, 384, 380, 60, -16, 800, 11)
  WAIT_MSEC(800)
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_2), 0, PTR("dimi"))
  USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(dimentio_stg5_3), 0, PTR("dimi"))
  USER_FUNC(spm::evt_npc::evt_npc_tribe_agb_async, 225)
  USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("dimi"), LW(0), LW(1), LW(2))
  USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, 225, LW(0), LW(1), LW(2), LW(10), EVT_NULLPTR)
  USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, LW(10), 8, PTR(dimiDeath))
  USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("dimi"), 0, -1000, 0)
  USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 0, PTR("doa1_l"))
  SET(GSW(0), 190)
RETURN_FROM_CALL()

EVT_BEGIN(checkForDimentio) //If stone tablet is active during dimentio fight it causes a crash, so check for dimentio via GSW and disable it if its too low
  IF_SMALL(GSW(0), 191)
    RETURN()
  END_IF()
RETURN_FROM_CALL()

void hookShadooScripts()
{
  //evtpatch::hookEvtReplaceBlock(spm::dan::dan_shadoo_fight_evt, 1, (spm::evtmgr::EvtScriptCode*)shadooFight, 40);
  evtpatch::hookEvt(spm::npcdrv::npcEnemyTemplates[281].unkScript6, 27, (spm::evtmgr::EvtScriptCode*)oChunksFix); //Fix for if O'Chunks is killed outside of gsw 35, not needed right now
}

void hookBowserScripts()
{
  //spm::evtmgr::EvtScriptCode* bowserOnSpawn = spm::npcdrv::npcEnemyTemplates[285].onSpawnScript;
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[285].unkScript7;
  evtpatch::hookEvt(mainLogic, 37, (spm::evtmgr::EvtScriptCode*)changeBowserScript);
  evtpatch::hookEvt(mainLogic, 172, (spm::evtmgr::EvtScriptCode*)makeBowserJump);
}

void hookDimentioScripts()
{
  spm::evtmgr::EvtScriptCode* dimentioOnSpawn = spm::npcdrv::npcEnemyTemplates[142].onSpawnScript;
  wii::os::OSReport("%p\n", dimentioOnSpawn);
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

void hookSuperDimentioScripts()
{
  spm::evtmgr::EvtScriptCode* mainLogic = spm::npcdrv::npcEnemyTemplates[255].unkScript7;
  evtpatch::hookEvtReplace(mainLogic, 10, (spm::evtmgr::EvtScriptCode*)increaseLuigiShits);
  evtpatch::hookEvtReplace(mainLogic, 34, (spm::evtmgr::EvtScriptCode*)makePhase1LastLessTime);
  //spm::evtmgr::EvtScriptCode* throwAttack = getInstructionEvtArg(mainLogic, 65, 0);
  //evtpatch::hookEvt(mainLogic, 21, (spm::evtmgr::EvtScriptCode*)throwAttack);
}

EVT_BEGIN(activateHampter)
  USER_FUNC(giveHampterCheese)
RETURN_FROM_CALL()

void hampter()
{
    spm::map_data::MapData * mi3_01_md = spm::map_data::mapDataPtr("mi3_01");
    evtpatch::hookEvtReplaceBlock(mi3_01_md->initScript, 47, (spm::evtmgr::EvtScriptCode*)activateHampter, 57);
}

void hookChunkScripts()
{
  spm::map_data::MapData * gn2_03_md = spm::map_data::mapDataPtr("gn2_03");
  spm::evtmgr::EvtScriptCode* gn2_03_init = gn2_03_md->initScript;
  spm::evtmgr::EvtScriptCode* oChunksDeath = getInstructionEvtArg(gn2_03_init, 52, 0);
  spm::evtmgr::EvtScriptCode* stoneTabletScript = getInstructionEvtArg(gn2_03_init, 56, 0);
  evtpatch::hookEvtReplace(oChunksDeath, 99, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(oChunksDeath, 102, (spm::evtmgr::EvtScriptCode*)spawnDimiGn2);
  evtpatch::hookEvt(oChunksDeath, 143, (spm::evtmgr::EvtScriptCode*)dimentioDialogue1);
  evtpatch::hookEvt(stoneTabletScript, 1, (spm::evtmgr::EvtScriptCode*)checkForDimentio);
  evtpatch::hookEvtReplace(oChunksDeath, 146, (spm::evtmgr::EvtScriptCode*)dimentioMusic1);
  evtpatch::hookEvtReplaceBlock(oChunksDeath, 147, (spm::evtmgr::EvtScriptCode*)turnNull, 150);
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
  spm::evtmgr::EvtScriptCode* mimiOnHitScript = spm::npcdrv::npcEnemyTemplates[130].unkScript3;
  spm::evtmgr::EvtScriptCode* mimiMovement = getInstructionEvtArg(mimiUnk2, 56, 0);
  spm::evtmgr::EvtScriptCode* mimiMainAttack = getInstructionEvtArg(mimiUnk2, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingMovement = getInstructionEvtArg(mimiUnk2, 60, 0);
  spm::evtmgr::EvtScriptCode* mimiCeilingAttack = getInstructionEvtArg(mimiCeilingMovement, 58, 0);
  spm::evtmgr::EvtScriptCode* mimiMoneyWave = getInstructionEvtArg(mimiUnk2, 65, 0);
  #ifdef SPM_EU0
    spm::evtmgr::EvtScriptCode* mimiTrueHit = getInstructionEvtArg(mimiOnHitScript, 55, 0);
  #else
    spm::evtmgr::EvtScriptCode* mimiTrueHit = getInstructionEvtArg(mimiOnHitScript, 54, 0);
  #endif
  //evtpatch::hookEvt(mimiUnk2, 54, (spm::evtmgr::EvtScriptCode*)removeMimiBasicWalk);
  evtpatch::hookEvtReplace(mimiMainAttack, 31, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 25, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 29, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 39, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 43, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvtReplace(mimiTrueHit, 53, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvt(mimiTrueHit, 116, (spm::evtmgr::EvtScriptCode*)mimiFlag8_2048);
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
  evtpatch::hookEvt(mimiRollAttack, 58, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 60, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 75, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 77, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 92, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 94, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed);
  evtpatch::hookEvt(mimiRollAttack, 135, (spm::evtmgr::EvtScriptCode*)changeMimiAi1);
  evtpatch::hookEvt(mimiRollAttack, 1, (spm::evtmgr::EvtScriptCode*)removeSkills);
  //evtpatch::hookEvtReplace(mimiRollAttack, 114, (spm::evtmgr::EvtScriptCode*)turnNull);
  evtpatch::hookEvt(mimiMoneyWave, 19, (spm::evtmgr::EvtScriptCode*)changeMimiSpeed2);
  evtpatch::hookEvt(mimiMoneyWave, 52, (spm::evtmgr::EvtScriptCode*)changeMimiAi2);
  evtpatch::hookEvtReplace(mimiMoneyWave, 48, (spm::evtmgr::EvtScriptCode*)turnNull);
}

void getStandardDeathScript()
{
  spm::evtmgr::EvtScriptCode* dimentioOnDeath = spm::npcdrv::npcEnemyTemplates[225].unkScript6;
  standard_death_script = getInstructionEvtArg(dimentioOnDeath, 20, 0);
}

s32 dimen_determine_move_pos_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
{
    spm::mario::MarioWork *marioWork = spm::mario::marioGetPtr();
    spm::npcdrv::NPCEntry *npc = entry->ownerNPC;
    double destYPos = 0;
    f32 marioZ = ((marioWork->position).z);
    f32 destXPos = 0;
    u32 dimenMoveRand = 0;
    f32 negXBoundary = spm::evtmgr_cmd::evtGetFloat(entry, entry->lw[11]);
    f32 posXBoundary = spm::evtmgr_cmd::evtGetFloat(entry, entry->lw[12]);
    s32 i = 0;
    do
    {
        while (true)
        {
            do
            {
                i = i + 1;
                dimenMoveRand = spm::system::irand(400);
                destXPos = ((marioWork->position).x + (f32)dimenMoveRand - 200);
                if (i > 50)
                {
                    destXPos = npc->position.x;
                    goto outOfBounds;
                }
            } while ((destXPos <= negXBoundary) || (posXBoundary <= destXPos));
        outOfBounds:
            u32 yMoveBehavior = spm::system::irand(100);
            if (yMoveBehavior < 67)
            {
                dimenMoveRand = spm::system::irand(4);
                destYPos = (10.0 * (f32)dimenMoveRand + 20.0);
            }
            else
            {
                dimenMoveRand = spm::system::irand(3);
                destYPos = (32.0 * (f32)dimenMoveRand + 40.0);
            }
            if (npc->flippedTo3d != 0)
                break;
            if ((100.0 < __builtin_abs((destXPos - (marioWork->position).x))) || (80.0 < destYPos))
                goto setFloats;
        }
        destYPos = spm::system::distABf(destXPos, marioZ, ((marioWork->position).x), marioZ);
    } while ((destYPos <= 120.0) && (destYPos <= 80.0));
setFloats:
    spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
    spm::evtmgr_cmd::evtSetFloat(entry, args[0], destXPos);
    spm::evtmgr_cmd::evtSetFloat(entry, args[1], destYPos);
    spm::evtmgr_cmd::evtSetFloat(entry, args[2], marioZ);
    return 2;
}

    EVT_BEGIN(patch_dimen)
    SWITCH(GSW(0))
    CASE_SMALL(117) // 3-3 Dimentio, vanilla coordinates
    SET(LW(11), -350)
    SET(LW(12), 350)
    SWITCH_BREAK()
    CASE_SMALL(195) // 5-2 Dimentio, for use with Hard Mode R&M's gn2_03 Dimentio fight
    SET(LW(11), -958)
    SET(LW(12), 955)
    SWITCH_BREAK()
    CASE_SMALL(404) // 8-3 Dimentio, widened coordinates to fit the larger room
    SET(LW(11), -465)
    SET(LW(12), 465)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    static void dimenPatch()
        { //thanks to eemuh
            patch::hookFunction(spm::evt_npc::evt_npc_dimen_determine_move_pos, dimen_determine_move_pos_new);

            evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_1, 1, patch_dimen);
            evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_2, 1, patch_dimen);
            evtpatch::hookEvt(spm::npc_dimeen_l::dimen_unk_fight_script_3, 1, patch_dimen);
        }

        void patchTpl(u32 destId, u32 srcId, wii::tpl::TPLHeader *destTpl, wii::tpl::TPLHeader *srcTpl, const char *filePath = nullptr, bool free = false) {

          // Loads the tpl if not already loaded by the stated filePath
          if (srcTpl == nullptr){
          spm::filemgr::FileEntry * srcFile = spm::filemgr::fileAllocf(4, filePath);
          s32 tplSize = srcFile->length;
          srcTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAIN, tplSize);
          msl::string::memcpy(srcTpl, srcFile->sp->data, tplSize);
          spm::filemgr::fileFree(srcFile);
          }

          // Patches the destination tpl with the one given by the mod.rel
          destTpl->imageTable[destId] = srcTpl->imageTable[srcId];

          // Free the memory of the tpl loaded from mod.rel to prevent a leak
          if (free == true){
          spm::memory::__memFree(spm::memory::Heap::HEAP_MAIN, srcTpl);
          }
          return;
        }

void main() {
  wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
  titleScreenCustomTextPatch();
  getStandardDeathScript();
  setBossHP();
  setBossXp();
  setBossDef();
  patchMarioDamage();
  patchItems();
  patchCooking();
  reduceXpGained();
  patchVariables();
  evtpatch::evtmgrExtensionInit(); // Initialize EVT scripting extension
  hookBowserScripts();
  hookDimentioScripts();
  hookSuperDimentioScripts();
  hookBleckScripts();
  hookMimiScripts();
  hookChunkScripts();
  dimenPatch();
  ninjaPatch();
  patchStandardDeathScript();
  wii::tpl::TPLHeader *myTplHeader = nullptr;
  patchTpl(116, 0, (wii::tpl::TPLHeader *)spm::icondrv::icondrv_wp->wiconTpl->sp->data, myTplHeader, "./a/n_mg_flower-", true);
  //hampter(); //will always live in our memories
}

}
