#include <QString>
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QFile>

#include "spdrguimainwindow.h"
//#include "spdrguitags.h"

/*!
  \addtogroup wigetgui Spdr Widget GUI

  \brief widgetgui is a GUI application offering full functionality of the Spdr
  library.
  */

/*!
  \ingroup widgetgui

  Application's main function. Runs spdr dialog, optionally fires
  the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Spdr Widget GUI");
    app.setApplicationVersion("0.1.0");

    QTranslator translator;
    QString settingsPath = app.applicationDirPath() + "/spdr-gui-settings.ini";
    {
        QFile settingsFile(settingsPath);

        if (settingsFile.exists()) {
            QSettings settings(settingsPath, QSettings::IniFormat);
            QString language(settings.value("language").toString());

            if (!language.isEmpty()) {
                translator.load(app.applicationDirPath() + "/locale/libspdr_" + language + ".qm");
                translator.load(app.applicationDirPath() + "/locale/spdr_widgetgui_" + language + ".qm");
                app.installTranslator(&translator);
            }
        }
    }

    SpdrGuiMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
