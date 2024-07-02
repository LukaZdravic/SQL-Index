#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <stack>
#include <algorithm>
#include <queue>

using namespace std;

class RedRacuna {
public:
	long long int CA_ID, CA_B_ID, CA_C_ID;
	int CA_TAX_ST;
	string CA_NAME;
	double CA_BAL;

	RedRacuna(long long int AID, long long int BID, long long CID, string NAME, int TAX, double BAL) :CA_ID(AID), CA_B_ID(BID), CA_C_ID(CID), CA_NAME(NAME), CA_TAX_ST(TAX), CA_BAL(BAL) {}
	void print();
	friend class Cvor;
	friend class BStablo;
};
void RedRacuna::print() {
	cout << fixed;
	cout << CA_ID << " " << CA_B_ID << " " << CA_C_ID << " " << CA_NAME << " " << CA_TAX_ST << " " << CA_BAL << endl;
}

class Cvor {
public:
	Cvor(bool list, int redStabla);
	friend class BStablo;
	bool jeList;
	Cvor* otac;
	vector<long long int> kljucevi;
	vector <Cvor*> deca;
	vector<RedRacuna> sviRacuni;
};
Cvor::Cvor(bool list, int redStabla) {
	jeList = list;

	kljucevi.resize(redStabla - 1);
	fill(kljucevi.begin(), kljucevi.end(), 0);
	deca.resize(redStabla);
	//fill(deca.begin(), deca.end(), NULL);
	Cvor* otac = NULL;
}

class BStablo {
public:
	BStablo(int red, Cvor* koren = NULL) :redStabla(red), koren(koren) {}
	BStablo() = default;
	void ubaci(RedRacuna);
	void izbrisi();
	void nadjiSVE(long long int);
	Cvor* koren;

	int redStabla;

};

int prebroj(vector<long long int> v1) {
	int br = 0;
	if (v1.size() == 0) return 0;
	for (int i = 0; i < v1.size(); i++) if (v1[i] != 0) br++;
	return br;
}
void BStablo::nadjiSVE(long long int CAID) {
	for (int i = 0; i < koren->sviRacuni.size(); i++) {
		if (koren->sviRacuni[i].CA_ID == CAID) koren->sviRacuni[i].print();
	}


}

