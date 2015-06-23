#include <stdio.h>
#include <limits>       // std::numeric_limits
	
#include <hungarian.h>
	
Hungarian::Hungarian(double *assignment, double *cost, double *distMatrix, double *distMatrixIn, int nOfRows, int nOfColumns)
{
  nOfRows_ = nOfRows;
  nOfColumns_ = nOfColumns;
  assignment_ = assignment; 
  cost_ = cost;
  distMatrix_ = distMatrix;
  distMatrixIn_ = distMatrixIn;
	
	nOfElements_   = nOfRows_ * nOfColumns_;
	distMatrixEnd_ = distMatrix_ + nOfElements_;	
							
	/* memory allocation */		
	coveredColumns_ = new bool [nOfColumns_]();
	coveredRows_    = new bool [nOfRows_]();
	starMatrix_     = new bool [nOfElements_]();
	primeMatrix_    = new bool [nOfElements_]();
	newStarMatrix_  = new bool [nOfElements_](); 

}

void Hungarian::computeAssignment()
{
  double value, minValue;
	int row, col;
	double  *columnEnd, *distMatrixTemp;
	  
	/* preliminary steps */
	if(nOfRows_ <= nOfColumns_)
	{
		minDim_ = nOfRows_;
		
		for(row=0; row<nOfRows_; row++)
		{
			/* find the smallest element in the row */
			distMatrixTemp = distMatrix_ + row;
			minValue = *distMatrixTemp;
			distMatrixTemp += nOfRows_;			
			while(distMatrixTemp < distMatrixEnd_)
			{
				value = *distMatrixTemp;
				if(value < minValue)
					minValue = value;
				distMatrixTemp += nOfRows_;
			}
			
			/* subtract the smallest element from each element of the row */
			distMatrixTemp = distMatrix_ + row;
			while(distMatrixTemp < distMatrixEnd_)
			{
				*distMatrixTemp -= minValue;
				distMatrixTemp += nOfRows_;
			}
		}
		
		/* Steps 1 and 2a */
		for(row=0; row<nOfRows_; row++)
			for(col=0; col<nOfColumns_; col++)
				if(distMatrix_[row + nOfRows_*col] == 0)
					if(!coveredColumns_[col])
					{
						starMatrix_[row + nOfRows_*col] = true;
						coveredColumns_[col]           = true;
						break;
					}
	}
	else /* if(nOfRows_ > nOfColumns_) */
	{
		minDim_ = nOfColumns_;
		
		for(col=0; col<nOfColumns_; col++)
		{
			/* find the smallest element in the column */
			distMatrixTemp = distMatrix_     + nOfRows_*col;
			columnEnd      = distMatrixTemp + nOfRows_;
			
			minValue = *distMatrixTemp++;			
			while(distMatrixTemp < columnEnd)
			{
				value = *distMatrixTemp++;
				if(value < minValue)
					minValue = value;
			}
			
			/* subtract the smallest element from each element of the column */
			distMatrixTemp = distMatrix_ + nOfRows_*col;
			while(distMatrixTemp < columnEnd)
				*distMatrixTemp++ -= minValue;
		}
		
		/* Steps 1 and 2a */
		for(col=0; col<nOfColumns_; col++)
			for(row=0; row<nOfRows_; row++)
				if(distMatrix_[row + nOfRows_*col] == 0)
					if(!coveredRows_[row])
					{
						starMatrix_[row + nOfRows_*col] = true;
						coveredColumns_[col]           = true;
						coveredRows_[row]              = true;
						break;
					}
		for(row=0; row<nOfRows_; row++)
			coveredRows_[row] = false;
		
	}	
	
	/* move to step 2b */
	step2b();

	/* compute cost and remove invalid assignments */
	computeassignmentcost();
	
return;

}

Hungarian::~Hungarian()
{
	/* free allocated memory */
	delete coveredColumns_;	
	delete coveredRows_;
	delete starMatrix_;
	delete primeMatrix_;
	delete newStarMatrix_;	
}

/********************************************************/
void Hungarian::buildassignmentvector()
{
	int row, col;
	
	for(row=0; row<nOfRows_; row++)
		for(col=0; col<nOfColumns_; col++)
			if(starMatrix_[row + nOfRows_*col])
			{
				assignment_[row] = col;
				break;
			}
}

/********************************************************/
void Hungarian::computeassignmentcost()
{
	int row, col;
	
	for(row=0; row<nOfRows_; row++)
	{
		col = assignment_[row];

		if(col >= 0)
		{
			*cost_ += distMatrixIn_[row + nOfRows_*col]; 	
		}
	}

}

