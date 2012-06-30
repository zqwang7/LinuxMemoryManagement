#include "test1.h"
#include "ui_test1.h"

test1::test1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test1)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    resize(200,200);
}

test1::~test1()
{
    delete ui;
}

void test1::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();

    QPainter paint(this);
    paint.translate(100,100);
    paint.setRenderHint(QPainter::Antialiasing);

    paint.save();
    paint.rotate(6.0*time.second());
    paint.setPen(Qt::red);
    paint.setBrush(Qt::red);
    paint.drawConvexPolygon(sec,4);
    paint.restore();

    paint.save();
    paint.rotate(6.0*(time.minute()+time.second()/60.0));
    paint.setPen(Qt::blue);
    paint.setBrush(Qt::blue);
    paint.drawConvexPolygon(min,4);
    paint.restore();

    paint.save();
    paint.setPen(Qt::green);
    paint.setBrush(Qt::green);
    paint.rotate(30.0*(time.hour()+time.minute()/60.0));
    paint.drawConvexPolygon(hour,4);
    paint.restore();

    for(int i = 0;i < 12;i++)
    {
        paint.rotate(30);
        paint.setPen(Qt::black);
        paint.drawLine(0,-88,0,-98);
    }

    for(int j = 0;j<60;j++)
    {
        paint.rotate(6);
        paint.drawLine(0,-91,0,-95);
    }

}
