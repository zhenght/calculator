/***************************************************************
 * Name:      wxParserMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    todor milev (todor.milev@gmail.com)
 * Created:   2011-09-13
 * Copyright: todor milev ()
 * License: use however you wish
 **************************************************************/

#include "wxParserMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(wxParserFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxParserFrame)
const long wxParserFrame::ID_CHOICE1 = wxNewId();
const long wxParserFrame::ID_SPINCTRL1 = wxNewId();
const long wxParserFrame::ID_STATICTEXT1 = wxNewId();
const long wxParserFrame::ID_SPINCTRL2 = wxNewId();
const long wxParserFrame::ID_TEXTCTRL1 = wxNewId();
const long wxParserFrame::ID_BUTTON1 = wxNewId();
const long wxParserFrame::ID_BUTTON2 = wxNewId();
const long wxParserFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxParserFrame,wxFrame)
    //(*EventTable(wxParserFrame)
    //*)
END_EVENT_TABLE()

class WorkThreadClass
{
public:
#ifdef WIN32
	HANDLE ComputationalThread;
#else
	pthread_t ComputationalThreadLinux;
#endif
};

wxParserFrame* theMainWindow;
WorkThreadClass theComputationalThread;
Parser theParser;
GlobalVariables theGlobalVariables;
extern GeneralizedVermaModuleCharacters tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis;

void EnsureBitmapsSuffice()
{ AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  int oldSize=theMainWindow->theBitmapList.size;
  int currentPhysicalFrame=theOps.GetNumPhysicalFramesNoStillFrame();
  if (oldSize<=currentPhysicalFrame)
  { theMainWindow->theBitmapList.SetSize(currentPhysicalFrame+50);
    for (int i=oldSize; i<theMainWindow->theBitmapList.size; i++)
      theMainWindow->theBitmapList[i]=new wxBitmap(theMainWindow->bitmapW, theMainWindow->bitmapW);
  }
}


void drawtext(double X1, double Y1, const char* theText, int length, int ColorIndex, int fontSize)
{ AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  EnsureBitmapsSuffice();
//  if (!theMainWindow->flagUseBitmapBufferForDrawing)
  { wxMemoryDC dc;
    dc.SelectObject(*theMainWindow->theBitmapList[theOps.GetIndexCurrentPhysicalFrame()]);
#ifndef WIN32
//wtf???? I thought wxWidgets was cross-platform???????? why the f it says "error C2039: 'DrawTextA': Ist kein Element von 'wxMemoryDC'"?
    dc.DrawText(wxString(theText, wxConvUTF8), X1, Y1);
#else
    wxString tempS(theText, wxConvUTF8);
    theMainWindow->theStatus->TextCtrlStatusString->AppendText(wxT("\n"));
    theMainWindow->theStatus->TextCtrlStatusString->AppendText(tempS);
#endif

    dc.SelectObject(wxNullBitmap);
  }
//  else
//  { wxPaintDC dc(theMainWindow->theDrawPanel);
//    dc.DrawText(wxString(theText, wxConvUTF8), X1, Y1);
//  }//  wxPen tempPen;
//  tempPen.SetColour((ColorIndex/(256*256))%256, (ColorIndex/256)%256, ColorIndex%256);
//  dc.SetPen(tempPen);
}

void drawCircle(double X1, double Y1, double radius, unsigned long thePenStyle, int ColorIndex)
{ //wxWindowDC dc(theMainWindow->theDrawPanel);//->Panel1);
  AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  EnsureBitmapsSuffice();
  wxPen tempPen;
  switch (thePenStyle)
  { case DrawingVariables::PenStyleNormal:
      tempPen.SetStyle(::wxSOLID);
      break;
		case DrawingVariables::PenStyleDashed:
      tempPen.SetStyle(::wxSHORT_DASH);
      break;
		case DrawingVariables::PenStyleDotted:
      tempPen.SetStyle(::wxDOT);
      break;
		default:
      return;
  }
  tempPen.SetColour((ColorIndex/(256*256))%256, (ColorIndex/256)%256, ColorIndex%256);
  wxMemoryDC dc;
  dc.SelectObject(*theMainWindow->theBitmapList[theOps.GetIndexCurrentPhysicalFrame()]);
  dc.SetPen(tempPen);
  dc.DrawCircle((int)X1, (int)Y1, radius);
  dc.SelectObject(wxNullBitmap);
}

void drawline(double X1, double Y1, double X2, double Y2, unsigned long thePenStyle, int ColorIndex)
{ //wxWindowDC dc(theMainWindow->theDrawPanel);//->Panel1);
  AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  EnsureBitmapsSuffice();
  wxPen tempPen;
  switch (thePenStyle)
  { case DrawingVariables::PenStyleNormal:
      tempPen.SetStyle(::wxSOLID);
      break;
		case DrawingVariables::PenStyleDashed:
      tempPen.SetStyle(::wxSHORT_DASH);
      break;
		case DrawingVariables::PenStyleDotted:
      tempPen.SetStyle(::wxDOT);
      break;
		default:
      return;
  }
  tempPen.SetColour((ColorIndex/(256*256))%256, (ColorIndex/256)%256, ColorIndex%256);
  wxMemoryDC dc(*theMainWindow->theBitmapList[theOps.GetIndexCurrentPhysicalFrame()]);
  dc.SetPen(tempPen);
  dc.DrawLine((int)X1, (int)Y1, (int) X2, (int) Y2);
  dc.SelectObject(wxNullBitmap);
}

