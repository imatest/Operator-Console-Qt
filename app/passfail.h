#ifndef PASSFAIL_H
#define PASSFAIL_H

#include <string>
#include <sstream>
#include "passfailsettings.h"
#include "mclmcrrt.h"
#include <QString>

using namespace std;

class PassFail
{
public:
    PassFail() {}
    virtual ~PassFail() {}

    static bool Read(PassFailSettings &settings);
    static bool Write(PassFailSettings &settings);

    static bool ReadFile(PassFailSettings &settings);
    static bool WriteFile(PassFailSettings &settings);

protected:
    static void addKeysAndValues(const string& candidateKey, const string& candidateVal, std::vector<string>& keys, std::vector<string>& vals, bool b_addKey);
    static void addDataTypeAndKey(const string& candidateDT, const string& candidateKey, std::vector<string>& data_types, std::vector<string>& keys);
    static void remove_crlf(const mwString &src, std::string &str);
    static void setValue(vecEntry<long> &entry, QString &value);
    static void setValue(entry<long> &entry, QString &value) {entry.value = value.toLong(); entry.b_isUsed = true;}
    static void setValue(entry<double> &entry, QString &value) {entry.value = value.toDouble(); entry.b_isUsed = true;}
    static void setValue(minMaxEntry<double> &entry, QString &value);
    static void ProcessBlemish(QString *section, Blemish &b);
    static void ProcessSfrplus(QString *section, SFRplus &s);
    static void ProcessOIS(QString *section, OIS &o);

    template <typename T> static const string writeEntValueString(const entry<T>& ent);
    template <typename T> static const string writeMinMaxEntValueString(const minMaxEntry<T>& ent);
    template <typename T> static const string writeVecEntValueString(const vecEntry<T>& ent);
};

#endif // PASSFAIL_H
