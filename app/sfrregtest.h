#ifndef SFRREGTEST_H
#define SFRREGTEST_H
#include "imagetest.h"

class SFRregTest : public ImageTest
{
public:
    SFRregTest(void);
    virtual ~SFRregTest(void);

    void    	InitResults(void);

protected:
    void		GetResults(JSONNode &passfail);
    void		ParseResults(std::string &results);

};

#endif // SFRREGTEST_H
