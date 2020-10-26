#include "version.h"
#include "ui_version.h"
#include <QGridLayout>
#include <QLabel>

Version::Version(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Version)
{
    ui->setupUi(this);
    setFixedSize(400,100);

    setWindowTitle(tr("版本信息"));

    QGridLayout * pLayout = new QGridLayout(this);

    QLabel *pVersion = new QLabel(this);
    pVersion->setText(QString(tr("软件版本: ")) + VERSION);

    QString szCompileTime = __DATE__;
    szCompileTime += " ";
    szCompileTime += __TIME__;

    QLabel * pCompileMsg = new QLabel(this);
    pCompileMsg->setText(QString(tr("编译时间: "))+szCompileTime);

    pLayout->addWidget(pVersion,0,0);
    pLayout->addWidget(pCompileMsg,1,0);
}

Version::~Version()
{
    delete ui;
}
