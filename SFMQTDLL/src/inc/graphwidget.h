

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui/QGraphicsView>
#include "QoccMainWindow.h"


class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent);

	void addnewwidget(QWidget *widget);

    void itemMoved();
	QoccMainWindow* myMainwindow;




protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
	void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

	

	
	
	
	//Holds the current centerpoint for the view, used for panning and zooming
    QPointF CurrentCenterPoint;
 
    //From panning the view
    QPoint LastPanPoint;
 
    //Set the current centerpoint in the
    void SetCenter(const QPointF& centerPoint);
    QPointF GetCenter() { return CurrentCenterPoint; }

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    //Node *centerNode;
};

#endif
