#include "l3m.h"

using namespace l3m;

static const char* const L3M_BOM = "L3M\x01";
static const float L3M_VERSION = 0.79f;
static const enum
{
    L3M_MACHINE_ENDIAN,
    L3M_LOW_ENDIAN,
    L3M_BIG_ENDIAN
} L3M_SAVE_ENDIANNESS = L3M_LOW_ENDIAN;

bool Model::Load(const std::istream& fp)
{
    return true;
}

bool Model::Save(const std::ostream& fp)
{
    return true;
}
