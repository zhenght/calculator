"use strict";
var calculatorCanvases=new Object;
var firstCriticalRunTimeError="";

function calculatorError(x)
{ if (firstCriticalRunTimeError!="")
    return;  
  firstCriticalRunTimeError=x;
  if (firstCriticalRunTimeError!="")
    alert(firstCriticalRunTimeError+" All further error messages are suppressed.");
}

function vectorScalarVector(s,t)
{ var result=0;
  if (s.length!=t.length)
    calculatorError("Scalar product of vectors of different length: " + s + " and " + t);
  for (var i=0; i<s.length; i++)
    result+=s[i]*t[i];
  return result;
}

function vectorTimesScalar(vector, s)
{ for (var i=0;i<vector.length; i++)
    vector[i]*=s;
}

function vectorPlusVector(left, right)
{ var output=new Array(left.length);
  for (var i=0;i<left.length; i++)
    output[i]=left[i]+right[i];
  return output;
}

function vectorCrossVector(left, right)
{ return [left[1]*right[2]-left[2]*right[1], -left[0]*right[2]+left[2]*right[0], left[0]*right[1]-left[1]*right[0] ];
}

function vectorMinusVector(left, right)
{ var output=new Array(left.length);
  for (var i=0;i<left.length; i++)
    output[i]=left[i]-right[i];
  return output;
}

function vectorAddVectorTimesScalar(output, otherVector, scalar)
{ for (var i=0;i<output.length; i++)
    output[i]+=otherVector[i]*scalar;
}

function vectorLength(vector)
{ return Math.sqrt(vectorScalarVector(vector, vector));
}

function deepCopy(from, to)
{ to = new Array(from.length)
  for (var i=0; i<from.length; i++)
    if (from[i] instanceof Array)
      deepCopy(from[i], to[i]);
    else
      to[i]=from[i];
}

function vectorNormalize(vector)
{ vectorTimesScalar(vector, 1/vectorLength(vector));
}

function Patch(inputBase, inputEdge1, inputEdge2, inputColor)
{ this.base=inputBase;
  this.edge1=inputEdge1;
  this.edge2=inputEdge2;
  this.color=inputColor;
  this.v1=vectorPlusVector(this.base, this.edge1);
  this.v2=vectorPlusVector(this.base, this.edge2);
  this.vEnd=vectorPlusVector(this.v1, this.edge2);
}

function Contour(inputPoints, inputColor)
{ this.thePoints=inputPoints;
  this.color=inputColor;
}

