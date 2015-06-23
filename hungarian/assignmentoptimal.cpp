/*
function [assignment, cost] = assignmentoptimal(distMatrix)
*/

#include <mex.h>
#include <matrix.h>

#include <hungarian.h>

//#define CHECK_FOR_INF

void assignmentoptimal(double *assignment, double *cost, double *distMatrix, int nOfRows, int nOfColumns);

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	double *assignment, *cost, *distMatrix;
	int nOfRows, nOfColumns;
	
	/* Input arguments */
	nOfRows    = mxGetM(prhs[0]);
	nOfColumns = mxGetN(prhs[0]);
	distMatrix = mxGetPr(prhs[0]);
	
	/* Output arguments */
	plhs[0]    = mxCreateDoubleMatrix(nOfRows, 1, mxREAL);
	plhs[1]    = mxCreateDoubleScalar(0);
	assignment = mxGetPr(plhs[0]);
	cost       = mxGetPr(plhs[1]);
	
	/* Call C++-function */
	assignmentoptimal(assignment, cost, distMatrix, nOfRows, nOfColumns);	
}

void assignmentoptimal(double *assignment, double *cost, double *distMatrixIn, int nOfRows, int nOfColumns)
{
	double *distMatrix, *distMatrixTemp, *distMatrixEnd, value;
	bool *coveredColumns, *coveredRows, *starMatrix, *newStarMatrix, *primeMatrix;
	int nOfElements, row, col;
#ifdef CHECK_FOR_INF
	bool infiniteValueFound;
	double maxFiniteValue, infValue;
#endif
	
	/* initialization */
	*cost = 0;
	for(row=0; row<nOfRows; row++)
		assignment[row] = -1.0;

	
	/* generate working copy of distance Matrix */
	/* check if all matrix elements are positive */
	nOfElements   = nOfRows * nOfColumns;

  //	distMatrix    = (double *)mxMalloc(nOfElements * sizeof(double));	
	distMatrix    = new double [nOfElements]();
  
	distMatrixEnd = distMatrix + nOfElements;
	for(row=0; row<nOfElements; row++)
	{
		value = distMatrixIn[row];
		if(mxIsFinite(value) && (value < 0))
			mexErrMsgTxt("All matrix elements have to be non-negative.");
		distMatrix[row] = value;
	}

#ifdef CHECK_FOR_INF
	/* check for infinite values */
	maxFiniteValue     = -1;
	infiniteValueFound = false;
	
	distMatrixTemp = distMatrix;
	while(distMatrixTemp < distMatrixEnd)
	{
		value = *distMatrixTemp++;
		if(mxIsFinite(value))
		{
			if(value > maxFiniteValue)
				maxFiniteValue = value;
		}
		else
			infiniteValueFound = true;
	}
	if(infiniteValueFound)
	{
		if(maxFiniteValue == -1) /* all elements are infinite */
			return;
		
		/* set all infinite elements to big finite value */
		if(maxFiniteValue > 0)
			infValue = 10 * maxFiniteValue * nOfElements;
		else
			infValue = 10;
		distMatrixTemp = distMatrix;
		while(distMatrixTemp < distMatrixEnd)
			if(mxIsInf(*distMatrixTemp++))
				*(distMatrixTemp-1) = infValue;
	}
#endif

Hungarian h_assign(assignment, cost, distMatrix, distMatrixIn, nOfRows, nOfColumns);

h_assign.computeAssignment();	

for(row=0; row<nOfRows; row++)
  assignment[row] = assignment[row] + 1; /* MATLAB-Indexing */
 
//mxFree(distMatrix);
delete distMatrix;
	
return;
				
}