void BStablo::ubaci(RedRacuna r1) {

	int maxkljucCvora = redStabla - 1;
	int maxPokPodstabla = redStabla;
	int velicina, ubacenIsti;
	int gotovo = 0;
	if (koren == NULL) {
		koren = new Cvor(true, redStabla);
		koren->kljucevi[0] = r1.CA_C_ID;
		koren->sviRacuni.push_back(r1);
	}
	else {
		int prethodni, trenutni = 0;
		Cvor* current = koren;
		stack<Cvor*> stack;
		for (int i = 0; i < current->kljucevi.size() - 1; i++) {
			if (r1.CA_C_ID < current->kljucevi[i] && current->deca[i] != NULL) {

				current = current->deca[i];
				i = -1;
			}
			else if (r1.CA_C_ID > current->kljucevi[i] && r1.CA_C_ID < current->kljucevi[i + 1] && current->deca[i + 1] != NULL) {

				current = current->deca[i + 1];
				i = -1;
			}
			else if (r1.CA_C_ID > current->kljucevi[i] && current->kljucevi[i + 1] == 0 && current->deca[i + 1] != NULL) {

				current = current->deca[i + 1];
				i = -1;
			}
			else if (r1.CA_C_ID > current->kljucevi[i + 1] && current->deca[i + 2] != NULL && i == current->kljucevi.size() - 2) {

				current = current->deca[i + 2];
				i = -1;
			}
		}

		velicina = prebroj(current->kljucevi) + 1;
		ubacenIsti = 0;

		for (int i = 0; i < current->kljucevi.size(); i++) {
			if (current->kljucevi[i] == r1.CA_C_ID) {
				current->sviRacuni.push_back(r1);
				ubacenIsti = 1;
				gotovo = 1;
				break;
			}
		}

		if (velicina <= maxkljucCvora && ubacenIsti == 0) {
			for (int i = 0; i < current->kljucevi.size(); i++) {
				if (current->kljucevi[i + 1] != 0) {
					if (r1.CA_C_ID > current->kljucevi[i] && r1.CA_C_ID < current->kljucevi[i + 1]) {
						for (int j = current->kljucevi.size() - 1; j > i; j--) current->kljucevi[i] = current->kljucevi[i - 1];
						current->kljucevi[i + 1] = r1.CA_C_ID;
						current->sviRacuni.push_back(r1);
						gotovo = 1;
						break;
					}
				}
				else if (current->kljucevi[i + 1] == 0) {
					if (current->kljucevi[i] < r1.CA_C_ID) {
						current->kljucevi[i + 1] = r1.CA_C_ID;
						current->sviRacuni.push_back(r1);
						gotovo = 1;
						break;
					}
					else if (current->kljucevi[i] > r1.CA_C_ID) {
						for (int j = current->kljucevi.size() - 1; j > i; j--) current->kljucevi[j] = current->kljucevi[j - 1];
						current->kljucevi[i] = r1.CA_C_ID;
						current->sviRacuni.push_back(r1);
						gotovo = 1;
						break;
					}
				}
			}
		}
		else if (velicina > maxkljucCvora && ubacenIsti == 0) {
			Cvor* prethodnidesni = NULL;
			current->kljucevi.push_back(r1.CA_C_ID);
			current->sviRacuni.push_back(r1);
			while (velicina > maxkljucCvora && current) {

				if (current == koren) {
					// PROMENI DA SE PRVO TAJ NOVI DODA U RED OD M CVOROVA PA SE SORTIRA LEPO PA SE IZABERE SREDNJI A NE OD STARIH DA SE BIRA SREDNJI I IDE GORE!

					sort(current->kljucevi.begin(), current->kljucevi.end());

					Cvor* noviKoren = new Cvor(false, redStabla);
					noviKoren->kljucevi[0] = current->kljucevi[ceil(current->kljucevi.size() / 2.0) - 1];
					noviKoren->deca[0] = current;
					current->otac = noviKoren;

					koren = noviKoren;
					vector<int> za_brisanje;

					for (int i = 0; i < current->sviRacuni.size(); i++) {
						if (noviKoren->kljucevi[0] == current->sviRacuni[i].CA_C_ID) {
							noviKoren->sviRacuni.push_back(current->sviRacuni[i]);
							if (i < current->sviRacuni.size()) za_brisanje.push_back(i);
						}
					}
					//desni od srednjeg idu u novi list
					Cvor* desni = new Cvor(true, redStabla);
					desni->otac = noviKoren;
					int tmp = 0;
					for (int i = ceil(current->kljucevi.size() / 2.0); i < current->kljucevi.size(); i++) {
						desni->kljucevi[tmp++] = current->kljucevi[i];


					}
					tmp = 0;
					int br = 0;
					for (br = ceil(current->kljucevi.size() / 2.0); br < current->kljucevi.size(); br++) {
						desni->deca[tmp++] = current->deca[br];
						if (current->deca[br] != NULL) current->deca[br]->otac = desni;
						current->deca[br] = NULL;

					}
					desni->deca[tmp] = current->deca[current->deca.size() - 1];
					current->deca[current->deca.size() - 1] = NULL;

					for (int i = 0; i < current->sviRacuni.size(); i++) {
						for (int j = 0; j < desni->kljucevi.size(); j++)
						{
							if (current->sviRacuni[i].CA_C_ID == desni->kljucevi[j]) {
								desni->sviRacuni.push_back(current->sviRacuni[i]);
								if (i < current->sviRacuni.size()) za_brisanje.push_back(i);
								//current->sviRacuni.erase(current->sviRacuni.begin() + i);
								//j--;
							}
						}
					}


					if (prethodnidesni != NULL) {
						prethodnidesni->otac = desni;
						prethodnidesni = NULL;
					}
					noviKoren->deca[1] = desni;
					sort(za_brisanje.begin(), za_brisanje.end());
					current->sviRacuni.erase(current->sviRacuni.begin() + za_brisanje[0], current->sviRacuni.end());

					// srednji ide u roditeljski ali posto je ovo koren ide u novi cvor, da nije koren moze samo parent->kljucevi.push_back(srednji), to gore u ovom stack delu ubaci!

					//levi od srednjeg samo zadrzis u starom listu

					for (int i = ceil((current->kljucevi.size()) / 2.0) - 1; i < current->kljucevi.size(); i++) {

						current->kljucevi[i] = 0;
					}
					//for (int i = ceil((current->kljucevi.size()) / 2.0) - 1; i < current->kljucevi.size() - 1; i++) current->deca[i + 1] = NULL;
					current->kljucevi.resize(redStabla - 1);
					break;
				}
				else if (current != koren) {
					//NASTAVI SA OVOM LOGIKOM ALI PROBAJ DA STAVIS U WHILE I OVAJ PRETHODNI IF SA PROVEROM NA KOREN I OVAJ NEKAKO NZM BRT POMAGAJ


					vector<int> za_brisanje;
					sort(current->kljucevi.begin(), current->kljucevi.end());


					// -------------- DESNI
					Cvor* desni = new Cvor(true, redStabla);

					int tmp = 0;

					for (int i = ceil(current->kljucevi.size() / 2.0); i < current->kljucevi.size(); i++) {
						desni->kljucevi[tmp++] = current->kljucevi[i];
					}
					tmp = 0;
					int br = 0;
					for (br = ceil(current->kljucevi.size() / 2.0); br < current->kljucevi.size(); br++) {
						desni->deca[tmp++] = current->deca[br];
						if (current->deca[br] != NULL) current->deca[br]->otac = desni;
						current->deca[br] = NULL;

					}
					desni->deca[tmp] = current->deca[current->deca.size() - 1];
					current->deca[current->deca.size() - 1] = NULL;

					for (int i = 0; i < current->sviRacuni.size(); i++) {
						for (int j = 0; j < desni->kljucevi.size(); j++)
						{
							if (current->sviRacuni[i].CA_C_ID == desni->kljucevi[j]) {
								desni->sviRacuni.push_back(current->sviRacuni[i]);
								if (i < current->sviRacuni.size()) za_brisanje.push_back(i);


							}
						}
					}
					// --------------- DESNI
					if (prethodnidesni != NULL) {
						prethodnidesni->otac = desni;
						prethodnidesni = NULL;
					}
					Cvor* otac = current->otac;
					int tmpi;
					for (int i = 0; i < otac->deca.size(); i++) {
						if (otac->deca[i] == current) {
							tmpi = i;
							break;
						}

					}

					// ovaj ^ deo ne radi jer ako mi je popunjen cvor i ovaj iznad njega za decu izlazi van niza... ovaj deo resi samo sutra plz
					// STARI

					int tmp3 = prebroj(otac->kljucevi);
					if (tmp3 == redStabla - 1) {
						otac->kljucevi.push_back(current->kljucevi[ceil(current->kljucevi.size() / 2.0) - 1]);
						otac->deca.push_back(desni);
						prethodnidesni = desni;

					}
					else {
						otac->kljucevi[tmp3] = (current->kljucevi[ceil(current->kljucevi.size() / 2.0) - 1]);
						otac->deca[tmpi + 1] = desni;
						desni->otac = otac;
					}

					for (int i = 0; i < current->sviRacuni.size(); i++) {
						if (current->sviRacuni[i].CA_C_ID == current->kljucevi[ceil(current->kljucevi.size() / 2.0) - 1]) {
							za_brisanje.push_back(i);
							otac->sviRacuni.push_back(current->sviRacuni[i]);
						}
					}

					for (int i = ceil((current->kljucevi.size()) / 2.0) - 1; i < current->kljucevi.size(); i++) {

						current->kljucevi[i] = 0;
					}
					//for (int i = ceil((current->kljucevi.size()) / 2.0) -1; i < current->kljucevi.size() - 1; i++) current->deca[i + 1] = NULL;
					// STARI 


					sort(za_brisanje.begin(), za_brisanje.end());
					current->sviRacuni.erase(current->sviRacuni.begin() + za_brisanje[0], current->sviRacuni.end());



					current->kljucevi.resize(redStabla - 1);
					current = current->otac;
					velicina = prebroj(current->kljucevi);


				}
			}

			//break;
			//cepaj
		}

	}

}



