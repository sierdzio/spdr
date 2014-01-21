#include "spdrglobal.h"
#include "SpdrBase"

#include <QObject>
#include <QDebug>
#include <QtTest/QtTest>

class TstSpdrBase : public QObject
{
    Q_OBJECT

private slots:
    void testDefault();
};

void TstSpdrBase::testDefault()
{
    //qDebug() << "Testing defaults for SpdrBase";
    SpdrBase testObject;
    QCOMPARE(testObject.copyMode(), Spdr::Copy);
    QCOMPARE(testObject.updateMode(), Spdr::Ask);
    QCOMPARE(testObject.logLevel(), Spdr::OnlyErrors);
    QCOMPARE(testObject.isUsingLogFile(), false);
}

QTEST_MAIN(TstSpdrBase)
#include "tst_spdrbase.moc"
