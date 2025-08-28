#include <iostream>
#include <string.h>
using std::cout;
using std::endl;

class Computer{
public:
    //构造函数
    Computer(const char *brand,double price)
    :_brand(new char [strlen(brand)+1]())
    ,_price(price)
    {
        strcpy(_brand,brand);
        _totalPrice += _price;
    }
    //析构函数
    ~Computer(){
        if(_brand){
            delete []_brand;
            _brand=nullptr;
        }
    }
    //拷贝构造函数
    Computer(const Computer &rhs)
    :_brand(new char[strlen(rhs._brand)+1]())
    ,_price(rhs._price)
    {
        strcpy(_brand,rhs._brand);
        _totalPrice+=rhs._price;
    }
    //赋值运算符函数
    Computer &operator=(const Computer &rhs)
    {
        if(this!=&rhs){
            delete []_brand;
            _brand=new char[strlen(rhs._brand)+1]();
            strcpy(_brand,rhs._brand);
            _price=rhs._price;
        }
        _totalPrice+=rhs._price;
        return *this;
    }
    static void printTotalPrice()
	{
		cout << "totalPrice:" << _totalPrice << endl;
	}
private:
    char *_brand;
    double _price;
    static double _totalPrice;
};
double Computer::_totalPrice = 0;
void test(){
    Computer c1("acc",1500);
    Computer c2=c1;
    Computer c3("kkk",6000);
    Computer c4("kkk",6000);
    Computer c5("kkk",6000);
    Computer c6("kkk",6000);
    Computer::printTotalPrice();
}

int main()
{
    test();
    return 0;
}

