#ifndef VPFIMPLEMENTATIONHEADER2MATH0_GENERAL_H
#define VPFIMPLEMENTATIONHEADER2MATH0_GENERAL_H
#include "vpfHeader2Math0_General.h"
static ProjectInformationInstance ProjectInfoVPFIMPLEMENTATIONHEADER2MATH0_GENERAL_H(__FILE__, "Header implementation, math routines. ");
template <class coefficient>
void Matrix<coefficient>::ComputeDeterminantOverwriteMatrix(coefficient& output, const coefficient& theRingOne, const coefficient& theRingZero)
{ MacroRegisterFunctionWithName("Matrix::ComputeDeterminantOverwriteMatrix");
  bool doReport = false;
  if (theGlobalVariables.flagReportEverything || theGlobalVariables.flagReportGaussianElimination)
    doReport = this->NumCols > 10 && this->NumRows > 10 && this->NumCols * this->NumRows >= 400;
  ProgressReport theReport, theReport2;
  int tempI;
  output = theRingOne;
  coefficient tempRat;
  if (this->NumCols != this->NumRows)
    crash << "Error: determinant computation: number of columns different from number of rows. " << crash;
  int dim = this->NumCols;
  for (int i = 0; i < dim; i ++)
  { //stOutput << "Computing determinant, matrix current state: " << this->ToString() << "<hr>";
    tempI = this->FindPivot(i, i);
    if (tempI == - 1)
    { output = theRingZero;
      return;
    }
    this->SwitchTwoRows(i, tempI);
    if (tempI != i)
      output *= - 1;
    tempRat = this->elements[i][i];
    output *= tempRat;
    tempRat.Invert();
    this->RowTimesScalar(i, tempRat);
    if (doReport)
    { std::stringstream reportStream;
      reportStream << "Pivot row " << i + 1 << " out of " << dim << ": ";
      for (int colCounter = 0; colCounter < this->NumCols; colCounter ++)
      { reportStream << (*this)(i, colCounter).ToString();
        if (colCounter != this->NumCols - 1)
          reportStream << ", ";
      }
      theReport.Report(reportStream.str());
    }
    for (int j = i + 1; j < dim; j ++)
      if (!this->elements[j][i].IsEqualToZero())
      { tempRat = this->elements[j][i];
        tempRat.Minus();
        this->AddTwoRows (i, j, i, tempRat);
        if (doReport)
        { std::stringstream reportStream;
          reportStream << "Computing large determinant: pivot " << i + 1 << ", row " << j << " out of "
          << dim <<  " times  " << dim << " total.";
          theReport2.Report(reportStream.str());
        }
      }
  }
}

template<class coefficient>
std::ostream& operator<< (std::ostream& output, const Matrix<coefficient>& theMat)
{ output << "\\left(\\begin{array}{";
  for (int j = 0; j < theMat.NumCols; j ++)
    output << "c";
  output << "}";
  int firstMatRowIndexToHide = theMat.NumRows;
  int lastMatRowIndexToHide = theMat.NumRows;
  int firstMatColIndexToHide = theMat.NumCols;
  int lastMatColIndexToHide = theMat.NumCols;
  FormatExpressions& theFormat = theGlobalVariables.theDefaultFormat.GetElement();
  if (theFormat.flagSuppressLongMatrices)
  { if (theMat.NumRows > theFormat.MaxMatrixDisplayedRows)
    { firstMatRowIndexToHide = theFormat.MaxMatrixDisplayedRows / 2;
      lastMatRowIndexToHide = theMat.NumRows - 1 - theFormat.MaxMatrixDisplayedRows / 2;
    }
    if (theMat.NumCols > theFormat.MaxMatrixLineLength)
    { firstMatColIndexToHide = theFormat.MaxMatrixLineLength / 2;
      lastMatColIndexToHide = theMat.NumCols - 1 - theFormat.MaxMatrixLineLength / 2;
    }
  }
  for (int i = 0; i < theMat.NumRows; i ++)
  { if (firstMatRowIndexToHide <= i && i <= lastMatRowIndexToHide)
    { if (firstMatRowIndexToHide == i)
        output << "...\\\\\n";
      continue;
    }
    for (int j = 0; j < theMat.NumCols; j ++)
    { if (firstMatColIndexToHide <= j && j <= lastMatColIndexToHide)
      { if (firstMatColIndexToHide == j)
          output << "...&";
        continue;
      }
      output << theMat(i, j);
      if (j != theMat.NumCols - 1)
        output << " & ";
      else
        output << "\\\\";
    }
    output << "\n";
  }
  output << "\\end{array}\\right)";
  return output;
}

