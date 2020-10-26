#include "mainwindow.h"

#include <QApplication>
#include "../Src/Log/Log.h"
#include "selectcmd.h"
#include <QTranslator>
#include <QTextCodec>
#include "../Src/Server/FileOpt.h"

#define DEFAULTAPPCFG "[{\"Type\":\"ssh\",\"CMD\":\"CMD of ssh\"},{\"Type\":\"ftp\",\"CMD\":\"CMD of ftp\"}]"

void LogErrorHook(std::string _szLog)
{
    QMessageBox::critical(nullptr,"Error",_szLog.c_str(),QMessageBox::Ok,QMessageBox::Ok);
    //exit(1);
}

int main(int argc, char *argv[])
{
    JsonOpt jo;
    Log::m_gErrHook = LogErrorHook;
    QApplication a(argc, argv);
    QTranslator translator;
    QLocale locale;
    if(locale.language() == QLocale::Chinese)
    {
        translator.load(":/qm/widgets_zh_CN.qm");
    }
    else
    {
      translator.load(":/qm/zh_EN.qm");
    }
    a.installTranslator(&translator);
    FileOpt::SetPath(QCoreApplication::applicationDirPath().toStdString() + DIRDIV);
    MainWindow w;
    w.show();
    return a.exec();
}
