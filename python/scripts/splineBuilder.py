import pysplinekernel
import numpy as np
#import matplotlib              # Uncomment for faster version
#matplotlib.use('qt5agg')       # (uncomment alternative builder at the end too)
import matplotlib.pyplot as plt

# -----------------------------------------------------------------------------
# CLASS DEFINITION
# -----------------------------------------------------------------------------
class SplineBuilder:
    # MEMBER FUNCTIONS --------------------------------------------------------
    def __init__(self):
        #
        self.fig = plt.figure()
        self.ax  = self.fig.add_subplot(111)
        self.title = ' ESC      : reset \n 1..9      : set polynomial degree \n SPACE  : toggle editor mode \n ENTER  : toggle control polygon'
        #
        self.reset()
        
    def reset(self):
        self.ax.cla()
        # Spline setup
        self.p = 3
        self.interpolationPoints = [[],[]]
        # Figure setup
        self.ax.set_title(self.title, loc='left', fontsize=14)
        self.fig.tight_layout()
        # Resize plot
        self.ax.set_xlim( left=0, right=1, auto=False )
        self.ax.set_ylim( bottom=0, top=1, auto=False )
        # Figure objects
        self.spline, = self.ax.plot(0,0,' ')
        self.polygon, = self.ax.plot(0,0,' ')
        self.cursor, = self.ax.plot(0,0,' ')
        # Initialize settings
        self.showControlPolygon = False
        self.inEditor = True
        
    def drawCursor(self, xCursor, yCursor):
        # Draw point at cursor location
        if len(xCursor) is not 0:
            self.cursor.remove()
            self.cursor, = self.ax.plot( xCursor[0], yCursor[0], 'b+' )
        
    def drawSpline(self, xCursor, yCursor):
        # Track cursor location
        self.drawCursor(xCursor,yCursor)
        # Check if number of points is sufficient
        if len(self.interpolationPoints[0]) + len(xCursor)<self.p+1:
            return
        # Get spline
        controlPoints, knotVector = pysplinekernel.interpolateWithBSplineCurve( [self.interpolationPoints[0] + xCursor, self.interpolationPoints[1] + yCursor], self.p )
        # Get parameters (10 per segments)
        t = np.linspace(0.0, 1.0, (len(self.interpolationPoints[0])-1)*10 )
        # Remove previous spline and draw new one
        xc, yc = pysplinekernel.evaluate2DCurve( t, controlPoints[0], controlPoints[1], knotVector )
        self.spline.remove()
        self.spline, = self.ax.plot( xc, yc, 'b' )
        # Draw control polygon
        self.drawControlPolygon(controlPoints)
        #Update figure
        self.fig.canvas.draw()
    
    def drawControlPolygon(self, controlPoints):
        if self.showControlPolygon:
            self.polygon.remove()
            self.polygon, = self.ax.plot(controlPoints[0], controlPoints[1],'r')
        
    # EVENT HANDLERS ----------------------------------------------------------
    def onClick(self,event):
        if not self.inEditor:
            return
        # check if within figure
        if event.inaxes != self.spline.axes: 
            return
        # Check if click location on previous point 
        if len(self.interpolationPoints[0]) != 0:
            if event.xdata==self.interpolationPoints[0][-1] and event.ydata==self.interpolationPoints[1][-1]:
                return
        # Append interpolation points
        self.interpolationPoints[0].append(event.xdata)
        self.interpolationPoints[1].append(event.ydata)
        # Draw new point
        self.ax.plot(event.xdata, event.ydata, 'bo')
        self.fig.canvas.draw()
        
    def onMotion(self,event):
        if not self.inEditor:
            return
        # check if within figure
        if event.inaxes != self.spline.axes: 
            return
        # Check if cursor location on previous point
        if len(self.interpolationPoints[0]) != 0:
            if event.xdata==self.interpolationPoints[0][-1] and event.ydata==self.interpolationPoints[1][-1]:
                return
        # Draw spline
        self.drawSpline([event.xdata], [event.ydata])
        self.fig.canvas.draw()
        
    def onKeyPress(self, event):
        # ESC - reset
        if event.key == 'escape':
            self.reset()
        # ENTER - show/hide control polygon
        elif event.key == 'enter':
            if self.showControlPolygon:
                self.polygon.remove()
                self.polygon, = self.ax.plot(0,0,' ')
            self.showControlPolygon = not self.showControlPolygon
            if self.inEditor:
                self.drawSpline([event.xdata],[event.ydata])
            else:
                self.drawSpline([],[])
        # SPACE - stop/start drawing
        elif event.key == ' ':
            self.inEditor = not self.inEditor
            if self.inEditor:
                # Delete temporary interpolation points
                self.interpolationPoints[0] = self.interpolationPoints[0][:-1]
                self.interpolationPoints[1] = self.interpolationPoints[1][:-1]
                # Draw
                self.drawSpline([event.xdata],[event.ydata])
            else:
                # Temporarily append interpolation points
                if event.xdata!=self.interpolationPoints[0][-1] and event.ydata!=self.interpolationPoints[1][-1]:
                    self.interpolationPoints[0].append(event.xdata)
                    self.interpolationPoints[1].append(event.ydata)
                # Draw
                self.drawSpline([],[])
        # NUMBERS - polynomial degree
        elif 49 <= ord(event.key) and ord(event.key) <= 57:
            self.p = int(event.key)
            if self.inEditor:
                self.drawSpline([event.xdata],[event.ydata])
            else:
                self.drawSpline([],[])
        
        