// svaki cvor da ima C_ID kao kljuc-eve u sebi i niz nekih kao struct TABELA gde svaki clan niza sadrzi sve info o tom racunu
// kad se trazi pojedini racun => binarna pretraga nad nizom, kada se traze svi racuni ispise se svaki element niza sekv.

string vrati_treci(string str) {
	char delimiter = '|';
	string temp = "";
	int br = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] != delimiter) temp += str[i];
		else {
			br++;
			if (br == 3) return temp;
			temp = "";


		}
	}


}


void level_order(BStablo* btree) {
	queue<Cvor*> que;
	Cvor* current;
	Cvor* last = btree->koren;
	que.push(btree->koren);
	while (que.empty() == false) {
		current = que.front();
		que.pop();

		for (int i = 0; i < current->kljucevi.size(); i++) {
			if (current->kljucevi[i] != 0) cout << current->kljucevi[i] << " ";
		}
		for (int i = 0; i < current->deca.size(); i++) {
			if (current->deca[i] != NULL) que.push(current->deca[i]);
		}

		if (last != current->otac)	putchar('\n');
		last = current;
	}


}
void level_orderK(BStablo* btree) {
	queue<Cvor*> que;
	Cvor* current;
	Cvor* last = btree->koren;
	que.push(btree->koren);
	while (que.empty() == false) {
		current = que.front();
		que.pop();

		for (int i = 0; i < current->deca.size(); i++) {
			if (current->deca[i] != NULL) {
				current->jeList = false;
				que.push(current->deca[i]);
			}
		}

		last = current;
	}


}
void level_orderDEL(BStablo* btree) {
	queue<Cvor*> que;
	Cvor* current;
	Cvor* last = btree->koren;
	que.push(btree->koren);
	while (que.empty() == false) {
		current = que.front();
		que.pop();

		for (int i = 0; i < current->deca.size(); i++) {
			if (current->deca[i] != NULL) que.push(current->deca[i]);
		}

		last = current;
		delete current;
	}
	delete btree;

	btree = NULL;
}


void AID_SR(BStablo* btree, long long int CID, long long AID, int& postoji) {
	Cvor* current = btree->koren;

	current->kljucevi.resize(btree->redStabla - 1);

	for (int i = 0; i < current->kljucevi.size() - 1; i++) {
		if (CID == current->kljucevi[i]) {
			postoji = 1;
			break;
		}
		if (CID == current->kljucevi[i + 1]) {
			postoji = 1;
			break;
		}
		if (CID < current->kljucevi[i] && current->deca[i] != NULL) {

			current = current->deca[i];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
		}
		else if (CID > current->kljucevi[i] && CID < current->kljucevi[i + 1] && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
		}
		else if (CID > current->kljucevi[i] && current->kljucevi[i + 1] == 0 && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
		}
		else if (CID > current->kljucevi[i + 1] && current->deca[i + 2] != NULL && i == current->kljucevi.size() - 2) {

			current = current->deca[i + 2];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
		}
	}
	for (int i = 0; i < current->sviRacuni.size(); i++) {
		/*if (current->sviRacuni[i].CA_C_ID == CID && current->sviRacuni[i].CA_ID == AID) {
			postoji = 1;
		}*/
	}

}



