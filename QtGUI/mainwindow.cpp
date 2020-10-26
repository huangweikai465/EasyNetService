#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <QInputDialog>
#include "editterminalcmd.h"
#include <QClipboard>
#include "version.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_oFo(APPCONFIGFILE,EXAMPLEAPPCFG)
{
    ui->setupUi(this);
    setWindowTitle(tr("ENS客户端"));
#if defined(MACOS)
    //setWindowIcon(QIcon(":/img/logo.icns"));
#elif defined(WIN32)
    setWindowIcon(QIcon(":/img/logo.ico"));
#endif

    //操作菜单栏
    QMenu *pOperationMenu = new QMenu(tr("操作"),this);
    QAction * pOptOpen = new QAction(tr("打开"),this);
    pOperationMenu->addAction(pOptOpen);
    connect(pOptOpen,SIGNAL(triggered()),this,SLOT(OnOpen()));

    QAction * pOptCopy = new QAction(tr("复制地址"),this);
    pOperationMenu->addAction(pOptCopy);
    connect(pOptCopy,SIGNAL(triggered()),this,SLOT(OnCopy()));

    QAction * pInstallServer = new QAction(tr("安装服务端"),this);
    pOperationMenu->addAction(pInstallServer);
    connect(pInstallServer,SIGNAL(triggered()),this,SLOT(OnInstallServer()));

    ui->menubar->addMenu(pOperationMenu);

    //配置
    QMenu *pConfigMenu = new QMenu(tr("配置"),this);
    QAction * pTcpPortConfig = new QAction(tr("TCP端口"),this);
    pConfigMenu->addAction(pTcpPortConfig);
    connect(pTcpPortConfig,SIGNAL(triggered()),this,SLOT(OnConfigTcpPort()));

    QAction * pAppConfig = new QAction(tr("应用配置"),this);
    pConfigMenu->addAction(pAppConfig);
    connect(pAppConfig,SIGNAL(triggered()),this,SLOT(OnConfigApp()));

    QAction * pTerminalCmdConfig = new QAction(tr("终端命令"),this);
    pConfigMenu->addAction(pTerminalCmdConfig);
    connect(pTerminalCmdConfig,SIGNAL(triggered()),this,SLOT(OnConfigTerminal()));

    ui->menubar->addMenu(pConfigMenu);

    //帮助
    QMenu *pHelpMenu = new QMenu(tr("帮助"),this);
    QAction * pUsage = new QAction(tr("使用帮助"),this);
    pHelpMenu->addAction(pUsage);
    connect(pUsage,SIGNAL(triggered()),this,SLOT(OnUsage()));

    QAction * pVersion = new QAction(tr("版本"),this);
    pHelpMenu->addAction(pVersion);
    connect(pVersion,SIGNAL(triggered()),this,SLOT(OnVersion()));

    ui->menubar->addMenu(pHelpMenu);



    m_oAppConfig.LoadFromString(m_oFo.GetFileContent());
    if(m_oAppConfig.IsNull())
    {
        QMessageBox::information(nullptr,nullptr,QString(tr("应用配置错误，请检查")) + QString(m_oFo.GetFilePath().c_str()) + QString(m_oFo.GetFileName().c_str()));
        m_oAppConfig.Clear();
        exit(1);
    }

    QGridLayout * pGlobalLayout = new QGridLayout(ui->centralwidget);
    pGlobalLayout->setSpacing(10);

    //主显示区
    m_pServiceTree = new QTreeWidget(this);
    m_pServiceTree->setHeaderLabels(QStringList() << tr("地址/端口")<< tr("服务类型") << tr("名称"));
    m_pServiceTree->setColumnWidth(0,150);
    m_pServiceTree->show();
    connect(m_pServiceTree,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(OnDoubleClicked(const QModelIndex &)));
    QAction * pActionTemp = nullptr;
    pActionTemp = new QAction(tr("打开"),this);
    m_pServiceTree->addAction(pActionTemp);
    connect(pActionTemp,SIGNAL(triggered()),this,SLOT(OnOpen()));
    pActionTemp = new QAction(tr("复制地址"),this);
    m_pServiceTree->addAction(pActionTemp);
    connect(pActionTemp,SIGNAL(triggered()),this,SLOT(OnCopy()));
    pActionTemp = new QAction(tr("安装服务端"),this);
    m_pServiceTree->addAction(pActionTemp);
    connect(pActionTemp,SIGNAL(triggered()),this,SLOT(OnInstallServer()));
    m_pServiceTree->setContextMenuPolicy(Qt::ActionsContextMenu);

    //UDP端口输入框和设置按钮
    m_pUdpPortEdit = new QLineEdit(this);
    m_pUdpPortEdit->setPlaceholderText(tr("UDP端口"));
    m_pUdpPortEdit->setFixedWidth(80);
    m_pUdpPortEdit->setValidator(new QIntValidator(0,65535,this));
    connect(m_pUdpPortEdit,SIGNAL(textChanged(const QString &)),this,SLOT(OnUdpinputChanged(const QString &)));
    connect(m_pUdpPortEdit,SIGNAL(returnPressed()),this,SLOT(OnGetService()));
    m_pUdpPortEdit->show();

    QPushButton * pBtnGet = new QPushButton(this);
    pBtnGet->setText(tr("获取服务"));
    pBtnGet->setFixedWidth(100);
    connect(pBtnGet,&QPushButton::clicked,this,&MainWindow::OnGetService);
    pBtnGet->show();

    //搜索功能
    m_pSearchType = new QComboBox(this);
    m_pSearchType->addItem(c_szSearchPort);
    m_pSearchType->addItem(c_szSearchType);
    m_pSearchType->addItem(c_szSearchName);
    m_pSearchType->setFixedWidth(100);
    connect(m_pSearchType,SIGNAL(currentTextChanged(const QString &)),this,SLOT(OnSearchTypeChanged(const QString &)));
    m_pSearchType->setCurrentIndex(0);
    m_pSearchType->show();

    m_pSearch = new QLineEdit(this);
    m_pSearch->setFixedWidth(200);
    this->OnSearchTypeChanged(m_pSearchType->currentText());
    connect(m_pSearch,SIGNAL(textChanged(const QString &)),this,SLOT(OnSearchTextChanged(const QString &)));
    connect(m_pSearch,SIGNAL(returnPressed()),this,SLOT(OnSearch()));
    m_pSearch->show();

    QPushButton * pBtnSearch = new QPushButton(this);
    pBtnSearch->setText(tr("搜索"));
    pBtnSearch->setFixedWidth(80);
    connect(pBtnSearch,&QPushButton::clicked,this,&MainWindow::OnSearch);
    pBtnSearch->show();

    //布局
    pGlobalLayout->addWidget(m_pUdpPortEdit,0,0);
    pGlobalLayout->addWidget(pBtnGet,0,1);
    pGlobalLayout->addWidget(m_pSearch,0,3);
    pGlobalLayout->addWidget(m_pSearchType,0,4);
    pGlobalLayout->addWidget(pBtnSearch,0,5);
    pGlobalLayout->addWidget(m_pServiceTree,1,0,1,6);

    m_uTcpPort = DEFAULTTCPPORT;
    if(m_oAppConfig["TcpPort"].IsNull() == false)
    {
        m_uTcpPort = m_oAppConfig["TcpPort"].ToInt();
    }
    m_uUdpPort = DEFAULTUDPPORT;
    if(m_oAppConfig["UdpPort"].IsNull() == false)
    {
        m_uUdpPort = m_oAppConfig["UdpPort"].ToInt();
    }
    m_pEnsClient = new EasyNetServiceClient(m_uUdpPort,m_uTcpPort,GetServiceMsg,this);
    m_pEnsClient->AllService();
    m_pUdpPortEdit->setText(QString::number(m_uUdpPort));

    connect(this,SIGNAL(GetNewItem(QTreeWidgetItem*)),this,SLOT(OnNewItem(QTreeWidgetItem*)));
    setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint));

    connect(this,SIGNAL(RunCMD(const QString &,bool)),this,SLOT(OnRunCMD(const QString &,bool)));

    m_pSelectCmd = nullptr;
}

