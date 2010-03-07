void static_html_header_dump( std::stringstream& output)
{ output << "<HTML>";
  output << "<HEAD><TITLE>Vector partition</TITLE>";
</HEAD>
<BODY>
<img src="../jacobs_logo.png" alt="Jacobs University">
<FORM method="POST" name="formMain" action="../cgi-bin/vector_partition_linux_cgi.cgi">
Dimension(<=3): <input type="text" size ="1" name="textDim" onkeyup="generatePageFromDimAndNum(this.value,formMain.textNumVectors.value,this.size);">
Num Vectors(<=9): <input type="text" size="1" name="textNumVectors" onkeyup="generatePageFromDimAndNum(formMain.textDim.value,this.value,this.size);">
<input type="submit" name="buttonGo" value="Go">
</FORM>
<div style="width:400;height:400;border:solid 1px" id="canvasMain" onmouseup="clickCanvasMain(event.clientX, event.clientY);"></div>
<SCRIPT TYPE="text/javascript" src="../dojo.xd.js" djConfig = "parseOnLoad: true"></SCRIPT>

<script type="text/javascript">
//<SCRIPT TYPE="text/javascript" src="http://ajax.googleapis.com/ajax/libs/dojo/1.4/dojo/dojo.xd.js" djConfig = "parseOnLoad: true">
	var oldDimension=-1;
	var oldNumVectors=-1;
	var NumElements=0;
	var node;
	var surface;
	var MaxNumCoords=5;
	var Basis= new Array(MaxNumCoords);
	var Xshift=150;
	var Yshift=250;
	var selectedBasisIndex=-1;//-2 = center of coord system is selected
	var GraphicsUnit=100;
	var clickTolerance=0.1;
	function ptsWithinClickTolerance(x1,y1,x2,y2)
	{	if (x1-x2>clickTolerance || x2-x1>clickTolerance ||
				y1-y2>clickTolerance || y2-y1>clickTolerance )
		{ return false;
		}
		return true;
	}
	function clickCanvasMain(cx,cy)
	{	divPosX=0;
		divPosY=0;
		thePointer= canvasMain;
		while(thePointer)
		{ divPosX += thePointer.offsetLeft;
    	divPosY += thePointer.offsetTop;
    	thePointer = thePointer.offsetParent;
		}
//		alert(cy-divPosY+document.body.scrollTop);
		posx=(cx-divPosX+document.body.scrollLeft-Xshift)/GraphicsUnit;
		posy=(cy-divPosY+document.body.scrollTop-Yshift)/GraphicsUnit;
		if (!(selectedBasisIndex==-1))
		{	if (selectedBasisIndex==-2)
			{	Xshift=(cx-divPosX+document.body.scrollLeft);
				Yshift=(cy-divPosY+document.body.scrollTop);
			}
			else
			{	Basis[selectedBasisIndex][0]=posx;
				Basis[selectedBasisIndex][1]=posy;
			}
			surface.clear();
			selectedBasisIndex=-1;
			drawOnCanvasMain();
			//nothing after this procedure gets executed (dunno why, wtf)
		} else
		{ if (ptsWithinClickTolerance(posx,posy,0,0))
			{ selectedBasisIndex=-2;
			}
			for (i=0;i<MaxNumCoords;i++)
			{ if  (ptsWithinClickTolerance(posx,posy,Basis[i][0],Basis[i][1]))
				{ selectedBasisIndex=i;
				}
			}
			//alert("selected index"+selectedBasisIndex);
		}
	}
	function generatePageFromDimAndNum(inputDim,inputNum,posIntextDim)
	{ dim = Number(inputDim);
		num = Number(inputNum);
//		alert("dim: "+dim+" num: "+num);s
		if (dim<1 || dim>3 || num>9 || num<1)
		{	return;
		}
		if ((oldDimension!=dim) || (oldNumVectors!=num))
		{	oldDimension=dim;
			oldNumVectors=num;
			document.formMain.textDim.value=dim;
			document.formMain.textNumVectors.value=num;
			totalCoords=dim*num+num+1;
			if (NumElements!=totalCoords)
			{	firstChildinLine=document.formMain.firstChild;
				for (i=0;i<4;i++)
				{	firstChildinLine=firstChildinLine.nextSibling;
				}
	//			alert("Num elements to be killed:"+ NumElements);
				for(i=0;i<NumElements;i++)
				{ nextchild=firstChildinLine.nextSibling;
					//alert(firstChildinLine.value);
					document.formMain.removeChild(firstChildinLine);
					firstChildinLine=nextchild;
				}
				NumElements=totalCoords;
				mytext=document.createElement("br");
				document.formMain.insertBefore(mytext,formMain.buttonGo);
				for (i=0;i<num;i++)
				{ for (j=0;j<dim;j++)
					{	mytext=document.createElement("input");
						mytext.type="text";
						mytext.name= "textCoord"+i;
						mytext.value=rootsArray[i][j];
						mytext.size=1;
						document.formMain.insertBefore(mytext,formMain.buttonGo);
					}
					mytext=document.createElement("br");
					document.formMain.insertBefore(mytext,formMain.buttonGo);
				}
				NumElements=totalCoords;
			}
		}
	}
	for (i=0;i<5;i++)
	{ Basis[i]= new Array(2);
	}
	Basis[0][0]=1.7; Basis[0][1]=0.8;
	Basis[1][0]=0;   Basis[1][1]=-2;
	Basis[2][0]=1.5; Basis[2][1]=-1.8;
	Basis[3][0]=-0.9; Basis[3][1]=1.6;
	Basis[4][0]=1.6; Basis[4][1]=0.9;
	function getCoord(thePoint,Coord)
	{	result=Number(0);
		for (counterI=0;counterI<oldDimension;counterI++)
		{	result= result+ thePoint[counterI]*Basis[counterI][Coord];
		}
		result=Math.floor(result);
		return result;
	}
	function drawOnCanvasMain()
	{//	alert("paint");
		for (CounterK=0;CounterK<numdaLines;CounterK++)
		{	xCoord1=getCoord(da1[CounterK],0)+Xshift;
			yCoord1=getCoord(da1[CounterK],1)+Yshift;
			xCoord2=getCoord(da2[CounterK],0)+Xshift;
			yCoord2=getCoord(da2[CounterK],1)+Yshift;
			surface.createLine({ x1 : xCoord1, y1 : yCoord1, x2 : xCoord2, y2 : yCoord2 }).setStroke(clrda[CounterK]);
		}
		for (CounterK=0;CounterK<numlLines;CounterK++)
		{	xCoord1=getCoord(l1[CounterK],0)+Xshift;
			yCoord1=getCoord(l1[CounterK],1)+Yshift;
			xCoord2=getCoord(l2[CounterK],0)+Xshift;
			yCoord2=getCoord(l2[CounterK],1)+Yshift;
			surface.createLine({ x1 : xCoord1, y1 : yCoord1, x2 : xCoord2, y2 : yCoord2 }).setStroke(clrl[CounterK]);
		}
	}
	function drawOnCanvasMainPlusInit()
	{	node = dojo.byId(canvasMain);
		surface = dojox.gfx.createSurface(node, 400, 400);
		drawOnCanvasMain();
	}
	dojo.require("dojox.gfx");
	dojo.addOnLoad(drawOnCanvasMainPlusInit);
</script>
}

