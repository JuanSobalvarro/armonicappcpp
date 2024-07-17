#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>

class Utils
{
public:
    static void showErrorMsgBox(const QString &msg, QMessageBox::Icon icon);
};


#endif // UTILS_H
