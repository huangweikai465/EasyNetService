#ifndef CONFIGAPP_H
#define CONFIGAPP_H

#include <QDialog>
#include <QGridLayout>
#include <QTreeWidget>
#include <QKeyEvent>
#include "../Src/cJSON/JsonOpt.h"
#include "addappconfig.h"

namespace Ui {
class ConfigApp;
}

class ConfigApp : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigApp(QWidget *parent = nullptr);
    ~ConfigApp();
    JsonOpt GetAppConfig();
    void LoadConfig(JsonOpt & _oCfg);

public slots:
    void AddItem(void);
    void EditItem(void);
    void DoubleClickItem(const QModelIndex &);
    void DeleteItem(void);

private:
    Ui::ConfigApp *ui;

    QTreeWidget * m_pConfigTable;

    AddAppConfig * m_pAddConfig;

};

#endif // CONFIGAPP_H
