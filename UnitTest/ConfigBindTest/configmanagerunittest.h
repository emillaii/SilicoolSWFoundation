#ifndef CONFIGMANAGERUNITTEST_H
#define CONFIGMANAGERUNITTEST_H

#include "configManager/configarray.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class TestClassA : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int editableInt READ editableInt WRITE seteditableInt NOTIFY editableIntChanged)
    Q_PROPERTY(int readonlyInt READ readonlyInt WRITE setreadonlyInt NOTIFY readonlyIntChanged)
    Q_PROPERTY(int optionalInt READ optionalInt WRITE setoptionalInt NOTIFY optionalIntChanged)
    Q_PROPERTY(int hiddenInt READ hiddenInt WRITE sethiddenInt NOTIFY hiddenIntChanged)

    Q_PROPERTY(double editableDouble READ editableDouble WRITE seteditableDouble NOTIFY editableDoubleChanged)
    Q_PROPERTY(double readonlyDouble READ readonlyDouble WRITE setreadonlyDouble NOTIFY readonlyDoubleChanged)
    Q_PROPERTY(double optionalDouble READ optionalDouble WRITE setoptionalDouble NOTIFY optionalDoubleChanged)
    Q_PROPERTY(double hiddenDouble READ hiddenDouble WRITE sethiddenDouble NOTIFY hiddenDoubleChanged)

public:
    Q_INVOKABLE TestClassA(QObject *parent = nullptr) : ConfigObject(parent)
    {
        hide("hiddenInt");
        hide("hiddenDouble");

        setReadOnlyProperty("readonlyDouble");
        setReadOnlyProperty("readonlyInt");

        QVariantList intOptions;
        for (int i = 0; i < 10; i++)
        {
            intOptions.append(i + 4);
        }
        QVariantList doubleOptions;
        for (int i = 0; i < 10; i++)
        {
            doubleOptions.append(i / 3.4);
        }
        setOptionalProperty("optionalInt", intOptions);
        setOptionalProperty("optionalDouble", doubleOptions);

        init();
    }

    int editableInt() const
    {
        return m_editableInt;
    }
    int readonlyInt() const
    {
        return m_readonlyInt;
    }

    int optionalInt() const
    {
        return m_optionalInt;
    }

    int hiddenInt() const
    {
        return m_hiddenInt;
    }

    double editableDouble() const
    {
        return m_editableDouble;
    }

    double readonlyDouble() const
    {
        return m_readonlyDouble;
    }

    double optionalDouble() const
    {
        return m_optionalDouble;
    }

    double hiddenDouble() const
    {
        return m_hiddenDouble;
    }

public slots:
    void seteditableInt(int editableInt)
    {
        if (m_editableInt == editableInt)
            return;

        m_editableInt = editableInt;
        emit editableIntChanged(m_editableInt);
    }
    void setreadonlyInt(int readonlyInt)
    {
        if (m_readonlyInt == readonlyInt)
            return;

        m_readonlyInt = readonlyInt;
        emit readonlyIntChanged(m_readonlyInt);
    }

    void setoptionalInt(int optionalInt)
    {
        if (m_optionalInt == optionalInt)
            return;

        m_optionalInt = optionalInt;
        emit optionalIntChanged(m_optionalInt);
    }

    void sethiddenInt(int hiddenInt)
    {
        if (m_hiddenInt == hiddenInt)
            return;

        m_hiddenInt = hiddenInt;
        emit hiddenIntChanged(m_hiddenInt);
    }

    void seteditableDouble(double editableDouble)
    {

        if (qFuzzyCompare(m_editableDouble, editableDouble))
            return;

        m_editableDouble = editableDouble;
        emit editableDoubleChanged(m_editableDouble);
    }

    void setreadonlyDouble(double readonlyDouble)
    {

        if (qFuzzyCompare(m_readonlyDouble, readonlyDouble))
            return;

        m_readonlyDouble = readonlyDouble;
        emit readonlyDoubleChanged(m_readonlyDouble);
    }

    void setoptionalDouble(double optionalDouble)
    {

        if (qFuzzyCompare(m_optionalDouble, optionalDouble))
            return;

        m_optionalDouble = optionalDouble;
        emit optionalDoubleChanged(m_optionalDouble);
    }

    void sethiddenDouble(double hiddenDouble)
    {

        if (qFuzzyCompare(m_hiddenDouble, hiddenDouble))
            return;

        m_hiddenDouble = hiddenDouble;
        emit hiddenDoubleChanged(m_hiddenDouble);
    }

signals:
    void editableIntChanged(int editableInt);
    void readonlyIntChanged(int readonlyInt);
    void optionalIntChanged(int optionalInt);
    void hiddenIntChanged(int hiddenInt);
    void editableDoubleChanged(double editableDouble);
    void readonlyDoubleChanged(double readonlyDouble);
    void optionalDoubleChanged(double optionalDouble);
    void hiddenDoubleChanged(double hiddenDouble);

private:
    int m_editableInt = 0;
    int m_readonlyInt = 1;
    int m_optionalInt = 2;
    int m_hiddenInt = 3;
    double m_editableDouble = 4;
    double m_readonlyDouble = 5;
    double m_optionalDouble = 6;
    double m_hiddenDouble = 7;
};

