#include "Bot.h"
#include "BotSet.h"
#include <iostream>
#include <algorithm>

vector<Bot*> BotSet::bot_intersection(vector<Bot*>* v1, vector<Bot*>* v2) {

		vector<Bot*> result(std::max(v1->size(), v2->size()));

		vector<Bot*>::iterator it;



		it = std::set_intersection(v1->begin(), v1->end(), v2->begin(), v2->end(), 
			result.begin(), Bot::lessThan);
		

		result.resize(it-result.begin());



		return result;


} 

vector<Bot*> BotSet::bot_union(vector<Bot*>* v1, vector<Bot*>* v2) {

		vector<Bot*> result(v1->size() + v2->size());

		vector<Bot*>::iterator it;



		it = std::set_union(v1->begin(), v1->end(), v2->begin(), v2->end(), 
			result.begin(), Bot::lessThan);
		

		result.resize(it-result.begin());



		return result;


} 

vector<Bot*> BotSet::bot_difference(vector<Bot*>* v1, vector<Bot*>* v2) {

		vector<Bot*> result(std::max(v1->size(), v2->size()));

		vector<Bot*>::iterator it;



		it = std::set_difference(v1->begin(), v1->end(), v2->begin(), v2->end(), 
			result.begin(), Bot::lessThan);
		

		result.resize(it-result.begin());



		return result;

}

void BotSet::bot_sort(vector<Bot*>* v) {
	std::sort(v->begin(), v->end(), Bot::lessThan);
} 
