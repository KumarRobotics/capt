/*
function [assignment, cost] = assignmentoptimal(distMatrix)
*/

#include <mex.h>
#include <matrix.h>

#include <hungarian.h>

//#define CHECK_FOR_INF

void assignmentoptimal(double *d_assignment, double *cost, double *distMatrix, int nOfRows, int nOfColumns);

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	double *d_assignment, *cost, *distMatrix;
	int nOfRows, nOfColumns;
	
	/* Input arguments */
	nOfRows    = mxGetM(prhs[0]);
	nOfColumns = mxGetN(prhs[0]);
	distMatrix = mxGetPr(prhs[0]);
	
	/* Output arguments */
	plhs[0]    = mxCreateDoubleMatrix(nOfRows, 1, mxREAL);
	plhs[1]    = mxCreateDoubleScalar(0);
	d_assignment = mxGetPr(plhs[0]);
	cost       = mxGetPr(plhs[1]);
	
	/* Call C++-function */
	assignmentoptimal(d_assignment, cost, distMatrix, nOfRows, nOfColumns);	
}

void assignmentoptimal(double *d_assignment, double *cost, double *distMatrixIn, int nOfRows, int nOfColumns)
{
	double *distMatrix, *distMatrixTemp, *distMatrixEnd, value;
	int nOfElements, row, col;
	
	/* initialization */
	*cost = 0;
	for(row=0; row<nOfRows; row++)
		d_assignment[row] = -1.0;
	
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
	bool infiniteValueFound;
	double maxFiniteValue, infValue;
	
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

  int* assignment;
  assignment = new int[nOfRows];
	for(int row=0; row<nOfRows; row++)
	  assignment[row] = -1;
		    
  //Input for the library 
  /*
  assignment - Pointer to int array  1 x nOfRows that gives the indices of final assignemnt
  cost - Total path cost of the final assignment
  distMatrix - Pointer to the distance matrix - double array of size 1 x nOfRows * nOfColumns (Column-major order from Matlab)
  nOfRows - number of robots
  nOfColumns - number of goals
  */
  Hungarian h_assign(assignment, cost, distMatrix, nOfRows, nOfColumns);
  h_assign.computeAssignment();	

  for(row=0; row<nOfRows; row++)
    d_assignment[row] = assignment[row] + 1; /* MATLAB-Indexing */
   
  //mxFree(distMatrix);
  delete distMatrix;
	
return;
				
}
