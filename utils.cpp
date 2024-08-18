#include "utils.h"


void Utils::showErrorMsgBox(const QString &msg, QMessageBox::Icon icon)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(icon);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

std::vector<std::complex<double> > Utils::qByteArray2fftvector(const QByteArray &array)
{
    std::vector<std::complex<double>> fftVector;

    const int resolution = 4; // 4 bytes per sample
    int len = array.size() / resolution; // Number of samples
    fftVector.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        int sample = 0;
        std::memcpy(&sample, array.data() + i * resolution, resolution);

        // Assuming the data is centered around 128 and normalized (as in writeData)
        double normalizedSample = static_cast<double>(sample - 128) / 128.0;
        fftVector.emplace_back(normalizedSample, 0.0);
    }

    return fftVector;
}