function calculatorGetCanvas(inputCanvas)
{ if (calculatorCanvases[inputCanvas.id]==undefined)
  { calculatorCanvases[inputCanvas.id]=
    { surface: inputCanvas.getContext("2d"),
      theIIIdObjects:
      { thePatches: [],
        theContours: []
      },
      screenBasisUser: [[2,1,0],[0,1,1]],
      screenNormal: [],
      drawPatch: function(base, edge1, edge2, color)
      { this.theIIIdObjects.thePatches.push(new Patch(base, edge1, edge2, color));
      },
      drawLine: function (leftPt, rightPt, inputColor)
      { var newContour;
        newContour.thePoints=[];
        newContour.color=inputColor;
        var numPoints=100;
        var incrementScalar=1/numPoints;
        var incrementVector=vectorTimesScalar( vectorMinusVector(rightPt, leftPt), incrementScalar);
        var currentPoint=leftPt.slice();
        for (var i=0; i<numPoints+1; i++)
        { newContour.thePoints[i]=currentPoint;
          currentPoint=vectorPlusVector(currentPoint, incrementVector);
        }
        this.theIIIdObjects.theContours.push(newContour);
      },
      computePatch: function(thePatch)
      { thePatch.normalScreen1= vectorCrossVector(thePatch.screenNormal, thePatch.edge1);
        thePatch.normalScreen2= vectorCrossVector(thePatch.screenNormal, thePatch.edge2);
        thePatch.normal= vectorCrossVector(thePatch.edge1, thePatch.edge2);
      },
      pointIsBehindPatch: function(thePoint, thePatch)
      { if (vectorScalarVector(vectorMinusVector(thePoint, thePatch.base), thePatch.normalScreen1) *
            vectorScalarVector(vectorMinusVector(thePatch.v2, thePatch.base), thePatch.normalScreen1)
            >=0)
          if (vectorScalarVector(vectorMinusVector(thePoint, thePatch.base), thePatch.normalScreen2) *
              vectorScalarVector(vectorMinusVector(thePatch.v1, thePatch.base), thePatch.normalScreen2)
              >=0)
          { if (vectorScalarVector(vectorMinusVector(thePoint, thePatch.base), thePatch.normal) *
                vectorScalarVector(this.screenNormal, thePatch.normal)>0.000001)
              return true;
          }
        return false;
      },
      pointIsInForeGround: function(thePoint, containerPatch)
      { var thePatches=this.theIIIdObjects.thePatches;
        for (var i=0; i<thePatches.length; i++)
        { if (containerPatch===i)
            continue;
          if (this.pointIsBehindPatch(thePoint, thePatches[i]))
            return false;
        }
        return true;
      },
      paintOneContour: function(theContour)
      { if (theContour.thePoints.length<2)
          return;
        var theSurface=this.surface;
        var thePts=theContour.thePoints;
        theSurface.beginPath();
        theSurface.strokeStyle=theContour.color;
        theSurface.lineWidth=1;
        var currentPt=this.getScreenCoordinates(thePts[0]);
        theSurface.moveTo(currentPt[0], currentPt[1]);
        var oldIsInForeGround=this.pointIsInForeGround(theContour.thePoints[0],-1);
        var dashIsOn=false;
        for (var i=1; i<thePts.length; i++)
        { var newIsInForeground=this.pointIsInForeGround(theContour.thePoints[i],-1);
          if (!newIsInForeground && !oldIsInForeGround && !dashIsOn)
          { if (i>1)
            { theSurface.stroke();
              theSurface.beginPath();
              theSurface.moveTo(currentPt[0], currentPt[1]);
            }
            theSurface.setLineDash([4,4]);
            dashIsOn=true;
          } else if (dashIsOn && newIsInForeground)
          { theSurface.stroke();
            theSurface.beginPath();
            theSurface.moveTo(currentPt[0], currentPt[1]);
            dashIsOn=false;
          }
          currentPt=this.getScreenCoordinates(thePts[i]);
          theSurface.lineTo(currentPt[0], currentPt[1]);
          oldIsInForeGround=newIsInForeground;
        }
        theSurface.stroke();
      },
      paintOnePatch: function(thePatch)
      { var theSurface=this.surface;
        theSurface.fillStyle=thePatch.color;
        theSurface.beginPath();
        var
        theCoords=this.getScreenCoordinates(thePatch.base);
        theSurface.moveTo(theCoords[0], theCoords[1]);
        theCoords=this.getScreenCoordinates(thePatch.v1);
        theSurface.lineTo(theCoords[0], theCoords[1]);
        theCoords=this.getScreenCoordinates(thePatch.vEnd);
        theSurface.lineTo(theCoords[0], theCoords[1]);
        theCoords=this.getScreenCoordinates(thePatch.v2);
        theSurface.lineTo(theCoords[0], theCoords[1]);
        theCoords=this.getScreenCoordinates(thePatch.base);
        theSurface.lineTo(theCoords[0], theCoords[1]);
        theSurface.fill();
//        theSurface.stroke();
      },
      accountOnePatch: function(patchIndex)
      { var thePatch=this.theIIIdObjects.thePatches(patchIndex);
        var ptOne=thePatch.base;
        var ptTwo=thePatch.base;
      },
      computeBuffers: function()
      { var thePatches=this.theIIIdObjects.thePatches;
        for (var i=0; i<this.zBuffer.length; i++)
          for (var j=0; j<this.zBuffer.length; j++)
            this.zBuffer[i][j]=[];
        for (var i=0; i<thePatches.length; i++)
          this.accountOnePatch(i);
      },
      redraw: function()
      { var theContours=this.theIIIdObjects.theContours;
        var thePatches=this.theIIIdObjects.thePatches;
        var theSurface=this.surface;
        //this.drawZbuffer();
        //this.computeBuffers();
        for (var i=0; i<thePatches.length; i++)
          this.computePatch(thePatches[i]);
        for (var i=0; i<thePatches.length; i++)
          this.paintOnePatch(thePatches[i]);
        for (var i=0; i<theContours.length; i++)
          this.paintOneContour(theContours[i]);
      },
      allocateZbuffer: function()
      { if (this.zBufferRowCount<1)
          this.zBufferRowCount=1;
        if (this.zBufferColCount<1)
          this.zBufferColCount=1;
        if (this.zBuffer.length<this.zBufferRowCount)
        { this.zBuffer=new Array(this.zBufferRowCount);
          for (var i=0; i< this.zBufferRowCount; i++)
          { this.zBuffer[i]= new Array(this.zBufferColCount);
            for (var j=0; j< this.zBufferColCount; j++)
              this.zBuffer[i][j]=[];
          }
        }
      },
      drawZbuffer: function()
      { this.allocateZbuffer();
        var theSurface=this.surface;
        theSurface.strokeStyle="gray";
        for (var i=0; i<this.zBuffer.length+1; i++)
        { theSurface.beginPath();
          theSurface.moveTo(0,         i*this.height/this.zBuffer.length);
          theSurface.lineTo(this.width,i*this.height/this.zBuffer.length);
          theSurface.stroke();
        }
        for (var i=0; i<this.zBuffer[0].length+1; i++)
        { theSurface.beginPath();
          theSurface.moveTo(i*this.width/this.zBuffer[0].length, 0);
          theSurface.lineTo(i*this.width/this.zBuffer[0].length, this.width);
          theSurface.stroke();
        }
      },
      screenBasisOrthonormal: [],
      computeBasis: function ()
      { //if (this.screenBasisOrthonormal.length<2)
        //  this.screenBasisOrthonormal.length=2;
        this.screenBasisOrthonormal[0]=this.screenBasisUser[0].slice();
        this.screenBasisOrthonormal[1]=this.screenBasisUser[1].slice();
        var e1=this.screenBasisOrthonormal[0];
        var e2=this.screenBasisOrthonormal[1];
        vectorNormalize(e1);
        vectorAddVectorTimesScalar(e2, e1, - vectorScalarVector(e1,e2));
        vectorNormalize(e2);
        this.screenNormal=vectorCrossVector(e1, e2);
      },
      init: function()
      { this.computeBasis();
        if (this.zBuffer.length==0)
          this.allocateZbuffer();
      },
      getScreenCoordinates: function(vector)
      { return [this.scale*vectorScalarVector(vector, this.screenBasisOrthonormal[0])+this.centerX,
           (-1)*this.scale*vectorScalarVector(vector, this.screenBasisOrthonormal[1])+this.centerY];
      },
      zBufferColCount: 50,
      zBufferRowCount: 50,
      zBuffer: [],
      width: inputCanvas.width,
      height: inputCanvas.height,
      centerX: inputCanvas.width/2,
      centerY: inputCanvas.height/2,
      scale: 50
    };
  }
  return calculatorCanvases[inputCanvas.id];
}

function calculatorCanvasMouseMoveRedraw(inputCanvas, x, y)
{ var theCanvas=calculatorGetCanvas(inputCanvas);
  theCanvas.init();
  if (theCanvas.theIIIdObjects.theLines.length==0)
  { theCanvas.drawLine([-1,0,0],[1,0,0], 'black');
    theCanvas.drawLine([0,-1,0],[0,1,0], 'black');
    theCanvas.drawLine([0,0,-1],[0,0,1], 'black');
    theCanvas.drawLine([0,0,0] ,[1,2,0], 'red');
    theCanvas.drawLine([0,0,0] ,[-2,0,1], 'blue');
    theCanvas.drawPatch([0,0,0], [1,1,0], [1,1,1], 'cyan');
    //console.log(theCanvas.theIIIdObjects.thePatches);
   }
   theCanvas.redraw();
}

function calculatorCanvasMouseWheel(inputCanvas, event)
{
}

function calculatorCanvasClick(inputCanvas, x,y)
{
}

function calculatorCanvasMouseUp(inputCanvas)
{
}
