#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "3rdparty/qcustomplot/qcustomplot.h"

namespace Ui {
class WaveformWidget;
}

class WaveformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformWidget(QWidget *parent = nullptr);
    ~WaveformWidget();

    void setWaveformTitle(const QString& title);
    void addData(const QVector<double> &Values);
    void clearData();
    void resetXrange();

private:
    Ui::WaveformWidget *ui;
    QCustomPlot *plot;
    QCPGraph *dataGraph;

    void setQCPGraphVisible(QCPGraph *graph,int state);

    double x=0;
    int xRange;
};

#endif // WAVEFORMWIDGET_H
