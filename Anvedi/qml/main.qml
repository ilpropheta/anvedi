import QtQuick 2.0
import CustomPlot 1.0

Item {	
		CustomPlotItem {
 
            id: customPlot
            anchors.fill: parent
			background : "white"
					
			graphs : [
				Graph {
					name : "sin"
					pen : Pen { color : "blue"; width : 2.0 }
					scatter : ScatterStyle {
						brush : "white"
						type : Scatter.ssCircle
						size : 9
						pen : Pen {	color : "black"; width: 1.5	}					
					}
					xAxis : Axis {
						useDefault : true
						visible : false
						label : Label {
							text : "domain"
							font : "helvetica,-1,30,5,0,0,0,0,0,0"
							color: "blue"
						}
					}
					yAxis : Axis
					{
						label : Label {						
							text : "sin"
							font : "helvetica,-1,30,5,0,0,0,0,0,0"
							color : "red"
						}
					}
				},
				Graph {
					name : "parabola"
					pen : Pen { color : "red"; width : 2.0 }
					xAxis : Axis {
						useDefault : true
					}
					yAxis : Axis
					{
						useDefault : false
					}
				}
			]
			
			// Timer {
				// interval: 150; running: true; repeat: true	
				
				// property double xCurr: 0.0
						
				// onTriggered: {
					
					// var newX = [];
					// var newY = [];
					// for (var i = 0; i < 5; i++) 
					// {
						// newX[i] = xCurr;
						// newY[i] = Math.sin(xCurr*Math.PI/100.0);
						// xCurr += i;
					// }
					// customPlot.addData(0, newX, newY);
					// customPlot.addData(1, newX, newY);
				// }
			// }

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