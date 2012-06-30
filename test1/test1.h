#ifndef TEST1_H
#define TEST1_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QTime>
#include <QTimer>

static QPoint sec[4]={QPoint(0,-75),QPoint(2,0),QPoint(0,10),QPoint(-2,0)};
static QPoint min[4]={QPoint(0,-60),QPoint(4,0),QPoint(0,8),QPoint(-4,0)};
static QPoint hour[4]={QPoint(0,-40),QPoint(6,0),QPoint(0,6),QPoint(-6,0)};

namespace Ui {
    class test1;
}

class test1 : public QWidget
{
    Q_OBJECT

public:
    explicit test1(QWidget *parent = 0);
    ~test1();


protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::test1 *ui;

};

#endif // TEST1_H
