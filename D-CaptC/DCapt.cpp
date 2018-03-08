#include "mex.h"
#include "matrix.h"

#include "Bot.h"
#include "BotMaster.h"
#include <vector>
#include <iostream>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{

	//Extract information from input arguments
	double *start_arg = mxGetPr(prhs[0]);
	int numBots = mxGetM(prhs[0]);
	int dim = mxGetN(prhs[0]);

	double *goal_arg = mxGetPr(prhs[1]);

	double h = mxGetScalar(prhs[3]);
	double r = mxGetScalar(prhs[2]);

	//create variables to copy input arguments into to avoid changing input args
	double *start = new double[numBots*dim];
	double *goal = new double[numBots*dim];

	//Check to make sure start and goal locations are valid
	for (int i = 0; i < numBots; i++) {
		
		for (int j = i+1; j < numBots; j++) {

				double dStart = 0;
				double dGoal = 0;
				for (int d = 0; d < dim; d++) {

					dStart += (start_arg[d*numBots + i]-start_arg[d*numBots+j])*
							  (start_arg[d*numBots + i]-start_arg[d*numBots+j]);
					dGoal += (goal_arg[d*numBots + i]-goal_arg[d*numBots+j])*
							  (goal_arg[d*numBots + i]-goal_arg[d*numBots+j]);


				}

				if (dStart < 8*r*r) {
					mexErrMsgTxt("Start locations must be at least 2*sqrt(2)*R apart");
				}
				if (dGoal < 8*r*r) {
					mexErrMsgTxt("Goal locations must be at least 2*sqrt(2)*R apart");
				}
			}
	}

	//copy start and goal locations into new arrays, changing col-major to row-major
	for (int i = 0; i < numBots; i++) {
		
		for (int j = 0; j < dim; j++) {

			start[i*dim + j] = start_arg[j*numBots + i];
			
			goal[i*dim + j] = goal_arg[j*numBots + i];

			
		}
		
	}
	

	//create output cell array
	int dims[] = {1, numBots};
	mxArray* trajs = mxCreateCellArray(2, dims);


	//create BotMaster and Bots
	vector<Bot*> bots;
	bots.reserve(numBots);

	BotMaster bm(h);

	for (int i = 0; i < numBots; i++) {
		bots[i] = new Bot(1.0, start + i*dim, goal + i*dim, dim, &bm);;
	}

	delete[] start;
	delete[] goal;

	//Run update loop to simulate the motion of the bots
	for (double t = 0; t < 1.01; t+=0.01) {
		for (int i = 0; i < numBots; i++) {
			bots[i]->update(t);
		}
	}

	//Copy data about where the bots went into output cell array
	for (int n = 0; n < numBots; n++) {
		Bot b = *bots[n];
		int M = b.getTrajRows();
		mxArray* temp = mxCreateDoubleMatrix(M, dim, mxREAL);
		double* temp_ptr = mxGetPr(temp);
		
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < dim; j++) {
				temp_ptr[j*M + i] = b.traveled[i*dim + j];
				
			}
			
		}
		mxSetCell(trajs, n, temp);
	}
	plhs[0] = trajs;

}