void drawClearScreen()
{ AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  EnsureBitmapsSuffice();
  wxMemoryDC dc;
  dc.SetBackground(wxBrush (wxColour(255,255,255),wxSOLID));
  dc.SelectObject(*theMainWindow->theBitmapList[theOps.GetIndexCurrentPhysicalFrame()]);
  dc.Clear();
  dc.SelectObject(wxNullBitmap);
#ifdef WIN32
  theMainWindow->theStatus->TextCtrlStatusString->SetValue(wxT(""));
#endif

}

void FeedDataToIndicatorWindowWX(IndicatorWindowVariables& output)
{ if (theMainWindow->mutexRuN.isLockedUnsafeUseForWINguiOnly())
    return;
  theMainWindow->mutexRuN.LockMe();
  std::stringstream out, out2;
  for (int i=0; i<output.ProgressReportStrings.size; i++)
    out << output.ProgressReportStrings[i] << "\n";
  theMainWindow->ProgressReportString=out.str();
  theMainWindow->StatusString=output.StatusString1;
  wxPostEvent(theMainWindow->GetEventHandler(), theMainWindow->eventProgressReport);
  theMainWindow->mutexRuN.UnlockMe();
}

wxParserFrame::wxParserFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxParserFrame)
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer3;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    this->TimerReady=false;
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    Choice1 = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->Append(_("A"));
    Choice1->Append(_("B"));
    Choice1->Append(_("C"));
    Choice1->Append(_("D"));
    Choice1->Append(_("E"));
    Choice1->SetSelection( Choice1->Append(_("F")) );
    Choice1->Append(_("G"));
    BoxSizer3->Add(Choice1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl1 = new wxSpinCtrl(this, ID_SPINCTRL1, _T("4"), wxDefaultPosition, wxSize(36,27), 0, 2, 8, 4, _T("ID_SPINCTRL1"));
    SpinCtrl1->SetValue(_T("4"));
    BoxSizer3->Add(SpinCtrl1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("frame:"), wxDefaultPosition, wxSize(42,17), 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl2 = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxSize(54,27), 0, 0, 5000, 0, _T("ID_SPINCTRL2"));
    SpinCtrl2->SetValue(_T("0"));
    BoxSizer3->Add(SpinCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer3, 0, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("animateRootSystemDefault(4,6,1)+animatePause(380)+animateRootSystemBlueDot(0,2,1, (1,0))+animatePause(49)\n    +animateRootSystemBlueDot(0,2,1, (1,1))+animatePause(19)+animateRootSystemBlueDot(0,2,1, (0,1))+animatePause(19)\n+animateRootSystemBlueDot(0,2,1, (-1,0))+animatePause(19)\n+animateRootSystemBlueDot(0,2,1, (-1,-1))\n    +animatePause(19)+animateRootSystemBlueDot(0,2,1, (0,-1))\n    +animatePause(19)+animateRootSystemDefault(0,2,1)+animatePause(79)+animateRootSystemDefault(1,3,100)+animatePause(20)\n    +animateRootSystemDefault(2,4,100)+animatePause(20)+animateRootSystemDefault(3,5,1)+animatePause(79)+animateRootSystemDefault(6,2,1)\n    +animatePause(29)+animateRootSystemDefault(5,4,1)+animatePause(59)+animateRootSystemDefault(4,6,1)+animatePause(59)\n    +animateRootSystemDefault(4,7,1)+animatePause(59)+animateRootSystemDefault(4,8,1)+animatePause(59)+animateRootSystemDefault(6,2,1)\n    +animatePause(99)+animateRootSystemDefault(4,8,1)+animatePause(119)+animateRootSystemDefault(4,7,1)+animatePause(119)\n    +animateRootSystemDefault(5,4,100)+animatePause(50)+animateRootSystemDefault(4,6,100)+animatePause(230)"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    BoxSizer1->Add(TextCtrl1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("Go"), wxDefaultPosition, wxSize(137,29), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(this, ID_BUTTON2, _("Pause Animation"), wxDefaultPosition, wxSize(155,29), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(100, false);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&wxParserFrame::OnChoice1Select);
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&wxParserFrame::OnSpinCtrl1Change);
    Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&wxParserFrame::OnSpinCtrl2Change);
    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&wxParserFrame::OnButton1Click);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxParserFrame::OnButton1Click);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxParserFrame::OnButton2Click);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&wxParserFrame::OnTimer1Trigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxParserFrame::OnClose);
    //*)
    this->theDrawPanel= new wxDrawPanel(this);
    this->theParserOutput= new wxParserOutput(this);
    this->theStatus= new wxStatus(this);
    this->thePNGdisplay= new wxPNGdisplay(this);
    this->Timer1.Stop();
    this->TimerReady=true;

    this->Connect(id, wxEVT_ComputationOverMakeReport, (wxObjectEventFunction)&wxParserFrame::OnComputationOver);
    this->Connect(id, wxEVT_ComputationProgressReport, (wxObjectEventFunction)&wxParserFrame::OnProgressReport);
    this->eventComputationOver.SetId(this->GetId());
    this->eventComputationOver.SetEventObject(this);
    this->eventComputationOver.SetEventType(wxEVT_ComputationOverMakeReport);
    this->eventProgressReport.SetId(this->GetId());
    this->eventProgressReport.SetEventObject(this);
    this->eventProgressReport.SetEventType(wxEVT_ComputationProgressReport);
    theGlobalVariables.SetFeedDataToIndicatorWindowDefault(&FeedDataToIndicatorWindowWX);
    theGlobalVariables.theDrawingVariables.SetDrawLineFunction(&drawline);
    theGlobalVariables.theDrawingVariables.SetDrawCircleFunction(&drawCircle);
    theGlobalVariables.theDrawingVariables.SetDrawClearFunction(&drawClearScreen);
    theGlobalVariables.theDrawingVariables.SetDrawTextFunction(&drawtext);
    theParser.DefaultWeylRank=4;
    theParser.DefaultWeylLetter='F';

    this->thePath=std::string(wxStandardPaths::Get().GetExecutablePath().mb_str());
    for (unsigned i=this->thePath.size()-1; i>=0; i--)
    { if (this->thePath[i]=='/' || this->thePath[i]=='\\')
        break;
      this->thePath.resize(this->thePath.size()-1);
    }
    this->theSettingsFileName=this->thePath+"Settings.txt";
    this->ReadSettings();
    this->scalePNG=1;
    theMainWindow=this;
    this->theStatus->Show();
    this->theParserOutput->Show();
    this->theDrawPanel->Show();
    this->thePNGdisplay->Show();
    this->thePNGdisplay->Refresh();
