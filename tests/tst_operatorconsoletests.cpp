#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class OperatorConsoleTests : public QObject
{
    Q_OBJECT

public:
    OperatorConsoleTests();
    ~OperatorConsoleTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

OperatorConsoleTests::OperatorConsoleTests()
{

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

void OperatorConsoleTests::test_case1()
{

}

QTEST_MAIN(OperatorConsoleTests)

#include "tst_operatorconsoletests.moc"
