#include "Bot.h"
#include "BotMaster.h"
#include <vector>
#include <math.h>
#include <iostream>
#include "BotSet.h"

using namespace std;

typedef vector<Bot*>::iterator bot_iter_t;

Bot::Bot(double tf, double* start, double* goal, int dim, BotMaster* bm): tf(tf),
														goal(goal), dim(dim), bm(bm)
{
	
	//Generate time sweep
	timeStops = 100*tf+1;
	timeSweep = new double[timeStops];
	int i = 0;
	for (double t = 0; tf-t > -0.001; t += 0.01) {
		timeSweep[i] = t;
		
		i++;
	}

	//initialize traj
	traj = new double[timeStops*dim];
	tl = 0;

	//add this bot to the bot master and set the id recieved from the bot master
	id = bm->addBot(this);

	//initialize traveled
	traveled = new double[timeStops*dim];

	//set the starting position in traveled and traj
	for (int i = 0; i < dim; i++) {
		traveled[i] = start[i];
		traj[i] = start[i];
	}

	//compute the trajectory
	computeTraj(0);

	

}

double* Bot::pos(double t) {

	//caluclate index of traj corresponding to the time
	int i = round(t/tf*(timeStops-1));
	

	//copy the position into a new variable
	double* temp = new double[dim];
	copy(traj+i*dim, traj+(i+1)*dim, temp);
	//memcpy(temp, &(traj[i*dim]), sizeof(double)*dim);
	return temp;
}

void Bot::update(double t) {

	//calculate index of traj corresponding to the current time and the last 
	//time update was called on
	int iLast = (int)round(tl/tf*(timeStops-1));
	int iCur = (int)round(t/tf*(timeStops-1));

	
	//set the last time to this time
	tl = t;


	
	//add the trajectory traveled in between tl and t to traveled
	copy(traj+iLast*dim, traj+(iCur+1)*dim, traveled+iLast*dim);
	
	//Get the bots within the communication range
	vector<Bot*> C = bm->getC(this, t);

	//Caulcute the current update list
	vector<Bot*> temp = BotSet::bot_difference(&C, &cLast);

	updateL = BotSet::bot_union(&temp, &updateL);

	updateL = BotSet::bot_intersection(&C, &updateL);



	double* newGoal = new double[dim];

	//go through updaste list checking for potential swaps until exhausted
	while (updateL.size() > 0) {
		Bot* j = updateL.back();

		double* u = Bot::u(this, j, t);
		double* w = Bot::w(this, j);

		double p = 0;

		for (int d = 0; d < dim; d++) {
			p += u[d]*w[d];
		}


		//If p < 0 this bot and and bot j need to swap
		if (p < 0) {
			
			
			//swap
			copy(j->goal, j->goal+dim, newGoal);
			j->swap(goal, t);
			copy(newGoal, newGoal+dim, goal);
			
			//reset update list
			updateL = C;

			//recompute trajectory
			computeTraj(t);


		}

		delete[] u;
		delete[] w;

		//find j and delete it
		int in = 0;
		for (int i = updateL.size()-1; i >= 0; i--) {
			if (j->equals(updateL[i])) {
				in = i;
				break;
			}
		}
		updateL.erase(updateL.begin()+in);

	}

	
	//Set cLast to C
	cLast = C;



	delete[] newGoal;
	
	

}

void Bot::swap(double* newGoal, double t) {

	//copy the new goal into this position
	copy(newGoal, newGoal+dim, goal);

	//recompute trajectory
	computeTraj(t);
}

void Bot::computeTraj(double t) {
	//get the current position
	double* current = pos(t);

	//update all of traj to respect the current position and the new goal
	for (int i = 0; i < timeStops; i++) {
		double frac = (timeSweep[i]-t)/(tf-t);
		for (int d = 0; d < dim; d++) {
			traj[i*dim + d] = (goal[d]-current[d])*frac+current[d];
		}

	}
	delete[] current;
}

double Bot::dist(Bot* b1, Bot* b2, double t) {
	int dim = b1->dim;

	double dist = 0;

	double* p1 = b1->pos(t);
	double* p2 = b2->pos(t);

	for (int d = 0; d < dim; d++) {
		dist += (p1[d]-p2[d])*(p1[d]-p2[d]);
	}

	delete[] p1;
	delete[] p2;

	return sqrt(dist);
}

double* Bot::r(Bot* bi, Bot* bj, double t) {

	int dim = bi->dim;


	double* r = new double[dim];
	double* p = bi->pos(t);

	for (int d = 0; d < dim; d++) {
		r[d] = bj->goal[d]-p[d];
	}

	delete[] p;

	return r;
}

double* Bot::u(Bot* bi, Bot* bj, double t) {

	int dim = bi->dim;
	double* u = new double[dim];
	double* pi = bi->pos(t);
	double* pj = bj->pos(t);


	for (int d = 0; d < dim; d++) {
		u[d] = pj[d]-pi[d];
	}

	delete[] pi;
	delete[] pj;

	return u;
}

double* Bot::w(Bot* bi, Bot* bj) {

	int dim = bi->dim;
	double* w = new double[dim];

	for (int d = 0; d < dim; d++) {
		w[d] = bj->goal[d] - bi->goal[d];
	}

	return w;
}

bool Bot::equals(Bot* i) {
	return id == i->id;
}

void Bot::print() {
	cout << "Bot " << id << endl;
}

void Bot::print(double t) {
	int i = round(t/tf*(timeStops-1));
	cout << "Bot " << id << ": " << traveled[i*2] << ", " << traveled[i*2+1] << endl; 
}

bool Bot::lessThan(Bot* i, Bot* j) {
	return i->id < j->id;
}

int Bot::getTrajRows() {
	return timeStops;
}



