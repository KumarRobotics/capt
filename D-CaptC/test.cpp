#include <iostream>
#include "Bot.h"
#include "BotMaster.h"
#include "BotSet.h"

using namespace std;

int main() {
	double start[] = {0, 0};
	double goal[] = {5, 5};
	BotMaster bm(2.0);

	Bot b1(1.0, start, goal, 2, &bm);
	Bot b2(1.0, (double[2]){2,3}, (double[2]){1, 3}, 2, &bm);
	Bot b3(1.0, (double[2]){5,5}, (double[2]){0, 0}, 2, &bm);

	Bot* botArr[] = {&b3, &b1, &b2};


	vector<Bot*> bots(botArr, botArr+3);

	for (double t = 0; t < 1.01; t+=0.1) {
		cout << t << endl;
		for (int i = 0; i < bots.size(); i++) {
			bots[i]->update(t);
		}
	}
	

	for (int i = 0; i < bots.size(); i++) {
		for (double t = 0; t < 1.01; t += 0.1) {
			
			bots[i]->print(t);
		}
		
	}


}