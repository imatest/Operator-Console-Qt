#include "criticalmwarray.h"

CriticalMWArray::CriticalMWArray()
{
    m_data = mwArray();
}

void CriticalMWArray::Get(mwArray &array)
{
    QMutexLocker locker(&m_mutex);
    array = m_data.Clone();
}

void CriticalMWArray::Set(const mwArray &array)
{
    QMutexLocker locker(&m_mutex);
    m_data = array.Clone();
}
