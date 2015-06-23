#ifndef HUNGARIAN_ALGO_H_
#define HUNGARAIN_ALGO_H_

class Hungarian {

public:
  Hungarian(double *assignment, double *cost, double *distMatrix, double *distMatrixIn, int nOfRows, int nOfColumns);
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
	double *distMatrixEnd_, *assignment_, *cost_, *distMatrix_, *distMatrixIn_;
	
};

#endif  