//    this->theDrawPanel->GetSize(&this->bitmapW, &this->bitmapH);
    this->indexCurrentPng=-1;
    this->bitmapH=600;
    this->bitmapW=600;
    this->theBitmapList.SetSize(1);
    this->theBitmapList[0]= new wxBitmap(this->bitmapW, this->bitmapH);
  wxMemoryDC tmpDC;
    tmpDC.SelectObject(*this->theBitmapList[0]);
    tmpDC.SetBackground(wxBrush (wxColour(255,255,255),wxSOLID));
    tmpDC.Clear();
    tmpDC.SelectObject(wxNullBitmap);
    this->Quitting=false;
}

wxParserFrame::~wxParserFrame()
{ this->Quitting=true;
  if (this->theStatus!=0)
    this->WriteSettings();
  //std::exit(0);
  this->theStatus=0;
  this->theParserOutput=0;
  this->theDrawPanel=0;
}

void wxParserFrame::OnQuit(wxCommandEvent& event)
{ this->Close();
}

void wxParserFrame::OnClose(wxCloseEvent& event)
{ this->Destroy();
}

void wxDrawPanel::OnPaint(wxPaintEvent& event)
{ wxPaintDC dc(this);
  if (theMainWindow->Quitting)
    return;
  theMainWindow->mutexRuN.LockMe();
  EnsureBitmapsSuffice();
  AnimationBuffer& theOps= theParser.theValue.theAnimation.GetElement();
  int theIndex=theOps.GetIndexCurrentPhysicalFrame();
  assert(theIndex>=0 && theIndex<theMainWindow->theBitmapList.size);
  wxBitmap& theBM=*theMainWindow->theBitmapList[theIndex];
  dc.DrawBitmap(theBM, 0, 0);
  theMainWindow->mutexRuN.UnlockMe();
}

void wxDrawPanel::OnPanel1LeftDown(wxMouseEvent& event)
{ DrawOperations& theOps= theParser.theValue.theAnimation.GetElement().GetCurrentDrawOps();
  if (theMainWindow->GetTimer().IsRunning())
  { theMainWindow->GetTimer().Stop();
    theMainWindow->Button2->SetLabel(wxT("Continue animation"));
  }
  theOps.click(event.GetX(), event.GetY());
}

void wxDrawPanel::OnPanel1MouseWheel(wxMouseEvent& event)
{ // scroll in drop down list using mouse wheel
  if (theMainWindow==0)
    return;
  int rot = event.GetWheelRotation()/event.GetWheelDelta();
//  theGlobalVariables.theDrawingVariables.LockedWhileDrawing.LockMe();
  AnimationBuffer& theAniBuffer=theParser.theValue.theAnimation.GetElement();
  DrawOperations& theOps=theAniBuffer.GetCurrentDrawOps();

  theOps.GraphicsUnit[theOps.SelectedPlane]+=rot*5;
  theGlobalVariables.theDrawingVariables.drawBufferNoIniT(theOps);
//  wxMemoryDC dc(*theMainWindow->theBitmapList[theAniBuffer.GetIndexCurrentPhysicalFrame()]);
//#ifdef win32
  wxClientDC
//#else
//  wxPaintDC
//#endif
  otherDC(theMainWindow->theDrawPanel);
  otherDC.DrawBitmap(*theMainWindow->theBitmapList[theAniBuffer.GetIndexCurrentPhysicalFrame()],0,0);
//  theGlobalVariables.theDrawingVariables.LockedWhileDrawing.UnlockMe();
  //wxPaintDC dc(this);
  //dc.SetBackground(theMainWindow->GetBackgroundColour());
  //dc.DrawRectangle(wxPoint(0,0), this->GetSize());
  //theGlobalVariables.DrawBufferNoInit();
  this->Refresh();
}

