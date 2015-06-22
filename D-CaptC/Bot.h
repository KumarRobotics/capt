#if ! defined _BOT_H
#define _BOT_H
#include <vector>



using namespace std;

class BotMaster;

class Bot {
private:
	int timeStops;
	double tf, tl;


	double *traj, *timeSweep;
	vector<Bot*> cLast, updateL;
	BotMaster* bm;



public:
	int dim;
	int id;
	double* goal;
	double* traveled;

	Bot(double tf, double* start, double* goal, int dim, BotMaster* bm);

	double* pos(double t);

	void update(double t);

	void swap(double* newGoal, double t);

	void computeTraj(double t);

	bool equals(Bot* i);

	void print();

	void print(double t);

	int getTrajRows();

	static bool lessThan(Bot* i, Bot* j);

	static double dist(Bot* b1, Bot* b2, double t);

	static double* r(Bot* b1, Bot* b2, double t);

	static double* u(Bot* b1, Bot* b2, double t);

	static double* w(Bot* b1, Bot* b2);



};
#endif