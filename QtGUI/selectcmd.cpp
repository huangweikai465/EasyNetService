#include "selectcmd.h"
#include "ui_selectcmd.h"
#include <QGridLayout>
#include <QMessageBox>

SelectCMD::SelectCMD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCMD)
{
    ui->setupUi(this);
    setWindowTitle(tr("请选择命令"));

    m_pCMDList = new QTreeWidget(this);
    m_pCMDList->setHeaderLabels(QStringList()<<tr("命令列表"));
    connect(m_pCMDList,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(OnSelect(const QModelIndex &)));
    QGridLayout * pLayout = new QGridLayout(this);
    pLayout->addWidget(m_pCMDList,0,0);
    pLayout->setSpacing(10);
}

SelectCMD::~SelectCMD()
{
    while(m_pCMDList->topLevelItemCount() > 0)
    {
        QTreeWidgetItem * pItem = m_pCMDList->topLevelItem(0);
        delete pItem;
        m_pCMDList->removeItemWidget(pItem,0);
    }
    delete ui;
}

void SelectCMD::AppendList(const QString &_szCmd)
{
    QTreeWidgetItem * pItem = new QTreeWidgetItem(QStringList() << _szCmd);
    m_pCMDList->addTopLevelItem(pItem);
}

int SelectCMD::GetSelectedIndex()
{
    return m_sSelectIndex;
}

void SelectCMD::OnSelect(const QModelIndex & _index)
{
    m_sSelectIndex = _index.row();
    emit done(QDialog::Accepted);
}
