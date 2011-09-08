

#include "graphwidget.h"
#include "edge.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QSlider>

#include "qgraphicsitem.h"
#include <QGraphicsRectItem>
#include "genericgraphicitem.h"
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVarLengthArray>
#include <math.h>


#include <QtOpenGL\QGLWidget>
class QGLWidget;
GraphWidget::GraphWidget(QWidget *parent)
    : timerId(0)
{

	
	
	myMainwindow = dynamic_cast<QoccMainWindow*>(parent);
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 8000, 8000);
    setScene(scene);
	//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	//setViewport(new QGLWidget());
	setRenderHint(QPainter::Antialiasing, false);
    //setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setCacheMode(CacheBackground);
	
	
    //setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Down:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Left:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Right:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Plus:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Minus:
        QGraphicsView::keyPressEvent(event);
        break;
    case Qt::Key_Space:
		QGraphicsView::keyPressEvent(event);
    case Qt::Key_Enter:
        QGraphicsView::keyPressEvent(event);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    //Q_UNUSED(event);

    //QList<Node *> nodes;
    //foreach (QGraphicsItem *item, scene()->items()) {
    //    if (Node *node = qgraphicsitem_cast<Node *>(item))
    //        nodes << node;
    //}

    //foreach (Node *node, nodes)
    //    node->calculateForces();

    //bool itemsMoved = false;
    //foreach (Node *node, nodes) {
    //    if (node->advance())
    //        itemsMoved = true;
    //}

    //if (!itemsMoved) {
    //    killTimer(timerId);
    //    timerId = 0;
    //}
}

//void GraphWidget::wheelEvent(QWheelEvent *event)
//{
//    scaleView(pow((double)2, -event->delta() / 240.0));
//}

void GraphWidget::wheelEvent(QWheelEvent* event) {
 
    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));
 
    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());
 
    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
 
    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));
 
    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;
 
    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{

	
    Q_UNUSED(rect);
	//return;

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
	painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
	painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
	gradient.setColorAt(0, QColor(100,107,126));
	gradient.setColorAt(1, QColor(100,107,126));
    painter->fillRect(rect.intersect(sceneRect), gradient);
	//painter->setBrush(Qt::BrushStyle::DiagCrossPattern);
    painter->drawRect(sceneRect);

    // Text
    //QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
    //                sceneRect.width() - 4, sceneRect.height() - 4);
    //QString message(tr("Click and drag the nodes around, and zoom with the mouse "
    //                   "wheel or the '+' and '-' keys"));

    //QFont font = painter->font();
    //font.setBold(true);
    //font.setPointSize(14);
    //painter->setFont(font);
    //painter->setPen(Qt::lightGray);
    ////painter->drawText(textRect.translated(2, 2), message);
    //painter->setPen(Qt::black);

    //painter->drawText(textRect, message);
painter->setPen(QColor(135,142,157));
	const int gridSize = 25;

qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

QVarLengthArray<QLineF, 100> lines;

for (qreal x = left; x < rect.right(); x += gridSize)
lines.append(QLineF(x, rect.top(), x, rect.bottom()));
for (qreal y = top; y < rect.bottom(); y += gridSize)
lines.append(QLineF(rect.left(), y, rect.right(), y));

//qDebug() << lines.size();

painter->drawLines(lines.data(), lines.size());



}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

/**
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */
//Set the current centerpoint in the
void GraphWidget::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
 
    //Get the scene area
    QRectF sceneBounds = sceneRect();
 
    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;
 
    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);
 
    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {
 
            CurrentCenterPoint = centerPoint;
 
            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }
 
            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }
 
        }
    }
 
    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}
 
/**
  * Handles when the mouse button is pressed
  */
void GraphWidget::mousePressEvent(QMouseEvent* e) {
    //For panning the view
	if ( e->button() == Qt::RightButton ){
    LastPanPoint = e->pos();
    setCursor(Qt::ClosedHandCursor);
	setCursor(Qt::OpenHandCursor);
	}
	QGraphicsView::mousePressEvent(e);
}
 
/**
  * Handles when the mouse button is released
  */
void GraphWidget::mouseReleaseEvent(QMouseEvent* e) {
	if ( e->button() == Qt::RightButton ){
 
    LastPanPoint = QPoint();
	setCursor(Qt::OpenHandCursor);
		}
	QGraphicsView::mouseReleaseEvent(e);
}
 
/**
*Handles the mouse move event
*/
void GraphWidget::mouseMoveEvent(QMouseEvent* e) {
	
    if(!LastPanPoint.isNull()) {
        //Get how much we panned
        QPointF delta = mapToScene(LastPanPoint) - mapToScene(e->pos());
        LastPanPoint = e->pos();
 
        //Update the center ie. do the pan
        SetCenter(GetCenter() + delta);
    }
		
	QGraphicsView::mouseMoveEvent(e);
	
}


void GraphWidget::addnewwidget(QWidget *widget)
{
 genericgraphicitem *proxy = new genericgraphicitem(this,widget);
 scene()->addItem(proxy);
 this->centerOn(proxy);

}