/********************************************************/
void Hungarian::step2a()
{
	bool *starMatrixTemp, *columnEnd;
	int col;
	
	/* cover every column containing a starred zero */
	for(col=0; col<nOfColumns_; col++)
	{
		starMatrixTemp = starMatrix_     + nOfRows_*col;
		columnEnd      = starMatrixTemp + nOfRows_;
		while(starMatrixTemp < columnEnd){
			if(*starMatrixTemp++)
			{
				coveredColumns_[col] = true;
				break;
			}
		}	
	}

	/* move to step 3 */
	step2b();
}

/********************************************************/
void Hungarian::step2b()
{
	int col, nOfCoveredColumns;
	
	/* count covered columns */
	nOfCoveredColumns = 0;
	for(col=0; col<nOfColumns_; col++)
		if(coveredColumns_[col])
			nOfCoveredColumns++;
			
	if(nOfCoveredColumns == minDim_)
	{
		/* algorithm finished */
		buildassignmentvector();
	}
	else
	{
		/* move to step 3 */
		step3();
	}
	
}

/********************************************************/
void Hungarian::step3()
{
	bool zerosFound;
	int row, col, starCol;

	zerosFound = true;
	while(zerosFound)
	{
		zerosFound = false;		
		for(col=0; col<nOfColumns_; col++)
			if(!coveredColumns_[col])
				for(row=0; row<nOfRows_; row++)
					if((!coveredRows_[row]) && (distMatrix_[row + nOfRows_*col] == 0))
					{
						/* prime zero */
						primeMatrix_[row + nOfRows_*col] = true;
						
						/* find starred zero in current row */
						for(starCol=0; starCol<nOfColumns_; starCol++)
							if(starMatrix_[row + nOfRows_*starCol])
								break;
						
						if(starCol == nOfColumns_) /* no starred zero found */
						{
							/* move to step 4 */
							step4(row, col);
							return;
						}
						else
						{
							coveredRows_[row]        = true;
							coveredColumns_[starCol] = false;
							zerosFound              = true;
							break;
						}
					}
	}
	
	/* move to step 5 */
	step5();
}

/********************************************************/
void Hungarian::step4(int row, int col)
{	
	int n, starRow, starCol, primeRow, primeCol;
	//int nOfElements_ = nOfRows_*nOfColumns_;
	
	/* generate temporary copy of starMatrix_ */
	for(n=0; n<nOfElements_; n++)
		newStarMatrix_[n] = starMatrix_[n];
	
	/* star current zero */
	newStarMatrix_[row + nOfRows_*col] = true;

	/* find starred zero in current column */
	starCol = col;
	for(starRow=0; starRow<nOfRows_; starRow++)
		if(starMatrix_[starRow + nOfRows_*starCol])
			break;

	while(starRow<nOfRows_)
	{
		/* unstar the starred zero */
		newStarMatrix_[starRow + nOfRows_*starCol] = false;
	
		/* find primed zero in current row */
		primeRow = starRow;
		for(primeCol=0; primeCol<nOfColumns_; primeCol++)
			if(primeMatrix_[primeRow + nOfRows_*primeCol])
				break;
								
		/* star the primed zero */
		newStarMatrix_[primeRow + nOfRows_*primeCol] = true;
	
		/* find starred zero in current column */
		starCol = primeCol;
		for(starRow=0; starRow<nOfRows_; starRow++)
			if(starMatrix_[starRow + nOfRows_*starCol])
				break;
	}	

	/* use temporary copy as new starMatrix_ */
	/* delete all primes, uncover all rows */
	for(n=0; n<nOfElements_; n++)
	{
		primeMatrix_[n] = false;
		starMatrix_[n]  = newStarMatrix_[n];
	}
	for(n=0; n<nOfRows_; n++)
		coveredRows_[n] = false;
	
	/* move to step 2a */
	step2a();
}

/********************************************************/
void Hungarian::step5()
{
	double h, value;
	int row, col;
	
	/* find smallest uncovered element h */
	h = std::numeric_limits<int>::max(); //TODO  use #define or MAX_DBL for faster assignment_?
	for(row=0; row<nOfRows_; row++)
		if(!coveredRows_[row])
			for(col=0; col<nOfColumns_; col++)
				if(!coveredColumns_[col])
				{
					value = distMatrix_[row + nOfRows_*col];
					if(value < h)
						h = value;
				}
	
	/* add h to each covered row */
	for(row=0; row<nOfRows_; row++)
		if(coveredRows_[row])
			for(col=0; col<nOfColumns_; col++)
				distMatrix_[row + nOfRows_*col] += h;
	
	/* subtract h from each uncovered column */
	for(col=0; col<nOfColumns_; col++)
		if(!coveredColumns_[col])
			for(row=0; row<nOfRows_; row++)
				distMatrix_[row + nOfRows_*col] -= h;
	
	/* move to step 3 */
	step3();
}

