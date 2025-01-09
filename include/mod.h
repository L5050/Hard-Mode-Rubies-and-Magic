#pragma once
#include "evt_cmd.h"
namespace mod {

#define MOD_VERSION "Rubies and Magic"

extern bool gIsDolphin;

extern "C" {

EVT_DECLARE(evt_cutscene_pure_heart)
EVT_DECLARE(evt_memcard_save)

}

void main();

}