class TestClassB : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString editableString READ editableString WRITE seteditableString NOTIFY editableStringChanged)
    Q_PROPERTY(QString readonlyString READ readonlyString WRITE setreadonlyString NOTIFY readonlyStringChanged)
    Q_PROPERTY(QString optionalString READ optionalString WRITE setoptionalString NOTIFY optionalStringChanged)
    Q_PROPERTY(QString hiddenString READ hiddenString WRITE sethiddenString NOTIFY hiddenStringChanged)

    Q_PROPERTY(bool editableBool READ editableBool WRITE seteditableBool NOTIFY editableBoolChanged)
    Q_PROPERTY(bool readonlyBool READ readonlyBool WRITE setreadonlyBool NOTIFY readonlyBoolChanged)
    Q_PROPERTY(bool hiddenBool READ hiddenBool WRITE sethiddenBool NOTIFY hiddenBoolChanged)

    Q_PROPERTY(TestClassA *testClassA READ testClassA)
    Q_PROPERTY(ConfigArray *boolArray READ boolArray)
    Q_PROPERTY(ConfigObjectArray *testClassAArray READ testClassAArray)

public:
    Q_INVOKABLE TestClassB(QObject *parent = nullptr) : ConfigObject(parent)
    {
        hide("hiddenString");
        hide("hiddenBool");

        setReadOnlyProperty("readonlyString");
        setReadOnlyProperty("readonlyBool");

        QVariantList stringOptions;
        for (int i = 0; i < 10; i++)
        {
            stringOptions.append(QString::number(i) + "HiHi");
        }
        setOptionalProperty("optionalString", stringOptions);

        m_testClassA = new TestClassA(this);
        m_testClassA->setObjectName("m_testClassA");
        m_boolArray = new ConfigArray(ConfigElementInfo::Bool, this);
        m_boolArray->setObjectName("m_boolArray");
        m_testClassAArray = new ConfigObjectArray(&TestClassA::staticMetaObject, this);
        m_testClassAArray->setObjectName("m_testClassAArray");

        init();
    }
    QString editableString() const
    {
        return m_editableString;
    }
    QString readonlyString() const
    {
        return m_readonlyString;
    }

    QString optionalString() const
    {
        return m_optionalString;
    }

    QString hiddenString() const
    {
        return m_hiddenString;
    }

    bool editableBool() const
    {
        return m_editableBool;
    }

    bool readonlyBool() const
    {
        return m_readonlyBool;
    }

    bool hiddenBool() const
    {
        return m_hiddenBool;
    }

    TestClassA *testClassA() const
    {
        return m_testClassA;
    }

    ConfigArray *boolArray() const
    {
        return m_boolArray;
    }

    ConfigObjectArray *testClassAArray() const
    {
        return m_testClassAArray;
    }

public slots:
    void seteditableString(QString editableString)
    {
        if (m_editableString == editableString)
            return;

        m_editableString = editableString;
        emit editableStringChanged(m_editableString);
    }
    void setreadonlyString(QString readonlyString)
    {
        if (m_readonlyString == readonlyString)
            return;

        m_readonlyString = readonlyString;
        emit readonlyStringChanged(m_readonlyString);
    }

    void setoptionalString(QString optionalString)
    {
        if (m_optionalString == optionalString)
            return;

        m_optionalString = optionalString;
        emit optionalStringChanged(m_optionalString);
    }

    void sethiddenString(QString hiddenString)
    {
        if (m_hiddenString == hiddenString)
            return;

        m_hiddenString = hiddenString;
        emit hiddenStringChanged(m_hiddenString);
    }

    void seteditableBool(bool editableBool)
    {
        if (m_editableBool == editableBool)
            return;

        m_editableBool = editableBool;
        emit editableBoolChanged(m_editableBool);
    }

    void setreadonlyBool(bool readonlyBool)
    {
        if (m_readonlyBool == readonlyBool)
            return;

        m_readonlyBool = readonlyBool;
        emit readonlyBoolChanged(m_readonlyBool);
    }

    void sethiddenBool(bool hiddenBool)
    {
        if (m_hiddenBool == hiddenBool)
            return;

        m_hiddenBool = hiddenBool;
        emit hiddenBoolChanged(m_hiddenBool);
    }

signals:
    void editableStringChanged(QString editableString);
    void readonlyStringChanged(QString readonlyString);
    void optionalStringChanged(QString optionalString);
    void hiddenStringChanged(QString hiddenString);
    void editableBoolChanged(bool editableBool);
    void readonlyBoolChanged(bool readonlyBool);
    void hiddenBoolChanged(bool hiddenBool);

private:
    QString m_editableString;
    QString m_readonlyString;
    QString m_optionalString;
    QString m_hiddenString;
    bool m_editableBool = true;
    bool m_readonlyBool = false;
    bool m_hiddenBool = true;
    TestClassA *m_testClassA;
    ConfigArray *m_boolArray;
    ConfigObjectArray *m_testClassAArray;
};

class Test : public QObject
{
    Q_OBJECT

public:
    Test(TestClassB *tb1, TestClassB *tb2) : tb1(tb1), tb2(tb2) {}

public slots:
    void bind(bool isTwoWay)
    {
        tb1->bind(tb2, isTwoWay);
    }
    void unbind(bool isTwoWay)
    {
        tb1->unBind(isTwoWay);
    }

private:
    TestClassB *tb1;
    TestClassB *tb2;
};

#endif    // CONFIGMANAGERUNITTEST_H
