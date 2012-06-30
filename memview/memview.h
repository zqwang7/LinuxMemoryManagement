#ifndef MEMVIEW_H
#define MEMVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QSemaphore>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>
#include <fcntl.h>

int Mutex;
int Empty;
int Full;
int file1,file2;
int receive[5000];

bool flag = 0;

FILE *fp = NULL;

unsigned long buffer[10];

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    unsigned long getPoint();
    void sendPoint(int pos,unsigned long addr);
    ~Widget();


protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Widget *ui;
};

#endif // MEMVIEW_H
