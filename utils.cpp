#include "utils.h"


void Utils::showErrorMsgBox(const QString &msg, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(icon);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
