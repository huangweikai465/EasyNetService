#ifndef SELECTCMD_H
#define SELECTCMD_H

#include <QDialog>
#include <QStringList>
#include <QTreeWidget>
#include <QKeyEvent>

namespace Ui {
class SelectCMD;
}

class SelectCMD : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCMD(QWidget *parent = nullptr);
    ~SelectCMD();
    void AppendList(const QString & _szCmd);
    int GetSelectedIndex();

public slots:
    void OnSelect(const QModelIndex & _index);

private:
    Ui::SelectCMD *ui;
    QTreeWidget * m_pCMDList;
    int m_sSelectIndex;
};

#endif // SELECTCMD_H
