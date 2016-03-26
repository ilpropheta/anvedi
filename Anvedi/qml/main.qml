import QtQuick 2.0
import CustomPlot 1.0

Item {
        CustomPlotItem {
 
            id: customPlot
            anchors.fill: parent
			background : "white"
			legend : Legend {
				font : "helvetica,-1,18,5,0,0,0,0,0,0"
			}			
			
			graphs : [
				Graph {
					name : "signal"
					pen : Pen { color : "#787878"; width : 2.0 }
					scatter : ScatterStyle {
						brush : "white"
						type : Scatter.ssCircle
						size : 9
						pen : Pen {	color : "black"; width: 1.5	}					
					}
					xAxis : Axis {
						useDefault : true
						label : Label {
							text : "domain"
						}
					}
				},
				Graph {
					name : "impulses"
					style : Plot.lsImpulse
					pen : Pen { color : "blue"; width : 2.5 }
					scatter : ScatterStyle {
						brush : "gray"
						type : Scatter.ssStar
						size : 12
						pen : Pen {	color : "yellow"; width: 1.5	}					
					}
					yAxis : Axis {
						useDefault : false
						label : Label {
							text : "points"
						}
					}
				}
			]
			
			Timer {
				interval: 300; running: true; repeat: true	
				
				property double xCurr: 0.0
						
				onTriggered: {
					
					/*var newX = [];
					var newY = [];
					for (var i = 0; i < 5; i++) 
					{
						newX[i] = xCurr;
						newY[i] = Math.sin(xCurr*Math.PI/100.0);
						xCurr += i;
					}
					customPlot.addData(0, newX, newY);
					customPlot.addData(1, newX, newY);*/
				}
			}

			Component.onCompleted: {		
				var domain = [];
				var y = [];
				var y2 = [];
				
				for (var i=0; i<20; ++i)
				{
				  domain[i] = i/20.0*10.0;
				  y[i] = Math.cos(domain[i]*0.8+Math.sin(domain[i]*0.16+1.0))*Math.sin(domain[i]*0.54)+1.4;
				  y2[i] = domain[i]*domain[i];
				}
								
				addData(0, domain, y);
				addData(1, domain, y2);
				setYRange(1, {"lo":-10});
			}
        }	
}