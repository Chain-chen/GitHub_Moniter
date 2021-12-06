#include "waveformwidget.h"
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
    plot->xAxis->ticker()->setTickCount(5);

    plot->xAxis2->setVisible(false);
    plot->xAxis2->setTicks(false);
    plot->xAxis2->setTickLabels(false);

    plot->yAxis->setSubTicks(false);
    plot->yAxis->setRange(0,100);

//    plot->yAxis2->setVisible(true);
    plot->yAxis2->setVisible(false);
    plot->yAxis2->setSubTicks(false);

    plot->xAxis->grid()->setVisible(false);
    plot->yAxis->grid()->setVisible(false);

//    plot->axisRect()->addAxis(QCPAxis::atRight);
//    plot->axisRect()->axis(QCPAxis::atRight,1)->setSubTicks(false);

    plot->setNoAntialiasingOnDrag(true);    //禁用抗锯齿，以提高性能
    xRange=ui->xRangeSpinBox->text().toInt();
    plot->xAxis->setRange(0,0+xRange);
//    dataGraph=plot->addGraph(plot->xAxis,plot->yAxis);

    firstGraph=plot->addGraph(plot->xAxis,plot->yAxis);
//    secondGraph=plot->addGraph(plot->xAxis,plot->yAxis2);
//    thirdGraph=plot->addGraph(plot->xAxis,plot->axisRect()->axis(QCPAxis::atRight,1));

//    connect(ui->firstCheckBox,&QCheckBox::stateChanged,[&](int state){this->setQCPGraphVisible(firstGraph,state);});
//    connect(ui->secondCheckBox,&QCheckBox::stateChanged,[&](int state){this->setQCPGraphVisible(secondGraph,state);});
//    connect(ui->thirdCheckBox,&QCheckBox::stateChanged,[&](int state){this->setQCPGraphVisible(thirdGraph,state);});

    connect(ui->xRangeSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),[&](int value){
        this->xRange=value;
        if(x>xRange){
            plot->xAxis->setRange(x,xRange,Qt::AlignRight);
        }else{
            plot->xAxis->setRange(0,0+xRange);
        }
        plot->replot( QCustomPlot::rpQueuedReplot);
    });

    setColor(QColor(0,149,255),QColor(254,67,101),QColor(131,175,155));
    plot->axisRect()->axis(QCPAxis::atBottom,0)->setBasePen(QPen(Qt::white));
    plot->axisRect()->axis(QCPAxis::atBottom,0)->setTickPen(QPen(Qt::white));
    plot->axisRect()->axis(QCPAxis::atBottom,0)->setSubTickPen(QPen(Qt::white));
    plot->axisRect()->axis(QCPAxis::atBottom,0)->setTickLabelColor(Qt::white);

    plot->setBackground(QColor(41,45,57));

//    plot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);//模式：框选放大
    plot->setInteraction(QCP::iRangeDrag,true);//使能拖动

    ui->secondCheckBox->hide();
    ui->thirdCheckBox->hide();
    ui->firstCheckBox->hide();
}

WaveformWidget::~WaveformWidget()
{
    delete ui;
}
void WaveformWidget::setWaveformTitle(const QString &title)
{
    ui->waveformTitle->setText(title);
}

