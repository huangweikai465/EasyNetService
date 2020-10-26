#include "addappconfig.h"
#include "ui_addappconfig.h"
#include <QPushButton>
#include <QGridLayout>

AddAppConfig::AddAppConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppConfig)
{
    ui->setupUi(this);
    resize(640,200);
    setWindowTitle(tr("新增配置"));

    QPushButton * pBtnOK = new QPushButton(this);
    pBtnOK->setText(tr("确定"));
    pBtnOK->setFixedWidth(100);
    pBtnOK->show();
    connect(pBtnOK,SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton * pBtnCancel = new QPushButton(this);
    pBtnCancel->setText(tr("取消"));
    pBtnCancel->setFixedWidth(100);
    pBtnCancel->show();
    connect(pBtnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    QLabel * pTypeLable = new QLabel(this);
    pTypeLable->setText(tr("服务类型:"));
    pTypeLable->show();

    m_pTypeEdit = new QLineEdit(this);
    m_pTypeEdit->setPlaceholderText(tr("请输入服务类型"));
    m_pTypeEdit->show();

    QLabel * pTerminal = new QLabel(this);
    pTerminal->setText(tr("使用终端:"));
    pTerminal->show();

    m_pTerminalSelect = new QCheckBox(this);
    m_pTerminalSelect->setTristate(false);
    m_pTerminalSelect->setCheckState(Qt::Unchecked);
    m_pTerminalSelect->show();

    QLabel * pCMDLable = new QLabel(this);
    pCMDLable->setText(tr("启动命令:"));
    pCMDLable->show();

    m_pCMDEdit = new QLineEdit(this);
    m_pCMDEdit->setPlaceholderText(tr("请输如启动命令"));
    m_pCMDEdit->show();

    QLabel * pAlarm = new QLabel(this);
    pAlarm->setText(tr("(使用\"$(IP)\"引用网络服务的IP地址，使用\"$(PORT)\"引用端口。例如：当网络服务的IP是127.0.0.1,端口号为3456时，\"$(IP):$(PORT)\"表示地址 127.0.0.1:3456。)"));
    pAlarm->adjustSize();
    pAlarm->setWordWrap(true);
    pAlarm->show();

    QGridLayout * pLayout = new QGridLayout(this);
    pLayout->setSpacing(10);
    pLayout->addWidget(pTypeLable,0,0);
    pLayout->addWidget(m_pTypeEdit,0,1,1,5);
    pLayout->addWidget(pTerminal,1,0);
    pLayout->addWidget(m_pTerminalSelect,1,1);
    pLayout->addWidget(pCMDLable,2,0);
    pLayout->addWidget(m_pCMDEdit,2,1,1,5);
    pLayout->addWidget(pAlarm,3,0,1,6);
    pLayout->addWidget(pBtnOK,4,4);
    pLayout->addWidget(pBtnCancel,4,5);
}

AddAppConfig::~AddAppConfig()
{
    delete ui;
}

void AddAppConfig::LoadItem(QString _szType,bool _szUseTerminal, QString _szCMD)
{
    setWindowTitle(tr("编辑配置"));
    m_pTypeEdit->setText(_szType);
    m_pCMDEdit->setText(_szCMD);
    if(_szUseTerminal == true)
        m_pTerminalSelect->setCheckState(Qt::Checked);
}

QString AddAppConfig::GetType()
{
    return m_pTypeEdit->text();
}

bool AddAppConfig::GetUseTerminal()
{
    return (m_pTerminalSelect->checkState())?true:false;
}

QString AddAppConfig::GetCMD()
{
    return m_pCMDEdit->text();
}
