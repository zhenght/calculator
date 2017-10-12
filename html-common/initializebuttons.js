"use strict";
var answerIdsPureLatex=[];
var answerMathQuillObjects=[];
var answerMQspanIds=[];
var preferredButtonContainers=[];
var studentScoresInHomePage=[];
//var lastFocus;
var charsToSplit=['x','y'];

function processMathQuillLatex(theText)
{ for (var i=0; i<theText.length; i++)
    if (i+1<theText.length)
    { if ((theText[i]==='_' || theText[i]==='^') && theText[i+1]!=='{')
        theText=theText.slice(0,i+1)+'{'+theText[i+1]+'}'+theText.slice(i+2);
      if (theText[i]==='\\'  && theText[i+1]==='\\')
        theText=theText.slice(0,i+2)+' '+theText.slice(i+2);
    }
  if (charsToSplit!=undefined)
    for (i=0; i<theText.length-1; i++)
    { for (var j=0; j<charsToSplit.length; j++)
        if (theText[i]===charsToSplit[j] &&
            theText[i+1]!==' ' && theText[i+1]!=='\\'
            && theText[i+1]!=='+' && theText[i+1]!=='*'
            && theText[i+1]!=='/' && theText[i+1]!=='-'
            && theText[i+1]!=='='
            )
        { if (theText[i]==='x')
            if (theText.slice(i-5,i+1)==='matrix')
              continue;
          theText=theText.slice(0,i+1)+" "+theText.slice(i+1);
        }
    }
  return theText;
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

function initializeButtonsCommon()
{ ///initializing accordions
  var acc = document.getElementsByClassName("accordion");
  for (var i = 0; i < acc.length; i++)
  { acc[i].onclick = //async
    function()
    { if (this.firstLoad===undefined)
      { this.firstLoad=true;
        var theDeadlines=this.nextElementSibling.getElementsByClassName("modifyDeadlineInput");
        for (var j=0; j<theDeadlines.length; j++)
          $('#'+ theDeadlines[j].id).datepicker();
//        console.log("first run: "+theDeadlines);
        this.nextElementSibling.style.display="inline-block";
        //await sleep(400);
      }
      //this.nextElementSibling.style.transition="0.6s linear";
      this.classList.toggle("active");
      this.nextElementSibling.classList.toggle("show");
    }
  }
//  for (i=0; i<answerIdsPureLatex.length; i++){ 
//    currentButtonPanel=document.getElementById(answerMQspanIds[i]);	
//    document.addEventListener("blur", function(){
//      lastfocus=this;
//    });
//  }
}

var MathQuillCommandButtonCollection=new Object;

function mathQuillCommandButton(inputCommand, inputLabel, additionalStyle, doWriteInsteadOfCmdInput)
{ var commandObject=new Object;
  commandObject.theCommand=inputCommand;
  commandObject.theLabel=inputLabel;
  commandObject.isComposite=false;
  if (typeof(inputCommand)!=="string")
    commandObject.isComposite=true;
  if (!commandObject.isComposite)
    commandObject.id=commandObject.theCommand;
  else
  { commandObject.id="";
    for (var i=0; i<inputCommand.length; i++)
      commandObject.id+=inputCommand[i];
  }
  if (doWriteInsteadOfCmdInput!==undefined)
    commandObject.doWriteInsteadOfCmd=doWriteInsteadOfCmdInput;
  else
    commandObject.doWriteInsteadOfCmd=false;
  commandObject.getButton=function (indexMathField)
  { var resultString="";
    resultString+="<button style='";
    resultString+="width:25; height:21.2; padding:0; ";
    if (additionalStyle!=="" && additionalStyle!==undefined)
      resultString+=additionalStyle;
    resultString+="'";
    var commandObjectIDescaped="";
    for (var i=0; i<commandObject.id.length; i++)
    { commandObjectIDescaped+=commandObject.id[i];
      if (commandObject.id[i]==='\\')
        commandObjectIDescaped+="\\";
    }
    resultString+=" onmousedown=\"MathQuillCommandButtonCollection['" +
    commandObjectIDescaped + "']."+
    "clickFunction(answerMathQuillObjects[" + indexMathField + "]);\">" +
    commandObject.theLabel + "</button>";
    return resultString;
  }
  commandObject.clickFunction = function(currentMathField)
  { if (!commandObject.isComposite)
    { if (!this.doWriteInsteadOfCmd)
        currentMathField.cmd(commandObject.theCommand);
      else
        currentMathField.write(commandObject.theCommand);
    } else
    { for (var i=0; i<commandObject.theCommand.length; i++)
        if (!this.doWriteInsteadOfCmd)
          currentMathField.cmd(commandObject.theCommand[i]);
        else
          currentMathField.write(commandObject.theCommand[i]);
    }
    currentMathField.focus();
    event.preventDefault();
  }
  MathQuillCommandButtonCollection[commandObject.id]=commandObject;
}

mathQuillCommandButton("+","+");
mathQuillCommandButton("-","-");
mathQuillCommandButton("*","*");
mathQuillCommandButton("/","/");
mathQuillCommandButton("sqrt","&#8730;");
mathQuillCommandButton("nthroot","&#8731;");
mathQuillCommandButton("^","^");
mathQuillCommandButton("(","(");
mathQuillCommandButton(")",")");
mathQuillCommandButton(",",",");
mathQuillCommandButton("[","[");
mathQuillCommandButton("]","]");
mathQuillCommandButton("i","i");
mathQuillCommandButton("x","x");
mathQuillCommandButton("y","y");
mathQuillCommandButton("=","=");
mathQuillCommandButton(["log","_"],"log_", "font-size:10px; ");
mathQuillCommandButton("_","_");
mathQuillCommandButton("ln","ln");
mathQuillCommandButton("e","e");


mathQuillCommandButton("sin","sin", "font-size:10px; ");
mathQuillCommandButton("cos","cos", "font-size:10px; ");
mathQuillCommandButton("tan","tan", "font-size:10px; ");
mathQuillCommandButton("cot","cot", "font-size:10px; ");
mathQuillCommandButton("sec","sec", "font-size:10px; ");
mathQuillCommandButton("csc","csc", "font-size:10px; ");

mathQuillCommandButton("emptyset", "&#8709;");
mathQuillCommandButton("alpha", "&alpha;");
mathQuillCommandButton("beta", "&beta;");
mathQuillCommandButton("gamma", "&gamma;");
mathQuillCommandButton("theta", "&theta;");
mathQuillCommandButton("pi", "&pi;");
mathQuillCommandButton("sum", "&#8721;");
mathQuillCommandButton("infty", "&#8734;");
mathQuillCommandButton("!", "!");
mathQuillCommandButton("binom", "binom", "font-size : 7px;");
mathQuillCommandButton("cup", "&#8746;");
mathQuillCommandButton(" or ", "or");
mathQuillCommandButton("in", "&#8712;");
mathQuillCommandButton(["^","circ"], "&#176;");
mathQuillCommandButton("circ", "&#9675;");
mathQuillCommandButton("\\begin{pmatrix} \\\\ \\end{pmatrix}", "2x1","font-size : 7px;", true);
mathQuillCommandButton("\\begin{pmatrix} \\\\ \\\\ \\end{pmatrix}", "3x1", "font-size : 7px;", true);
mathQuillCommandButton("\\begin{pmatrix} & \\\\ & \\end{pmatrix}", "2x2", "font-size : 7px;", true);
mathQuillCommandButton("\\begin{pmatrix} & & \\\\ & & \\\\ & & \\end{pmatrix}", "3x3", "font-size : 7px;", true);

mathQuillCommandButton("\\mathbf{i}", "i", "font-weight: bold");
mathQuillCommandButton("\\mathbf{j}", "j", "font-weight: bold");
mathQuillCommandButton("\\mathbf{k}", "k", "font-weight: bold");

function modifyHeightForTimeout(currentButtonPanel, newHeight)
{ currentButtonPanel.style.maxHeight = newHeight;
  currentButtonPanel.style.height    = newHeight;
}

function initializeOneButtonPanel(IDcurrentButtonPanel, panelIndex, forceShowAll)
{ var currentButtonPanel=document.getElementById(IDcurrentButtonPanel);
  var buttonArray= currentButtonPanel.attributes.buttons.value.toLowerCase().split(/(?:,| )+/);
  //console.log(buttonArray);
  var buttonBindings = [];
  function addBinding(theFun)
  { if (buttonBindings.indexOf(theFun)>-1)
      return;
    buttonBindings.push(theFun);
  }
  function addCommand(theCmd)
  { buttonBindings.push(MathQuillCommandButtonCollection[theCmd].getButton);
  }
  var noOptions=false;
  var includeAll=false;
  if (forceShowAll)
    includeAll=true;
  if (buttonArray.indexOf("all")>-1)
    includeAll=true;
  if (buttonArray.length===0)
    noOptions=true;
  if (buttonArray.length===1)
    if (buttonArray[0]==="")
      noOptions=true;
  if (buttonArray.indexOf("algebra")>-1 || noOptions || includeAll)
  { addCommand("+");
    addCommand("-");
    addCommand("*");
    addCommand("/");

    addCommand("sqrt");
    addCommand("nthroot");
    addCommand("^");
    addCommand("(");
    addCommand(")");
  }
  if (buttonArray.indexOf("trig")>-1 || includeAll)
  { addCommand("sin");
    addCommand("cos");
    addCommand("tan");
    addCommand("cot");
    addCommand("sec");
    addCommand("csc");
  }

  if (buttonArray.indexOf("brackets")>-1 ||
      buttonArray.indexOf("comma")>-1 ||
      buttonArray.indexOf("intervals")>-1 ||
      includeAll)
    addCommand(",");
  if (buttonArray.indexOf("brackets")>-1 ||
      buttonArray.indexOf("intervals")>-1 || includeAll)
  { addCommand("[");
    addCommand("]");
  }
  if (buttonArray.indexOf("complex")>-1 || buttonArray.indexOf("imaginary")>-1 || includeAll)
  { addCommand("i");
  }
  if (buttonArray.indexOf("variables")>-1 || includeAll)
  { addCommand("x");
    addCommand("y");
    addCommand("=");
  }
  if (buttonArray.indexOf("logarithms")>-1 || noOptions || includeAll)
  { addCommand("log_");
    addCommand("_");
    addCommand("ln");
    addCommand("e");
  }
  if (buttonArray.indexOf("infinity")>-1 || buttonArray.indexOf("infty")>-1 ||
      buttonArray.indexOf("\infty")>-1 ||
      includeAll || noOptions)
    addCommand("infty");
  if (buttonArray.indexOf("sum")>-1 ||
      buttonArray.indexOf("series")>-1 || noOptions || includeAll)
  { addCommand("binom");
    addCommand("!");
    addCommand("sum");
  }
  if (noOptions || includeAll)
  { addCommand("circ");
  }
  if (buttonArray.indexOf("interval")>-1 || buttonArray.indexOf("intervals")>-1 ||
      buttonArray.indexOf("or")>-1 || noOptions || includeAll)
    addCommand(" or ");
  if (buttonArray.indexOf("interval")>-1 || buttonArray.indexOf("intervals")>-1 || noOptions || includeAll)
  { addCommand("cup");
    addCommand("in");
    addCommand("emptyset");
  }
  if (buttonArray.indexOf("matrix")>-1 || buttonArray.indexOf("matrices")>-1 || (includeAll && MathQuillHasMatrixSupport))
  { addCommand("\\begin{pmatrix} \\\\ \\end{pmatrix}");
    addCommand("\\begin{pmatrix} \\\\ \\\\ \\end{pmatrix}");
    addCommand("\\begin{pmatrix} & \\\\ & \\end{pmatrix}");
    addCommand("\\begin{pmatrix} & & \\\\ & & \\\\ & & \\end{pmatrix}");
  }
  if (buttonArray.indexOf("angle")>-1 || buttonArray.indexOf("angles")>-1 || noOptions || includeAll)
  { addCommand("pi");
    addCommand("^circ");
    addCommand("alpha");
    addCommand("beta");
    addCommand("gamma");
    addCommand("theta");
  }
  var theContent="<table>";
  var numButtonsPerLine=4;
  for (var j=0; j<buttonBindings.length; j++)
  { if (j%numButtonsPerLine===0)
    { if (j!==0)
        theContent+="</tr>";
      theContent+="<tr>";
    }
    theContent+="<td>"+ buttonBindings[j](panelIndex)+"</td>";
  }
  if (buttonBindings.length>0)
    theContent+="</tr>";
  theContent+="</table>";
  if (!forceShowAll && !includeAll)
    theContent+= "<small><a href=\"#" + IDcurrentButtonPanel + "\" onclick=\"initializeOneButtonPanel('" + IDcurrentButtonPanel + "'," +panelIndex +",true);\">Show all</a></small>";
  else
    theContent+= "<small><a href=\"#" + IDcurrentButtonPanel + "\" onclick=\"initializeOneButtonPanel('" + IDcurrentButtonPanel + "'," +panelIndex +",false);\">Show relevant</a></small>";

  var oldHeight=window.getComputedStyle(currentButtonPanel).height;
  //console.log("oldHeight: " + oldHeight);
  currentButtonPanel.style.maxHeight="";
  currentButtonPanel.style.height="";
  currentButtonPanel.innerHTML=theContent;
  if (oldHeight!==0 && oldHeight!=="0px")
  { var newHeight=window.getComputedStyle(currentButtonPanel).height;
    currentButtonPanel.style.maxHeight = oldHeight ;
    currentButtonPanel.style.height = oldHeight ;
    setTimeout(function(){ modifyHeightForTimeout(currentButtonPanel, newHeight)},0)
  }
  return false;
}

function initializeButtons()
{ for (var i=0; i<answerIdsPureLatex.length; i++)
    initializeOneButtonPanel(preferredButtonContainers[i], i, false);
  initializeButtonsCommon();
}


