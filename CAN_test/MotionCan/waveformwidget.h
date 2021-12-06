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
    Ui::WaveformWidget *ui;

    void setColor(const QColor &firstColor,const QColor &secondColor,const QColor &thirdColor);
    void setWaveformTitle(const QString& title);
    void addOneData(const QVector<double> &firstValues);
    void addMulData(const QVector<double>& firstValues,const QVector<double>& secondValues,const QVector<double>& thirdValues);

    void clearData();
    void resetXrange();
    void setColor(const QColor &Color);
    void changeCheckBoxName(QString,QString,QString);

private:


    QCustomPlot *plot;
//    QCPGraph *dataGraph;

    QCPGraph *firstGraph;
    QCPGraph *secondGraph;
    QCPGraph *thirdGraph;

    void setQCPGraphVisible(QCPGraph *graph,int state);

    double x=0;
    int xRange;
};

#endif // WAVEFORMWIDGET_H
