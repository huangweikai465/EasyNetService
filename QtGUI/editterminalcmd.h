#ifndef EDITTERMINALCMD_H
#define EDITTERMINALCMD_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class EditTerminalCmd;
}

class EditTerminalCmd : public QDialog
{
    Q_OBJECT

public:
    explicit EditTerminalCmd(QWidget *parent = nullptr);
    ~EditTerminalCmd();
    void LoadCmd(const QString & _szCMd);
    QString GetNewCmd();

private:
    Ui::EditTerminalCmd *ui;
    QString m_szCMD;

    QPlainTextEdit * m_pCMDEdit;
};

#endif // EDITTERMINALCMD_H