vector<RedRacuna> CID_SR(BStablo* btree, long long int CID, int& br, int& postoji) {
	Cvor* current = btree->koren;
	vector<RedRacuna> r1;
	current->kljucevi.resize(btree->redStabla - 1);

	for (int i = 0; i < current->kljucevi.size() - 1; i++) {
		if (CID == current->kljucevi[i]) {
			postoji = 1;
			break;
		}
		if (CID == current->kljucevi[i + 1]) {
			postoji = 1;
			break;
		}
		if (CID < current->kljucevi[i] && current->deca[i] != NULL) {

			current = current->deca[i];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && CID < current->kljucevi[i + 1] && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && current->kljucevi[i + 1] == 0 && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i + 1] && current->deca[i + 2] != NULL && i == current->kljucevi.size() - 2) {

			current = current->deca[i + 2];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
	}
	for (int i = 0; i < current->sviRacuni.size(); i++) {
		if (current->sviRacuni[i].CA_C_ID == CID) {
			r1.push_back(current->sviRacuni[i]);
		}
	}

	return r1;

}
void ispis_podataka(BStablo* btree, long long int CID) {
	int brkoraka = 1;
	int postoji = 0;
	Cvor* current = btree->koren;
	ifstream dat;
	dat.open("Customer.txt");
	if (!dat.is_open()) {
		cout << "Neuspesno otvaranje datoteke" << endl;
		return;
	}
	vector <string>s1;
	string linija;
	char delimiter = '|';
	string temp;
	string temp2 = "";
	while (getline(dat, linija)) {
		temp = "";
		temp2 = "";
		for (int i = 0; i < 10; i++) temp2 += linija[i];
		if (temp2 != to_string(CID)) continue;
		for (int i = 0; i < linija.size(); i++) {
			if (linija[i] != delimiter) temp += linija[i];
			else {
				s1.push_back(temp);
				temp = "";
			}
		}
		s1.push_back(temp);
		break;
	}

	dat.close();

	vector<RedRacuna> racuni = CID_SR(btree, CID, brkoraka, postoji);

	//for (int i = 0; i < racuni.size(); i++) racuni[i].print();
	//cout << endl << "BR. POREDJENJA: " << brkoraka<<" POSTOJI LI: "<<postoji;

	ofstream output("tester.txt");
	if (postoji == 0) {
		string s = s1[0] + "|" + s1[1] + "|" + s1[2] + "|" + s1[3] + "|" + "null" + "|" + "null" + "|" + "null" + "|" + "null";
		output << s;
	}
	else {

		for (int i = 0; i < racuni.size(); i++) {
			string s = s1[0] + "|" + s1[1] + "|" + s1[2] + "|" + s1[3] + "|" + to_string(racuni[i].CA_ID) + "|" + to_string(racuni[i].CA_C_ID) + "|" + to_string(racuni[i].CA_TAX_ST) + "|" + to_string(racuni[i].CA_BAL);
			output << s << endl;
		}

	}


	output.close();
	cout << endl << "UKUPAN BROJ POREDJENJA PRI PRETRAZI: " << brkoraka << endl;
}
void izbrisicvor(BStablo* btree, Cvor* zabrisanje, int id, int& br) {
	int minkljucuCvoru = ceil(btree->redStabla / 2.0) - 1;
	int minpodstablaIntern = ceil(btree->redStabla / 2.0);

	int ima = prebroj(zabrisanje->kljucevi);

	if (ima - 1 >= minkljucuCvoru && zabrisanje->jeList == true) {
		for (int i = id; i < zabrisanje->kljucevi.size() - 1; i++) {
			zabrisanje->kljucevi[i] = zabrisanje->kljucevi[i + 1];
		}
		zabrisanje->kljucevi[zabrisanje->kljucevi.size() - 1] = 0;
		return;

	}
	else if (zabrisanje->jeList == false) {
		Cvor* current = zabrisanje;
		Cvor* prethodni = NULL;
		current = current->deca[id];
		int koliko = prebroj(current->kljucevi);
		while (current) {
			prethodni = current;
			current = current->deca[koliko];
			br++;
			if (current) koliko = prebroj(current->kljucevi);

		}
		koliko = prebroj(prethodni->kljucevi);
		long long int menjaUnutrasnji = prethodni->kljucevi[koliko - 1];
		vector<RedRacuna> r1;
		for (int i = 0; i < prethodni->sviRacuni.size(); i++) {
			if (prethodni->sviRacuni[i].CA_C_ID == menjaUnutrasnji) {
				r1.push_back(prethodni->sviRacuni[i]);
				prethodni->sviRacuni.erase(prethodni->sviRacuni.begin() + i, prethodni->sviRacuni.begin() + i + 1);
				i--;
			}
		}
		for (int i = 0; i < zabrisanje->sviRacuni.size(); i++) {
			if (zabrisanje->sviRacuni[i].CA_C_ID == zabrisanje->kljucevi[id]) {
				zabrisanje->sviRacuni.erase(zabrisanje->sviRacuni.begin() + i, zabrisanje->sviRacuni.begin() + i + 1);
			}

		}
		zabrisanje->kljucevi[id] = menjaUnutrasnji;
		for (int i = 0; i < r1.size(); i++) {
			zabrisanje->sviRacuni.push_back(r1[i]);
		}
		prethodni->kljucevi[koliko - 1] = 0;
		zabrisanje = prethodni;

	}

	ima = prebroj(zabrisanje->kljucevi);
	zabrisanje->kljucevi[id] = 0;
	if (ima - 1 < minkljucuCvoru) {
		ima = ima - 1;
		while (ima < minkljucuCvoru && zabrisanje != btree->koren) {
			Cvor* otac = zabrisanje->otac;
			int tmp = -1;
			for (int i = 0; i < otac->deca.size(); i++) {
				if (otac->deca[i] == zabrisanje) {
					tmp = i;
					break;
				}
			}
			int pozajmio = 0;
			// prvo desni sin
			if ((tmp + 1) <= btree->redStabla - 1 && zabrisanje->jeList == true) {
				if (otac->deca[tmp + 1]) {
					if (prebroj(otac->deca[tmp + 1]->kljucevi) - 1 >= minkljucuCvoru) {
						long long int ideUoca = otac->deca[tmp + 1]->kljucevi[0];
						vector<RedRacuna> racunizaOca;

						for (int i = 0; i < otac->deca[tmp + 1]->kljucevi.size() - 1; i++) {
							otac->deca[tmp + 1]->kljucevi[i] = otac->deca[tmp + 1]->kljucevi[i + 1];
						}
						otac->deca[tmp + 1]->kljucevi[otac->deca[tmp + 1]->kljucevi.size() - 1] = 0;
						for (int i = 0; i < otac->deca[tmp + 1]->sviRacuni.size(); i++) {
							if (otac->deca[tmp + 1]->sviRacuni[i].CA_C_ID == ideUoca) {
								racunizaOca.push_back(otac->deca[tmp + 1]->sviRacuni[i]);
								otac->deca[tmp + 1]->sviRacuni.erase(otac->deca[tmp + 1]->sviRacuni.begin() + i, otac->deca[tmp + 1]->sviRacuni.begin() + i + 1);
								i--;
							}

						}
						vector<RedRacuna> racunizaSina;
						long long int uOcu = otac->kljucevi[tmp];
						for (int i = 0; i < racunizaOca.size(); i++) {
							otac->sviRacuni.push_back(racunizaOca[i]);
						}
						for (int i = 0; i < otac->sviRacuni.size(); i++) {
							if (otac->sviRacuni[i].CA_C_ID == uOcu) {
								racunizaSina.push_back(otac->sviRacuni[i]);
								otac->sviRacuni.erase(otac->sviRacuni.begin() + i, otac->sviRacuni.begin() + i + 1);
								i--;
							}

						}
						otac->kljucevi[tmp] = ideUoca;
						long long int uSinu = zabrisanje->kljucevi[id];
						//zabrisanje->kljucevi[id] = uOcu; umesto ovoga mora da se doda na kraj a ne gde je bio stari
						zabrisanje->kljucevi[id] = 0;
						for (int i = id; i < zabrisanje->kljucevi.size() - 1; i++) zabrisanje->kljucevi[i] = zabrisanje->kljucevi[i + 1];


						int imaih = prebroj(zabrisanje->kljucevi);
						zabrisanje->kljucevi[imaih] = uOcu;

						for (int i = 0; i < racunizaSina.size(); i++) {
							zabrisanje->sviRacuni.push_back(racunizaSina[i]);
						}
						for (int i = 0; i < zabrisanje->sviRacuni.size(); i++) {
							if (zabrisanje->sviRacuni[i].CA_C_ID == uSinu) {
								zabrisanje->sviRacuni.erase(zabrisanje->sviRacuni.begin() + i, zabrisanje->sviRacuni.begin() + i + 1);
								i--;
							}
						}


						pozajmio = 1;

					}
				}
			}
			// ako nema onda levi
			if ((tmp - 1) >= 0 && pozajmio == 0 && zabrisanje->jeList == true) {
				if (otac->deca[tmp - 1]) {
					if (prebroj(otac->deca[tmp - 1]->kljucevi) - 1 >= minkljucuCvoru) {

						long long int ideUoca = otac->deca[tmp - 1]->kljucevi[otac->deca[tmp - 1]->kljucevi.size() - 1];
						vector<RedRacuna> racunizaOca;
						otac->deca[tmp - 1]->kljucevi[otac->deca[tmp - 1]->kljucevi.size() - 1] = 0;

						for (int i = 0; i < otac->deca[tmp - 1]->sviRacuni.size(); i++) {
							if (otac->deca[tmp - 1]->sviRacuni[i].CA_C_ID == ideUoca) {
								racunizaOca.push_back(otac->deca[tmp - 1]->sviRacuni[i]);
								otac->deca[tmp - 1]->sviRacuni.erase(otac->deca[tmp - 1]->sviRacuni.begin() + i, otac->deca[tmp - 1]->sviRacuni.begin() + i + 1);
								i--;
							}

						}
						vector<RedRacuna> racunizaSina;
						long long int uOcu = otac->kljucevi[tmp - 1];
						for (int i = 0; i < racunizaOca.size(); i++) {
							otac->sviRacuni.push_back(racunizaOca[i]);
						}
						for (int i = 0; i < otac->sviRacuni.size(); i++) {
							if (otac->sviRacuni[i].CA_C_ID == uOcu) {
								racunizaSina.push_back(otac->sviRacuni[i]);
								otac->sviRacuni.erase(otac->sviRacuni.begin() + i, otac->sviRacuni.begin() + i + 1);
								i--;
							}

						}
						otac->kljucevi[tmp - 1] = ideUoca;
						long long int uSinu = zabrisanje->kljucevi[id];
						//zabrisanje->kljucevi[id] = uOcu; umesto ovoga dodavanje na pocetak
						zabrisanje->kljucevi[id] = 0;
						sort(zabrisanje->kljucevi.begin(), zabrisanje->kljucevi.end());

						for (int i = zabrisanje->kljucevi.size() - 1; i > 0; i--) zabrisanje->kljucevi[i] = zabrisanje->kljucevi[i - 1];
						zabrisanje->kljucevi[0] = uOcu;

						for (int i = 0; i < racunizaSina.size(); i++) {
							zabrisanje->sviRacuni.push_back(racunizaSina[i]);
						}
						for (int i = 0; i < zabrisanje->sviRacuni.size(); i++) {
							if (zabrisanje->sviRacuni[i].CA_C_ID == uSinu) {
								zabrisanje->sviRacuni.erase(zabrisanje->sviRacuni.begin() + i, zabrisanje->sviRacuni.begin() + i + 1);
								i--;
							}
						}

						sort(zabrisanje->kljucevi.begin(), zabrisanje->kljucevi.end());
						pozajmio = 1;
					}
				}
			}
			if (pozajmio == 0) {

				int desio = 0;
				int br = prebroj(zabrisanje->kljucevi);
				if ((tmp + 1) <= btree->redStabla - 1) {
					if (otac->deca[tmp + 1]) {
						desio = 1;
						sort(zabrisanje->kljucevi.rbegin(), zabrisanje->kljucevi.rend());
						zabrisanje->kljucevi[br++] = otac->kljucevi[tmp];

						vector<RedRacuna> racuniOca;
						for (int i = 0; i < otac->sviRacuni.size(); i++) {
							if (otac->sviRacuni[i].CA_C_ID == otac->kljucevi[tmp]) racuniOca.push_back(otac->sviRacuni[i]);
						}

						for (int i = 0; i < racuniOca.size(); i++) zabrisanje->sviRacuni.push_back(racuniOca[i]);
						int dete = 0;
						for (int i = 0; i < zabrisanje->deca.size(); i++) {
							if (zabrisanje->deca[i] == NULL) {
								dete = i;
								break;
							}
						}
						for (int i = 0; i < otac->deca[tmp + 1]->deca.size(); i++) {
							if (otac->deca[tmp + 1]->deca[i]) {
								zabrisanje->deca[dete++] = otac->deca[tmp + 1]->deca[i];
								otac->deca[tmp + 1]->deca[i]->otac = zabrisanje;
							}
						}




						for (int i = 0; i < prebroj(otac->deca[tmp + 1]->kljucevi); i++) {
							zabrisanje->kljucevi[br++] = otac->deca[tmp + 1]->kljucevi[i];
						}
						for (int i = 0; i < otac->deca[tmp + 1]->sviRacuni.size(); i++) zabrisanje->sviRacuni.push_back(otac->deca[tmp + 1]->sviRacuni[i]);


						otac->kljucevi[tmp] = 0;
						for (int i = tmp; i < otac->kljucevi.size() - 1; i++) otac->kljucevi[i] = otac->kljucevi[i + 1];
						otac->kljucevi[otac->kljucevi.size() - 1] = 0;
						for (int i = tmp + 1; i < otac->deca.size() - 1; i++) otac->deca[i] = otac->deca[i + 1];
						otac->deca[otac->deca.size() - 1] = NULL;
					}
				}
				if ((tmp - 1) >= 0) {

					if (otac->deca[tmp - 1] && desio == 0) {
						sort(zabrisanje->kljucevi.rbegin(), zabrisanje->kljucevi.rend());
						zabrisanje->kljucevi[br++] = otac->kljucevi[tmp - 1];

						vector<RedRacuna> racuniOca;
						for (int i = 0; i < otac->sviRacuni.size(); i++) {
							if (otac->sviRacuni[i].CA_C_ID == otac->kljucevi[tmp - 1]) racuniOca.push_back(otac->sviRacuni[i]);
						}

						for (int i = 0; i < racuniOca.size(); i++) zabrisanje->sviRacuni.push_back(racuniOca[i]);
						int dete = 0;
						for (int i = 0; i < zabrisanje->deca.size(); i++) {
							if (zabrisanje->deca[i] == NULL) {
								dete = i;
								break;
							}
						}
						for (int i = otac->deca[tmp - 1]->deca.size() - 1; i >= 0; i--) {
							if (otac->deca[tmp - 1]->deca[i]) {
								for (int i = zabrisanje->deca.size() - 1; i > 0; i--) zabrisanje->deca[i] = zabrisanje->deca[i - 1];

								zabrisanje->deca[0] = otac->deca[tmp - 1]->deca[i];
								otac->deca[tmp - 1]->deca[i]->otac = zabrisanje;

							}
						}




						for (int i = 0; i < prebroj(otac->deca[tmp - 1]->kljucevi); i++) {
							zabrisanje->kljucevi[br++] = otac->deca[tmp - 1]->kljucevi[i];
						}
						sort(zabrisanje->kljucevi.begin(), zabrisanje->kljucevi.end());
						for (int i = 0; i < otac->deca[tmp - 1]->sviRacuni.size(); i++) zabrisanje->sviRacuni.push_back(otac->deca[tmp - 1]->sviRacuni[i]);


						otac->kljucevi[tmp - 1] = 0;
						for (int i = tmp - 1; i < otac->kljucevi.size() - 1; i++) otac->kljucevi[i] = otac->kljucevi[i + 1];
						otac->kljucevi[otac->kljucevi.size() - 1] = 0;
						for (int i = tmp - 1; i < otac->deca.size() - 1; i++) otac->deca[i] = otac->deca[i + 1];
						otac->deca[otac->deca.size() - 1] = NULL;



					}
				}








			}

			if (zabrisanje->otac == btree->koren && prebroj(zabrisanje->otac->kljucevi) == 0) {
				btree->koren = zabrisanje;
				zabrisanje->otac = NULL;
				return;

			}
			zabrisanje = zabrisanje->otac;
			ima = prebroj(zabrisanje->kljucevi);
			level_orderK(btree);

		}

	}


}



