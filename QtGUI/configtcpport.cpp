#include "configtcpport.h"
#include "ui_configtcpport.h"
#include <QValidator>
#include <QMessageBox>
#include <QKeyEvent>

ConfigTcpPort::ConfigTcpPort(uint16_t _uTcpPort, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigTcpPort)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    setWindowTitle(tr("设置TCP端口"));
    setFixedSize(300,70);

    QHBoxLayout * pLayOut = new QHBoxLayout(this);

    m_pTcpPort = new QLineEdit(this);
    m_pTcpPort->setPlaceholderText(tr("当前端口:") + QString::number(_uTcpPort));
    m_pTcpPort->setValidator(new QIntValidator(0,65535,this));
    connect(m_pTcpPort,SIGNAL(textChanged(const QString &)),this,SLOT(OnTcpPortInputChanged(const QString &)));
    connect(m_pTcpPort,SIGNAL(returnPressed()),this,SLOT(OnConfirmed()));
    m_pTcpPort->show();

    QPushButton * pBtnConfirmed = new QPushButton(this);

    pBtnConfirmed->setText(tr("设置端口"));
    pBtnConfirmed->setFixedWidth(100);
    connect(pBtnConfirmed,SIGNAL(clicked()),this,SLOT(OnConfirmed()));
    pBtnConfirmed->show();

    pLayOut->addWidget(m_pTcpPort,0);
    pLayOut->addWidget(pBtnConfirmed,1);

    m_pTcpPort->setFocus();

}

ConfigTcpPort::~ConfigTcpPort()
{
    delete ui;
}

uint16_t ConfigTcpPort::GetNewTCPPort()
{
    return (uint16_t)m_pTcpPort->text().toUInt();
}

void ConfigTcpPort::OnTcpPortInputChanged(const QString & _szText)
{
    static int sLastPort = 0;
    int sPort = _szText.toInt();
    if((sPort >= 0) && (sPort <65536))
    {
        sLastPort = sPort;
    }
    else
    {
        m_pTcpPort->setText(QString::number(sLastPort));
    }
}

void ConfigTcpPort::OnConfirmed()
{
    emit accept();
}
