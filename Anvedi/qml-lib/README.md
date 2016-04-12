## QCustomPlot QML

[QCustomPlot](http://qcustomplot.com) can be wrapped to be used into QML snippets. Inheriting from [QQuickPaintedItem](http://doc.qt.io/qt-5/qquickpainteditem.html) is a good compromise.

I also wrapped some QCustomPlot components so they can be instantiated in QML code. This way one can create graphs easily in QML code. This is just an experimental porting, use it judiciously.

### Example

Just create a Qt project depending on QCustomPlot (you can also add .h and .cpp files directly into your project) and on these Qt modules:
* Core
* GUI
* Print Support (needed by QCustomPlot)
* QML
* Quick
* Widgets

Here is a possible main file:

```cpp
#include <QtWidgets/QApplication>
#include <QtQuick/QQuickView>
#include "../Anvedi/qml-lib/RegisterAll.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QmlModulesInstaller::Install();
	QQuickView view(QUrl("../Anvedi/qml/main.qml")); // qml to load
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.resize(800, 600);
	view.show();
	return a.exec();
}
```

QmlModulesInstaller::Install() will register all implemented types.

#### A QML file

Once you instantiate the main component (*CustomPlotItem*), you can customize it a bit:

```css
import QtQuick 2.0
import CustomPlot 1.0

Item {	
		CustomPlotItem {
		  id : customPlot
		  background : "white"
		  legend : Legend {
		    font : "helvetica,-1,18,5,0,0,0,0,0,0"
		  }
		...
```

*Legend* is a custom type (it's pretty much incomplete, but it just to give you an idea).

CustomPlotItem has a property called *graphs* that is an array of *Graph*:

```css
graphs : [
				Graph {
					name : "parabola"
					pen : Pen { 
					  color : "blue"
					  width : 2.0 
					}
					scatter : ScatterStyle {
						brush : "white"
						type : Scatter.ssCircle
						size : 9
						pen : Pen {
						  color : "black"
						  width: 1.5	
						}					
					}
					xAxis : Axis {
						useDefault : true
						label : Label {
							text : "domain"
						}
					}
					yAxis : Axis {
						useDefault : true
						label : Label {
							text : "parabola"
						}
					}
				},
				...
```

As you see I wrapped several properties to customize a graph. Obviously this porting is really incomplete, but it should be simple - and long - to implement missing things.

Adding data and setting range can be done via Javascript, for example in the [OnCompleted](http://doc.qt.io/qt-5/qml-qtqml-component.html#completed-signal) signal:

```js
Component.onCompleted: {		
				var domain = [];
				var y = [];
				
				for (var i=0; i<20; ++i)
				{
				  domain[i] = i;
				  y[i] = i*i;
				}
								
				addData(0, domain, y); // adding data to 0th graph
				setYRange(0, [-10, 10]); // setting range of 0th graph
			}
```



QCustomPlot's addData function is provided also to simulate real-time behavior:

```js
Timer {
				interval: 300; running: true; repeat: true	
				
				property double xCurr: 0.0
						
				onTriggered: {
					
					var newX = [];
					var newY = [];
					for (var i = 0; i < 5; i++) 
					{
						newX[i] = xCurr;
						newY[i] = Math.sin(xCurr*Math.PI/100.0);
						xCurr += i;
					}
					customPlot.addData(0, newX, newY);
					customPlot.addData(1, newX, newY);
				}
			}
```
