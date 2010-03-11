#include<sstream>
void static_html1(std::stringstream& output){
output <<" <HTML>\n";
output <<" <HEAD><TITLE>Vector partition</TITLE>\n";
output <<" </HEAD>\n";
output <<" <BODY>\n";
output <<" **********************************\n";
output <<" \n";
output <<" ***NOT FUNCTIONAL***\n";
output <<" \n";
output <<" Testing the infrastructure atm :)\n";
output <<" \n";
output <<" <img src=\"http://vector-partition.jacobs-university.de/jacobs_logo.png\" alt=\"Jacobs University\">\n";
output <<" <FORM method=\"POST\" name=\"formMain\" action=\"./vector_partition_linux_cgi\">\n";
output <<" Dimension(<=3): <input type=\"text\" size =\"1\" name=\"textDim\" onkeyup=\"generatePageFromDimAndNum(this.value,formMain.textNumVectors.value,this.size);\">\n";
output <<" Num Vectors(<=9): <input type=\"text\" size=\"1\" name=\"textNumVectors\" onkeyup=\"generatePageFromDimAndNum(formMain.textDim.value,this.value,this.size);\">\n";
output <<" <input type=\"submit\" name=\"buttonGo\" value=\"Go\">\n";
output <<" </FORM>	\n";
output <<" <div style=\"width:400;height:400;border:solid 1px\" id=\"canvasMain\" onmouseup=\"clickCanvasMain(event.clientX, event.clientY);\"></div>\n";
output <<" <SCRIPT TYPE=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/dojo/1.4/dojo/dojo.xd.js\" djConfig = \"parseOnLoad: true\"></SCRIPT>\n";
output <<" \n";
output <<" <script type=\"text/javascript\">\n";
output <<" //<SCRIPT TYPE=\"text/javascript\" src=\"../dojo.xd.js\" djConfig = \"parseOnLoad: true\">\n";
output <<" 	var oldDimension=-1;\n";
output <<" 	var oldNumVectors=-1;\n";
output <<" 	var NumElements=0;\n";
output <<" 	var node;\n";
output <<" 	var surface;		\n";
output <<" 	var MaxNumCoords=5;		\n";
output <<" 	var Basis= new Array(MaxNumCoords);\n";
output <<" 	var Xshift=150;\n";
output <<" 	var Yshift=250; \n";
output <<" 	var selectedBasisIndex=-1;//-2 = center of coord system is selected\n";
output <<" 	var GraphicsUnit=100;\n";
output <<" 	var clickTolerance=0.1;\n";
output <<" 	function ptsWithinClickTolerance(x1,y1,x2,y2)\n";
output <<" 	{	if (x1-x2>clickTolerance || x2-x1>clickTolerance ||\n";
output <<" 				y1-y2>clickTolerance || y2-y1>clickTolerance )\n";
output <<" 		{ return false;\n";
output <<" 		}\n";
output <<" 		return true;\n";
output <<" 	}\n";
output <<" 	function clickCanvasMain(cx,cy)\n";
output <<" 	{	divPosX=0;\n";
output <<" 		divPosY=0;\n";
output <<" 		thePointer= canvasMain;\n";
output <<" 		while(thePointer)\n";
output <<" 		{ divPosX += thePointer.offsetLeft;\n";
output <<"     	divPosY += thePointer.offsetTop;\n";
output <<"     	thePointer = thePointer.offsetParent;\n";
output <<" 		}\n";
output <<" //		alert(cy-divPosY+document.body.scrollTop);\n";
output <<" 		posx=(cx-divPosX+document.body.scrollLeft-Xshift)/GraphicsUnit;		\n";
output <<" 		posy=(cy-divPosY+document.body.scrollTop-Yshift)/GraphicsUnit;\n";
output <<" 		if (!(selectedBasisIndex==-1))\n";
output <<" 		{	if (selectedBasisIndex==-2)\n";
output <<" 			{	Xshift=(cx-divPosX+document.body.scrollLeft);\n";
output <<" 				Yshift=(cy-divPosY+document.body.scrollTop);\n";
output <<" 			}\n";
output <<" 			else\n";
output <<" 			{	Basis[selectedBasisIndex][0]=posx;\n";
output <<" 				Basis[selectedBasisIndex][1]=posy;		\n";
output <<" 			}\n";
output <<" 			surface.clear();\n";
output <<" 			selectedBasisIndex=-1;\n";
output <<" 			drawOnCanvasMain();\n";
output <<" 			//nothing after this procedure gets executed (dunno why, wtf)\n";
output <<" 		} else\n";
output <<" 		{ if (ptsWithinClickTolerance(posx,posy,0,0))\n";
output <<" 			{ selectedBasisIndex=-2;\n";
output <<" 			}\n";
output <<" 			for (i=0;i<MaxNumCoords;i++)\n";
output <<" 			{ if  (ptsWithinClickTolerance(posx,posy,Basis[i][0],Basis[i][1]))\n";
output <<" 				{ selectedBasisIndex=i;\n";
output <<" 				}\n";
output <<" 			}\n";
output <<" 			//alert(\"selected index\"+selectedBasisIndex);\n";
output <<" 		}\n";
output <<" 	}\n";
output <<" 	function generatePageFromDimAndNum(inputDim,inputNum,posIntextDim)\n";
output <<" 	{ dim = Number(inputDim);\n";
output <<" 		num = Number(inputNum);\n";
output <<" //		alert(\"dim: \"+dim+\" num: \"+num);s\n";
output <<" 		if (dim<1 || dim>3 || num>9 || num<1)\n";
output <<" 		{	return;\n";
output <<" 		}\n";
output <<" 		if ((oldDimension!=dim) || (oldNumVectors!=num))\n";
output <<" 		{	oldDimension=dim;\n";
output <<" 			oldNumVectors=num;\n";
output <<" 			document.formMain.textDim.value=dim;\n";
output <<" 			document.formMain.textNumVectors.value=num;\n";
output <<" 			totalCoords=dim*num+num+1;\n";
output <<" 			if (NumElements!=totalCoords)\n";
output <<" 			{	firstChildinLine=document.formMain.firstChild;\n";
output <<" 				for (i=0;i<4;i++)				\n";
output <<" 				{	firstChildinLine=firstChildinLine.nextSibling;			\n";
output <<" 				}\n";
output <<" 	//			alert(\"Num elements to be killed:\"+ NumElements);			\n";
output <<" 				for(i=0;i<NumElements;i++)\n";
output <<" 				{ nextchild=firstChildinLine.nextSibling;\n";
output <<" 					//alert(firstChildinLine.value);\n";
output <<" 					document.formMain.removeChild(firstChildinLine);\n";
output <<" 					firstChildinLine=nextchild;\n";
output <<" 				}\n";
output <<" 				NumElements=totalCoords;\n";
output <<" 				mytext=document.createElement(\"br\");					\n";
output <<" 				document.formMain.insertBefore(mytext,formMain.buttonGo);				\n";
output <<" 				for (i=0;i<num;i++)\n";
output <<" 				{ for (j=0;j<dim;j++)\n";
output <<" 					{	mytext=document.createElement(\"input\");					\n";
output <<" 						mytext.type=\"text\";\n";
output <<" 						mytext.name= \"textCoord\"+i;\n";
output <<" 						mytext.value=rootsArray[i][j];\n";
output <<" 						mytext.size=1;\n";
output <<" 						document.formMain.insertBefore(mytext,formMain.buttonGo);				\n";
output <<" 					}\n";
output <<" 					mytext=document.createElement(\"br\");					\n";
output <<" 					document.formMain.insertBefore(mytext,formMain.buttonGo);				\n";
output <<" 				}\n";
output <<" 				NumElements=totalCoords;\n";
output <<" 			}			\n";
output <<" 		}\n";
output <<" 	}\n";
output <<" 	for (i=0;i<5;i++)\n";
output <<" 	{ Basis[i]= new Array(2);\n";
output <<" 	}  \n";
output <<" 	Basis[0][0]=1.7; Basis[0][1]=0.8;\n";
output <<" 	Basis[1][0]=0;   Basis[1][1]=-2;\n";
output <<" 	Basis[2][0]=1.5; Basis[2][1]=-1.8;\n";
output <<" 	Basis[3][0]=-0.9; Basis[3][1]=1.6;\n";
output <<" 	Basis[4][0]=1.6; Basis[4][1]=0.9;\n";
output <<" 	function getCoord(thePoint,Coord)\n";
output <<" 	{	result=Number(0);\n";
output <<" 		for (counterI=0;counterI<oldDimension;counterI++)\n";
output <<" 		{	result= result+ thePoint[counterI]*Basis[counterI][Coord];\n";
output <<" 		}\n";
output <<" 		result=Math.floor(result);\n";
output <<" 		return result;\n";
output <<" 	}	\n";
output <<" 	function drawOnCanvasMain()\n";
output <<" 	{//	alert(\"paint\");\n";
output <<" 		for (CounterK=0;CounterK<numdaLines;CounterK++)\n";
output <<" 		{	xCoord1=getCoord(da1[CounterK],0)+Xshift;	\n";
output <<" 			yCoord1=getCoord(da1[CounterK],1)+Yshift; \n";
output <<" 			xCoord2=getCoord(da2[CounterK],0)+Xshift; \n";
output <<" 			yCoord2=getCoord(da2[CounterK],1)+Yshift;\n";
output <<" 			surface.createLine({ x1 : xCoord1, y1 : yCoord1, x2 : xCoord2, y2 : yCoord2 }).setStroke(clrda[CounterK]);\n";
output <<" 		}		\n";
output <<" 		for (CounterK=0;CounterK<numlLines;CounterK++)\n";
output <<" 		{	xCoord1=getCoord(l1[CounterK],0)+Xshift;	\n";
output <<" 			yCoord1=getCoord(l1[CounterK],1)+Yshift; \n";
output <<" 			xCoord2=getCoord(l2[CounterK],0)+Xshift; \n";
output <<" 			yCoord2=getCoord(l2[CounterK],1)+Yshift;\n";
output <<" 			surface.createLine({ x1 : xCoord1, y1 : yCoord1, x2 : xCoord2, y2 : yCoord2 }).setStroke(clrl[CounterK]);\n";
output <<" 		}		\n";
output <<" 	}	\n";
output <<" 	function drawOnCanvasMainPlusInit()\n";
output <<" 	{	node = dojo.byId(canvasMain);\n";
output <<" 		surface = dojox.gfx.createSurface(node, 400, 400);\n";
output <<" 		drawOnCanvasMain();\n";
output <<" 	}\n";
output <<" 	dojo.require(\"dojox.gfx\");\n";
output <<" 	dojo.addOnLoad(drawOnCanvasMainPlusInit);		\n";
output <<" </script>\n";
output <<" \n";
output <<" \n";
output <<" \n";
}
