#include "beamtest.h"

#include "ui.h"
#include "shapefactory.h"
#include "parametricsfordummies.h"
#include "bash.h"
#include "AIS_PointCloud.hxx"
#include <QFileDialog>
#include <QProgressDialog>
#include <QTime>
#include <Graphic3d_ArrayOfPoints.hxx>



beamtest::beamtest(QWidget *parent)
       : QWidget(parent)
{
       ui.setupUi(this);
       setup();
}

beamtest::~beamtest()
{

}

void beamtest::setup()
{
//SLIDERMOVED(update)
connect(ui.testbeam,SIGNAL(pressed()),this,SLOT(readfile()));
connect(ui.writebinary,SIGNAL(pressed()),this,SLOT(writebinary()));
connect(ui.writebinarychunks,SIGNAL(pressed()),this,SLOT(writebinarytochunks()));
connect(ui.readbinary,SIGNAL(pressed()),this,SLOT(readbinary()));
connect(ui.writebinarychunkswithcolor,SIGNAL(pressed()),this,SLOT(writebinarytochunkswithcolor()));
connect(ui.readbinarywithcolor,SIGNAL(pressed()),this,SLOT(readbinarywithcolor()));
}

void beamtest::update()
{
//INITPART
//
//double width = get(width)
//double height = get(height)
//double depth = get(depth)
//
//double thick = get(gap)
//double radius = get(radius)
//
//
//gp_Pnt p1(0,0,0);
//gp_Pnt p2(0,height,0);
//gp_Pnt p3(width,height,0);
//gp_Pnt p4(width,0,0);
//
//gp_Pnt p1b(0 +thick,0+thick,0);
//gp_Pnt p2b(0+ thick,height - thick,0);
//gp_Pnt p3b(width-thick,height-thick,0);
//gp_Pnt p4b(width-thick,0 +thick,0);
//
//
//pointlist(pline)
//pline << p1 << p2 << p3 << p4 << p1;
//
//pointlist(pline2)
//pline2 << p1b << p2b << p3b << p4b << p1b;
//
//TopoDS_Shape shape1 = hsf::AddNewPolyline(pline);
//TopoDS_Shape shape2 = hsf::AddNewPolyline(pline2);
////TopoDS_Shape fill = hsf::AddNewFillSurface(shape1);
//
//
//
//
//BRepBuilderAPI_MakeFace fm(TopoDS::Wire(shape1));
//fm.Add(TopoDS::Wire(shape2));
//TopoDS_Shape face = fm.Shape();
//TopoDS_Shape prism = hsf::AddNewExtrude(face,gp::DZ(),depth);
//
////vis(face)
//
////vis(fill)
//vis(prism)
//
//
//
//
//ENDPART

}


void beamtest::readbinary()
	{


QTime time;
time.start();
Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();

QString filename = QFileDialog::getOpenFileName ( this,
							 tr("Import File"),"");

QFile file(filename);
file.open(QIODevice::ReadOnly);
QDataStream in(&file);    // read the data serialized from the file

QList<gp_Pnt> vertlist;
double x ;
double y ;
double z ;

int i=0;
do {

i++;
in >> x >> y >> z ;

if (i % 100000 == 0) qDebug() << i;
gp_Pnt thevert(x,y,z);
vertlist.append(thevert);
} while ( in.status() ==QDataStream::Ok );

file.close();

Handle(AIS_PointCloud) mypc1 = new AIS_PointCloud(vertlist);
ic->Display(mypc1);

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;

QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

ui::getInstance()->Statusbar(timetxt);
qDebug() <<  "time ellapsed reading binary file:" << timetxt;

}