MainWindow::~MainWindow()
{
    delete m_pEnsClient;
    CleanTreeItem();
    delete ui;
}

//显示,改成发送信号，在槽函数里做处理
void MainWindow::GetServiceMsg(std::string _szIP,std::string _szHostName, ServiceList* _pPortSrv,void* _pParam)
{
    MainWindow * pThis = (MainWindow*)_pParam;
    QTreeWidgetItem * pQtreeItem = new QTreeWidgetItem(QStringList()<<_szIP.c_str()<<""<<_szHostName.c_str());
    for(size_t i =0;i<_pPortSrv->size();i++)
    {
        QTreeWidgetItem * pServiceItem = new QTreeWidgetItem;
        pServiceItem->setText(0,QString::number((*_pPortSrv)[i]->uPort));
        pServiceItem->setText(1,(*_pPortSrv)[i]->szServiceType.c_str());
        pServiceItem->setText(2,(*_pPortSrv)[i]->szServiceName.c_str());
        pQtreeItem->addChild(pServiceItem);
    }
    emit pThis->GetNewItem(pQtreeItem);
}

//清除当前显示的内容
void MainWindow::CleanTreeItem(void)
{
    for(int i = 0;i<m_pServiceTree->topLevelItemCount();i++)
    {
        QTreeWidgetItem * pTopItem = m_pServiceTree->topLevelItem(i);
        for(int j = 0;j<pTopItem->childCount();j++)
        {
            QTreeWidgetItem * pChildItem = pTopItem->child(j);
            pTopItem->removeChild(pChildItem);
            delete pChildItem;
        }
        delete pTopItem;
    }
    m_pServiceTree->clear();
}

