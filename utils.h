#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>

#include <vector>
#include <complex>

class Utils
{
public:
    static void showErrorMsgBox(const QString &msg, QMessageBox::Icon icon);
    static std::vector<std::complex<double>> qByteArray2fftvector(const QByteArray &array);
};


#endif // UTILS_H
