
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsProxyWidget>
#include <QLinearGradient>
#include <QPointF>
#include <GraphWidget.h>

#include "genericgraphicitem.h"
#include <qDebug>


genericgraphicitem::genericgraphicitem(GraphWidget *graphscene,QWidget *parent)
:graph(graphscene)
{
if (parent) {
         proxyWidget = new QGraphicsProxyWidget(this);
         proxyWidget->setFocusPolicy(Qt::StrongFocus);
         proxyWidget->setWidget(parent);
		 
         //proxyWidget->setGeometry(QRectF(0, 0, 100, 30));
     }

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
	inputcount = 1;
	setAcceptHoverEvents ( true );
	//dropshadow = new QGraphicsDropShadowEffect();
	///*dropshadow->setBlurRadius(14);
	//dropshadow->setOffset(4);
	//dropshadow->moveToThread(QCoreApplication::instance()->thread());*/

	//setGraphicsEffect(dropshadow);
	
	
	
}

genericgraphicitem::~genericgraphicitem()
{

}
QPainterPath genericgraphicitem::shape() const
{
    QPainterPath path;
    //path.addEllipse(-10, -10, 20, 20);
	path.addRect(boundingRect());

    return path;
}

void genericgraphicitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
	
	return;
	try 
		{
	  QLinearGradient gradient(0.5,1.0,0.5,0);

  		gradient.setStart(0.5,1.0);
		gradient.setFinalStop(0.5,0);

		gradient.setColorAt(0.7, QColor(228,228,228));
		gradient.setColorAt(0.7, QColor(228,228,228));
		gradient.setColorAt(1, QColor(102,209,255));

    painter->setBrush(gradient);
	
	painter->drawRoundRect(boundingRect(),10,10);
	
 if (proxyWidget && !proxyWidget->isVisible()) {

	     proxyWidget->show();
         proxyWidget->setFocus();
     }
     if (proxyWidget && proxyWidget->pos() != QPoint())
         proxyWidget->setGeometry(QRectF(0, 0, 180, 180));

	 if(!(inputnozzles)); inputnozzles = new QGraphicsLinearLayout(Qt::Vertical);
	 if ((this->inputnozzles->count() != this->inputcount)){
		
		
		for (int i = 0;i<inputcount;i++)
			{
			painter->setPen(QPen(Qt::black,1));
			QGraphicsEllipseItem* ellipseitem = new QGraphicsEllipseItem(-5,35+(i*20),10,10,this);
			ellipseitem->setBrush(QColor(251,245,45));
			QGraphicsWidget *curinput = new QGraphicsWidget(ellipseitem);
			
			inputnozzles->addItem(curinput);
			curinput->show();
			}

		 }

	 if (!(this->outputnozzle)){
			outputnozzle = new QGraphicsEllipseItem(0,0,6,6,this);
			outputnozzle->show();
			}
			painter->setBrush(Qt::black);
			painter->drawText(boundingRect().topLeft(),"Node");


		}
		catch(...)
			{
			qDebug() << "painting generic item crashed";
			
			}

		 }

	

QRectF genericgraphicitem::boundingRect() const
     {
	 QRectF mygeo = proxyWidget->geometry();
	
	 mygeo.adjust(-10,-20,10,25);
	
         return mygeo;
     }

void genericgraphicitem::addnext(genericgraphicitem* node)
	{
		outputelements << node;

	}
void genericgraphicitem::updatenode()
	{
		QListIterator<genericgraphicitem*> childreniterate(outputelements);
				
				while (childreniterate.hasNext())
				{
					childreniterate.next()->updatenode();	
				}

	}






QVariant genericgraphicitem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void genericgraphicitem::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> genericgraphicitem::edges() const
{
    return edgeList;
}




	

	
	