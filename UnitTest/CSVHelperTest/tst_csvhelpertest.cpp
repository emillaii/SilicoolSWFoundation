#include <QCoreApplication>
#include <QtTest>

// add necessary includes here

#include "../../../Utilities/csvHelper/csvhelper.h"
#include "testclass.h"

class CSVHelperTest : public QObject
{
    Q_OBJECT

public:
    CSVHelperTest();
    ~CSVHelperTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

private:
    QList<TestClass *> objs;
    CSVHelper csvHelper;
    QString dir = "C:\\Users\\Silicool\\Desktop\\Test\\";
};

CSVHelperTest::CSVHelperTest() : csvHelper((TestClass::staticMetaObject))
{
    TestClass *c1 = new TestClass();
    c1->setI(10);
    TestClass *c2 = new TestClass();
    c2->setB(true);
    TestClass *c3 = new TestClass();
    c3->setD(2.45);
    objs << c1 << c2 << c3;
}

CSVHelperTest::~CSVHelperTest() {}

void CSVHelperTest::initTestCase() {}

void CSVHelperTest::cleanupTestCase() {}

void CSVHelperTest::test_case1()
{
    CSVHelper::save(dir + "save.csv", objs);
    QList<TestClass *> loadData;
    CSVHelper::load(dir + "save.csv", loadData);

    for (int i = 0; i < loadData.count(); i++)
    {
        csvHelper.append(dir + "saveAfterLoad.csv", loadData[i]);
    }
}

QTEST_MAIN(CSVHelperTest)

#include "tst_csvhelpertest.moc"
