#ifndef ADDAPPCONFIG_H
#define ADDAPPCONFIG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QKeyEvent>
#include <QCheckBox>

namespace Ui {
class AddAppConfig;
}

class AddAppConfig : public QDialog
{
    Q_OBJECT

public:
    explicit AddAppConfig(QWidget *parent = nullptr);
    ~AddAppConfig();

    void LoadItem(QString _szType,bool _bUseTerminal, QString _szCMD);

    QString GetType();
    bool GetUseTerminal();
    QString GetCMD();

private:
    Ui::AddAppConfig *ui;

    QLineEdit * m_pTypeEdit;
    QLineEdit * m_pCMDEdit;
    QCheckBox * m_pTerminalSelect;

};

#endif // ADDAPPCONFIG_H
