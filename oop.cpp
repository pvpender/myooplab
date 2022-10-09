#include <iostream>
#include "functions.h"
#include <assert.h>
#include <limits.h>
#define DEBUG
#define DEBUG_NAME_1 "SomeName"
#define DEBUG_NAME_2 "Test"
#define DEBUG_NAME_3 "AnotherName"
#define DEBUG_AUTHOR_1 "SomeAuthor"
#define DEBUG_AUTHOR_2 "Author"
#define DEBUG_AUTHOR_3 "AnotherAuthor"
#define DEBUG_FILENAME "test.txt"
#define DEBUG_VAR_1 10
#define DEEBUG_VAR_2 -5
#define DEBUG_VAR_3 1

int main()
{
	Composition* comp1, * comp2;
	comp1 = new Composition();
	assert(comp1->getName() == NULL);
	assert(comp1->getAuthor() == NULL);
	assert(comp1->getFeelMood() == INT32_MIN);
	assert(comp1->getSpeedMood() == INT32_MIN);
	std::cout << "Test 1 complete\n";
	comp1->setName("SomeName");
	assert(strcmp(comp1->getName(), DEBUG_NAME_1) == 0);
	std::cout << "Test 2 complete\n";
	comp1->setAuthor("SomeAuthor");
	assert(strcmp(comp1->getAuthor(), DEBUG_AUTHOR_1) == 0);
	std::cout << "Test 3 complete\n";
	comp1->setFellMood(DEBUG_VAR_1);
	assert(comp1->getFeelMood() == DEBUG_VAR_1);
	std::cout << "Test 4 complete\n";
	comp1->setSpeedMood(DEEBUG_VAR_2);
	assert(comp1->getSpeedMood() == DEEBUG_VAR_2);
	std::cout << "Test 5 complete\n";
	delete comp1;
	comp1 = new Composition(DEBUG_NAME_2, DEBUG_AUTHOR_2, DEBUG_VAR_3, DEBUG_VAR_3);
	assert(strcmp(comp1->getName(), DEBUG_NAME_2) == 0);
	assert(strcmp(comp1->getAuthor(), DEBUG_AUTHOR_2) == 0);
	assert(comp1->getFeelMood() == DEBUG_VAR_3);
	assert(comp1->getSpeedMood() == DEBUG_VAR_3);
	std::cout << "Test 6 complete\n";
	comp2 = new Composition(*comp1);
	assert(strcmp(comp2->getName(), comp1->getName()) == 0);
	assert(strcmp(comp2->getAuthor(), comp1->getAuthor()) == 0);
	assert(comp2->getFeelMood() == comp1->getSpeedMood());
	assert(comp2->getSpeedMood() == comp1->getSpeedMood());
	comp2->setName(DEBUG_NAME_3);
	assert(strcmp(comp2->getName(), comp1->getName()) == -1);
	std::cout << "Test 7 complete\n";
	delete comp1;
	delete comp2;
	
	Playlist play;
	comp1 = new Composition(DEBUG_NAME_2, DEBUG_AUTHOR_2, DEBUG_VAR_1, 5);
	comp2 = new Composition(DEBUG_NAME_3, DEBUG_AUTHOR_3, 1, -7);
	play.add(*comp1);
	Playlist copyPlay(play);
	copyPlay.add(*comp2);
	copyPlay.write(DEBUG_FILENAME);
	copyPlay.delAll();
	assert(strcmp(play.getData(0).getName(), comp1->getName()) == 0);
	assert(strcmp(play.getData(0).getAuthor(), comp1->getAuthor()) == 0);
	assert(play.getData(0).getFeelMood() ==  comp1->getFeelMood());
	assert(play.getData(0).getSpeedMood() == comp1->getSpeedMood());
	std::cout << "Test 8 complete\n";
	play.add(*comp2);
	Composition *comp3 = new Composition("n", "i", 0, 0);
	Playlist play2(play);
	assert(play.compare(play2));
	play.insert(1, *comp3);
	assert(!play.compare(play2));
	std::cout << "Test 9 complete\n";
	Composition c = play.getListFeel(-3, 10);
	Composition c2 = play.getListSpeed(-100, 100);
	Playlist playTest(play);
	play.del(0);
	assert(!play.compare(playTest));
	std::cout << "Test 10 complete\n";
	play.del(0);
	play.insert(0, *comp1);
	Playlist play3;
	play3.read(DEBUG_FILENAME);
	assert(play.compare(play3));
	std::cout << "Test 11 complete\n";
	delete comp1;
	delete comp2;
	delete comp3;

	outPlaylist(play);
	Composition* comp = new Composition("Test", "Author", 1, 1);
	std::cout << comp->getName() << "\t" << comp->getAuthor() << std::endl;
	delete comp;
}
