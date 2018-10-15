#ifndef PASSFAIL_H
#define PASSFAIL_H

#include <QObject>

#include "passfailsettings.h"

class PassFail
{
public:
    PassFail();
    ~PassFail();

    bool Read(PassFailSettings &settings);
    bool Write();
};

#endif // PASSFAIL_H