template <class coefficient, typename theIntegerType>
void MathRoutines::RaiseToPower
(coefficient& theElement, const theIntegerType& thePower, const coefficient& theRingUnit)
{ MacroRegisterFunctionWithName("MathRoutines::RaiseToPower");
  theIntegerType thePowerCopy;
  thePowerCopy = thePower;
  if (thePowerCopy < 0)
    return;
  if (thePowerCopy == 1)
    return;
  if (thePowerCopy == 0)
  { theElement = theRingUnit;
    return;
  }
  ProgressReport theReport;
  std::stringstream reportStream;
  coefficient squares;
  squares = theElement;
  if (thePowerCopy < 4)
  { for (theIntegerType i = 1; i < thePowerCopy; i ++)
      theElement *= squares;
    return;
  }
  if (theGlobalVariables.flagReportEverything)
  { reportStream << "Raising " << theElement.ToString()
    << " to power: " << thePowerCopy;
    theReport.Report(reportStream.str());
  }
  theElement = theRingUnit;
  int counter = 0;
  while (thePowerCopy > 0)
  { counter ++;
    bool doReport = false;
    if (theGlobalVariables.flagReportEverything)
      if (counter > 3)
        doReport = true;
    if (thePowerCopy % 2 == 1)
    { if (doReport)
      { std::stringstream reportStream2;
        reportStream2 << "Remaining exponent: " << thePowerCopy << "<br>";
        reportStream2 << "Multiplying " << theElement.ToString()
        << " by " << squares.ToString();
        theReport.Report(reportStream2.str());
      }
      theElement *= squares;
    }
    thePowerCopy /= 2;
    if (thePowerCopy > 0)
    { if (doReport)
      { std::stringstream reportStream2;
        reportStream2 << "Remaining exponent: " << thePowerCopy << "<br>";
        reportStream2 << "Squaring: " << squares.ToString();
        theReport.Report(reportStream2.str());
      }
      squares *= squares;
    }
  }
}

template <class templateMonomial, class coefficient>
void ElementMonomialAlgebra<templateMonomial, coefficient>::MultiplyBy
(const ElementMonomialAlgebra<templateMonomial, coefficient>& other, ElementMonomialAlgebra<templateMonomial, coefficient>& output,
 ElementMonomialAlgebra<templateMonomial, coefficient>& bufferPoly, templateMonomial& bufferMon) const
{ MacroRegisterFunctionWithName("ElementMonomialAlgebra::MultiplyBy");
  if (other.IsEqualToZero())
  { output.MakeZero();
    return;
  }
  if (&bufferPoly == this || &bufferPoly == &other)
  { crash << "Bad buffer in ElementMonomialAlgebra::MultiplyBy." << crash;
  }
  this->CheckConsistency();
  other.CheckConsistency();
  int maxNumMonsFinal = this->size() * other.size();
  if (maxNumMonsFinal > 2000000)
    maxNumMonsFinal = 2000000;
  bool shouldReport = false;
  int totalMonPairs = 0;
  ProgressReport theReport1, theReport2;
  if (theGlobalVariables.flagReportEverything ||
      theGlobalVariables.flagReportProductsMonomialAlgebras)
  { totalMonPairs = other.size() * this->size();
    shouldReport = totalMonPairs > 2000 && other.size() > 10 && this->size() > 10;
  }
  if (shouldReport)
  { std::stringstream reportStream;
    reportStream << "Large polynomial computation: " << this->size() << " x " << other.size() << "=" << totalMonPairs << " monomials:\n<br>\n"
    << this->ToString() << " times " << other.ToString();
    theReport1.Report(reportStream.str());
  }
  bufferPoly.MakeZero();
  bufferPoly.SetExpectedSize(maxNumMonsFinal);
  bufferPoly.CheckConsistency();
  bufferMon.CheckConsistency();
  coefficient theCoeff;
  int counter = 0;
  for (int i = 0; i < other.size(); i ++)
    for (int j = 0; j < this->size(); j ++)
    { if (shouldReport)
      { counter ++;
        if (counter % 400 == 0)
        { std::stringstream reportStream2;
          reportStream2 << "Multiplying monomials: "
          << i + 1 << " out of " << other.size() << " by " << j + 1
          << " out of " << this->size() << ". ";
          theReport2.Report(reportStream2.str());
        }
      }
      bufferMon = (*this)[j];
      bufferMon *= other[i];
      theCoeff = this->theCoeffs[j];
      theCoeff *= other.theCoeffs[i];
      bufferPoly.AddMonomial(bufferMon, theCoeff);
      ParallelComputing::SafePointDontCallMeFromDestructors();
    }
  output = bufferPoly;
}