void MainWindow::SearchService(std::string _szService )
{
    if(_szService.empty())
        m_pEnsClient->AllService();
    else if(_szService.compare("all") == 0)
        m_pEnsClient->AllService();
}

void MainWindow::GetService(uint16_t _uPort)
{
    if(_uPort != m_uUdpPort)
    {
        if(m_oAppConfig["UdpPort"].IsNull())
            m_oAppConfig.AddItem("UdpPort",_uPort);
        else
            m_oAppConfig.ReplaceItem("UdpPort",_uPort);
        m_uUdpPort = _uPort;
        m_oFo.ReWrite(m_oAppConfig.ToString());
    }
    m_pEnsClient->ModifyUDPPort(_uPort);
    CleanTreeItem();
    m_pEnsClient->AllService();
}

void MainWindow::OnGetService(void)
{
    QString szUdpPortText = m_pUdpPortEdit->text();
    if(szUdpPortText.isEmpty())
        QMessageBox::information(nullptr,tr("输入错误"),tr("请输入UDP端口"));
    else
    {
        int sPort = szUdpPortText.toInt();
        if((sPort >0) && (sPort <65536))
            GetService((uint16_t)sPort);
        else
            QMessageBox::information(NULL, tr("UDP端口错误"), tr("UDP端口输入错误,请检查"));
    }
}

void MainWindow::OnSearchTextChanged(const QString & _szCurrentText)
{
    static int sLastPort = 0;
    if(m_pSearchType->currentText().compare(c_szSearchPort) == 0)
    {
        int sPort = _szCurrentText.toInt();
        if((sPort >= 0) && (sPort <65536))
        {
            sLastPort = sPort;
        }
        else
        {
            m_pSearch->setText(QString::number(sLastPort));
        }
    }
}

void MainWindow::OnSearch()
{
    QString szSearchType = m_pSearchType->currentText();
    QString szSearchText = m_pSearch->text();
    CleanTreeItem();
    if(szSearchText.isEmpty() == true)
    {
        m_pEnsClient->AllService();
    }
    else if(szSearchType.compare(c_szSearchName) == 0)
    {
        m_pEnsClient->SearchServiceName(szSearchText.toStdString());
    }
    else if(szSearchType.compare(c_szSearchType) == 0)
    {
        m_pEnsClient->SearchServiceType(szSearchText.toStdString());
    }
    else if(szSearchType.compare(c_szSearchPort) == 0)
    {
        m_pEnsClient->SearchServicePort(szSearchText.toInt());
    }
}

