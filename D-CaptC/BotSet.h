#if ! defined _BOTSET_H
#define _BOTSET_H 1

#include "Bot.h"
#include <vector>

namespace BotSet {

	//all of these functions provide wrappers for the std::set_intersection,
	//std::set_union, std::set_difference, and std::sort functions for the case
	//of collections of type vector<Bot*>. All vectors are assumed to be sorted
	//prior to the calling of the function except for bot_sort.
	
	vector<Bot*> bot_intersection(vector<Bot*>* v1, vector<Bot*>* v2);
	vector<Bot*> bot_union(vector<Bot*>* v1, vector<Bot*>* v2);
	vector<Bot*> bot_difference(vector<Bot*>* v1, vector<Bot*>* v2);
	void bot_sort(vector<Bot*>* v);
}

#endif