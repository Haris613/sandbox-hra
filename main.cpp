#include <iostream>
#include "menu.h"
using namespace std;

int main() {
	try {
		menu();
	}
	catch(char * err){
		cout << err;
	}
   return 0;
}