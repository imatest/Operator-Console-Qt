#include "inisetting.h"

#ifdef INI_INCLUDE_SUBSECTION
const size_t IniSetting::numReadFields = 5;
const size_t IniSetting::numWriteFields = 4;
#else
const size_t IniSetting::numReadFields = 4;
const size_t IniSetting::numWriteFields = 3;
#endif

IniSetting::IniSetting(QString section, QString key, QString subsection): section(section), subsection(subsection), key(key)
{

}