template <class coefficient>
void Matrix<coefficient>::GaussianEliminationEuclideanDomain
(Matrix<coefficient>* otherMatrix, const coefficient& theRingMinusUnit, const coefficient& theRingUnit, bool (*comparisonGEQFunction) (const coefficient& left, const coefficient& right))
{ MacroRegisterFunctionWithName("Matrix_Element::GaussianEliminationEuclideanDomain");
  ProgressReport theReport;
  if (otherMatrix == this)
    crash << "This is a programming error: the Carbon copy in the Gaussian elimination coincides with the matrix which we are row-reducing "
    << "(most probably this is a wrong pointer typo). " << crash;
  int col = 0;
  coefficient tempElt;
  int row = 0;
  while (row < this->NumRows && col < this->NumCols)
  { //stOutput << "<br>****************row: " << row << " status: " << this->ToString(true, false);
    int foundPivotRow = - 1;
    for (int i = row; i < this->NumRows; i ++)
      if (!this->elements[i][col].IsEqualToZero())
      { foundPivotRow = i;
        break;
      }
    if (foundPivotRow != - 1)
    { /*if (this->elements[foundPivotRow][col].IsEqualToZero())
      { crash << "This is a programming error. "
        << "Something is very wrong: I am getting 0 for a pivot element in "
        << "Gaussian elimination over Euclidean domains. "
        << crash;
      }*/
      this->SwitchTwoRowsWithCarbonCopy(row, foundPivotRow, otherMatrix);
      /*if (this->elements[row][col].IsEqualToZero())
      { crash << "This is a programming error. "
        << "Something is very wrong: I am getting 0 for a pivot element in "
        << "Gaussian elimination over Euclidean domains. "
        << crash;
      }*/
      if (this->elements[row][col].IsNegative())
        this->RowTimesScalarWithCarbonCopy(row, theRingMinusUnit, otherMatrix);
      int ExploringRow = row + 1;
//      stOutput << "<br>before second while: " << this->ToString(true, false);
      while (ExploringRow< this->NumRows)
      { if (theGlobalVariables.flagReportEverything || theGlobalVariables.flagReportGaussianElimination)
        { std::stringstream out;
          out << "Pivotting on row of index " << row + 1 << " with exploring row of index "
          << ExploringRow + 1 << "; total rows: " << this->NumRows;
          theReport.Report(out.str());
        }
        coefficient& PivotElt = this->elements[row][col];
        coefficient& otherElt = this->elements[ExploringRow][col];
        /*if (PivotElt.IsEqualToZero())
        { crash << "This is a programming error. "
          << "Something is very wrong: I am getting 0 for a pivot element in "
          << "Gaussian elimination over Euclidean domains. "
          << crash;
        }*/
        if (otherElt.IsNegative())
          this->RowTimesScalarWithCarbonCopy(ExploringRow, theRingMinusUnit, otherMatrix);
        bool isSmallerOrEqualTo = comparisonGEQFunction == 0 ? PivotElt <= otherElt :
        comparisonGEQFunction(otherElt, PivotElt);
        if (isSmallerOrEqualTo)
        { tempElt = otherElt;
          tempElt /= PivotElt;
          tempElt.AssignFloor();
          this->SubtractRowsWithCarbonCopy(ExploringRow, row, 0, tempElt, otherMatrix);
        }
        if (this->elements[ExploringRow][col].IsEqualToZero())
          ExploringRow ++;
        else
          this->SwitchTwoRowsWithCarbonCopy(ExploringRow, row, otherMatrix);
//        stOutput << "<br>second while cycle end: " << this->ToString(true, false);
      }
      coefficient& PivotElt = this->elements[row][col];
      for (int i = 0; i < row; i ++)
      { tempElt = this->elements[i][col];
        tempElt /= PivotElt;
//        stOutput << " the floor of " << tempElt.ToString();
        tempElt.AssignFloor();
//        stOutput << " is " << tempElt.ToString();
        this->SubtractRowsWithCarbonCopy(i, row, 0, tempElt, otherMatrix);
        if (this->elements[i][col].IsNegative())
          this->AddTwoRowsWithCarbonCopy(row, i, 0, theRingUnit, otherMatrix);
      }
      row ++;
    }
    col ++;
//    stOutput << "end of cycle status: " << this->ToString(true, false) << "<br>****************";
  }
}