void WaveformWidget::setColor(const QColor &firstColor, const QColor &secondColor, const QColor &thirdColor)
{
    plot->axisRect()->axis(QCPAxis::atLeft,0)->setBasePen(firstColor);
    plot->axisRect()->axis(QCPAxis::atLeft,0)->setTickPen(firstColor);
    plot->axisRect()->axis(QCPAxis::atLeft,0)->setSubTickPen(firstColor);
    plot->axisRect()->axis(QCPAxis::atLeft,0)->setTickLabelColor(firstColor);

//    plot->axisRect()->axis(QCPAxis::atRight,0)->setBasePen(secondColor);
//    plot->axisRect()->axis(QCPAxis::atRight,0)->setTickPen(secondColor);
//    plot->axisRect()->axis(QCPAxis::atRight,0)->setSubTickPen(secondColor);
//    plot->axisRect()->axis(QCPAxis::atRight,0)->setTickLabelColor(secondColor);

//    plot->axisRect()->axis(QCPAxis::atRight,1)->setBasePen(thirdColor);
//    plot->axisRect()->axis(QCPAxis::atRight,1)->setTickPen(thirdColor);
//    plot->axisRect()->axis(QCPAxis::atRight,1)->setSubTickPen(thirdColor);
//    plot->axisRect()->axis(QCPAxis::atRight,1)->setTickLabelColor(thirdColor);

    firstGraph->setPen(QPen(firstColor));
//    secondGraph->setPen(QPen(secondColor));
//    thirdGraph->setPen(QPen(thirdColor));

    ui->firstCheckBox->setStyleSheet(QString("QCheckBox { color: #fff; }\
                                           QCheckBox::indicator:unchecked { border: 1px solid %1; background-color: #FFFFFF; }\
                                           QCheckBox::indicator:checked { border: 1px solid %1; background-color: %1; }").arg(firstColor.name()));
    ui->secondCheckBox->setStyleSheet(QString("QCheckBox { color: #fff; }\
                                           QCheckBox::indicator:unchecked { border: 1px solid %1; background-color: #FFFFFF; }\
                                           QCheckBox::indicator:checked { border: 1px solid %1; background-color: %1; }").arg(secondColor.name()));
    ui->thirdCheckBox->setStyleSheet(QString("QCheckBox { color: #fff; }\
                                           QCheckBox::indicator:unchecked { border: 1px solid %1; background-color: #FFFFFF; }\
                                           QCheckBox::indicator:checked { border: 1px solid %1; background-color: %1; }").arg(thirdColor.name()));
}


//一个plot多个波形
void WaveformWidget::addMulData(const QVector<double> &posValues, const QVector<double> &velValues, const QVector<double> &eleValues)
{
    int size=posValues.size();
    if(velValues.size()!=size || eleValues.size()!=size){
        QMessageBox::critical(this, QString::fromLocal8Bit("数据错误"), QString::fromLocal8Bit("波形曲线接收的数据量不相同！"), QMessageBox::Close);
    }

    QVector<double> keys;
    for(int i=0;i<size;++i){
        keys.push_back(x++);
    }

    firstGraph->addData(keys,posValues,true);
//    secondGraph->addData(keys,velValues,true);
//    thirdGraph->addData(keys,eleValues,true);

    if(x>xRange){
        plot->xAxis->setRange(x,xRange,Qt::AlignRight);
    }
    firstGraph->rescaleValueAxis(false,true);
//    secondGraph->rescaleValueAxis(false,true);
//    thirdGraph->rescaleValueAxis(false,true);

//    firstGraph->setPen(QPen(QColor(0,149,255)));
//    secondGraph->setPen(QPen(QColor(254,67,101)));
//    thirdGraph->setPen(QPen(QColor(220,87,18)));

    plot->replot( QCustomPlot::rpQueuedReplot);
}

//一个plot一个波形
void WaveformWidget::addOneData(const QVector<double> &posValues)
{
    int size=posValues.size();
    QVector<double> keys;
    for(int i=0;i<size;++i){
        keys.push_back(x++);
    }

    firstGraph->addData(keys,posValues,true);


    if(x>xRange){
        plot->xAxis->setRange(x,xRange,Qt::AlignRight);
    }
    firstGraph->rescaleValueAxis(false,true);

//    firstGraph->setPen(QPen(QColor(0,149,255)));
//    secondGraph->setPen(QPen(QColor(254,67,101)));
//    thirdGraph->setPen(QPen(QColor(220,87,18)));

    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveformWidget::clearData()
{
//    dataGraph->setData({},{},true);
    firstGraph->setData({},{},true);
//    secondGraph->setData({},{},true);
//    thirdGraph->setData({},{},true);
    x=0;
    plot->xAxis->setRange(0,0+xRange);
    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveformWidget::setQCPGraphVisible(QCPGraph *graph,int state)
{
    if(state==Qt::Unchecked){
        graph->setVisible(false);
    }else if(state==Qt::Checked){
        graph->setVisible(true);
    }
    plot->replot( QCustomPlot::rpQueuedReplot);
}

void WaveformWidget::resetXrange(){
    ui->xRangeSpinBox->setValue(500);
}

void WaveformWidget::changeCheckBoxName(QString first,QString second,QString third)
{
    ui->firstCheckBox->setText(first);
//    ui->secondCheckBox->setText(second);
//    ui->thirdCheckBox->setText(third);
}
