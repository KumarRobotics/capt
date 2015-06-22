#if ! defined _BOTMASTER_H
#define _BOTMASTER_H 1

#include <vector>

class Bot;

class BotMaster {
private:
	std::vector<Bot*> bots;
	double h;

public:
	BotMaster(double h);
	std::vector<Bot*> getC(Bot* i, double t);
	int addBot(Bot *b);
};
#endif