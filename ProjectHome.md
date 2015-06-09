Help us research how to make openshapefactory better: if you have downloaded OpenShapeFactory and you have used it, could you please fill the following survey on usability :
http://www.surveymonkey.com/s/FQ3HLWB

http://openshapefactory.googlecode.com/files/kmeans2.PNG
# What is "OpenShapeFactory": #

OpenShapefactory is a Middle-Man between you, OpenCascade and QT, it serves as a Geometry Factory wrapping OpenCascade, and providing a single-point of entry to the creation of most geometrical operations.

In a Gecko - OpenShapeFactory allows you to
  * create Brep-Geometry: i.e ( sweeps, loft surface, revolvesurface, face,edge,etc);
  * visualize the geometry into a 3d viewport.
  * link QtWidget's created manually in the Qt-Designer to Update Functions that Create Geometry.
  * Get direct access to the HSF library using QtScript( ECMA/Javascript) using the ScriptWidget.

![http://openshapefactory.googlecode.com/files/user%20interface%20diagram.png](http://openshapefactory.googlecode.com/files/user%20interface%20diagram.png)

![http://openshapefactory.googlecode.com/files/qt_ambassador_logo1.png](http://openshapefactory.googlecode.com/files/qt_ambassador_logo1.png)

This Project is a member of the Qt Ambassador program: [link to showcase](http://qt.nokia.com/qt-in-use/project?id=a0F20000006NC3lEAG)


OpenShapeFactory Download Manual Linkhttp://openshapefactory.googlecode.com/files/openshapefactory.pdf

## Download Compiled Binary for **_Windows_** ##
011(**Latest**):[(pre-alpha)v000.011](http://openshapefactory.googlecode.com/files/openshapefactory_pre_alpha_v000.011.rar)


  * Step1: Download from the link above.
  * Step2: Unrar the package.
  * Step3: Double Click on occinstaller.bat(Do only once this installs system variables for OCC, if you already have OCC skip this step.).
  * Step4: Load SFMGUI.Exe and Have fun!!!.

> note: OCC = OpenCascade.

# About #

OpenShapeFactory started on top of [QtOCC](http://qtocc.sourceforge.net/) developed by Peter Dolbey, I have embedded the QoccViewport and all other QtOCC classes, there have been some minor modification here and there to the original source code to make it work on my computer.





# The Main Interface: #

http://openshapefactory.googlecode.com/files/scintilla%20qgraphics%202.PNG

## The Application: ##

the openshapefactory is divided literally between a 2d surface left and a 3d surface right. the 2d surface is a diagrammatic panel where widgets are loaded, in the case of the precompiled download, a QScintilla text-editor widget with programmatic access to the openshapefactory is automatically added as the only widget. To the right of the 2d panel you have the 3d view port, any change to the code that results from a visible shape will be immediately visible in the 3d view port.

## Menus and Toolbars: ##

![http://openshapefactory.googlecode.com/files/menus%20and%20toolbars.png](http://openshapefactory.googlecode.com/files/menus%20and%20toolbars.png)

### basic 2d manipulations: ###
  * panning : right-click and drag on 2d panel background
  * zoomming : use the scroll-wheel on your mouse, or the right side of your touch pad on a laptop
  * moving widgets : left-click and drag on the gray area around any widget

### basic 3d viewport manipulation ###
  * panning : press control + holding down the middle mouse button(scroll wheel)
  * zoom : press control + holding down the left mouse button
  * orbit : press control + holding right mouse button


## The Code Editor: ##

![http://openshapefactory.googlecode.com/files/editor_annotated.png](http://openshapefactory.googlecode.com/files/editor_annotated.png)

# Sample Tutorial: #

## Kilian Roof Example ##

<a href='http://www.youtube.com/watch?feature=player_embedded&v=cNZcVrlx52E' target='_blank'><img src='http://img.youtube.com/vi/cNZcVrlx52E/0.jpg' width='800' height=600 /></a>

In this example we demonstrate how to create a loft surface by developing a custom crossection using user defined functions, also populating panels on the surface using a user defined panel delegate function.

I have provided an utility function called panelize, this function subdivides a surface into rectangular regions, as many regions you want on the u direction of the surface, and as many regions you want on the v direction. on each region you will always get 4 points, this is the signature required by your custom function, in this case called mypanel, this function accepts 4 points, inside the function you can do anything you want with those 4 points, the 4 points represent the 4 corners of each panel, in this case we draw 2 diagonals across p1-p3 and p2-4, since this executed over all panels, we get the effect of diagonals running across the overall surface, when in fact the effect is only being executed on a per panel basis, have fun!.

when running this example make sure the x1 x2 x3 and x4 sliders are distributed in an incrementing order at first, for example
set x1 at 0 , x2 at 25 , x3 at 50 and x4 at 100

the sliders represent the 3 points of the spine controlling the surface, if they are twisting in space the surface might not generate as nicely, the z value of each point controls the width of the crossection at each interval.

```

minval  =  -300
maxval  =  300
x1  =  getval(0,minval,maxval)
y1  =  getval(1,minval,maxval)
z1  =  getval(2,0,maxval)

x2  =  getval(3,minval,maxval)
y2  =  getval(4,minval,maxval)
z2  =  getval(5,0,maxval)

x3  =  getval(6,minval,maxval)
y3  =  getval(7,minval,maxval)
z3  =  getval(8,0,maxval)

x4  =  getval(9,minval,maxval)
y4  =  getval(10,minval,maxval)
z4  =  getval(11,0,maxval)

p1  =  makepoint(x1,y1,0)
p2  =  makepoint(x2,y2,0)
p3  =  makepoint(x3,y3,0)
p4  =  makepoint(x4,y4,0)

splineset  =  makepointlist()
splineset  =  addpointtolist(splineset,p1)
splineset  =  addpointtolist(splineset,p2)
splineset  =  addpointtolist(splineset,p3)
splineset  =  addpointtolist(splineset,p4)
spline1  =  makebspline(splineset)
vis(spline1)

crs1  =  crossection(spline1,0,z1,z1)
crs2  =  crossection(spline1,0.25,z2,z2)
crs3  =  crossection(spline1,0.75,z3,z3)
crs4  =  crossection(spline1,1,z4,z4)

loftlist= makeshapelist()
loftlist  =  addshapetolist(loftlist,crs1)
loftlist  =  addshapetolist(loftlist,crs2)
loftlist  =  addshapetolist(loftlist,crs3)
loftlist  =  addshapetolist(loftlist,crs4)
loft1  =  makeloft(loftlist)


panelize(loft1,10,10,mypanel)




function  crossection(spline,percent,width,height)
{
ratio  = percent
width  =  (width/2)  +1  
height  =  height  +  1
up  =  makevector(0,0,1)
p1  =  makepointoncurve(spline1,ratio)
v1  =  makevectortangenttocurve(spline1,ratio)
l1  =  makelineptdir(p1,v1,0,width)
left  =    makerotate(l1,p1,up,-90)
right  =    makerotate(l1,p1,up,90)
endpl  =  makepointoncurve(left,1)
endpr  =  makepointoncurve(right,1)
upline  =  makelineptdir(p1,up,0,height)
endpup  =  makepointoncurve(upline,1)

curvelist  =  makepointlist()
curvelist  =  addpointtolist(curvelist,endpl)
curvelist  =  addpointtolist(curvelist,  endpup)
curvelist  =  addpointtolist(curvelist,endpr)
curve  =  makebspline(curvelist)
//vis(curve)

return  curve
}


function  mypanel(p1,p2,p3,p4)
{

vis(p1)
vis(p3)
//vis(p2)
diag1  =  makelineptpt(p1,p3)
diag2  =  makelineptpt(p2,p4)
midp1  =  makepointoncurve(diag1,0.5)
vis(midp1)
vis(diag1)
vis(diag2)

}


```


<a href='http://www.youtube.com/watch?feature=player_embedded&v=dgsu6xTAvMY' target='_blank'><img src='http://img.youtube.com/vi/dgsu6xTAvMY/0.jpg' width='800' height=600 /></a>

# Sample code: #

## make hyperboloid points and ellipsoid points ##
```
ucount  =  getval(0,1,100)
vcount  =  getval(0,1,100)
upercent  =  getval(0,1,100)
vpercent  =  getval(0,1,100)
a  =  getval(1,1,100)
b  =  getval(2,1,100)
c  =  getval(3,1,100)
upival  =  2  *  3.14  *  (upercent/100)
vpival  =  2  *  3.14  *  (vpercent/100)
a1  =  a  /100
b1  =  b  /100
c1  =  c  /100
a2  =  a/4  /100
b2 =  b  /100
c2 =  c /3 /100

for  (i=0;i<ucount;i++)  {
u  =  (upival/  ucount)  *  i
	for  (j=0;j<vcount;j++)  {
	v  =  (vpival /  vcount)  *  j

	x=a1  *  Math.cos(u)  *  Math.sin(v)
	y=b1  *  Math.sin(u)  *  Math.sin(v)	
	z=  c1 *  Math.cos(v)

	x2=a2  * mcosh(v)  *  Math.cos(u)	
	y2  = a2  *  mcosh(v)  *  Math.sin(u)	
	z2= c2  *  msinh(v)  -0.3

	p1  =  makepoint(x,y,z)
	p2  =  makepoint(x2,y2,-z2)
	l1    =  makelineptpt(p1,p2)
	vis(l1)
	vis(p2)
	vis(p1)
	}
}

```

## Make a single Point ##
Type the following code in the code editor and see the geometry update as you type.
```
p1 = makepoint(0,0,0) // create a point
vis(p1) // showpoint on viewport
```

## Make a Point Loop ##
Type the following code in the code editor and see the geometry update as you type.
```
for(i = 1 ; i < 100 ; i++)
{
 p1 = makepoint(i*i,i*10,0) // create a point
 vis(p1) // showpoint on viewport
}
```

## Make a Line Between 2 points ##
Type the following code in the code editor and see the geometry update as you type.
```
p1 = makepoint(0,0,0) // create a point
vis(p1) // showpoint on viewport

p2 = makepoint(0,30,0) // create a point
vis(p2) // showpoint on viewport

l1 = makelineptpt(p1,p2) // make a line between 2 points
vis(l1) //show the line in the viewport
```

## make a surface bulge as function of its proximity to the mouse ##
width controls the x of the surface
height controls the y dimension of the surface
vcount control the number of row CV's
ucount controls the number of column CV's
mult controls the effect the mouse has over the bulge

move the mouse on the surface and see the model update

```
width  =  getval(0,1,2000)
height  =  getval(1,1,2000)
vcount  =  getval(2,1,20)
ucount  =  getval(3,1,20)
mult  =  getval(4,1,20)
orig  =  getmousepos()


if  (width   >  0  &&  height  >  0  &&  vcount  >2  &&  ucount  >  2)
{
plist  =  makepointlist()
for  (  v  =  0;v<vcount;v++)
{
vstep  =  height/vcount

for  (  u =  0;u<ucount;u++)
{
ustep  =  width/ucount
zval  =  Math.sin((u*v)*u/10)*20*mult/30;
p1  =  makepoint(u*  ustep,v*  vstep  ,zval)
dis  =  getdistance(orig,p1)
if(dis  >  width/1)
{
dis  =  0
}
up  =  makevector(0,0,1)
p2  =  makepointmovebyvector(p1,up,(dis/100 )*(mult+10)*10)

plist  =  addpointtolist(plist,p2)
vis(p2)
}
}

srf  =  makebeziersurface(plist,ucount,vcount)
vis(srf)
}

```


## make an adjustable parallelogram ##
```

A  =  getval(0,0,300)
B  =  getval(1,0,300)
C  =  getval(2,0,300)
D  =  getval(3,0,300)

p1  =  makepoint(A,0,0)
vis(p1)

p2  =  makepoint(B,0,0)
vis(p2)

p3  =  makepoint(100, C,0)
vis(p3)

p4  =  makepoint(0,D,0)
vis(p4)

l1  =  makelineptpt(p1,p2)
l2  =  makelineptpt(p2,p3)
l3  =  makelineptpt(p3,p4)
l4  =  makelineptpt(p4,p1)
vis(l1)
vis(l2)
vis(l3)
vis(l4)

```

## make Ellipsoid Points ##
```
ucount  =  getval(0,1,100)
vcount  =  getval(0,1,100)
upercent  =  getval(0,1,100)
vpercent  =  getval(0,1,100)

a  =  getval(1,1,100)
b  =  getval(2,1,100)
c  =  getval(3,1,100)


upival  =  2  *  3.14  *  (upercent/100)
vpival  =  2  *  3.14  *  (vpercent/100)

print(pival)
a  =  a  /100
b  =  b  /100
c  =  c  /100

for  (i=0;i<ucount;i++)
{
u  =  (upival/  ucount)  *  i

for  (j=0;j<vcount;j++)
{
v  =  (vpival /  vcount)  *  j

x=a  *  Math.cos(u)  *  Math.sin(v)
y=b  *  Math.sin(u)  *  Math.sin(v)	
z=  c  *  Math.cos(v)

p1  =  makepoint(x,y,z)
vis(p1)

}
}



```


## same as before but with a loop ##
```
minval  =  0
maxval  =  300
A  =  getval(0,minval,maxval)
B  =  getval(1,minval,maxval)
C  =  getval(2,minval,maxval)
D  =  getval(3,minval,maxval)
alt  =  getval(4,2,20)

for(i  =  1  ;  i  <  alt  ;  i++)
{
p1  =  makepoint(A,0,0  +  i*10)
vis(p1)

p2  =  makepoint(B,0,0  +  i*10)
vis(p2)

p3  =  makepoint(100, C,0  +  i*10)
vis(p3)

p4  =  makepoint(0,D,0  +  i*10)
vis(p4)

l1  =  makelineptpt(p1,p2)
l2  =  makelineptpt(p2,p3)
l3  =  makelineptpt(p3,p4)
l4  =  makelineptpt(p4,p1)
vis(l1)
vis(l2)
vis(l3)
vis(l4)
}
```

## Move a circle centered on a point using a slider from the Front-End ##
Type the following code in the code editor and see the geometry update as you type.
```
x = getval(0,1,200)
y = getval(1,1,200)
z = getval(2,1,200)
radius = getval(3,10,200) 
p1 = makepoint(x,y,z) // create a point and update when slider moves
vis(p1) // showpoint on viewport

upvec = makevector(0,0,1)
c1 = makecircle(p1,upvec,radius)
vis(c1)
```

## Previous example using a loop ##
Type the following code in the code editor and see the geometry update as you type.
```

x = getval(0,1,200)
y = getval(1,1,200)
z = getval(2,1,200)
radius = getval(3,10,200)
countz = getval(4,10,200)
for(i  =  1  ;  i  <  countz  ;  i++)
{
p1 = makepoint(x,y,z  +  i*10) // create a point and update when slider moves
vis(p1) // showpoint on viewport

upvec = makevector(0,0,1)
c1 = makecircle(p1,upvec,radius)
vis(c1)
}

```

## Make hyperbolas using a user defined Functions ##
Type the following code in the code editor and see the geometry update as you type.
```
radio  =  getval(0,1,200)  +10
cantidad  =  getval(0,1,100)  +2
height1  =  getval(0,1,500)  +  30

for(j=0;j  <  cantidad;  j++)
{
        hb  =  hyperbola(j*200/2,j*j,0,radio,height1*10)
        vis(hb)
        
}

function  hyperbola(x,y,z,radius,height)
{
lista1  =  makeshapelist
count  =  5
for  (i  =  1;  i  <  count;  i++)
{
                p1  =  makepoint(x,y,z  +  (i*(height/count)) )
                v1  =  makevector(0,0,1)
                circulo  =  makecircle(p1,v1,radius*i*i)
                if(i  >  1)
                {
                                lista1  =  addshapetolist(lista1,circulo)
                }
                //vis(circulo)
                //vis(p1)
        }

        loft1  =  makeloft(lista1)
        return  loft1
}

```

## Same as Previous with Gaussian Curvature Analysis ##
Type the following code in the code editor and see the geometry update as you type.
```

radio  =  getval(0,1,200)  +10
cantidad  =  getval(0,1,100)  +2
height1  =  getval(0,1,500)  +  30

for(j=0;j  <  cantidad;  j++)
{
        hb  =  hyberbola(j*200/2,j*j,0,radio,height1*10)
        viscurvature(hb,true,10)
        
}

function  hyberbola(x,y,z,radius,height)
{
lista1  =  makeshapelist
count  =  5
for  (i  =  1;  i  <  count;  i++)
{
                p1  =  makepoint(x,y,z  +  (i*(height/count)) )
                v1  =  makevector(0,0,1)
                circulo  =  makecircle(p1,v1,radius*i*i)
                if(i  >  1)
                {
                                lista1  =  addshapetolist(lista1,circulo)
                }
                //vis(circulo)
                //vis(p1)
        }

        loft1  =  makeloft(lista1)
        return  loft1
}

```

## point attractor on fenestration ##
using the getmousepos() function, we can extract the position of the mouse on the preference plane (the grid), and combined with the getdistance() function,we can obtain a gradient effect, from the discreet measurements of a field of points in relationship to another point as reference, each local value can in-turn be converted to angles and other relationships, this type of operations are ideal for mapping images on to discreet parametric models.

```
p1  =  getmousepos()
vis(p1)
weight  =  getval(2,1,100)/10

xcount  =  getval(0,1,200)
ycount  =  getval(1,1,200)

previ  =0
for  (i=0;i  <  xcount;  i++)
{



srflist  =  makeshapelist()
for(j=0;j<ycount;j++)
{
p2  =  makepoint(i*10,j*10,0)
p2b  =  makepoint(previ*10,j*10,0)

dis  =  getdistance(p1,p2)+1
up  =  makevector(0,0,1)
p3  =  makepoint(i*10,j*10,dis/(10*weight ))
v1  =  makevector(p2,p1)

length  =  10
if  (dis<10)  length  =  dis

l1  =  makelineptdir(p2,v1,0,length-2)
angle  =  (dis*(weight)  +1)  /  ((j+1)  *  10)
if  (angle  >  90)  angle  =  90
if  (angle  <  0)  angle  =  0

l2  =  makerotate(l1,p2,up,angle)

//vis(l2)
//vis(l1)
rad  =  dis/(20*weight )
c1  =  makecircle(p2,up,rad+1)

if   (j>0)
{
el  =makelineptpt(p2,prevj)
l3  =  makerotate(el,p2,v1,angle)
//vis(el)
el2  =makelineptpt(p2b,prevj)
srflist  =  addshapetolist(srflist,l3)
//vis(el2)
}

//c2  =  makecircle(p3,up,dis/(20*weight ))
//vis(c1)
//vis(c2)
endp  =  makepointoncurve(l2,1)

prevj  =  endp
}
srf  =  makeloft(srflist)
vis(srf)
previ  =i
}

```

## point attractor on spherical grid ##
```

number  =  getval(0,1,200)
numpoints  =  getval(0,1,20)
myradius  =  getval(1,1,200)


p1  =   makepoint(0,0,0)
mp  =  getmousepos()
vis(p1)
up  =  makevector(0,0,1)
c1  =  makecircle(p1,up,myradius)

//gizmo
xdir  =  makevector(1,0,0)
ydir  =  makevector(0,1,0)
gx  = vis( makecircle(mp,xdir,3))
gy  = vis( makecircle(mp,ydir,3))
gz  = vis( makecircle(mp,up,3))
vis(mp)

vis(c1)
prevp=0
for  (i=0;i<=number;i++)
{
	pcrv  =  makepointoncurve(c1,i/number)
	vis(pcrv)
	if  (i  >  0)
	{
		edge1  =  makelineptpt(prevp,pcrv)
		l1  =  makelineptpt(p1,pcrv)
		//vis(l1)
		for(j=1;j<numpoints;j++)
		{
			p2  =  makepointoncurve(l1,j/numpoints)
			dis  =  getdistance(p2,mp)
			c2  =  makecircle(p2,up,dis/10)
			l2  =  makelineptpt(p2,mp)
			//vis(l2)
			vis(c2)
			//vis(p2)
		}
		vis(edge1)
	}
}


```

## Importing an Igs File Once ##

Type the following code in the code editor and see the geometry update as you type.
**when using the getfileonce(), a radiobutton appears at the top of the editor and unchecks itself automatically on the firstuse, now everytime the code evaluates it wont popup the openfile dialog, if you check the radiobutton, it will open the openfile dialog on the next evaluate, if you remove the getfileonce from your code, the radiobutton removes it self from the top of the widget.**

the importigs(filename): command accepts a string with the filename, it returns a TopoDS\_Shape so it can be fed to any function that works on Topology objects.

fitall(): fits the view to all extents.

```

f  =  getfileonce()
face  =  importigs(f)
vis(face)
fitall()

```



## Apply a pattern over an imported igs, the igs must contain a single surface generated in another application ##
it uses the panelize function with the pointer to the user defined feature mypanel accepting the 4 points of the panels as signature, this means the panelize function is expecting a function with 4 parameters, the names of input parameters are not important, just the order. the function is allowed to use global values , for instance the parameter grow is defined outside the mypanel function, and mypanel is allowed to consume the grow parameter within.
```
grow  =  getval(0,1,200)/200
path  =  getfileonce()
surface  =  importigs(path)
panelize(surface,20,20,mypanel)

function  mypanel(p1,p2,p3,p4)
{

l1  =  makelineptpt(p1,p2)
l2  =  makelineptpt(p2,p3)
l3  =  makelineptpt(p3,p4)
l4  =  makelineptpt(p4,p1)
vis(l1)
vis(l2)
vis(l3)
vis(l4)

np1  =  makepointoncurve(l1,0.5)
np2  =  makepointoncurve(l2,0.5)
np3  =  makepointoncurve(l3,0.5)
np4 =  makepointoncurve(l4,0.5)

ratio  =  grow  /4

ml  =  makelineptpt(np1,np3)
top  =  makepointoncurve(ml,0.5-grow)
bottom  =  makepointoncurve(ml,0.5+grow)
//vis(bottom)
//vis(top)
midp  =  makepointoncurve(ml,0.5)
upcurve  =  makepointlist()
upcurve  =  addpointtolist(upcurve,np4)
upcurve  =  addpointtolist(upcurve,top)
upcurve  =  addpointtolist(upcurve,np2)
upcrv  =  makebspline(upcurve)

botcurve  =  makepointlist()
botcurve  =  addpointtolist(botcurve,np2)
botcurve  =  addpointtolist(botcurve,bottom)
botcurve  =  addpointtolist(botcurve,np4)
botcrv  =  makebspline(botcurve)
//vis(upcrv)
//vis(botcrv)
upsrflist  =  makeshapelist()
upsrflist  =  addshapetolist(upsrflist,l1)
upsrflist  =  addshapetolist(upsrflist,upcrv)
upflap  =  makeloft(upsrflist)
//vis(upflap)
botsrflist  =  makeshapelist()
botsrflist  =  addshapetolist(botsrflist,l3)
botsrflist  =  addshapetolist(botsrflist,botcrv)
downflap  =  makeloft(botsrflist)
//vis(downflap)

tl1  =  makelineptpt(p2,midp)
tl2  =  makelineptpt(midp,p3)
tl3  =  makelineptpt(midp,np4)
vis(tl1)
vis(tl2)
vis(tl3)

}

```


> ### Available Commands (working on it, not all functions are working on the sample download) ###

at the moment we have the following commands working in the language:

```

*note: the gui.val#.value command has been deprecated, to link to a slider follow the getval command:
variablename = getval(positioninsliderset,minvalue,maxvalue)
where variable name will be the label assigned in the front-end to the slider and also the code variable in your script. 
you can change the position, minvalue,maxvalue without deleting the slider, it will update the position and min and max value for you.


 *utility functions.
     vis  *(working V000.001)*.
     viscurvature *(working V000.001)*.
     initpart *(working V000.001)*.
     endpart *(working V000.001)*.
     panelize *(working v000.005)*.

     
     listopertation.
     makeshapelist *(working V000.001)*.
     makepointlist *(working V000.005)*
     addshapetolist *(working V000.001)*.
     addpointtolist *(working V000.005)*

       

*points.
     makepoint *(working V000.001)*.
     makepointoncurve *(working v000.005)*.
     makepointmovebyvector *(working v000.005)*.
     makeprojectpointonsurface.
     makeprojectpointoncurve. 
     makecenterofgravitypoint. 
     makepointbetween. 
     makeuvpoint. 
     makeintersectionpoint. 
     makeintersectionpointmulti.
   
*curves.
     makelineptpt *(working V000.001)*.
     makelinenormal.
     makebspline*(working V000.005)*. 
     makeinterpolatespline. 
     makebeziercurve. 
     makecorner. 
     makecircle. 
     makeellipse. 
     makearc. 
     makearcofellipse. 
     makehyberbola. 
     makeparabola.
     makeoffsetcurve. 
 

*mesh.
     makemesh. 
     makecoloredmesh. 

*surfaces.
    makeloft *(working V000.001)*.
  

*vectors.	
    makevector *(working V000.001)*.
    makevectortangenttocurve*(working V000.005)*.
  
*operations.
     movepointtopoint. 
     rotate*(working V000.005)*. 
     mirror. 
     movebyvector. 
     axistoaxis. 

     intersect. 
     split. 
     booleancut. 
     booleancommon. 
     booleanglue. 

*get info.
     getlength. 
     getarea. 
     getmindistance. 
     getmaxdistance. 
     getuvpointonsurface. 
     getbiggestface. 
     getbiggestshell. 
     getbiggestsolid. 

*filesystem operations.
     getfile *(working V000.002)*.
     getfileonce *(working V000.002)*.
     loadtextfile. 
     loadbinaryfile. 
     importigs *(working V000.002)*.
     importstp. 
     importstl. 
     exportigs. 
     exportstp. 
```






## Some Basic Project Stats ##



&lt;wiki:gadget url="http://www.ohloh.net/p/586019/widgets/project\_basic\_stats.xml" height="220" border="1"/&gt;

&lt;wiki:gadget url="http://www.ohloh.net/p/586019/widgets/project\_cocomo.xml" height="240" border="0"/&gt;