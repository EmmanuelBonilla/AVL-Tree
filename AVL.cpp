/**
* @ author: Emmanuel Bonilla (emmanuel.bonilla@wsu.edu)
* @ description: This program is designed to create an AVL tree
*
* compiling instructions: g++ -std=c++11 -Wall AVL.cpp -o AVL
*/

#include "AVL.h"

int main(void)
{
	AvlTree <int> manny;
	manny.readFile("test.txt");
	manny.print();
	manny.remove(4);
	manny.print();
	std::cout << manny.search(6) << std::endl;
	system("pause");
	return 0;
}