# -----------------------------------------------------------------------------
# END OF CLASS DEFINITION
# -----------------------------------------------------------------------------





# -----------------------------------------------------------------------------
# CLASS DEFINITIONS
# -----------------------------------------------------------------------------
class Splinekernel:
    # Constructor -------------------------------------------------------------
    def __init__(self):
        self.reset()
        
    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        self.polynomialOrder = 3
        self.interpolationPoints = [[],[]]
        self.controlPoints = [[],[]]
        self.knotVector = []
        self.samplePoints = []
        self.curvePoints = [[],[]]
        
    # CALCULATION -------------------------------------------------------------
    def getSamplePoints(self):
        return np.linspace(0.0, 1.0, (len(self.interpolationPoints[0])-1)*10 )
    
    def updateSpline(self, lastPoint=[]):
        if lastPoint != []:
            self.controlPoints, self.knotVector = pysplinekernel.interpolateWithBSplineCurve(
                    [self.interpolationPoints[0]+[lastPoint[0]], self.interpolationPoints[1]+[lastPoint[1]]],
                    self.polynomialOrder)
        else:
            self.controlPoints, self.knotVector = pysplinekernel.interpolateWithBSplineCurve(
                    self.interpolationPoints,
                    self.polynomialOrder)
    
    def updatePoints(self):
        x, y = pysplinekernel.evaluate2DCurveDeBoor(
                self.samplePoints,
                self.controlPoints[0],
                self.controlPoints[1],
                self.knotVector)
        return (x,y)
        
    # SET/GET -----------------------------------------------------------------
    def push(self, point):
        if len(self.interpolationPoints[0]) > 0:
            if point[0] != self.interpolationPoints[0][-1] and point[1] != self.interpolationPoints[1][-1]:
                self.interpolationPoints[0].append(point[0])
                self.interpolationPoints[1].append(point[1])
        else:
            self.interpolationPoints[0].append(point[0])
            self.interpolationPoints[1].append(point[1])
        
    def pop(self):
        if len(self.interpolationPoints[0]) > 0:
            self.interpolationPoints[0] = self.interpolationPoints[0][:-1]
            self.interpolationPoints[1] = self.interpolationPoints[1][:-1]
    
    def getPoints(self,lastPoint=[]):
        if len(self.interpolationPoints[0]) + len(lastPoint)/2 > self.polynomialOrder:
            self.updateSpline(lastPoint)
            self.samplePoints = self.getSamplePoints()
            self.curvePoints = self.updatePoints()
        
    
    
    
