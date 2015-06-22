BotMaster.o : BotMaster.cpp BotMaster.h BotSet.h
	g++ -c BotMaster.cpp

BotSet.o : BotSet.cpp BotSet.h Bot.h
	g++ -c BotSet.cpp

Bot.o : Bot.cpp Bot.h BotSet.h
	g++ -c Bot.cpp

test.o: test.cpp Bot.h BotMaster.h BotSet.h
	g++ -c test.cpp

test : test.o BotMaster.o Bot.o BotSet.o
	g++ test.o BotMaster.o Bot.o BotSet.o -o test