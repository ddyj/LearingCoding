#include <iostream>

using namespace std;


class base_class_no_virtual{
public:
        int val;
        char ch;
	void func(){
	}
};

class base_class{
public:
        int val;
        char ch;
        virtual void show(){
            std::cout << " base class" << std::endl;
        }
};


class son_1_class:public base_class{
public:
        int val;
        char ch;    
};
class son_2_class:public base_class{
public:
        int val;
        char ch;
        virtual void show()override {
            std::cout << " son class" << std::endl;
        }
};

int main(){
    base_class_no_virtual bcnv;
    base_class bc;
    son_1_class s1;
    son_2_class s2;
    return 0;
}
