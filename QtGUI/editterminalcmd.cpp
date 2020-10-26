#include "editterminalcmd.h"
#include "ui_editterminalcmd.h"
#include "QGridLayout"
#include "QLabel"
#include "QPushButton"

EditTerminalCmd::EditTerminalCmd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTerminalCmd)
{
    ui->setupUi(this);
    setWindowTitle(tr("启动新终端命令"));

    QPushButton * pOK = new QPushButton(this);
    pOK->setText(tr("确定"));
    pOK->show();
    connect(pOK,SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton * pCancel = new QPushButton(this);
    pCancel->setText(tr("取消"));
    pCancel->show();
    connect(pCancel,SIGNAL(clicked()),this,SLOT(reject()));

    //QLabel * pLab = new QLabel(this);
    //pLab->setText("编辑");

    m_pCMDEdit = new QPlainTextEdit(this);
    m_pCMDEdit->show();
    //connect(m_pCMDEdit,SIGNAL(returnPressed()),this,SLOT(accept()));

    QGridLayout * pLayout = new QGridLayout(this);
    pLayout->setSpacing(10);
    pLayout->addWidget(m_pCMDEdit,0,0,1,6);
    pLayout->addWidget(pOK,1,4);
    pLayout->addWidget(pCancel,1,5);
}

EditTerminalCmd::~EditTerminalCmd()
{
    delete ui;
}

void EditTerminalCmd::LoadCmd(const QString &_szCMd)
{
    m_pCMDEdit->setPlainText(_szCMd);
}

QString EditTerminalCmd::GetNewCmd()
{
    return m_pCMDEdit->toPlainText();
}