void izbrisijedan(BStablo* btree, RedRacuna r1, int& br, int& post) {
	Cvor* current = btree->koren;
	long long int CID = r1.CA_C_ID;
	long long int AID = r1.CA_ID;
	for (int i = 0; i < current->kljucevi.size() - 1; i++) {
		if (CID == current->kljucevi[i]) break;
		if (CID < current->kljucevi[i] && current->deca[i] != NULL) {

			current = current->deca[i];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && CID < current->kljucevi[i + 1] && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && current->kljucevi[i + 1] == 0 && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i + 1] && current->deca[i + 2] != NULL && i == current->kljucevi.size() - 2) {

			current = current->deca[i + 2];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
	}
	int tmp = -1;
	int kolikoel = 0;
	for (int i = 0; i < current->sviRacuni.size(); i++) {
		if (current->sviRacuni[i].CA_C_ID == CID && current->sviRacuni[i].CA_ID == AID) {
			tmp = i;
		}
		if (current->sviRacuni[i].CA_C_ID == CID) kolikoel++;
	}

	if (tmp == -1) {
		post = 0;
		return;
	}
	else post = 1;
	current->sviRacuni.erase(current->sviRacuni.begin() + tmp, current->sviRacuni.begin() + tmp + 1);
	if (kolikoel >= 2) return;
	else {
		int id = 0;
		for (int i = 0; i < current->kljucevi.size(); i++) {
			if (current->kljucevi[i] == CID) {
				id = i;
				break;
			}
		}

		izbrisicvor(btree, current, id, br);

	}




}

