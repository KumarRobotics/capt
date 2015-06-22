#include "BotMaster.h"
#include "BotSet.h"
#include "Bot.h"
#include <vector>


using namespace std;

BotMaster::BotMaster(double h): h(h) {};


vector<Bot*> BotMaster::getC(Bot* b, double t) {
	vector<Bot*> C;
	C.reserve(bots.size()-1);
	for (int i = 0; i < bots.size(); i++) {
		if (!bots[i]->equals(b)) {
			double d = Bot::dist(bots[i], b, t);
			if (d < h) 
				C.push_back(bots[i]);
		}
	}
	return C;
}
int BotMaster::addBot(Bot *b) {
	bots.push_back(b);
	BotSet::bot_sort(&bots);
	return bots.size();
}
