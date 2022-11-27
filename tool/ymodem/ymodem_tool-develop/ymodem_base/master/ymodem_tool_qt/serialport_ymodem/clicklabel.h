#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QWidget>
#include <QLabel>

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=0): QLabel(parent){}
    ~ClickedLabel() {}
signals:
    void clicked(ClickedLabel* click); // 点击信号
protected:
    void mouseReleaseEvent(QMouseEvent*); // 重载了鼠标释放事件

};

#endif
