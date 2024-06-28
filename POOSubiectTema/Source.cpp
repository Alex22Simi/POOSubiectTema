#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<fstream>
#include<map>
using namespace std;


class ExceptieCustom : public exception {
	string mesaj;
public:
	ExceptieCustom() {
		this->mesaj = "Nu stiu ce eroare e!";
	}
	ExceptieCustom(string mesaj) {
		this->mesaj = mesaj;
	}
	string afiseazaEroarea() {
		return this->mesaj;
	}
};
class Tema {

private:
	const int idTema;
protected:
	char* materie;
	string tipTema;//doc,pdf, ppt
	float punctaj;
public:
	static int nrTemeTrimise;

	//constructorul fara parametrii/default/implicit
	Tema() :idTema(nrTemeTrimise++) {
		this->materie = new char[strlen("Anonim") + 1];//asa alocam memorie pentru vector
		strcpy(this->materie, "Anonim");
		this->tipTema = "Nu stiu";
		this->punctaj = 0.0;
	}

	//constructorul cu toti parametrii
	Tema(const char* materie, string tipTema, float punctaj) :idTema(nrTemeTrimise++) {
		this->materie = new char[strlen(materie) + 1];//asa alocam memorie pentru vector
		strcpy(this->materie, materie);
		this->tipTema = tipTema;
		this->punctaj = punctaj;
	}

	//constructor de copiere 
	Tema(const Tema& t) :idTema(t.idTema) {
		this->materie = new char[strlen(t.materie) + 1];//asa alocam memorie pentru vector
		strcpy(this->materie, t.materie);
		this->tipTema = t.tipTema;
		this->punctaj = t.punctaj;
	}

	//destructor
	~Tema() {
		if (this->materie != NULL) {
			delete[]this->materie;
		}

	}

	//operator este compus din evitarea auto-asignarii, destructor, constructor de copiere + return *this
	Tema& operator=(const Tema& t) {
		if (this != &t) {
			if (this->materie != NULL) {
				delete[]this->materie;
			}
			this->materie = new char[strlen(t.materie) + 1];//asa alocam memorie pentru vector
			strcpy(this->materie, t.materie);
			this->tipTema = t.tipTema;
			this->punctaj = t.punctaj;
		}
		return *this;
	}


	//cand spune 2 metode accesor(si nu va specifica exact daca vor getteri si/sau setteri)
	//faceti o pereche de get si set


	//getter --> functie/metoda accesor care permite extragerea valorii unui atribut din clasa
	float getPunctaj() {
		return this->punctaj;
	}

	const int getIdTema() {
		return this->idTema;
	}

	char* getMaterie() {
		/*char* aux = new char[strlen(this->materie) + 1];
		strcpy(aux, this->materie);
		return aux;*/
		return this->materie;
	}

	string getTipTema() {
		return this->tipTema;
	}


	//setter --> functie/metoda accesor care permite modificarea valorii unu atribut din clasa
	void setPunctaj(float punctaj) {
		if (punctaj >= 0.0 && punctaj <= 10) {
			this->punctaj = punctaj;
		}
		else throw ExceptieCustom("Valoarea introdusa nu indeplineste anumite criterii!");
	}

	//operator += (valoare)
	Tema& operator+=(float valoare) {
		//this->punctaj += valoare;
		this->punctaj = punctaj + valoare;
		return *this;
	}

	friend ostream& operator<<(ostream& out, const Tema& t) {
		out << t.idTema << endl;
		out << t.materie << endl;
		out << t.tipTema << endl;
		out << t.punctaj << endl;
		return out;
	}

	/*friend istream& operator>>(istream& in,  Tema& t) {

	}*/

	//IN ORICE TIP DE FISIER(FIE TEXT, FIE BINAR, FIE ALTCEVA) NU CITIM/SCRIEM ATRIBUTELE STATICE/CONSTANTE
	void scriereInBinar(string numeFisier) {
		//serializare --> reprezinta procesul de transformare a unui obiect intr-o secventa de biti
		ofstream f(numeFisier, ios::out | ios::binary);
		//asa scriem un char*
		int dim = strlen(this->materie);
		f.write((char*)&dim, sizeof(dim));
		f.write(this->materie, dim + 1);

		//asa scriem un string
		int dimS = this->tipTema.size();
		f.write((char*)&dimS, sizeof(dimS));
		f.write(this->tipTema.c_str(), dimS + 1);

		//asa scriem un int/float/double/bool
		f.write((char*)&this->punctaj, sizeof(this->punctaj));
		f.close();
	}