template <class coefficient>
void Vectors<coefficient>::SelectABasisInSubspace
(const List<Vector<coefficient> >& input, List<Vector<coefficient> >& output, Selection& outputSelectedPivotColumns)
{ if (&input == &output)
  { List<Vector<coefficient> > inputCopy = input;
    Vectors<coefficient>::SelectABasisInSubspace(inputCopy, output, outputSelectedPivotColumns);
    return;
  }
  if (input.size == 0)
  { output = input;
    return;
  }
  MacroRegisterFunctionWithName("Vectors::SelectABasisInSubspace");
  ProgressReport theReport;
  int theDim = input[0].size;
  bool doProgressReport = theGlobalVariables.flagReportEverything || theGlobalVariables.flagReportGaussianElimination;
  if (doProgressReport)
  { std::stringstream reportStream;
    reportStream << "Selecting a basis of a vector space with " << input.size
    << " generators in dimension " << theDim << "... " ;
    theReport.Report(reportStream.str());
  }
  Matrix<coefficient> theMat;
  int MaxNumRows = MathRoutines::Minimum(input.size, theDim);
  theMat.init(MaxNumRows, theDim);
  int currentRow = 0;
  for (int i = 0; i < input.size; i ++)
  { for (int j = 0; j < theDim; j ++)
      theMat(currentRow, j) = input[i][j];
    currentRow ++;
    if (currentRow == MaxNumRows || i == input.size - 1)
    { theMat.GaussianEliminationByRows(0, 0, &outputSelectedPivotColumns);
      currentRow = outputSelectedPivotColumns.CardinalitySelection;
    }
    if (currentRow == MaxNumRows)
      break;
  }
  output.SetSize(outputSelectedPivotColumns.CardinalitySelection);
  for (int i = 0; i < output.size; i ++)
    theMat.GetVectorFromRow(i, output[i]);
  if (doProgressReport)
  { std::stringstream reportStream;
    reportStream << "Selecting a basis of a vector space with " << input.size << " generators in dimension " << theDim << "... done. " ;
    theReport.Report(reportStream.str());
  }
}

template <class Object>
bool List<Object>::ReadFromFile(std::fstream& input, int UpperLimitForDebugPurposes)
{ std::string tempS;
  int ActualListSize; int NumWordsBeforeTag;
  XML::ReadThroughFirstOpenTag(input, NumWordsBeforeTag, this->GetXMLClassName());
  if (NumWordsBeforeTag != 0)
    crash << "Bad file input. " << crash;
  input >> tempS >> ActualListSize;
  if (tempS != "size:")
    crash << crash;
  if (tempS != "size:")
    return false;
  int CappedListSize = ActualListSize;
  if (UpperLimitForDebugPurposes > 0 && UpperLimitForDebugPurposes < CappedListSize)
    CappedListSize = UpperLimitForDebugPurposes;
  this->SetSize(CappedListSize);
  for (int i = 0; i < CappedListSize; i ++)
  { this->TheObjects[i].ReadFromFile(input);
    //<reporting_and_safepoint_duties>
    if (theGlobalVariables.flagReportFileIO)
    { if (ActualListSize > 30)
      { std::stringstream report;
        report << "Reading object number " << i + 1 << " out of " << ActualListSize;
        if (CappedListSize < ActualListSize)
          report << " capped at " << CappedListSize;
        ProgressReport tempReport(report.str());
      }
      theGlobalVariables.theLocalPauseController.SafePointDontCallMeFromDestructors();
    }
    //</reporting_and_safepoint_duties>
  }
  bool tempBool = XML::ReadEverythingPassedTagOpenUntilTagClose(input, NumWordsBeforeTag, this->GetXMLClassName());
  if (!tempBool)
    crash << "Bad input file. " << crash;
  return true;
}

