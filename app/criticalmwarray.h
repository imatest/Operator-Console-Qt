#ifndef CRITICALMWARRAY_H
#define CRITICALMWARRAY_H

#include <QMutex>
#include "imatestdefines.h"
IMA_WARNING_PUSH
IMA_DISABLE_UNUSED_PARAM_WARNING
#include "mclmcrrt.h"
#include "mclcppclass.h"

IMA_WARNING_POP

///
/// a mwArray wrapper-class where all access (read or write) to the mwArray is protected by critical sections
///
class CriticalMWArray
{
public:
    CriticalMWArray(void);
    ~CriticalMWArray(void) { }
    void	Get(mwArray& array);
    void	Set(const mwArray& array);

private:
    QMutex          m_mutex;
    mwArray         m_data;
};

#endif // CRITICALMWARRAY_H