void wxDrawPanel::OnPanel1MouseMove(wxMouseEvent& event)
{ AnimationBuffer& theAniBuffer=theParser.theValue.theAnimation.GetElement();
  DrawOperations& theOps= theAniBuffer.GetCurrentDrawOps();
  if (theOps.SelectedCircleMinus2noneMinus1Center==-2)
    return;
//  theMainWindow->flagUseBitmapBufferForDrawing=true;
//  theGlobalVariables.theDrawingVariables.LockedWhileDrawing.LockMe();
//  theMainWindow->flagUseBitmapBufferForDrawing=false;
  if (theOps.mouseMoveRedraw(event.GetX(), event.GetY()))
  { theGlobalVariables.theDrawingVariables.drawBufferNoIniT(theOps);
  }
//  wxMemoryDC dc(*theMainWindow->theBitmapList[theAniBuffer.GetIndexCurrentPhysicalFrame()]);
//#ifdef win32
  wxClientDC
//#else
//  wxPaintDC
//#endif
  otherDC(theMainWindow->theDrawPanel);
  otherDC.DrawBitmap(*theMainWindow->theBitmapList[theAniBuffer.GetIndexCurrentPhysicalFrame()],0,0);
  //theMainWindow->theDrawPanel->Refresh();
//  dc.Blit(0,0, theMainWindow->bitmapW, theMainWindow->bitmapH, &otherDC,0,0 );
//  theMainWindow->flagUseBitmapBufferForDrawing=false;
//  theGlobalVariables.theDrawingVariables.LockedWhileDrawing.UnlockMe();
//  theMainWindow->theStatus->TextCtrlStatusString->SetValue
//  (wxString(theOps.DebugString.c_str(), wxConvUTF8));
 // this->Refresh();
}

void wxDrawPanel::OnPanel1LeftUp(wxMouseEvent& event)
{ DrawOperations& theOps= theParser.theValue.theAnimation.GetElement().GetCurrentDrawOps();
  theOps.SelectedCircleMinus2noneMinus1Center=-2;
}

void wxParserFrame::OnProgressReport(wxCommandEvent& ev)
{ if (this->mutexRuN.isLockedUnsafeUseForWINguiOnly())
    return;
  this->mutexRuN.LockMe();
  this->theStatus->TextCtrlProgressString->SetValue(wxString(theMainWindow->ProgressReportString.c_str(), wxConvUTF8));
  this->theStatus->TextCtrlStatusString->SetValue(wxString(theMainWindow->StatusString.c_str(), wxConvUTF8));
  this->mutexRuN.UnlockMe();
}

void wxParserFrame::OnComputationOver(wxCommandEvent& ev)
{ this->theParserOutput->TextCtrl1->SetValue(wxString(theParser.DebugString.c_str(), wxConvUTF8));
  this->GetButton1()->SetLabel(wxT("Go"));
  AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  if (theParser.theValue.ExpressionType==ParserNode::typeAnimation)
  { theOps.flagAnimating=true;

    theOps.DrawNoInit(theGlobalVariables.theDrawingVariables, theGlobalVariables);
    theMainWindow->Button2->SetLabel(wxT("Pause animation"));

//    this->theStatus->TextCtrlStatusString->SetValue(wxString(theOps.ElementToString().c_str() , wxConvUTF8));
    theMainWindow->StartTimer();
  } else
    theOps.flagAnimating=false;
  theMainWindow->theDrawPanel->Refresh();
}

void wxParserFrame::WriteSettings()
{ std::fstream fileSettings;
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(fileSettings, this->theSettingsFileName, false, true, false);
  fileSettings << "mainDialogX_Y_Width_Height: " <<  this->GetRect().x << " " << this->GetRect().y << " "
  << this->GetRect().width << " " << this->GetRect().height << "\n";
  fileSettings << "drawDialogX_Y_Width_Height: " <<  this->theDrawPanel->GetRect().x << " " << this->theDrawPanel->GetRect().y << " "
  << this->theDrawPanel->GetRect().width << " " << this->theDrawPanel->GetRect().height << "\n";
  fileSettings << "indicatorDialogX_Y_Width_Height: " <<  this->theStatus->GetRect().x << " " << this->theStatus->GetRect().y << " "
  << this->theStatus->GetRect().width << " " << this->theStatus->GetRect().height << "\n";
  fileSettings << "outputDialogX_Y_Width_Height: " <<  this->theParserOutput->GetRect().x << " " << this->theParserOutput->GetRect().y << " "
  << this->theParserOutput->GetRect().width << " " << this->theParserOutput->GetRect().height << "\n";
  fileSettings << "outputDialogX_Y_Width_Height: " <<  this->thePNGdisplay->GetRect().x << " " << this->thePNGdisplay->GetRect().y << " "
  << this->thePNGdisplay->GetRect().width << " " << this->thePNGdisplay->GetRect().height << "";
  for (int i=0; i<this->theSlidesFileNames.size; i++)
    fileSettings <<"\n" << this->theSlidesFileNames[i] << "\tframe:\t" << this->theSlideFrameIndices[i];

}

