#ifndef VERSION_H
#define VERSION_H

#include <QDialog>

#define VERSION "1.0.0"

namespace Ui {
class Version;
}

class Version : public QDialog
{
    Q_OBJECT

public:
    explicit Version(QWidget *parent = nullptr);
    ~Version();

private:
    Ui::Version *ui;
};

#endif // VERSION_H