class SplineGUI:
    # CONSTRUCTOR -------------------------------------------------------------
    def __init__(self):
        self.fig    = plt.figure()
        self.ax     = self.fig.add_subplot(111)
        self.kernel = Splinekernel()
        self.title  = "Spline GUI"
        self.reset()
    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        # Data
        self.kernel.reset()
        self.cursorPoint = []
        # State
        self.enableMotion   = True
        self.enableClick    = True
        self.enableKeyPress  = True
        # Figure
        self.ax.set_title(self.title, loc='left', fontsize=14)
        self.fig.tight_layout()
        # Axes
        self.ax.set_xlim( left=0, right=1, auto=False )
        self.ax.set_ylim( bottom=0, top=1, auto=False )
        # Reset objects
        self.artists = {
        "spline"        : self.ax.plot(2,2,'b-',linewidth=1),
        "polygon"       : self.ax.plot(2,2,'r-.',linewidth=0.5),
        "cursor"        : self.ax.plot(2,2,'b+'),
        "points"        : self.ax.plot(2,2,'bo')
        }
        self.enableArtists = {
        "spline"        : True,
        "polygon"       : False,
        "cursor"        : True,
        "points"        : True    
        }
        
    # DRAW FUNCTIONS ----------------------------------------------------------
    def draw(self, targets=None, data=None):
        # Hide object if called with no data argument
        if targets!=None and data!=None:
            for target,dat in zip(targets,data):
                if self.enableArtists[target]:
                    self.artists[target][0].set_xdata(dat[0])
                    self.artists[target][0].set_ydata(dat[1])
                else:
                    self.artists[target][0].set_xdata(2)
                    self.artists[target][0].set_ydata(2)
        # Clean background
        self.ax.draw_artist(self.ax.patch)
        # Draw every enabled artist
        for key,artist in self.artists.items():
            if self.enableArtists[key]:
                self.ax.draw_artist(artist[0])
        # Update canvas
        self.fig.canvas.update()
        #self.fig.canvas.flush_events()
        
    def show(self,target,forceDraw=False):
        self.enableArtists[target] = True
        if forceDraw:
            self.draw()
        
    def hide(self,target,forceDraw=False):
        self.enableArtists[target] = False
        if forceDraw:
            self.draw()
            
    def switch(self,target,forceDraw=False):
        if self.enableArtists[target]:
            self.hide(target,forceDraw)
        else:
            self.show(target,forceDraw)
        
    # EVENT HANDLERS ----------------------------------------------------------
    def onMotion(self,event,bypass=False):
        if self.enableMotion or bypass:
            if event.inaxes != None:
                # Re-enable cursor if needed
                if self.enableArtists['cursor'] == False:
                    self.show('cursor',True)
                # Gather cursor data
                self.cursorPoint = [event.xdata,event.ydata]
                # Gather spline data
                self.kernel.getPoints(self.cursorPoint)
                # Draw
                self.draw(['cursor','spline','polygon'],[self.cursorPoint,self.kernel.curvePoints, self.kernel.controlPoints])
            else:
                self.hide('cursor',True)
                
    def onClick(self,event,bypass=False):
        if self.enableClick or bypass:
            if event.inaxes != None:
                # Record interpolation point
                self.kernel.push([event.xdata,event.ydata])
                # Draw interpolation points
                self.draw(['points'],[self.kernel.interpolationPoints])
        
    def onKeyPress(self,event,bypass=False):
        if self.enableKeyPress or bypass:
            # ESCAPE
            if event.key == 'escape':
                self.reset()
            # ENTER
            elif event.key == 'enter':
                self.switch('polygon',False)
                self.draw(['polygon'],[self.kernel.controlPoints])
            # SPACE
            elif event.key == ' ':
                self.enableMotion = not self.enableMotion
                self.enableClick = not self.enableClick
            # NUMBERS
            elif 49 <= ord(event.key) and ord(event.key) <= 57:
                self.kernel.polynomialOrder = int(event.key)
                self.kernel.getPoints(self.cursorPoint)
                self.draw(['cursor','spline','polygon'],[self.cursorPoint,self.kernel.curvePoints, self.kernel.controlPoints])
# -----------------------------------------------------------------------------
# END OF CLASS DEFINITION
# -----------------------------------------------------------------------------






# Instantiate SplineBuilder
builder = SplineBuilder()           # Original
#builder = SplineGUI()              # New (bit faster, needs qt backend)
    
# Define events
motionEvent = builder.ax.figure.canvas.mpl_connect('motion_notify_event', builder.onMotion)
clickEvent = builder.ax.figure.canvas.mpl_connect('button_press_event', builder.onClick)
keyPressEvent = builder.ax.figure.canvas.mpl_connect('key_press_event', builder.onKeyPress)

plt.show()