void wxParserFrame::ReadSettings()
{ if (!CGIspecificRoutines::FileExists(this->theSettingsFileName))
    return;
  std::fstream fileSettings;
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(fileSettings, this->theSettingsFileName, false, false, false);
  std::string tempS;
  wxRect tempRect;
  fileSettings >> tempS >> tempRect.x >> tempRect.y >> tempRect.width >> tempRect.height;
  this->SetSize(tempRect);
  fileSettings >> tempS >> tempRect.x >> tempRect.y >> tempRect.width >> tempRect.height;
  this->theDrawPanel->SetSize(tempRect);
  fileSettings >> tempS >> tempRect.x >> tempRect.y >> tempRect.width >> tempRect.height;
  this->theStatus->SetSize(tempRect);
  fileSettings >> tempS >> tempRect.x >> tempRect.y >> tempRect.width >> tempRect.height;
  this->theParserOutput->SetSize(tempRect);
  fileSettings >> tempS >> tempRect.x >> tempRect.y >> tempRect.width >> tempRect.height;
  this->thePNGdisplay->SetSize(tempRect);
  std::string newFileName, oldfileName;
  //int framecount=0, tempInt;
  this->theSlides.MakeActualSizeAtLeastExpandOnTop(100);
  List<std::string>& slides=this->theSlidesFileNames;
  List<int>& ind=this->theSlideFrameIndices;
  int s=0;
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=0+                    1; slides[s-1]="jacobs.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+        40; slides[s-1]="karlin.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_1.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_2.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_3.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_4.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_5.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       50; slides[s-1]="page_6.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_7.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_8.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_9.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_10.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_11.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_12.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       120; slides[s-1]="page_13.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       120; slides[s-1]="page_14.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_15.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       270; slides[s-1]="page_16.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_17.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       20; slides[s-1]="page_18.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_19.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_20.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       80; slides[s-1]="page_21.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       150; slides[s-1]="page_22.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       150; slides[s-1]="page_23.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       50; slides[s-1]="page_24.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       50; slides[s-1]="page_25.png";
  s++; ind.SetSize(s); slides.SetSize(s); ind[s-1]=ind[s-2]+       30; slides[s-1]="";
  this->theSlides.SetSize(s);
  for (int i=0; i<s-1; i++)
  { slides[i]=this->thePath+slides[i];
    if (::CGIspecificRoutines::FileExists(slides[i]))
      this->theSlides[i].LoadFile(wxString(slides[i].c_str(), wxConvUTF8), wxBITMAP_TYPE_PNG);
  }
  /*
  while (!fileSettings.eof())
  { tempInt=0;
    fileSettings >> newFileName >> tempS >> tempInt;
    framecount+=tempInt;
    if (oldfileName==newFileName)
      break;
    oldfileName=newFileName;
    if (CGIspecificRoutines::FileExists(newFileName))
    { this->theSlides.SetSize(this->theSlides.size+1);
      this->theSlidesFileNames.AddObjectOnTop(newFileName);
      this->theSlideFrameIndices.AddObjectOnTop(framecount);
      this->theSlides[this->theSlides.size-1].LoadFile(wxString((this->theSlidesFileNames[this->theSlides.size-1]).c_str(), wxConvUTF8), wxBITMAP_TYPE_PNG);
    }
  }*/
  if (this->theSlideFrameIndices.size>0)
  { this->numSlideFrames=*this->theSlideFrameIndices.LastObject()+2;
    this->indexCurrentPng=0;

  }
}

#ifdef WIN32
void RunComputationalThread()
#else
void* RunComputationalThread(void*ptr)
#endif
{ theGlobalVariables.theLocalPauseController.InitComputation();
  theParser.DebugString= theParser.ParseEvaluateAndSimplify(theParser.StringBeingParsed, false, theGlobalVariables);
  theGlobalVariables.theLocalPauseController.ExitComputation();
  wxPostEvent(theMainWindow->GetEventHandler(),
             theMainWindow->eventComputationOver
              );
  //theMainWindow->frameInput->Refresh();
#ifndef WIN32
  pthread_exit(NULL);
#endif
}

void wxParserFrame::RunTheComputation()
{ theGlobalVariables.theLocalPauseController.mutexHoldMeWhenReadingOrWritingInternalFlags.LockMe();
  if (!theGlobalVariables.theLocalPauseController.GetFlagIsRunningUnsafeUseWithMutexHoldMe())
  { theGlobalVariables.theLocalPauseController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
    this->GetButton1()->SetLabel(wxT("Pause computation"));
    theParser.StringBeingParsed= std::string(this->TextCtrl1->GetValue().mb_str());
#ifdef WIN32
    theComputationalThread.ComputationalThread=CreateThread(0,0, (LPTHREAD_START_ROUTINE)RunComputationalThread,0,0,0);
#else
    pthread_create(&theComputationalThread.ComputationalThreadLinux, NULL, RunComputationalThread, 0);
#endif
  } else
  { if (theGlobalVariables.theLocalPauseController.GetFlagIsPausedWhileRunningUnsafeUseWithMutexHoldMe())
    { this->Button1->SetLabel(wxT("Pause computation"));
      theGlobalVariables.theLocalPauseController.UnlockSafePoint();
      theGlobalVariables.theLocalPauseController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
    }
    else
    { theGlobalVariables.theLocalPauseController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
      theGlobalVariables.theLocalPauseController.SignalPauseToSafePointCallerAndPauseYourselfUntilOtherReachesSafePoint();
      theGlobalVariables.theLocalPauseController.GetFlagIsPausedWhileRunningUnsafeUseWithMutexHoldMe()=true;
      this->Button1->SetLabel(wxT("Continue computation"));
    }
  }
}

