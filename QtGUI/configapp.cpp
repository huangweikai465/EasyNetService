#include "configapp.h"
#include "ui_configapp.h"
#include <QPushButton>
#include <QMessageBox>

ConfigApp::ConfigApp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigApp)
{
    ui->setupUi(this);
    setWindowTitle(tr("服务启动命令配置"));
    resize(640,size().rheight());

    m_pConfigTable = new QTreeWidget(this);
    QStringList szlHeadLables;
    szlHeadLables << tr("序号") << tr("服务类型") << tr("使用终端") << tr("启动命令");
    m_pConfigTable->setColumnCount(szlHeadLables.size());
    m_pConfigTable->setHeaderLabels(szlHeadLables);
    m_pConfigTable->setColumnWidth(0,70);
    m_pConfigTable->setColumnWidth(1,60);
    m_pConfigTable->setColumnWidth(2,60);
    connect(m_pConfigTable,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(DoubleClickItem(const QModelIndex &)));
    m_pConfigTable->show();

    QPushButton * pBtnOK = new QPushButton(this);
    pBtnOK->setFixedWidth(100);
    pBtnOK->setText(tr("确定"));
    pBtnOK->show();
    connect(pBtnOK, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton * pBtnCancel = new QPushButton(this);
    pBtnCancel->setFixedWidth(100);
    pBtnCancel->setText(tr("取消"));
    pBtnCancel->show();
    connect(pBtnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    QPushButton * pBtnAdd = new QPushButton(this);
    pBtnAdd->setText("+");
    pBtnAdd->setFixedWidth(50);
    pBtnAdd->show();
    connect(pBtnAdd,SIGNAL(clicked()),this,SLOT(AddItem()));

    QPushButton * pBtnDelete = new QPushButton(this);
    pBtnDelete->setText("-");
    pBtnDelete->setFixedWidth(50);
    pBtnDelete->show();
    connect(pBtnDelete,SIGNAL(clicked()),this,SLOT(DeleteItem()));

    QPushButton * pBtnEdit = new QPushButton(this);
    pBtnEdit->setText("···");
    pBtnEdit->setFixedWidth(50);
    pBtnEdit->show();
    connect(pBtnEdit,SIGNAL(clicked()),this,SLOT(EditItem()));

    QGridLayout * pLayout = new QGridLayout(this);
    pLayout->setSpacing(5);
    pLayout->addWidget(m_pConfigTable,0,0,2,6);
    pLayout->addWidget(pBtnAdd,2,0);
    pLayout->addWidget(pBtnDelete,2,1);
    pLayout->addWidget(pBtnEdit,2,2);
    pLayout->addWidget(pBtnOK,3,4);
    pLayout->addWidget(pBtnCancel,3,5);
}

ConfigApp::~ConfigApp()
{
    while(m_pConfigTable->topLevelItemCount() > 0)
    {
        QTreeWidgetItem * pItem = m_pConfigTable->topLevelItem(0);
        m_pConfigTable->removeItemWidget(pItem,0);
        delete pItem;
    }
    delete ui;
}

void ConfigApp::LoadConfig(JsonOpt & _oCfg)
{
    _oCfg.LoadFromString(_oCfg.ToString());
    if(_oCfg.IsArray() == true)
    {
        ForEachArrayItem(oNode,_oCfg)
        {
            std::string szType = oNode["Type"].ToString();
            if(szType.empty())
                continue;
            std::string szCMD = oNode["CMD"].ToString();
            int count = m_pConfigTable->topLevelItemCount();
            QTreeWidgetItem * pItem = new QTreeWidgetItem;
            pItem->setText(0,QString::number(count + 1));
            pItem->setText(1,szType.c_str());
            pItem->setText(3,szCMD.c_str());
            if((oNode["Terminal"].IsNull()==false) && (oNode["Terminal"].ToBool()))
            {
                pItem->setCheckState(2,Qt::Checked);
            }
            else
            {
                pItem->setCheckState(2,Qt::Unchecked);
            }
            m_pConfigTable->addTopLevelItem(pItem);
        }
    }
}

JsonOpt ConfigApp::GetAppConfig()
{
    JsonOpt oJoCfg;
    while(m_pConfigTable->topLevelItemCount() > 0)
    {
        QTreeWidgetItem * pItem = m_pConfigTable->topLevelItem(0);
        QString  szType = pItem->text(1);
        bool bNeedTerminal = pItem->checkState(2);
        QString  szCMD = pItem->text(3);
        JsonOpt oJsonItem;
        oJsonItem.AddItem("Type",szType.toStdString());
        oJsonItem.AddItem("CMD",szCMD.toStdString());
        oJsonItem.AddItem("Terminal",bNeedTerminal);
        oJoCfg.AddArrayItem(oJsonItem);
        m_pConfigTable->removeItemWidget(pItem,0);
        delete pItem;
    }
    printf("Output : %s\r\n",oJoCfg.ToString().c_str());
    return oJoCfg;
}

void ConfigApp::AddItem()
{
    m_pAddConfig = new AddAppConfig(this);
    m_pAddConfig->setModal(true);
    m_pAddConfig->show();
    int ret = m_pAddConfig->exec();
    if(ret == QDialog::Accepted)
    {
        QTreeWidgetItem * pItem = new QTreeWidgetItem;
        pItem->setText(0,QString::number(m_pConfigTable->topLevelItemCount()+1));
        m_pConfigTable->addTopLevelItem(pItem);
        pItem->setText(1,m_pAddConfig->GetType());
        pItem->setCheckState(2,m_pAddConfig->GetUseTerminal()==true?Qt::Checked:Qt::Unchecked);
        pItem->setText(3,m_pAddConfig->GetCMD());
    }
    delete m_pAddConfig;
}

void ConfigApp::EditItem()
{
    QTreeWidgetItem * pItem = m_pConfigTable->currentItem();
    if(pItem != nullptr)
    {
        m_pAddConfig = new AddAppConfig(this);
        m_pAddConfig->LoadItem(pItem->text(1),pItem->checkState(2),pItem->text(3));
        m_pAddConfig->setModal(true);
        m_pAddConfig->show();
        int ret = m_pAddConfig->exec();
        if(ret == QDialog::Accepted)
        {
            m_pConfigTable->addTopLevelItem(pItem);
            pItem->setText(1,m_pAddConfig->GetType());
            pItem->setCheckState(2,m_pAddConfig->GetUseTerminal()==true?Qt::Checked:Qt::Unchecked);
            pItem->setText(3,m_pAddConfig->GetCMD());
        }
        delete m_pAddConfig;
    }
    else
        QMessageBox::information(nullptr,tr("未找到条目"),tr("请选中要编辑的数据"));
}

void ConfigApp::DoubleClickItem(const QModelIndex &)
{
    EditItem();
}

void ConfigApp::DeleteItem()
{
    QTreeWidgetItem * pItem = m_pConfigTable->currentItem();
    m_pConfigTable->removeItemWidget(pItem,0);
    delete pItem;
}


