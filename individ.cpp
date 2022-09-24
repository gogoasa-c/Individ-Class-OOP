#include "individ.h"

class Individ {


	int i; //pozitia dintr-un vector de 30 de elem
	char type;//tip individ: 0 sau +
	int age; //de la 0 pana la 80
	double energy;
	bool alive;

	void Feed();
	void Multiply();
	void Attack();
	void Age();
	void Die();

	void Child(Individ ind, int newPos);

public:
	Individ() : age(-1), energy(-1), alive(false), i(-1), type('-') {} //constructor neparametrizat
	Individ(int newPos, char newType) : age(0), energy(10), alive(true), i(newPos), type(newType) {} //constructor parametrizat
	Individ(const Individ& newInd) : i(newInd.i), type(newInd.type), age(newInd.age), energy(newInd.energy), alive(newInd.alive) {} //constructor copiere
	~Individ() {}

	void Update();
	bool IsAlive();
	char GetType();
	int GetAge();
	double GetEnergy();
	int GetPos();
	void PrintInd();
	void SetPos();
	void SetType();
	void SetAge();
	void SetEnergy();
	void SetAlive();



	friend class Population;

	friend std::ostream& operator <<(std::ostream& out, const Individ& ind);
	friend std::istream& operator >>(std::istream& in, Individ& ind);
	Individ& operator =(const Individ& assign);
	Individ& operator +(const Individ& toAdd);
};

class Population {
	Individ pop[30];
public:

	void ReplaceIndividual(int pos, Individ newIndivid) {
		pop[pos] = newIndivid;
	}

	void PrintPopulation();

	void UpdatePopulation();
	
	void UpdatePopulationMulti(int times); 

	bool CheckIfNoIndividual(int pos);

	bool CheckIfFull();

	bool CheckIfSameType();

	void ClearPopulation();
};



void Population::PrintPopulation() {
	for (int i = 0; i < 30; i++) {
		if (pop[i].alive == true) {
			std::cout << pop[i];
		}
	}
}

void Population::ClearPopulation() {
	for (int i = 0; i < 30; i++) {
		pop[i].type = '-';
		pop[i].age = -1;
		pop[i].alive = false;
		pop[i].energy = -1;
	}
}

bool Population::CheckIfNoIndividual(int pos) {
	return pop[pos].alive == false;
}

bool Population::CheckIfFull() {
	for (int i = 0; i < 30; i++) {
		if (!pop[i].IsAlive()) {
			return false;
		}
	}
	return true;
}

bool Population::CheckIfSameType() {
	if (!this->CheckIfFull()) {
		return false;
	}
	char tip = pop[0].type;
	for (int i = 1; i < 30; i++) {
		if (pop[i].type != tip) {
			return false;
		}
	}
	return true;

}

std::ostream& operator <<(std::ostream& out, const Individ& ind) {
	out << "Individual " << ind.i + 1 << " of type " << ind.type << " has an age of " << ind.age << " and " << ind.energy << " energy.\n";
	return out;
}
std::istream& operator >>(std::istream& in, Individ& ind) {
	int newPos;
	std::cout << "Pick a position for individual (1 to 30): ";
	in >> newPos;
	ind.i = newPos - 1;
	std::cout << "Pick a type for you individual (0 or +): ";
	in >> ind.type;
	ind.age = 0;
	ind.energy = 10;
	ind.alive = true;
	return in;
}


Individ& Individ::operator =(const Individ& assign) {
	type = assign.type;
	age = assign.age;
	alive = assign.alive;
	energy = assign.energy;
	i = assign.i;
	return *this;
}
Individ& Individ::operator +(const Individ& toAdd) {
	Individ newIndivid;
	if (toAdd.type == type) {
		newIndivid.type = type;
	}
	else {
		if (rand() % 2 == 1) {
			newIndivid.type = '+';
		}
		else {
			newIndivid.type = '0';
		}
	}
	newIndivid.age = age + toAdd.age;
	newIndivid.alive = 1;
	newIndivid.energy = energy + toAdd.energy;
	newIndivid.i = i;
	return newIndivid;
}

void Individ::PrintInd() {
	std::cout << "Position: " << i << " Type: " << type << " Age: " << age << " Energy: " << energy << " Alive " << alive << "\n";
}

void Individ::Update() {
	this->Feed();
	this->Multiply();
	this->Attack();
	this->Age();
}



