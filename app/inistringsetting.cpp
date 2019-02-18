#include "inistringsetting.h"

IniStringSetting::IniStringSetting(QString inSection, QString inKey, QString inDefaultValue, QString subsection):IniSetting(inSection,inKey, subsection)
{
    storageType = "s";
    dataType = mxCHAR_CLASS;
    defaultValue = inDefaultValue;
}


mwArray IniStringSetting::getReadCommand(){

    mwArray sectionArray(section.toStdString().c_str());

    mwArray keyArray(key.toStdString().c_str());
    mwArray typeArray(storageType.toStdString().c_str());
    mwArray defaultArray(defaultValue.toStdString().c_str());

    mwArray readCommand(1, numReadFields, mxCELL_CLASS);

    mwIndex getIndex = 1;
    readCommand.Get(1, getIndex++).Set(sectionArray);
#ifdef INI_INCLUDE_SUBSECTION
    readCommand.Get(1, getIndex++).Set(mwArray(subsection.toStdString().c_str()));
#endif
    readCommand.Get(1, getIndex++).Set(keyArray);
    readCommand.Get(1, getIndex++).Set(typeArray);
    readCommand.Get(1, getIndex++).Set(defaultArray);

    return readCommand;

}

mwArray IniStringSetting::getWriteCommand() {
    mwArray writeCommand(1, numWriteFields, mxCELL_CLASS);

    mwArray sectionArray(section.toStdString().c_str());
    mwArray keyArray(key.toStdString().c_str());
    mwArray valueArray(value.toStdString().c_str());

    mwSize getIndex = 1;
    writeCommand.Get(1,getIndex++).Set(sectionArray);
#ifdef INI_INCLUDE_SUBSECTION
    writeCommand.Get(1,getIndex++).Set(mwArray(subsection.toStdString().c_str()));
#endif
    writeCommand.Get(1,getIndex++).Set(keyArray);
    writeCommand.Get(1,getIndex++).Set(valueArray);

    return writeCommand;
}
