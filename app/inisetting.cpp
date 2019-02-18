#include "inisetting.h"

IniSetting::IniSetting(QString section, QString key, QString subsection): section(section), key(key), subsection(subsection)
{
    #ifdef INI_INCLUDE_SUBSECTION
    numReadFields = 5;
    numWriteFields = 4;
#else
    numReadFields = 4;
    numWriteFields = 3;
#endif
}