void beamtest::readbinarywithcolor()
	{


QTime time;
time.start();
Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();

QString filename = QFileDialog::getOpenFileName ( this,
												 tr("Import File"),"");

QFile file(filename);
 file.open(QIODevice::ReadOnly);
 QDataStream in(&file);    // read the data serialized from the file

 //double pcount;
 //in >> pcount ;         // extract "the answer is" and 42


//QProgressDialog progress("loading cloudpoint", "Abort Copy", 1,100);

//QList<Graphic3d_Vertex> vertlist;
QList<gp_Pnt> vertlist;




//for (int i=0;i < pcount;i++)
//{
	       
	double x ;
	double y ;
	double z ;
	double intensity;
	int i=0;
	do {
		
	i++;
	in >> x >> y >> z >> intensity;

	if (i % 100000 == 0) qDebug() << i;
	

			//Graphic3d_Vertex thevert(x,y,z);
			gp_Pnt thevert(x,y,z);
			vertlist.append(thevert);

//			Quantity_Color rgbval(intensity,0,0,Quantity_TOC_RGB);
//			colorlist.append(rgbval);


	//if (i > 30000000) break;
			
		} while ( in.status() ==QDataStream::Ok );

file.close();

Handle(AIS_PointCloud) mypc1 = new AIS_PointCloud(vertlist);
ic->Display(mypc1);

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;

QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

ui::getInstance()->Statusbar(timetxt);
qDebug() <<  "time ellapsed:" << timetxt;

}


void beamtest::readfile()
{

QTime time;
time.start();
Handle_AIS_InteractiveContext ic = ui::getInstance()->getWindowContext();

double chunk = 30000000;
double chunkcount = 3;
//double maxpoints = chunkcount * chunk;
double maxpoints = chunk-1;
Graphic3d_Array1OfVertex* thepointcloud1 = new Graphic3d_Array1OfVertex(1,chunk);


//Graphic3d_Array1OfVertex thepointcloud2(1,chunk);
//Graphic3d_Array1OfVertex thepointcloud3(1,chunk);


QString filename = QFileDialog::getOpenFileName ( this,
												 tr("Import File"),"",
												 tr( "All point cloud formats (*.ptx);;" 
												 "XYZ (*.xyz *XYZ);;" 
												 "PTX (*.ptx *.PTX)"));

QFile file(filename);
if (!file.open(QFile::ReadOnly)) return ;

QMap<QString,gp_Pnt> pointmap;


QProgressDialog progress("loading cloudpoint", "Abort Copy", 1,100);

int count = 0;
int filepos = 0;
QTextStream stream(&file );
QString line;
 do {
        filepos++;
        //qDebug() << "file-linepos:" << filepos;


    line = stream.readLine();


        QStringList linelist = line.split(tr(" "));
        if (linelist.size() == 4)
        {
           
               double x = linelist.at(0).toDouble();
               double y = linelist.at(1).toDouble();
               double z = linelist.at(2).toDouble();
               QString index = linelist.at(0) + linelist.at(1) + linelist.at(2) ;

               if(!pointmap.contains(index))
               {
			    	   count++;
					   if (count % 100000 == 0){
					   progress.setValue((count *100/maxpoints));
						   }
                      // double intensity = linelist.at(3).toDouble();
                       gp_Pnt p1(x,y,z);
                       //TopoDS_Shape point =hsf::AddNewPoint(p1);
                       //vis(point)
					   Graphic3d_Vertex thevert(x,y,z);
					   thepointcloud1->SetValue(count,thevert);
			/*		   if (count < chunk && count > 0)
					   {
						   thepointcloud1.SetValue(count,thevert);
					   } else if (count > chunk && count <= (2*chunk))
					   {
						   thepointcloud2.SetValue(count-chunk,thevert);
					   } else if (count > (2*chunk) && count < (3 * chunk))
					   {
						   thepointcloud3.SetValue(count-(chunk*2),thevert);
					   }*/
					   

                       pointmap.insert(index,p1);
               }

        }

if (count > maxpoints) break;

 } while (!line.isNull());

//Handle(AIS_PointCloud) mypc1 = new AIS_PointCloud(*thepointcloud1);
//ic->Display(mypc1);

//
//Handle(AIS_PointCloud) mypc2 = new AIS_PointCloud(thepointcloud2);
//ic->Display(mypc2);
//
//
//Handle(AIS_PointCloud) mypc3 = new AIS_PointCloud(thepointcloud3);
//ic->Display(mypc3);

//ENDPART

double milliseconds = time.elapsed();
double seconds = (milliseconds/1000);
double minutes = seconds/60;
double hours = minutes/60;


QString timetxt;
QTextStream linestream(&timetxt);
linestream << "h" << hours << ":m " << minutes << ":s " << seconds << ".ms" << milliseconds; 

ui::getInstance()->Statusbar(timetxt);
qDebug() <<  "time ellapsed:" << timetxt;



}




