#First Time Instructions for Users

# What is "OpenShapeFactory": #

OpenShapefactory is a Framework... for developers of c++/QT/OpenCascade and as bonus if you dont know how to use c++/Qt/OpenCascade, a precompiled version is released with a interpreter that lets you use the OpenShapeFactory.

# About #

OpenShapeFactory started as a fork of QtOCC developed by Peter Dolby, I have embedded the QoccViewport and all other QtOCC classes, there have been some minor modification here and there to the original source code to make it work on my computer.

The Main



# Interface: #

http://openshapefactory.googlecode.com/files/scintilla%20qgraphics%202.PNG

## The Application: ##

the openshapefactory is divided literally between a 2d surface left and a 3d surface right. the 2d surface is a diagrammatic panel where widgets are loaded, in the case of the precompiled download, a QScintilla text-editor widget with programmatic access to the openshapefactory is automatically added as the only widget. To the right of the 2d panel you have the 3d view port, any change to the code that results from a visible shape will be immediately visible in the 3d view port.

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

## Menus and Toolbars: ##

![http://openshapefactory.googlecode.com/files/menus%20and%20toolbars.png](http://openshapefactory.googlecode.com/files/menus%20and%20toolbars.png)