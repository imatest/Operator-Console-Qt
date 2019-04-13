#include <QtTest>
#include <QCoreApplication>
#include "../app/inifile.h"
#include "../app/sfrplustest.h"
#include <string>
#include <QDebug>
#include <cstdio>
#include <QFile>


class OperatorConsoleTests : public QObject
{
    Q_OBJECT

public:
    OperatorConsoleTests();
    ~OperatorConsoleTests();

private:
    std::string folderSep;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_ini_read();


};

OperatorConsoleTests::OperatorConsoleTests()
{
#ifdef _WIN32
    folderSep = "\\";
#else
    folderSep = "/";
#endif

}

OperatorConsoleTests::~OperatorConsoleTests()
{

}

void OperatorConsoleTests::initTestCase()
{

}

void OperatorConsoleTests::cleanupTestCase()
{

}

void OperatorConsoleTests::test_ini_read()
{
    setup_settings settings;
    //    Inifile::ReadSettings(settings);

    QVERIFY2(settings.bayer == 1, "Testing the bayer pattern failed");
    QVERIFY2(settings.height == 480, "Testing 'height' failed");
    QVERIFY2(settings.width == 640, "'width' failed.");
    QVERIFY2(settings.bits_per_pixel == 8, "'bits_per_pixel' failed");
    QVERIFY2(settings.sourceID == 128, "'sourceID' failed");
    QVERIFY2(settings.video_format == "YUY2_800x600", "'video_format' failed");
    QVERIFY2(settings.epiphan_deviceID == 1, "'epiphan_deviceID' failed");
    QVERIFY2(settings.omnivision_reg_file == "", "'omnivision_reg_file' failed");

}


class SfrplusJsonResultParseTests : public QObject
{
    Q_OBJECT

public:
    SfrplusJsonResultParseTests();
    ~SfrplusJsonResultParseTests();

private:
    json_string	center1;
    json_string outer1;
    json_string center2;
    json_string outer2;
    json_string fov;
    json_string hConverge;
    json_string vConverge;
    json_string rotation;
    SFRplusTest test;
    JSONNode	passfail;
    JSONNode	data;

private slots:
    void initTestCase();
    void cleanupTestCase() {}
    void testCenter1(){QCOMPARE(center1, "0.9999602719");}
    void testOuter1() {QCOMPARE(outer1, "0.9999648188");}
    void testCenter2() {QCOMPARE(center2, "0.9999205439");}
    void testOuter2() {QCOMPARE(outer2, "0.9999296375");}
    void testFov() {QCOMPARE(fov, "123.1349116");}
    void testHorizConverge() {QCOMPARE(hConverge, "0.2281690014");}
    void testVertConverge() {QCOMPARE(vConverge, "-1.016977096");}
    void testRotation() {QCOMPARE(rotation, "-0.06399309358");}

};

SfrplusJsonResultParseTests::SfrplusJsonResultParseTests(){

}

SfrplusJsonResultParseTests::~SfrplusJsonResultParseTests() {

}

void SfrplusJsonResultParseTests::initTestCase() {

    QString jsonFilePath = QFINDTESTDATA("Data/sfrplus.json");
    QFile jsonFile(jsonFilePath);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
        QFAIL("Could not open 'sfrplus.json'");

    QTextStream inStream(&jsonFile);
    QString jsonData = inStream.readAll();

    std::string jsonString = jsonData.toStdString();

    test.GetDataNode(jsonString, data);
    test.GetPassFailNode(data, passfail);
    test.GetString(passfail, "Secondary_readout_1_center_mean", center1);
    test.GetString(passfail, "Secondary_readout_1_outer_mean", outer1);
    test.GetString(passfail, "Secondary_readout_2_center_mean", center2);
    test.GetString(passfail, "Secondary_readout_2_outer_mean", outer2);
    test.GetString(passfail, "FOV_degrees_diagonal", fov);
    test.GetString(passfail, "Horizontal_convergence_angle_degrees", hConverge);
    test.GetString(passfail, "Vertical_convergence_angle_degrees", vConverge);
    test.GetString(passfail, "Rotation_degrees", rotation);

}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    //    QTEST_DISABLE_KEYPAD_NAVIGATION
    //            QTEST_ADD_GPU_BLACKLIST_SUPPORT
    int result = 0;
    {
        OperatorConsoleTests tc;
        result |= QTest::qExec(&tc, argc, argv);
    }
    {
        SfrplusJsonResultParseTests tc;
        result |= QTest::qExec(&tc, argc, argv);
    }

    //    QTEST_SET_MAIN_SOURCE_PATH
    return result;
}

#include "tst_operatorconsoletests.moc"
