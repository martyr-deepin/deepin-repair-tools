
#include "repairtools.h"

#include <DApplication>
#include <DLog>
#include <QSettings>
#include <QMessageBox>

#include <unistd.h>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

inline void apply_system_dpi_settings()
{
    QSettings settings("/etc/lightdm/lightdm-deepin-greeter.conf", QSettings::IniFormat);
    const auto ratio = settings.value("ScreenScaleFactor", "1").toString();
    setenv("QT_SCALE_FACTOR", const_cast<char *>(ratio.toStdString().c_str()), 1);
}

inline bool root_check()
{
    if (!geteuid())
        return true;

    QMessageBox::warning(nullptr, QApplication::translate("main", "Deepin Repair Tools"),
                                  QApplication::translate("main", "Must Run as ROOT"));
    return false;
}

int main(int argc, char *argv[])
{
    apply_system_dpi_settings();

    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);

#ifndef QT_DEBUG
    if (!root_check() || !app.setSingleInstance("deepin-repair-tools"))
        return -1;
#endif

    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-repair-tools");
    app.setApplicationVersion("1.0");
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/");
    app.loadTranslator();
    // app.loadTranslator(QList<QLocale>() << QLocale("zh_CN"));
    app.setProductName(QApplication::translate("main", "Deepin Repair Tools"));
    app.setApplicationDescription(QApplication::translate("main", "deepin System Repair Tool is a tool to help users quickly fix some issues in deepin operating system. Including hard disk detection, disk control cleaning, DPKG repairing, permission repairing, etc."));
    app.setTheme("light");
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    RepairTools tools;
    tools.show();

   return app.exec();
}
