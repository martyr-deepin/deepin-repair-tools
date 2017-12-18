
#include "repairtools.h"

#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    if (!app.setSingleInstance("deepin-repair-tools"))
        return -1;

    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-repair-tools");
    app.setApplicationVersion("1.0");
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/");
    app.loadTranslator();
    app.setProductName(QApplication::translate("main", "Deepin Repair Tools"));
    app.setApplicationDescription(QApplication::translate("main", "Deepin Repair Tools"));
    // app.loadTranslator(QList<QLocale>() << QLocale("zh_CN"));
    app.setTheme("light");

    DLogManager::registerConsoleAppender();

    RepairTools tools;
    tools.show();

   return app.exec();
}
