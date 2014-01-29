#include "spdrglobal.h"
#include "SpdrBase"

#include <QObject>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

class TstSpdrBase : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
    void testSignals();

    // TODO: add input and output path tests!
};

void TstSpdrBase::testDefaults()
{
    //qDebug() << "Testing defaults for SpdrBase";
    SpdrBase testObject;
    QCOMPARE(testObject.copyMode(), Spdr::Copy);
    QCOMPARE(testObject.updateMode(), Spdr::Ask);
    QCOMPARE(testObject.logLevel(), Spdr::OnlyErrors);
    QCOMPARE(testObject.isUsingLogFile(), false);
}

void TstSpdrBase::testSetters()
{
    SpdrBase testObject;

    testObject.setCopyMode(Spdr::Move);
    QCOMPARE(testObject.copyMode(), Spdr::Move);

    testObject.setUpdateMode(Spdr::Overwrite);
    QCOMPARE(testObject.updateMode(), Spdr::Overwrite);

    testObject.setLogLevel(Spdr::LogEverything);
    QCOMPARE(testObject.logLevel(), Spdr::LogEverything);

    QString randomPath("some/random/path");
    testObject.setLogFile(randomPath);
    QCOMPARE(testObject.isUsingLogFile(), true);
    QCOMPARE(testObject.logFile(), randomPath);
}

void TstSpdrBase::testSignals()
{
    Spdr::registerMetatypes();
    SpdrBase testObject;
    QSignalSpy spyCopyMode(&testObject, SIGNAL(copyModeChanged(Spdr::CopyMode)));
    QSignalSpy spyUpdateMode(&testObject, SIGNAL(updateModeChanged(Spdr::UpdateMode)));

    testObject.setCopyMode(Spdr::Move);
    testObject.setUpdateMode(Spdr::Ignore);

    QCOMPARE(spyCopyMode.count(), 1);
    QCOMPARE(spyUpdateMode.count(), 1);
}

QTEST_MAIN(TstSpdrBase)
#include "tst_spdrbase.moc"