void MainWindow::OnSearchTypeChanged(const QString &_szCurrentText)
{
    if(_szCurrentText.compare(c_szSearchName) == 0)
    {
        m_pSearch->show();
        m_pSearch->setPlaceholderText(tr("请输入服务名称"));
        m_pSearch->setValidator(nullptr);
    }
    else if(_szCurrentText.compare(c_szSearchType) == 0)
    {
        m_pSearch->show();
        m_pSearch->setPlaceholderText(tr("请输入服务类型"));
        m_pSearch->setValidator(nullptr);
    }
    else if(_szCurrentText.compare(c_szSearchPort) == 0)
    {
        m_pSearch->show();
        m_pSearch->setValidator(new QIntValidator(0,65535,this));
        m_pSearch->setPlaceholderText(tr("请输入服务端口号"));
    }
    m_pSearch->setText("");
}

void MainWindow::OnUdpinputChanged(const QString & _szCurrentText)
{
    static int sLastPort = 0;
    int sPort = _szCurrentText.toInt();
    if((sPort >= 0) && (sPort <65536))
    {
        sLastPort = sPort;
    }
    else
    {
        m_pUdpPortEdit->setText(QString::number(sLastPort));
    }
}

void MainWindow::OnNewItem(QTreeWidgetItem * _pNewItem)
{
    m_pServiceTree->addTopLevelItem(_pNewItem);
    m_pServiceTree->expandAll();
}

void MainWindow::OnOpen()
{
    if(m_pServiceTree->topLevelItemCount() > 0)
    {
        QTreeWidgetItem * pItem = m_pServiceTree->currentItem();
        QString szType = pItem->text(1);
        if(szType.isEmpty() == false)
        {
            QString szIP = pItem->parent()->text(0);
            QString szPort = pItem->text(0);
            QStringList szlCmd;
            QList<bool> blTerminal;
            JsonOpt oCmdList;
            oCmdList.LoadFromString(m_oAppConfig["CMDList"].ToString());
            ForEachArrayItem(oNode,oCmdList)
            {
                if(oNode["Type"].ToString().compare(szType.toStdString()) == 0)
                {
                    QString szCMD = oNode["CMD"].ToString().c_str();
                    szCMD.replace("$(IP)",szIP);
                    szCMD.replace("$(PORT)",szPort);
                    szlCmd << szCMD;
                    bool bNewTerminal = false;
                    if(oNode["Terminal"].IsNull()==false)
                        bNewTerminal = oNode["Terminal"].ToBool();
                    blTerminal << bNewTerminal;
                }
            }
            if(szlCmd.size() > 1)
            {
                m_pSelectCmd = new SelectCMD(this);
                for(int i = 0;i<szlCmd.size();i++)
                    m_pSelectCmd->AppendList(szlCmd[i]);
                m_pSelectCmd->setModal(true);
                m_pSelectCmd->show();
                int ret = m_pSelectCmd->exec();
                if(ret == QDialog::Accepted)
                {
                    int sSelectIndex = m_pSelectCmd->GetSelectedIndex();
                    emit RunCMD(szlCmd[sSelectIndex],blTerminal[sSelectIndex]);
                }
                delete m_pSelectCmd;
            }
            else if(szlCmd.size() == 1)
            {
                emit RunCMD(szlCmd[0],blTerminal[0]);
            }
            else
            {
                QMessageBox::information(nullptr,tr("未找到配置"),QString(tr("未找到打开"))+szType+QString(tr("类型的命令")));
            }
        }
    }
}

void MainWindow::OnCopy()
{
    if(m_pServiceTree->topLevelItemCount() > 0)
    {
        QTreeWidgetItem * pItem = m_pServiceTree->currentItem();
        if(pItem != nullptr)
        {
            QString szType = pItem->text(1);
            QString szAddress;
            if(szType.isEmpty() == false)
            {
                QString szIP = pItem->parent()->text(0);
                QString szPort = pItem->text(0);
                szAddress = szIP + ":" + szPort;
            }
            else
                szAddress = pItem->text(0);
            QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
            clipboard->setText(szAddress);		             //设置剪贴板内容</span>
        }
    }
}