	void citesteDinBinar(string numeFisier) {

		//serializare --> reprezinta procesul de transformare a unui obiect intr-o secventa de biti
		ifstream f(numeFisier, ios::in | ios::binary);
		if (f.is_open()) {
			//asa citim un char*
			delete[]this->materie;
			int dim = 0;
			f.read((char*)&dim, sizeof(dim));
			this->materie = new char[dim + 1];
			f.read(this->materie, dim + 1);

			//asa citim un string
			int dimS = 0;
			f.read((char*)&dimS, sizeof(dimS));
			char* aux = new char[dimS + 1];
			f.read(aux, dimS + 1);
			this->tipTema = aux;
			delete[]aux;


			//asa citim un int/float/double/bool
			f.read((char*)&this->punctaj, sizeof(this->punctaj));
			f.close();
		}
		else {
			cout << "Fisierul cautat nu exista!" << endl;
		}

	}
};

int Tema::nrTemeTrimise = 0;
//ostream& operator<<(ostream& out,  Tema t) {
//	out << t.getIdTema() << endl;
//	out << t.getMaterie() << endl;
//	out << t.getTipTema() << endl;
//	out << t.getPunctaj() << endl;
//	return out;
//}

class SerieFacultate {
	string numeSerie;
	map<string, Tema>mapaTeme;
public:
	SerieFacultate() {
		this->numeSerie = "X";
	}

	SerieFacultate(string numeSerie, map<string, Tema> mapaTemaParam) {
		this->numeSerie = numeSerie;
		map<string, Tema>::iterator itMap;
		for (itMap = mapaTemaParam.begin(); itMap != mapaTemaParam.end(); itMap++) {
			this->mapaTeme.insert(pair<string, Tema>(itMap->first, itMap->second));
		}
	}

	friend ostream& operator<<(ostream& out, SerieFacultate s) {
		out << s.numeSerie << endl;
		out << "-----------------------------------------------" << endl;
		map<string, Tema>::iterator itMapParcurg;
		for (itMapParcurg = s.mapaTeme.begin(); itMapParcurg != s.mapaTeme.end(); itMapParcurg++) {
			out << itMapParcurg->first << endl;
			out << itMapParcurg->second << endl;

			out << endl << endl << endl;
		}
		out << "-----------------------------------------------" << endl;
		return out;
	}

	void operator()(Tema t) {
		this->mapaTeme.insert(pair<string, Tema>(t.getTipTema(), t));
	}
};
int main() {
	//obiect construit pe baza constructorului fara parametrii
	Tema t;
	cout << t.getPunctaj() << endl;
	t.setPunctaj(7.7);
	try {
		t.setPunctaj(15.2);
	}
	/*catch (exception ex) {
		cout << ex.what() << endl;
	}*/
	catch (ExceptieCustom exC) {
		cout << exC.afiseazaEroarea() << endl;
	}

	cout << t.getPunctaj() << endl;

	cout << endl << endl;
	char aux[50] = "ATP";
	Tema t1(aux, "PPT", 8.6);
	Tema t2("BPC", "CPP", 8.6);
	Tema t3("ALGEBRA", "DOC", 8.6);
	Tema t4("ECONOMIE", "XLS", 8.6);
	Tema t5("TEHNOLOGII WEB", "HTML", 8.6);
	cout << t1.getPunctaj() << endl;
	cout << endl << endl;


	cout << t1 << endl;

	t1 += 2.2;

	cout << endl << endl;
	cout << t1 << endl;


	cout << endl << endl;
	t1.scriereInBinar("exemplu.bin");

	Tema tBinar;

	cout << tBinar << endl;
	cout << endl;
	tBinar.citesteDinBinar("exemplu.bin");
	cout << tBinar << endl;

	cout << endl << endl << endl;
	map<string, Tema> mapTest;
	mapTest.insert(pair<string, Tema>(t1.getTipTema(), t1));
	mapTest.insert(pair<string, Tema>(t2.getTipTema(), t2));
	mapTest.insert(pair<string, Tema>(t2.getTipTema(), t2));
	mapTest.insert(pair<string, Tema>(t3.getTipTema(), t3));
	mapTest.insert(pair<string, Tema>(t3.getTipTema(), t3));
	mapTest.insert(pair<string, Tema>(t4.getTipTema(), t4));
	mapTest.insert(pair<string, Tema>(t4.getTipTema(), t4));

	SerieFacultate sf("Seria 1003", mapTest);
	cout << sf << endl << endl;

	sf(t5);
	cout << sf << endl << endl;

}

//https://drive.google.com/drive/folders/1uTi9SyxZu5w5m1fRQ3zV78xrUa_KR0xU?usp=sharing