void beamtest::writebinarytochunks()
	{
QFileInfo fileInfo;

QString filename = QFileDialog::getOpenFileName ( this,tr("Import File"),
												 "",tr( "All point cloud formats (*.ptx);;"
												 "XYZ (*.xyz *XYZ);;"
												 "PTX (*.ptx *.PTX)"));



	
		 int chunk = 2000000;
		 //int numfiles = floor((double) shapecount / (double) chunk);
		 int shapecounter=0;
		 int filecounter = 0;

double chunksize= 2000000;
int count =0;
int debugcount = 0;
char buf[1024];
QFile file(filename);
qint64 lineLength=0;
QStringList linelist;

QString filename2 = QFileDialog::getSaveFileName(this, tr("SaveBinary"), " ",tr("dat (*.dat "));
QFile file2(filename2);
 file2.open(QIODevice::WriteOnly);
 QDataStream out(&file2);   // we will serialize the data into the file

 	fileInfo.setFile(filename2);

	QString dirname = fileInfo.absoluteDir().absolutePath();
	filename = fileInfo.fileName();
	QString basename = fileInfo.baseName();
	QString bundlename = fileInfo.bundleName();

 if (file.open(QFile::ReadOnly)) {
        do {

    lineLength = file.readLine(buf, sizeof(buf));
    if (lineLength != -1) {
        QString line(buf);

        linelist = line.split(tr(" "));

        if (linelist.size() == 3)
        {
			
			double x = linelist.at(0).toDouble();
            double y = linelist.at(1).toDouble();
            double z = linelist.at(2).toDouble();
			//double intensity= linelist.at(3).toDouble();
			if(x != 0 && y != 0 && z != 0 )
			{
			count++;
			shapecounter++;
				if(shapecounter == chunk)
					{
					
					shapecounter = 0;
					filecounter++;
					QString curfilename = dirname + tr("/") + basename + QString::number(filecounter) + tr(".data");
					file2.close();
					file2.setFileName(curfilename);
					file2.open(QFile::WriteOnly);
					
			
					}


			if (count % 100000 == 0) qDebug() << count;

			out << x << y << z ;
			}

        }

   //if (count > 10000) break;


    }
        }while (lineLength != -1);
 }


if(file2.isOpen()) file2.close();






	}
void beamtest::writebinarytochunkswithcolor()
	{
QFileInfo fileInfo;

QString filename = QFileDialog::getOpenFileName ( this,tr("Import File"),
												 "",tr( "All point cloud formats (*.ptx);;"
												 "XYZ (*.xyz *XYZ);;"
												 "PTX (*.ptx *.PTX)"));



	
		 int chunk = 2000000;
		 //int numfiles = floor((double) shapecount / (double) chunk);
		 int shapecounter=0;
		 int filecounter = 0;

double chunksize= 2000000;
int count =0;
int debugcount = 0;
char buf[1024];
QFile file(filename);
qint64 lineLength=0;
QStringList linelist;

QString filename2 = QFileDialog::getSaveFileName(this, tr("SaveBinary"), " ",tr("dat (*.dat "));
QFile file2(filename2);
 file2.open(QIODevice::WriteOnly);
 QDataStream out(&file2);   // we will serialize the data into the file

 	fileInfo.setFile(filename2);

	QString dirname = fileInfo.absoluteDir().absolutePath();
	filename = fileInfo.fileName();
	QString basename = fileInfo.baseName();
	QString bundlename = fileInfo.bundleName();

 if (file.open(QFile::ReadOnly)) {
        do {

    lineLength = file.readLine(buf, sizeof(buf));
    if (lineLength != -1) {
        QString line(buf);

        linelist = line.split(tr(" "));

        if (linelist.size() == 4)
        {
			
			double x = linelist.at(0).toDouble();
            double y = linelist.at(1).toDouble();
            double z = linelist.at(2).toDouble();
			double intensity= linelist.at(3).toDouble();
			if(x != 0 && y != 0 && z != 0 )
			{
			count++;
			shapecounter++;
				if(shapecounter == chunk)
					{
					
					shapecounter = 0;
					filecounter++;
					QString curfilename = dirname + tr("/") + basename + QString::number(filecounter) + tr(".data");
					file2.close();
					file2.setFileName(curfilename);
					file2.open(QFile::WriteOnly);
					
			
					}


			if (count % 100000 == 0) qDebug() << count;

			out << x << y << z << intensity;
			}

        }

   //if (count > 10000) break;


    }
        }while (lineLength != -1);
 }


if(file2.isOpen()) file2.close();






	}
