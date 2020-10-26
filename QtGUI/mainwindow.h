#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QProcess>
#include <QComboBox>
#include "configtcpport.h"
#include "configapp.h"
#include "configapp.h"
#include "../Src/Client/EasyNetServiceClient.h"
#include "../Src/Server/FileOpt.h"
#include "selectcmd.h"

#define APPCONFIGFILE "APPCcofig.json"
#if defined(MACOS)
#define EXAMPLEAPPCFG "{\"ServerSource\":\"https://github.com/huangweikai465/ENSServer\", \
                        \"HelpHtml\":\"help.html\", \
                        \"TerminalCMD\":\"osascript << END \\n tell application \\\"Terminal\\\" \\n\\t do script \\\"$(CMD)\\\" \\n end tell \\n END\",\
                        \"CMDList\":[{\"Type\":\"ssh\",\"CMD\":\"ssh test@$(IP)\",\"Terminal\":true},\
                        {\"Type\":\"ftp\",\"CMD\":\"open ftp://$(IP)\"},\
                        {\"Type\":\"smb\",\"CMD\":\"open smb://$(IP)\"},\
                        {\"Type\":\"web\",\"CMD\":\"open http://$(IP):$(PORT)\"},\
                        {\"Type\":\"web\",\"CMD\":\"open https://$(IP):$(PORT)\"}]}"
#elif defined(LINUX)

#elif defined(WIN32)
#define EXAMPLEAPPCFG "{\"ServerSource\":\"https://github.com/huangweikai465/ENSServer\", \
                        \"HelpHtml\":\"help.html\", \
                        \"TerminalCMD\":\"start cmd /k $(CMD)\",\
                        \"CMDList\":[{\"Type\":\"ssh\",\"CMD\":\"ssh test@$(IP)\",\"Terminal\":true},\
                        {\"Type\":\"ftp\",\"CMD\":\"start ftp://$(IP)\"},\
                        {\"Type\":\"smb\",\"CMD\":\"start \\\\\\\\$(IP)\"},\
                        {\"Type\":\"web\",\"CMD\":\"start http://$(IP):$(PORT)\"},\
                        {\"Type\":\"web\",\"CMD\":\"start https://$(IP):$(PORT)\"}]}"
#endif


/*
TODO:
2. 新增远程部署功能
3. 增加本地配置记录功能
*/

#define DEFAULTUDPPORT 4657
#define DEFAULTTCPPORT 4657

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct stCommand
{
    QString szCmd;
    bool bNewTerminal;
    stCommand & operator=(const stCommand & _oCmd)
    {
        szCmd = _oCmd.szCmd;
        bNewTerminal = _oCmd.bNewTerminal;
        return *this;
    }
} ;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnNewItem(QTreeWidgetItem * _pNewItem);
    void OnSearchTextChanged(const QString & _szCurrentText);
    void OnUdpinputChanged(const QString & _szCurrentText);
    void OnGetService(void);
    void OnSearch(void);
    void OnSearchTypeChanged(const QString & _szCurrentText);
    void OnOpen(void);
    void OnCopy();
    void OnConfigTcpPort(void);
    void OnConfigApp(void);
    void OnConfigTerminal(void);
    void OnInstallServer(void);
    void OnDoubleClicked(const QModelIndex & _index);
    void OnRunCMD(const QString & _szCMD,bool _bTerminal);
    void OnUsage();
    void OnVersion();

signals:
    void GetNewItem(QTreeWidgetItem * _pNewItem);
    void RunCMD(const QString & _szCMD,bool _bTerminal);

private:
    const QString c_szSearchPort = tr("端口");
    const QString c_szSearchType = tr("服务类型");
    const QString c_szSearchName = tr("名称");
    uint16_t m_uTcpPort;
    uint16_t m_uUdpPort;

    Ui::MainWindow *ui;

    QTreeWidget * m_pServiceTree;
    EasyNetServiceClient * m_pEnsClient;

    QLineEdit * m_pUdpPortEdit;

    QLineEdit * m_pSearch;
    QComboBox * m_pSearchType;

    FileOpt m_oFo;
    JsonOpt m_oAppConfig;
    SelectCMD * m_pSelectCmd;

    static void GetServiceMsg(std::string _szIP,std::string _szHostName, ServiceList* _pPortSrv,void* _pParam);
    void CleanTreeItem(void);
    void SearchService(std::string _szService );
    void GetService(uint16_t _uPort);
};
#endif // MAINWINDOW_H
