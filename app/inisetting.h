#ifndef INISETTING_H
#define INISETTING_H

#include <QString>
#include "mclcppclass.h"

class IniSetting
{
protected:
    QString section;
    QString subsection;
    QString key;
    QString storageType;
    mxClassID dataType;
    static size_t numWriteFields;
    static size_t numReadFields;
public:
    IniSetting(QString inSection, QString inKey, QString inSubsection);
    virtual ~IniSetting();

    virtual mwArray getReadCommand() = 0;
    virtual mwArray getWriteCommand() = 0;
};

#endif // INISETTING_H