void beamtest::writebinary()
	{

QString filename = QFileDialog::getOpenFileName ( this,tr("Import File"),
												 "",tr( "All point cloud formats (*.ptx);;"
												 "XYZ (*.xyz *XYZ);;"
												 "PTX (*.ptx *.PTX)"));

double chunksize= 5000000;
int count =0;
int debugcount = 0;
char buf[1024];
QFile file(filename);
qint64 lineLength=0;
QStringList linelist;

QString filename2 = QFileDialog::getSaveFileName(this, tr("SaveBinary"), " ",tr("dat (*.dat "));
QFile file2(filename2);
 file2.open(QIODevice::WriteOnly);
 QDataStream out(&file2);   // we will serialize the data into the file


 if (file.open(QFile::ReadOnly)) {
        do {

    lineLength = file.readLine(buf, sizeof(buf));
    if (lineLength != -1) {
        QString line(buf);

        linelist = line.split(tr(" "));

        if (linelist.size() == 4)
        {
			
			double x = linelist.at(0).toDouble();
            double y = linelist.at(1).toDouble();
            double z = linelist.at(2).toDouble();
			if(x != 0 && y != 0 && z != 0 )
			{
			count++;

			if (count % 100000 == 0) qDebug() << count;

			out << x << y << z;
			}

        }

   //if (count > 10000) break;


    }
        }while (lineLength != -1);
 }



file2.close();






	}




void beamtest::readfile1()
{



QString filename = QFileDialog::getOpenFileName ( this,
                                                                                         tr("Import File"),
                                                                                        "",
                                                                                         tr( "All point cloud formats (*.ptx);;"
                                                                                             "XYZ (*.xyz *XYZ);;"
                                                                                       "PTX (*.ptx *.PTX)"));


int count =0;
int debugcount = 0;
char buf[1024];
QFile file(filename);
qint64 lineLength=0;
QStringList linelist;
double x,y,z;
TopoDS_Shape point;

QString filename2 = QFileDialog::getSaveFileName(this, tr("SaveTopology"), " ",tr("csv (*.csv "));

        QFile file2(filename2);
        if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
    return;





 if (file.open(QFile::ReadOnly)) {
        do {

    lineLength = file.readLine(buf, sizeof(buf));
    if (lineLength != -1) {
        QString line(buf);


                //qDebug() << line;

        linelist = line.split(tr(" "));

        if (linelist.size() == 4)
        {
           count++;
               debugcount++;
               if (debugcount > 100000){ qDebug() << count << ","  << line; debugcount = 0;}

               QString index = linelist.at(0) + "," + linelist.at(1) + "," +
linelist.at(2) +"\n" ;
               file2.write(index.toAscii().data());
               //x = linelist.at(0).toDouble();
               //y = linelist.at(1).toDouble();
               //z = linelist.at(2).toDouble();
               //
               ////QString index = linelist.at(0) + linelist.at(1) + linelist.at(2) ;

               ////double intensity = linelist.at(3).toDouble();

               //point =hsf::AddNewPoint(gp_Pnt(x,y,z));
               //vis(point)





        }

       // if (count > 10000) {file2.close(); break;}




    }
        }while (lineLength != -1);
 }



file2.close();






}