//done
void Individ::Age() {//done
	++age;
	energy -= 5;
	if (energy <= 0) {
		this->Die();
	}
}

//done
void Individ::Die() {
	alive = false;
	age = -1;
	i = -1;
	energy = -1;
	type = '-';
}

//done
void Individ::Feed() {
	energy += rand() % 10 + 1; //creste energia cu o valoare intre 1 si 10
}

void Individ::Child(Individ ind, int newPos) {
	this->age = 0;
	this->i = newPos;
	this->type = ind.GetType();
	this->energy = 2 * ind.GetEnergy();
	this->alive = true;
}

void Individ::Multiply() {
	if ((*(this)).GetPos() == 0) {
		if ((*(this + 1)).GetAge() == -1) {
			(this + 1)->Child((*(this)), this->GetPos() + 1);
			/*
			sau alternativ:
			Individ aux = new Individ(this->GetPos() + 1, this->GetType());
			aux.SetEnergy(2 * this->GetEnergy());
			(this+1) = aux;
			*/
		}
	}
	else if ((*(this)).GetPos() == 29) {
		if ((*(this - 1)).GetAge() == -1) {
			(this - 1)->Child((*(this)), this->GetPos() - 1);
		}
	}
	else {
		if ((*(this + 1)).GetAge() == -1) {
			(this + 1)->Child((*(this)), this->GetPos() + 1);
		}
		if ((*(this - 1)).GetAge() == -1) {
			(this - 1)->Child((*(this)), this->GetPos() - 1);
		}
	}
}

void Individ::Attack() {
	if ((*(this)).GetPos() == 0) {
		if ((((*(this + 1)).type != '-' && ((*(this)).type != '-')) && ((*(this + 1)).type != (*(this)).GetType())))
		{ // if they are types that are not '-' (dead) AND they're different
			if ((*(this)).energy > (*(this + 1)).energy) {
				(*(this)).energy -= (*(this + 1)).energy;
			}
			else if ((*(this)).energy == (*(this + 1)).energy) {
				this->Die();
				(this + 1)->Die();
			}
			else {
				(*(this + 1)).energy -= (*(this)).energy;
			}
		}
	}
	else if ((*(this)).GetPos() == 29) {
		if ((((*(this - 1)).type != '-' && ((*(this)).type != '-')) && ((*(this + 1)).type != (*(this)).GetType())))
		{ // if they are types that are not '-' (dead) AND they're different
			if ((*(this)).energy > (*(this - 1)).energy) {
				(*(this)).energy -= (*(this - 1)).energy;
			}
			else if ((*(this)).energy == (*(this - 1)).energy) {
				this->Die();
				(this - 1)->Die();
			}
			else {
				(*(this - 1)).energy -= (*(this)).energy;
			}
		}
	}
	else {
		if ((((*(this + 1)).type != '-' && ((*(this)).type != '-')) && ((*(this + 1)).type != (*(this)).GetType())))
		{ // if they are types that are not '-' (dead) AND they're different
			if ((*(this)).energy > (*(this + 1)).energy) {
				(*(this)).energy -= (*(this + 1)).energy;
			}
			else if ((*(this)).energy == (*(this + 1)).energy) {
				this->Die();
				(this + 1)->Die();
			}
			else {
				(*(this + 1)).energy -= (*(this)).energy;
			}
		}
		if ((((*(this - 1)).type != '-' && ((*(this)).type != '-')) && ((*(this + 1)).type != (*(this)).GetType())))
		{ // if they are types that are not '-' (dead) AND they're different
			if ((*(this)).energy > (*(this - 1)).energy) {
				(*(this)).energy -= (*(this - 1)).energy;
			}
			else if ((*(this)).energy == (*(this - 1)).energy) {
				this->Die();
				(this - 1)->Die();
			}
			else {
				(*(this - 1)).energy -= (*(this)).energy;
			}
		}
	}
}

//done
bool Individ::IsAlive() {
	return alive;
}

//done
char Individ::GetType() {
	return type;
}

//done
int Individ::GetAge() {
	return age;
}

//done
int Individ::GetPos() {
	return i;
}

//done
double Individ::GetEnergy() {
	return energy;
}

//done
void Individ::SetPos() {
	std::cout << "New position for individual (from 0 to 30): ";
	std::cin >> i;
}

