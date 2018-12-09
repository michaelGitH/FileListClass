#include <iostream>
#include <iomanip>

template<typename T>
void displayBits(T value); //распечатать биты

void ConsolePause();

int main(){

	for(int i = 0; i <= 10; ++i)
		displayBits((int)-i);

	ConsolePause();

	return 0;
}

template<typename T>
void displayBits(T value){
	
	bool negative = value < 0;
	if(negative)
		value = -value;

	const int SHIFT = 8 * sizeof(T) - 1;

	const unsigned MASK = 1 << SHIFT;

	std::cout << std::endl << std::setw(10) << value << " = ";
	
	//вывести биты
	if(negative){
		std::cout << '1';
		value <<= 1;
	}
	for(int i = (negative? 2: 1); i <= SHIFT+1; ++i){
		std::cout << (value & MASK ? '1' : '0');
		value <<= 1; // value = value << 1 : Сдвинуть значение влево на 1
		if(i % 8 == 0)
			printf(" ");
	}
	printf("\n");
}

void ConsolePause(){
	if(!std::cin.good())
		std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
}