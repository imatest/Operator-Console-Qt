#ifndef CRITICALMWARRAY_H
#define CRITICALMWARRAY_H

#include <QMutex>
#include "mclcppclass.h"

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