void wxParserFrame::OnButton2Click(wxCommandEvent& event)
{ AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  if (theOps.flagAnimating)
  { if (theMainWindow->GetTimer().IsRunning())
    { theMainWindow->GetTimer().Stop();
      theMainWindow->Button2->SetLabel(wxT("Continue animation"));
      theOps.flagIsPausedWhileAnimating=true;
    } else
    { theOps.flagIsPausedWhileAnimating=false;
      theMainWindow->GetTimer().Start();
      theMainWindow->Button2->SetLabel(wxT("Pause animation"));
    }
  }
  return;
  Controller& theController=  tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.thePauseControlleR;
  theController.mutexHoldMeWhenReadingOrWritingInternalFlags.LockMe();
  if (theController.GetFlagIsRunningUnsafeUseWithMutexHoldMe())
  { if (theController.GetFlagIsPausedWhileRunningUnsafeUseWithMutexHoldMe())
    { theController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
      theController.UnlockSafePoint();
    } else
    { theController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
      theController.SignalPauseToSafePointCallerAndPauseYourselfUntilOtherReachesSafePoint();
      tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.WriteToDefaultFile(&theGlobalVariables);
    }
  }
  theController.mutexHoldMeWhenReadingOrWritingInternalFlags.UnlockMe();
}

void wxParserFrame::OnButton1Click(wxCommandEvent& event)
{ this->RunTheComputation();
}

void wxParserFrame::OnTimer1Trigger(wxTimerEvent& event)
{ if (!this->TimerReady)
    return;
  AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  bool needRedraw=theOps.IncrementOpReturnNeedsRedraw();
  this->SpinCtrl2->SetValue(theOps.indexVirtualOp);
  int candidateIndex=theMainWindow->theSlideFrameIndices.IndexOfObject(theOps.indexVirtualOp);

  if (candidateIndex!=-1)
  { theMainWindow->indexCurrentPng=candidateIndex;
    theMainWindow->thePNGdisplay->Refresh();
  }
  if (needRedraw)
    this->theDrawPanel->Refresh();
}

void wxParserFrame::UpdateChoices()
{ theMainWindow->SpinCtrl1->SetValue(theParser.DefaultWeylRank);
  theMainWindow->Choice1->SetSelection(theParser.DefaultWeylLetter- 'A');
}

void wxParserFrame::OnChoice1Select(wxCommandEvent& event)
{ switch(this->Choice1->GetSelection())
  { case 0: theParser.DefaultWeylLetter='A'; break;
    case 1: theParser.DefaultWeylLetter='B'; break;
    case 2: theParser.DefaultWeylLetter='C'; break;
    case 3: theParser.DefaultWeylLetter='D'; break;
    case 4: theParser.DefaultWeylLetter='E'; break;
    case 5: theParser.DefaultWeylLetter='F'; break;
    case 6: theParser.DefaultWeylLetter='G'; break;
    default: break;
  }
  char oldLetter=theParser.DefaultWeylLetter;
  int oldRank = theParser.DefaultWeylRank;
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theParser.DefaultWeylLetter, theParser.DefaultWeylRank);
  if (oldLetter!=theParser.DefaultWeylLetter || oldRank != theParser.DefaultWeylRank)
    this->UpdateChoices();
}

void wxParserFrame::OnSpinCtrl1Change(wxSpinEvent& event)
{ theParser.DefaultWeylRank=this->SpinCtrl1->GetValue();
  char oldLetter=theParser.DefaultWeylLetter;
  int oldRank = theParser.DefaultWeylRank;
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theParser.DefaultWeylLetter, theParser.DefaultWeylRank);
  if (oldLetter!=theParser.DefaultWeylLetter || oldRank != theParser.DefaultWeylRank)
    this->UpdateChoices();
}

void wxPNGdisplay::OnMouseWheel(wxMouseEvent& event)
{ /*double rot = event.GetWheelRotation()/event.GetWheelDelta();
  theMainWindow->scalePNG+=(rot/100);
  for (int i=0; i<theMainWindow->theSlides.size; i++)
  { theMainWindow->theSlides[i].LoadFile(wxString((theMainWindow->theSlidesFileNames[i]).c_str(), wxConvUTF8), wxBITMAP_TYPE_PNG);
    int newWidth= theMainWindow->scalePNG*theMainWindow->theSlides[i].GetWidth();
    int newHeight=theMainWindow->scalePNG*theMainWindow->theSlides[i].GetHeight();
    wxBitmap tmpBM(theMainWindow->theSlides[i]);
    wxImage tmpImg;

    tmpImg=tmpBM.ConvertToImage();
    tmpImg.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
    theMainWindow->theSlides[i]=tmpImg;
  }
  this->Refresh();*/
}

void wxPNGdisplay::OnPaint(wxPaintEvent& event)
{ wxPaintDC theDC(this);
  if (theMainWindow->indexCurrentPng>=0 && theMainWindow->indexCurrentPng<theMainWindow->theSlides.size)
    if (theMainWindow->theSlides[theMainWindow->indexCurrentPng].IsOk())  
      theDC.DrawBitmap(wxBitmap(theMainWindow->theSlides[theMainWindow->indexCurrentPng]), 0, 0);
}


