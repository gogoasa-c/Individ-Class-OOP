
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
	Individ(): age(-1), energy(-1), alive(false), i(-1), type('-') {} //constructor neparametrizat
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

	bool CheckForEnd();

	friend class Population;

	friend std::ostream& operator <<(std::ostream& out, const Individ& ind);
	friend std::istream& operator >>(std::istream& in, Individ& ind);
	Individ& operator =(const Individ& assign);
};

class Population {
	Individ pop[30];
public:

	void ReplaceIndividual(int pos, Individ newIndivid) {
		pop[pos] = newIndivid;
	}

	void PrintPopulation();

	void UpdatePopulation();

	bool CheckIfNoIndividual(int pos);

	bool CheckIfFull();

	bool CheckIfSameType();
};

void Population::PrintPopulation() {
	for (int i = 0; i < 30; i++) {
		if (pop[i].alive == true) {
			std::cout << pop[i];
		}
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
	out << "Individual " << ind.i+1 << " of type " << ind.type << " has an age of " << ind.age << " and " << ind.energy << " energy.\n";
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

void Individ::PrintInd() {
	std::cout << "Position: " << i << " Type: " << type << " Age: " << age << " Energy: " << energy << " Alive " << alive << "\n";
}

void Individ::Update() {
	this->Feed();
	this->Multiply();
	this->Attack();
	this->Age();
}

void Population::UpdatePopulation() {
	int posToUpdate[30] = { 0 };
	for (int i = 0; i < 30; i++) {
		if (pop[i].type != '-') {
			posToUpdate[i] = 1;
		}
	}
	for (int i = 0; i < 30; i++) {
		if (posToUpdate[i]) {
			if(pop[i].IsAlive()){
				pop[i].Update();
			}
		}
	}
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
				(*(this)).energy-=(*(this + 1)).energy;
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
	std::cout << "Set energy for individual";
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




void menu() {
	Population p;
	
	
	p.PrintPopulation();
	int first = 1;
	std::cout << "1. Play\n" << "2. Quit\n" << "Select option: ";
	int choice;
	std::cin >> choice;
	if (choice == 2) {
		return;
	}
	while (true) {
		int opt;
		std::cout << "1. Add new individual\n" << "2. Update population\n" << "3. Quit\n";
		std::cin >> opt;
		try {
			switch(opt){
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
						if(!first)
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
					return;
				default:
					throw(std::string("Invalid option!\n"));
			}
		}
		catch(const std::string exc){
			std::cout << exc << "\n";
		}
	}

}

int main()
{
	menu();
	
	return 0;
}
