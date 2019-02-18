#ifndef INISTRINGSETTING_H
#define INISTRINGSETTING_H
#include "inisetting.h"

class IniStringSetting : public IniSetting
{
private:
    QString value;
    QString defaultValue;
public:
    IniStringSetting(QString inSection, QString inKey, QString defaultValue = "", QString subsection = "");
    mwArray getReadCommand();
    mwArray getWriteCommand();
    QString get() {return value;}
    void set(QString inValue) {value = inValue;}
};

#endif // INISTRINGSETTING_H