void DrawOperations::operator+=(const DrawOperations& other)
{ if (this->theBilinearForm.NumRows!=other.theBilinearForm.NumRows)
    return;
  this->TypeNthDrawOperation.AddListOnTop(other.TypeNthDrawOperation);
  int shiftDrawText=this->theDrawTextOperations.size;
  int shiftDrawTextAtVector=this->theDrawTextAtVectorOperations.size;
  int shiftDrawLine=this->theDrawLineOperations.size;
  int shiftDrawLineBnVectors=this->theDrawLineBetweenTwoRootsOperations.size;
  int shiftDrawCircleAtVector=this->theDrawCircleAtVectorOperations.size;
  this->IndexNthDrawOperation.MakeActualSizeAtLeastExpandOnTop
  (this->IndexNthDrawOperation.size+other.IndexNthDrawOperation.size);
  for (int i=0; i<other.TypeNthDrawOperation.size; i++)
    switch(other.TypeNthDrawOperation[i])
    { case DrawOperations::typeDrawCircleAtVector:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawCircleAtVector);
        break;
      case DrawOperations::typeDrawLine:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawLine);
        break;
      case DrawOperations::typeDrawLineBetweenTwoVectors:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawLineBnVectors);
        break;
      case DrawOperations::typeDrawText:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawText);
        break;
      case DrawOperations::typeDrawTextAtVector:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawTextAtVector);
        break;
      default:
        assert(false);
        break;
    }
  this->theDrawTextOperations.AddListOnTop(other.theDrawTextOperations);
  this->theDrawLineOperations.AddListOnTop(other.theDrawLineOperations);
  this->theDrawLineBetweenTwoRootsOperations.AddListOnTop(other.theDrawLineBetweenTwoRootsOperations);
  this->theDrawTextAtVectorOperations.AddListOnTop(other.theDrawTextAtVectorOperations);
  this->theDrawCircleAtVectorOperations.AddListOnTop(other.theDrawCircleAtVectorOperations);
  //this->BasisProjectionPlane.AddListOnTop(other.BasisProjectionPlane);
  //this->centerX.AddListOnTop(other.centerX);
  //this->centerY.AddListOnTop(other.centerY);
  //this->GraphicsUnit.AddListOnTop(other.GraphicsUnit);
  this->SelectedPlane=0;
}

DrawOperations& AnimationBuffer::GetLastDrawOps()
{ if (this->thePhysicalDrawOps.size<=0)
    return this->stillFrame;
  return *this->thePhysicalDrawOps.LastObject();
}
/*
void AnimationBuffer::AddCloneLastFrameAppendOperations
(const DrawOperations& other)
{ // theOp.indexPhysicalFrame=0;
  if (this->thePhysicalDrawOps.size>0)
  { VirtualDrawOp theOp;
    theOp.theVirtualOp=this->typeCloneLastFrameAddOps;
    theOp.indexPhysicalDrawOp=this->thePhysicalDrawOps.size;
    DrawOperations theClone;
    theOp.indexPhysicalFrame=this->theVirtualOpS.LastObject()->indexPhysicalFrame+1;
    theOp.theVirtualOp=this->typeDrawOps;
    theClone=this->GetLastDrawOps();
    int lastIndex=theClone.BasisProjectionPlane.size-1;
    theClone.centerX.SwapTwoIndices(0, lastIndex);
    theClone.centerY.SwapTwoIndices(0, lastIndex);
    theClone.GraphicsUnit.SwapTwoIndices(0, lastIndex);
    theClone.BasisProjectionPlane.SwapTwoIndices(0, lastIndex);
    theClone.centerX.SetSize(1);
    theClone.centerY.SetSize(1);
    theClone.GraphicsUnit.SetSize(1);
    theClone.GraphicsUnit[0]=20;
    theClone.centerX[0]=100;
    theClone.centerY[0]=100;
    theClone.SelectedPlane=0;
//    theClone.BasisProjectionPlane.size--;
    if (theClone.theBilinearForm.NumRows==other.theBilinearForm.NumRows)
      theClone+=other;
  }
  else
  { int oldsize=this->theVirtualOpS.size;
    this->thePhysicalDrawOps.AddObjectOnTop(other);
    this->theVirtualOpS.SetSize(oldsize+1);
    VirtualDrawOp& tmpOp=*this->theVirtualOpS.LastObject();
    tmpOp.indexPhysicalDrawOp=0;
    tmpOp.indexPhysicalFrame=-1;
    tmpOp.selectedPlaneInPhysicalDrawOp=-1;

  }
}
*/


void wxParserFrame::OnSpinCtrl2Change(wxSpinEvent& event)
{ AnimationBuffer& theOps=theParser.theValue.theAnimation.GetElement();
  int candidate= this->SpinCtrl2->GetValue();
  if (candidate<theOps.theVirtualOpS.size && candidate>=0 && this->theSlideFrameIndices.size>0)
  { theOps.indexVirtualOp=candidate;
    this->theDrawPanel->Refresh();
    int oldSize=-1; int newSize=0;
    for (int i=-1; i<theMainWindow->theSlideFrameIndices.size; i++)
    { oldSize=newSize;
      if (i+1<this->theSlideFrameIndices.size)
        newSize=this->theSlideFrameIndices[i+1];
      else
        newSize=100000; //"infinity"
      if (theOps.indexVirtualOp>=oldSize && theOps.indexVirtualOp<newSize)
      { theMainWindow->indexCurrentPng=i;
          wxPostEvent(theMainWindow->thePNGdisplay->GetEventHandler(),
             theMainWindow->eventPNGrepaint
              );
        break;
      }
    }
  }
  if (candidate>=theOps.theVirtualOpS.size)
  { this->SpinCtrl2->SetRange(0, theOps.theVirtualOpS.size-1);
    this->SpinCtrl2->SetValue(theOps.theVirtualOpS.size-1);
  }
}