void Individ::SetType() {
	std::cout << "Set type for individual (0 or + only): ";
	std::cin >> type;
}

void Individ::SetAge() {
	std::cout << "Set age for individual (from 0 to 80): ";
	std::cin >> age;
}

void Individ::SetEnergy() {
	std::cout << "Set energy for individual: ";
	std::cin >> energy;
}

void Individ::SetAlive() {
	int aux;
	std::cout << "Set whether or not the individual is alive (0 - dead, 1 - alive): ";
	std::cin >> aux;
	if (aux) {
		alive = true;
	}
	else {
		alive = false;
	}
}

int TitleScreen() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "\t  * * * * *\n";
	std::cout << "\t* Welcome to *\n";
	SetConsoleTextAttribute(hConsole, 3);
	std::cout << "\t  P";
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << "O";
	SetConsoleTextAttribute(hConsole, 5);
	std::cout << "P";
	SetConsoleTextAttribute(hConsole, 6);
	std::cout << "U";
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "L";
	SetConsoleTextAttribute(hConsole, 8);
	std::cout << "A";
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "T";
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "I";
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << "O";
	SetConsoleTextAttribute(hConsole, 12);
	std::cout << "N\n";
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "1. Play\n" << "2. Quit\n" << "Select option: ";
	int choice;
	std::cin >> choice;
	if (choice == 2) {
		exit;
	}
	return choice;
}

void PlayMenu(Population p) {
	int first = 1;
	while (true) {
		int opt;
		std::cout << "1. Add new individual\n" << "2. Update population\n" << "3. Update population (multiple times, at most 100)\n" << "4. Quit\n";
		std::cin >> opt;
		try {
			switch (opt) {
			case 1:
				try
				{
					Individ aux;
					std::cin >> aux;
					if (aux.GetPos() < 0 || aux.GetPos() >= 30 || (aux.GetType() != '+' && aux.GetType() != '0')) {
						throw std::string("Invalid position or type!\n");
					}
					if (!p.CheckIfNoIndividual(aux.GetPos())) {
						throw std::string("Position occupied already!\n");
					}
					p.ReplaceIndividual(aux.GetPos(), aux);
					if (!first)
					{
						p.UpdatePopulation();
					}
					p.PrintPopulation();
					first = 0;
				}
				catch (const std::string exc) {
					std::cout << exc;
				}
				break;
			case 2:
				p.UpdatePopulation();
				p.PrintPopulation();
				break;
			case 3:
				int times;
				std::cout << "How many times to update population?\n";
				std::cin >> times;
				if (times > 100) {
					throw std::string("Too many updates!");
				}
				p.UpdatePopulationMulti(times);
				
				break;
			case 4:
				return;
			default:
				throw(std::string("Invalid option!\n"));
			}
		}
		catch (const std::string exc) {
			std::cout << exc << "\n";
		}
	}
}

void menu() {
	Population p;
	int choice = TitleScreen();

	if (choice == 2) {
		return;
	}
	PlayMenu(p);
}

int posToUpdate[30] = { 0 };

void Population::UpdatePopulationMulti(int times) {
	for (int i = 0; i < times; i++) {		
		for (int i = 0; i < 30; i++) {
			if (pop[i].type != '-') {
				++posToUpdate[i];
			}
		}
		for (int i = 0; i < 30; i++) {
			if (posToUpdate[i] != 0) {
				--posToUpdate[i];
				if (pop[i].IsAlive()) {
					pop[i].Update();
				}
			}
		}
	}
	if (this->CheckIfSameType()) {
		std::cout << "\t Game is over!\n";
		printf_s("\tThe %c's have won!\n", pop[0].type);
		this->ClearPopulation();
		menu();
	}
	else {
		this->PrintPopulation();
	}
}

void Population::UpdatePopulation() {
	int posToUpdate[30] = { 0 };
	for (int i = 0; i < 30; i++) {
		if (pop[i].type != '-') {
			++posToUpdate[i];
		}
	}
	for (int i = 0; i < 30; i++) {
		if (posToUpdate[i] != 0) {
			if (pop[i].IsAlive()) {
				pop[i].Update();
			}
		}
	}
	if (this->CheckIfSameType()) {
		std::cout << "\t Game is over!\n";
		printf_s("\tThe %c's have won!\n", pop[0].type);
		this->ClearPopulation();
		menu();
	}
}


int main()
{
	menu();
	return 0;
}