Cvor* nadji_cvor(BStablo* btree, long long int CID, int& id, int& br) {
	Cvor* current = btree->koren;
	for (int i = 0; i < current->kljucevi.size() - 1; i++) {
		if (CID == current->kljucevi[i]) {
			id = i;
			return current;
			break;
		}
		if (CID == current->kljucevi[i + 1]) {
			id = i + 1;
			return current;
			break;
		}
		if (CID < current->kljucevi[i] && current->deca[i] != NULL) {

			current = current->deca[i];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && CID < current->kljucevi[i + 1] && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i] && current->kljucevi[i + 1] == 0 && current->deca[i + 1] != NULL) {

			current = current->deca[i + 1];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
		else if (CID > current->kljucevi[i + 1] && current->deca[i + 2] != NULL && i == current->kljucevi.size() - 2) {

			current = current->deca[i + 2];
			current->kljucevi.resize(btree->redStabla - 1);
			i = -1;
			br++;
		}
	}

}


int main() {
	char c;
	BStablo* bstablo = NULL;
	while (1) {

		cout << "IZABERITE JEDNU OPCIJU IZ MENIJA:" << endl;
		cout << "1:Stvaranje i unistavanje indeksa nad tabelom CustomerAccount" << endl;
		cout << "2:Ispis indeksa na standardni izlaz" << endl;
		cout << "3:Ispis podataka o korisniku" << endl;
		cout << "4:Dodavanje novog zapisa" << endl;
		cout << "5:Brisanje zapisa iz indeksa" << endl;
		cout << "6:Brisanje svih racuna customera" << endl;
		cout << "7:Kraj programa" << endl;
		cin >> c;
		if (c == '1') {
			cout << "Pravljenje ili Unistavanje indeksa?(0,1)" << endl;
			bool radi;
			cin >> radi;
			if (!radi) {
				int redBstabla;
				cout << "Unesite red B stabla" << endl;

				cin >> redBstabla;
				if (redBstabla < 3 || redBstabla>10) {
					cout << "Netacan red stabla" << endl;
					break;
				}
				bstablo = new BStablo(redBstabla);
				cout << "Koji Customer fajl zelite ? (20,250,2500)" << endl;
				int kojiFajl;
				cin >> kojiFajl;
				if (kojiFajl != 20 && kojiFajl != 250 && kojiFajl != 2500) {
					cout << "Ne postoji fajl";
					break;
				}
				string s1 = "CustomerAccount";
				s1 = s1 + to_string(kojiFajl) + ".txt";
				ifstream datoteka;
				datoteka.open(s1);
				if (!datoteka.is_open()) {
					cout << "Neuspesno otvaranje datoteke";
					break;
				}

				string linija;
				char delimiter = '|';
				string temp = "";

				int br = 0;

				while (getline(datoteka, linija)) {
					vector <string>s1;
					temp = "";
					for (int i = 0; i < linija.size(); i++) {
						if (linija[i] != delimiter) temp += linija[i];
						else {
							s1.push_back(temp);
							temp = "";
						}
					}
					s1.push_back(temp);
					RedRacuna temporary(stoll(s1[0]), stoll(s1[1]), stoll(s1[2]), s1[3], stoi(s1[4]), stod(s1[5]));
					//temporary.print();
					br++;

					// long long int tmp2 = stoll(vrati_treci(linija));
					bstablo->ubaci(temporary);


					s1.clear();
				}

				datoteka.close();
				level_orderK(bstablo);
				cout << endl;
			}
			else {
				if (bstablo == NULL) {
					cout << "GRESKA NE POSTOJI STABLO" << endl;
					continue;
				}
				level_orderDEL(bstablo);

			}

		}
		if (c == '2') {
			level_order(bstablo);
			cout << endl;
		}
		if (c == '3') {
			// pretraga na korisnika 
			long long int TMP_CID;
			cout << "Unesite na koji C_ID zelite pretragu:" << endl;
			cin >> TMP_CID;
			ispis_podataka(bstablo, TMP_CID);




		}
		if (c == '4') {
			cout << "UNESITE CEO RACUN KOJI ZELITE DA UBACITE" << endl;
			vector<string> s1;
			string s;
			string temp = "";
			cin >> s;
			char delimiter = '|';
			for (int i = 0; i < s.size(); i++) {
				if (s[i] != delimiter) temp += s[i];
				else {
					s1.push_back(temp);
					temp = "";
				}
			}
			s1.push_back(temp);

			RedRacuna temporary(stoll(s1[0]), stoll(s1[1]), stoll(s1[2]), s1[3], stoi(s1[4]), stod(s1[5]));
			int postoji = 0;
			int brkoraka = 0;
			AID_SR(bstablo, stoll(s1[2]), stoll(s1[0]), postoji);

			if (postoji == 1) {
				cout << "VEC POSTOJI SA TIM A_ID" << endl;
				continue;
			}
			bstablo->ubaci(temporary);

		}
		if (c == '5') {

			cout << "UNESITE CEO RACUN KOJI ZELITE DA IZBACITE" << endl;
			vector<string> s1;
			string s;
			string temp = "";
			cin >> s;
			char delimiter = '|';
			for (int i = 0; i < s.size(); i++) {
				if (s[i] != delimiter) temp += s[i];
				else {
					s1.push_back(temp);
					temp = "";
				}
			}
			s1.push_back(temp);
			int brkoraka = 1;
			int postoji = 0;
			RedRacuna temporary(stoll(s1[0]), stoll(s1[1]), stoll(s1[2]), s1[3], stoi(s1[4]), stod(s1[5]));
			izbrisijedan(bstablo, temporary, brkoraka, postoji);
			if (postoji == 0) {
				cout << "BR KORAKA " << brkoraka << endl;
				cout << endl << "Ne postoji takav racun u indeksu" << endl;

				continue;
			}
			cout << endl << "Broj Koraka da se nadje: " << brkoraka << endl;

		}
		if (c == '6') {

			cout << "UNESITE CID KORISNIKA KOGA ZELITE DA OBRISETE:" << endl;
			long long int CID;
			cin >> CID;
			int id;
			int br = 0;
			Cvor* za_brisanje = nadji_cvor(bstablo, CID, id, br);
			izbrisicvor(bstablo, za_brisanje, id, br);
			cout << "BROJ KORAKA " << br << endl;
			cout << endl;

		}
		if (c == '7') break;
	}


	return 0;
}