void MainWindow::OnConfigTcpPort()
{
    ConfigTcpPort * pConfigTcpWindows = new ConfigTcpPort(m_uTcpPort,this);
    pConfigTcpWindows->setModal(true);
    pConfigTcpWindows->show();
    int ret = pConfigTcpWindows->exec();
    if(ret == QDialog::Accepted)
    {
        uint16_t uTcpPort = pConfigTcpWindows->GetNewTCPPort();
        if(uTcpPort != m_uTcpPort)
        {
            if(m_oAppConfig["TcpPort"].IsNull())
                m_oAppConfig.AddItem("TcpPort",uTcpPort);
            else
                m_oAppConfig.ReplaceItem("TcpPort",uTcpPort);
            m_uTcpPort = uTcpPort;
            m_oFo.ReWrite(m_oAppConfig.ToString());
        }
        m_pEnsClient->ModifyTCPPort((uint16_t)uTcpPort);
    }
    delete pConfigTcpWindows;
}

void MainWindow::OnConfigApp()
{
    ConfigApp * pConfigAppWindows = new ConfigApp(this);
    JsonOpt oCMDList = m_oAppConfig["CMDList"];
    pConfigAppWindows->LoadConfig(oCMDList);
    pConfigAppWindows->setModal(true);
    pConfigAppWindows->show();
    int ret = pConfigAppWindows->exec();
    if(ret == QDialog::Accepted)
    {
        m_oAppConfig.ReplaceItem("CMDList",pConfigAppWindows->GetAppConfig());
        m_oFo.ReWrite(m_oAppConfig.ToString());
    }
    delete pConfigAppWindows;
}

void MainWindow::OnConfigTerminal()
{
    EditTerminalCmd * pCmdEdit = new EditTerminalCmd(this);
    pCmdEdit->LoadCmd(m_oAppConfig["TerminalCMD"].ToString().c_str());
    int ret = pCmdEdit->exec();
    if(ret == QDialog::Accepted)
    {
        m_oAppConfig.ReplaceItem("TerminalCMD",pCmdEdit->GetNewCmd().toStdString());
        m_oFo.ReWrite(m_oAppConfig.ToString());
    }
    delete pCmdEdit;
}

void MainWindow::OnInstallServer()
{
    if(m_oAppConfig["ServerSource"].IsNull() == true)
    {
        JsonOpt jo;
        jo.LoadFromString(EXAMPLEAPPCFG);
        m_oAppConfig.AddItem("ServerSource",jo["ServerSource"]);
    }
    QString szURL = m_oAppConfig["ServerSource"].ToString().c_str();
    if(szURL.startsWith("http") == true)
    {
#if defined (WIN32)
        QString szCMD = "start ";
#elif defined (MACOS)
        QString szCMD = "open ";
#endif
        szCMD += szURL;
        emit RunCMD(szCMD,false);
    }
}

void MainWindow::OnDoubleClicked(const QModelIndex & )
{
    OnOpen();
}

void MainWindow::OnRunCMD(const QString & _szCMD,bool _bTerminal)
{
    QString szCMD = _szCMD;
    if(_bTerminal == true)
    {
        szCMD = m_oAppConfig["TerminalCMD"].ToString().c_str();
        szCMD.replace("$(CMD)",_szCMD);
    }
#if defined (MACOS) || defined (LINUX)
    if(fork() == 0)
    {
        system(szCMD.toStdString().c_str());
        exit(0);
    }
#elif defined (WIN32)
    std::thread([](QString _szCMD){
        system(_szCMD.toStdString().c_str());
        system("exit");
    },szCMD).detach();
#endif
}

void MainWindow::OnUsage()
{
    if(m_oAppConfig["HelpHtml"].IsNull() == true)
    {
        JsonOpt jo;
        jo.LoadFromString(EXAMPLEAPPCFG);
        m_oAppConfig.AddItem("HelpHtml",jo["HelpHtml"]);
    }
#if defined (WIN32)
    QString szCMD = "start "+ QCoreApplication::applicationDirPath() + DIRDIV;
#elif defined (MACOS)
    QString szCMD = "open " + QCoreApplication::applicationDirPath() + DIRDIV;
#endif
    szCMD += m_oAppConfig["HelpHtml"].ToString().c_str();
    emit RunCMD(szCMD,false);
}

void MainWindow::OnVersion()
{
    Version * pVer = new Version(this);
    pVer->setModal(true);
    pVer->show();
    pVer->exec();
}