class ImpreciseDouble
{
  private:
  double precision;
  double theValue;
  public:
  void ElementToString(std::string& output)
  { output=this->ElementToString();
  }
  std::string ElementToString()
  { std::stringstream out;
    out << this->theValue;
    return out.str();
  }
  void operator=(const ImpreciseDouble& other)
  { this->theValue=other.theValue;
    this->precision=other.precision;
  }
  void operator=(double other)
  { this->theValue=other;
  }
  ImpreciseDouble(const ImpreciseDouble& other)
  { this->operator=(other);
  }
  ImpreciseDouble()
  { this->theValue=0;
    this->precision=0.1;
  }
  ImpreciseDouble(double other)
  { this->operator=(other);
  }
  void operator+=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue+=other.theValue;
  }
  void operator-=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue-=other.theValue;
  }
  void operator=(const Rational& other)
  { this->theValue=other.DoubleValue();
  }
  bool IsEqualToZero()const
  { if (this->theValue<0)
      return (-theValue)< this->precision;
    return this->theValue<this->precision;
  }
  inline bool operator<=(const ImpreciseDouble& other)
  { return ! (other<*this);
  }
  bool IsPositive()const
  { return this->theValue>this->precision;
  }
  inline bool IsNegative()const
  { return *this<this->GetZero();
  }
  bool operator<(const ImpreciseDouble& other)const
  { ImpreciseDouble temp=other;
    temp-=*this;
    return temp.IsPositive();
  }
  void AssignFloor()
  { this->theValue=floor(this->theValue);
  }
  inline ImpreciseDouble operator/(const ImpreciseDouble& other)const
  { ImpreciseDouble result;
    result=*this;
    if (other.IsEqualToZero())
    { //the below is written like this to avoid this->theValue/0;
      //If the user attempts to divide by zero, I want a regular division by zero exception to be generated
      result.theValue=this->theValue/(other.theValue-other.theValue);
      return result;
    }
    result.theValue/=other.theValue;
    return result;
  }
  void operator*=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue*=other.theValue;
    else
      this->theValue=0;
  }
  bool operator==(const ImpreciseDouble& other)const
  { int diff=this->theValue-other.theValue;
    if (diff<0)
      diff=-diff;
    return diff<this->precision;
  }
  static inline ImpreciseDouble GetMinusOne()
  { return -1;
  }
  static inline ImpreciseDouble GetOne()
  { return 1;
  }
  static inline ImpreciseDouble GetZero()
  { return 0;
  }
};
template < > std::string Matrix<ImpreciseDouble>::MatrixElementSeparator="";

int ParserNode::EvaluateLatticeImprecise
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  int theDim=-1;
  if (!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, tempRoots, theDim))
    return theNode.SetError(theNode.errorBadOrNoArgument);
  Matrix<ImpreciseDouble> theMat;
  theMat.init(tempRoots.size, theDim);
  for (int i=0; i<theMat.NumRows; i++)
    for (int j=0; j<theMat.NumCols; j++)
      theMat.elements[i][j]=tempRoots[i][j];
  std::cout << "starting matrix: " << theMat.ElementToString(true, false);
  theMat.GaussianEliminationEuclideanDomain(ImpreciseDouble::GetMinusOne(), ImpreciseDouble::GetOne());
  std::cout << "<br>final matrix: " << theMat.ElementToString(true, false);
  return theNode.errorNoError;
}

void DrawOperations::projectionMultiplicityMergeOnBasisChange(DrawOperations& theOps)
{ Matrix<ImpreciseDouble> theMat;
  int theDim=theOps.theBilinearForm.NumRows;
  theMat.init(theDim, 2);
//we assume that the ComputeProjectionsEiVectors has been called
//  theOps.ComputeProjectionsEiVectors();
  for(int i=0; i<theOps.ProjectionsEiVectors.size; i++)
    for (int j=0; j<2; j++)
      theMat.elements[i][j]=theOps.ProjectionsEiVectors[i][j];
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  std::stringstream out;
  out << "before elimination:\n" << theMat.ElementToString(false, false);
  theMat.GaussianEliminationEuclideanDomain(ImpreciseDouble::GetMinusOne(), ImpreciseDouble::GetOne());
  out << "after elimination:\n" << theMat.ElementToString(false, false);
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();

}

//void ComputeMultRecursive(List<int>&

int ParserNode::EvaluateDrawG2InB3
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ //theNode.owner->theHmm.MakeG2InB3(theParser, theGlobalVariables);
  AnimationBuffer& Ani=theNode.theAnimation.GetElement();
  theNode.EvaluateDrawRootSystem(theNode, 'B', 3, theGlobalVariables, 0);
  SelectionWithMaxMultiplicity theSel;
  int theDim=3;
  int multSize=9;
  theSel.initMaxMultiplicity(theDim, multSize);
  int numPoints= theSel.NumSelectionsTotal();
  root tempRoot;
  DrawOperations& theOps=Ani.thePhysicalDrawOps[0];
  root highestWeight;
  highestWeight.MakeZero(theDim);
  hashedRoots theWeights;
  for (int i=0; i<numPoints; i++, theSel.IncrementSubset())
  { tempRoot=theSel;
    tempRoot.MinusRoot();
    theWeights.AddObjectOnTopHash(tempRoot);
    theOps.drawCircleAtVectorBuffer(tempRoot, 2, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(150,150,150));
  }
  theOps.indexStartingModifiableTextCommands=theOps.theDrawTextAtVectorOperations.size;
  theOps.specialOperationsOnBasisChange=&theOps.projectionMultiplicityMergeOnBasisChange;
  return theNode.errorNoError;
}
