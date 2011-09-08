#ifndef GENERICGRAPHICITEM_H
#define GENERICGRAPHICITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QGraphicsEllipseItem> 
#include <QGraphicsLinearLayout>
#include <edge.h>
#include <QGraphicsDropShadowEffect>



class GraphWidget;
class QGraphicsProxyWidget;
class QGraphicsSceneMouseEvent;


class genericgraphicitem : public QGraphicsItem
{
public:
	genericgraphicitem(GraphWidget *graphscene,QWidget *parent=0);
	~genericgraphicitem();

	void addEdge(Edge *edge);
	QRectF boundingRect() const;
 protected:
	 QGraphicsProxyWidget *proxyWidget;
	 GraphWidget *graph;
	 QGraphicsDropShadowEffect* dropshadow;
	
	 
	 
	 

	 
     QList<Edge *> edges() const;

     enum { Type = UserType + 1 };
     int type() const { return Type; }

	 void updatenode();
	 QList<Edge *> edgeList;

	 QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	 void setnumberofinputs(int inputnumberval){inputcount = inputnumberval;}

	 

signals:
     
	 

private:
	
	
	
	
	QPainterPath shape() const;
	QGraphicsLinearLayout* inputnozzles;
	QGraphicsEllipseItem*  outputnozzle;
	QList<genericgraphicitem*> outputelements;
	QList<genericgraphicitem*> inputelements;

	int inputcount;


	
	void addnext(genericgraphicitem* node);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		
		

	 
		
};

#endif // GENERICGRAPHICITEM_H
