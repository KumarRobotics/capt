#ifndef HUNGARIAN_ALGO_H_
#define HUNGARAIN_ALGO_H_

class Hungarian {

public:
  /*
  assignment - Pointer to double array  1 x nOfRows that gives the indices of final assignemnt
  cost - Total path cost of the final assignment
  distMatrix - Pointer to the distance matrix - double array of size 1 x nOfRows * nOfColumns (Column-major order from Matlab)
  nOfRows - number of robots
  nOfColumns - number of goals
  */
  Hungarian(int *assignment, double *cost, double *distMatrixIn, int nOfRows, int nOfColumns);
  ~Hungarian();
  void computeAssignment();
  

private:

  void buildassignmentvector();
  void computeassignmentcost();
  void step2a();
  void step2b();
  void step3();
  void step4(int row, int col);
  void step5();

	int minDim_, nOfColumns_, nOfRows_, nOfElements_;
					
	bool *coveredColumns_, *coveredRows_, *starMatrix_, *newStarMatrix_, *primeMatrix_;
	double *distMatrixEnd_, *cost_, *distMatrix_, *distMatrixIn_;
	int *assignment_;
	
};

#endif  

