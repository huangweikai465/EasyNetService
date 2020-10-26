#ifndef CONFIGTCPPORT_H
#define CONFIGTCPPORT_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class ConfigTcpPort;
}

class ConfigTcpPort : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigTcpPort(uint16_t _uTcpPort = 4657,QWidget *parent = nullptr);
    ~ConfigTcpPort();
    uint16_t GetNewTCPPort();

public slots:
    void OnTcpPortInputChanged(const QString & _szText);
    void OnConfirmed();

private:
    Ui::ConfigTcpPort *ui;

    QLineEdit * m_pTcpPort;

};

#endif // CONFIGTCPPORT_H