template <typename coefficient>
void Matrix<coefficient>::AddTwoRows(int fromRowIndex, int ToRowIndex, int StartColIndex, const coefficient& scalar)
{ ProgressReport theReport;
  bool doProgressReport = false;
  doProgressReport = theGlobalVariables.flagReportGaussianElimination || theGlobalVariables.flagReportEverything;
  coefficient tempElement;
  for (int i = StartColIndex; i < this->NumCols; i ++)
  { tempElement = this->elements[fromRowIndex][i];
    tempElement *= scalar;
    if (doProgressReport)
    { std::stringstream out;
      out << "Processing row, element " << i + 1 << " out of " << this->NumCols;
      theReport.Report(out.str());
    }
    this->elements[ToRowIndex][i] += tempElement;
  }
}

template <typename coefficient>
void Matrix<coefficient>::GaussianEliminationByRows
(Matrix<coefficient>* carbonCopyMat, Selection* outputNonPivotColumns,
 Selection* outputPivotColumns, std::stringstream* humanReadableReport, FormatExpressions* theFormat)
{ MacroRegisterFunctionWithName("Matrix::GaussianEliminationByRows");
  //Checking for bees
  if (this->NumRows == 0)
    crash << "This is a programming error: requesting to do Gaussian elimination on a matrix with "
    << " zero rows. " << crash;
  if (carbonCopyMat != 0)
    if (carbonCopyMat->NumRows != this->NumRows)
      crash << "This is a programming error: requesting to do Gaussian elimination with carbon copy, however the matrix has "
      << this->NumRows << " rows, while the carbon copy has " << carbonCopyMat->NumRows << " rows. " << crash;
  ///////////////////
  int tempI;
  int NumFoundPivots = 0;
  int MaxRankMat = MathRoutines::Minimum(this->NumRows, this->NumCols);
  coefficient tempElement;
  if (outputNonPivotColumns != 0)
    outputNonPivotColumns->init(this->NumCols);
  if (outputPivotColumns != 0)
    outputPivotColumns->init(this->NumCols);
  bool doProgressReport = theGlobalVariables.flagReportGaussianElimination || theGlobalVariables.flagReportEverything;
  bool formatAsLinearSystem = theFormat == 0 ? false : theFormat->flagFormatMatrixAsLinearSystem;
  bool useHtmlInReport = theFormat == 0 ? true : theFormat->flagUseHTML;
  ProgressReport theReport;
  if (humanReadableReport != 0)
  { if (useHtmlInReport)
      *humanReadableReport << "\n\n\n\n<table><tr><td style =\"border-bottom:3pt solid black;\">System status</td>"
      << "<td style =\"border-bottom:3pt solid black;\">action</td></tr>";
    else
      *humanReadableReport << "\n\n\\begin{longtable}{cc} System status&Action \\\\\\hline\n";
  }
  //Initialization done! Time to do actual work:
  for (int i = 0; i < this->NumCols; i ++)
  { if (NumFoundPivots == MaxRankMat)
    { if (outputNonPivotColumns != 0)
        for (int j = i; j < this->NumCols; j ++)
          outputNonPivotColumns->AddSelectionAppendNewIndex(j);
      break;
    }
    tempI = this->FindPivot(i, NumFoundPivots);
    if (tempI == - 1)
    { if (outputNonPivotColumns != 0)
        outputNonPivotColumns->AddSelectionAppendNewIndex(i);
      continue;
    }
    if (humanReadableReport != 0)
    { if (useHtmlInReport)
      { *humanReadableReport << "<tr><td style =\"border-bottom:1pt solid black;\">";
        if (formatAsLinearSystem)
          *humanReadableReport << HtmlRoutines::GetMathSpanPure(this->ToStringSystemLatex(carbonCopyMat, theFormat), - 1);
        else
          *humanReadableReport << HtmlRoutines::GetMathSpanPure(this->ToStringLatex(theFormat), - 1);
        *humanReadableReport << "</td><td style =\"border-bottom:1pt solid black;\">Selected pivot column "
        << i + 1 << ". ";
        if (NumFoundPivots != tempI)
          *humanReadableReport << "Swapping rows so the pivot row is number " << NumFoundPivots << ". ";
      } else
      { if (formatAsLinearSystem)
          *humanReadableReport << "$" << this->ToStringSystemLatex(carbonCopyMat, theFormat) << "$";
        else
          *humanReadableReport << "$" << this->ToStringLatex(theFormat) << "$";
        *humanReadableReport << "& Selected pivot column " << i + 1 << ". ";
        if (NumFoundPivots != tempI)
          *humanReadableReport << "Swapping rows so the pivot row is number " << NumFoundPivots << ". ";
      }
    }
    if (outputPivotColumns != 0)
      outputPivotColumns->AddSelectionAppendNewIndex(i);
    this->SwitchTwoRows(NumFoundPivots, tempI);
    if (carbonCopyMat != 0)
      carbonCopyMat->SwitchTwoRows(NumFoundPivots, tempI);
    tempElement = this->elements[NumFoundPivots][i];
    tempElement.Invert();
    this->RowTimesScalar(NumFoundPivots, tempElement);
    if (carbonCopyMat != 0)
      carbonCopyMat->RowTimesScalar(NumFoundPivots, tempElement);
    for (int j = 0; j < this->NumRows; j ++)
      if (j != NumFoundPivots)
        if (!this->elements[j][i].IsEqualToZero())
        { tempElement = this->elements[j][i];
          tempElement.Minus();
          if (doProgressReport)
          { std::stringstream reportStream;
            reportStream << "Gaussian elimination (" << this->NumRows << "x" << this->NumCols
            << "): column " << i + 1 << " out of " << this->NumCols
            << ".\n<br>Pivot row: " << NumFoundPivots + 1 << ", eliminating row " << j + 1 << " out of " << this->NumRows;
            theReport.Report(reportStream.str());
          }
          this->AddTwoRows(NumFoundPivots, j, i, tempElement);
          if (carbonCopyMat != 0)
            carbonCopyMat->AddTwoRows(NumFoundPivots, j, 0, tempElement);
          //if (!tempElement.checkConsistency())
          //  crash << crash;
          //this->ComputeDebugString();
        }
    if (humanReadableReport != 0)
    { if (useHtmlInReport)
        *humanReadableReport << "Eliminated the non-zero entries in the pivot column</td></tr>";
      else
        *humanReadableReport << "Eliminated the non-zero entries in the pivot column. \\\\\\hline\n";
    }
    NumFoundPivots ++;
  }
  if (humanReadableReport != 0)
  { if (useHtmlInReport)
    { if (formatAsLinearSystem)
        *humanReadableReport << "<tr><td>" << HtmlRoutines::GetMathSpanPure(this->ToStringSystemLatex(carbonCopyMat, theFormat),- 1)
        << "</td><td> Final result.</td></tr></table>\n\n\n\n";
      else
        *humanReadableReport << "<tr><td>" << HtmlRoutines::GetMathSpanPure(this->ToStringLatex(theFormat))
        << "</td><td> Final result.</td></tr></table>\n\n\n\n";
    } else
    { if (formatAsLinearSystem)
        *humanReadableReport << "$" << this->ToStringSystemLatex(carbonCopyMat, theFormat)
        << "$& Final result.\\\\\n";
      else
        *humanReadableReport << "$" << this->ToStringLatex(theFormat) << "$& Final result.\\\\\n";
      *humanReadableReport << "\\end{longtable}";
    }
  }
}

#endif // VPFIMPLEMENTATIONHEADER2MATH0_GENERAL_H

