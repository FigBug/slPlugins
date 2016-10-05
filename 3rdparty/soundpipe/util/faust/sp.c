#include <math.h>
#include "CUI.h"

#define max(a,b) ((a < b) ? b : a)
#define min(a,b) ((a < b) ? a : b)

<<includeIntrinsic>>

<<includeclass>>

/* -- layout groups */

static void openTabBox (void* ui_interface, const char* label)
{

}

static void openHorizontalBox (void* ui_interface, const char* label)
{

}
static void openVerticalBox (void* ui_interface, const char* label)
{

}

static void closeBox (void* ui_interface)
{

}

/* -- active widgets */

static void addButton (void* ui_interface, const char* label, FAUSTFLOAT* zone)
{

}

static void addCheckButton (void* ui_interface, const char* label, FAUSTFLOAT* zone)
{

}

static void addVerticalSlider (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{

}

static void addHorizontalSlider (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{

}

static void addNumEntry (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{

}

/* -- passive display widgets */

static void addHorizontalBargraph (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{

}

static void addVerticalBargraph (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{

}

static void declare (void* ui_interface, FAUSTFLOAT* zone, const char* key, const char* value)
{

}

