#include "waveformwidge1t.h"
#include "ui_waveformwidget.h"

WaveformWidget::WaveformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveformWidget)
{
    ui->setupUi(this);

    auto layout=new QVBoxLayout();
    ui->widget->setLayout(layout);
    plot=new QCustomPlot();
    layout->addWidget(plot);

    plot->xAxis->setSubTicks(false);
    plot->xAxis->ticker()->setTickCount(2);

    plot->xAxis2->setVisible(false);
    plot->xAxis2->setTicks(false);
    plot->xAxis2->setTickLabels(false);

    plot->yAxis->setSubTicks(false);

    xRange=ui->xRangeSpinBox->text().toInt();
    plot->xAxis->setRange(0,0+xRange);
    dataGraph=plot->addGraph(plot->xAxis,plot->yAxis);

    connect(ui->xRangeSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),[&](int value){
        this->xRange=value;
        if(x>xRange){
            plot->xAxis->setRange(x,xRange,Qt::AlignRight);
        }else{
            plot->xAxis->setRange(0,0+xRange);
        }
        plot->replot( QCustomPlot::rpQueuedReplot);
    });

}

WaveFormWidget::~WaveFormWidget()
{
    delete ui;
}
void WaveFormWidget::setWaveformTitle(const QString &title)
{
    ui->waveformTitle->setText(title);
}


void WaveFormWidget::addData(const QVector<double> &Values)
{
    int size=Values.size();
    QVector<double> keys;

    for(int i=0;i<size;++i){
        keys.push_back(x++);
    }

    dataGraph->addData(keys,Values,true);
    if(x>xRange){
        plot->xAxis->setRange(x,xRange,Qt::AlignRight);
    }
    dataGraph->rescaleValueAxis(false,true);
    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveFormWidget::clearData()
{
    dataGraph->setData({},{},true);
    x=0;
    plot->xAxis->setRange(0,0+xRange);
    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveFormWidget::setQCPGraphVisible(QCPGraph *graph,int state)
{
    if(state==Qt::Unchecked){
        graph->setVisible(false);
    }else if(state==Qt::Checked){
        graph->setVisible(true);
    }
    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveFormWidget::resetXrange(){
    ui->xRangeSpinBox->setValue(300);
}
