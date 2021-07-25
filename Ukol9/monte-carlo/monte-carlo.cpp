#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <array>
#include <stdexcept>
#include <vector>


double nahodneCislo() {
	return rand() / (double)(RAND_MAX);
}

struct Vektor3{
	double x, y, z;

	void kopiruj(Vektor3 zdroj) {
		x = zdroj.x;
		y = zdroj.y;
		z = zdroj.z;
	};

	// zavedl jsem metody pro přičítani vektorů, násobení vektoru číslem a čtverec délky vektoru
	void skaluj(double nasobek){ x *= nasobek; y *= nasobek; z *= nasobek;};
	void pricti(Vektor3 v) { x += v.x; y += v.y; z += v.z; };
	void odecti(Vektor3 v) { x -= v.x; y -= v.y; z -= v.z; };
	double ctverecDelky() { return x*x + y*y + z*z; };

	// spočítá střední hodnotu statistického souboru
	static Vektor3 prumer(std::vector<Vektor3> soubor) {
		Vektor3 prumer = {0,0,0};
		int delka = soubor.size();

		for (int i = 0; i < delka; i++) {
			prumer.pricti(soubor[i]);
		}

		prumer.skaluj(1.0 / delka);

		return prumer;
	}

	// spočítá směrodatnou odchylku statistického souboru
	static double smerodatnaOdchylka(std::vector<Vektor3> soubor){
		Vektor3 prumer = Vektor3::prumer(soubor);
		double variance = 0;
		Vektor3 v;

		int delka = soubor.size();

		for (int i =0; i< delka; i++ ){
			// variance += ‖ Aᵢ − ⟨A⟩ ∥²

			v.kopiruj(soubor[i]);
			v.odecti(prumer);
			variance += v.ctverecDelky();
		}

		variance /= delka;
		return sqrt(variance);
	}
};

typedef std::vector<Vektor3> SouborVektor3;

// generuje čísla v kvádru (-1,1)×(-1,1)×(-1,-1+vyska)
Vektor3 nahodnyBodVKvadru(double vyska) {
	return {2*nahodneCislo() - 1, 2*nahodneCislo() - 1, vyska*nahodneCislo() - 1 };
}

Vektor3 nahodnyBodVKuloveUseci(double vyska) {
	Vektor3 bod = nahodnyBodVKvadru(vyska);
	double R = bod.ctverecDelky();
	if (R <= 1) return bod;
	return nahodnyBodVKuloveUseci(vyska);
}

Vektor3 tezisteKuloveUsece(double vyska, int pocetBodu) {
	Vektor3 teziste = {0,0,0};

	for (int i = 0; i < pocetBodu; i++) {
		teziste.pricti(nahodnyBodVKuloveUseci(vyska));
	}
	teziste.skaluj(1.0/pocetBodu);
	return teziste;
}

Vektor3 validovaneTeziste(
	double vyska,
	int N, // pocet pokusu
	int POCET_BODU, // pocet bodu v kazdem pokusu
	double EPSILON // presnost
	){
	SouborVektor3 pokusy (N);

	for (int i=0; i<N; i++){
		pokusy[i] = tezisteKuloveUsece(vyska, POCET_BODU);
		// std::cout << "pokus cislo " << i << " je " << pokusy[i].z << "\n";
		// pouze pro debug
	}

	Vektor3 prumer = Vektor3::prumer(pokusy);
	// std::cout << "prumer: " << prumer.x << ", " << prumer.y << ", " << prumer.z << "\n";
	// pouze pro debug
	
	double odchylka = Vektor3::smerodatnaOdchylka(pokusy);
	double chyba = odchylka / sqrt(N);
	/*
	std::cout << "odchylka: " << odchylka << "\n";
	std::cout << "chyba: " << chyba;
	vypocet odchylky a chyby pouze pro debug
	*/
	if (chyba > EPSILON) {
		throw std::runtime_error("Chyba byla prilis vysoka!");
	}
	
	return prumer;

}

int main(){

	double vysky[] = {0.25, 0.5, 0.75, 1.0};
	const int pocet_pokusu = 10;
	const int pocet_bodu = 1e7;
	const double presnost = 1e-4;

	std::cout 
		<< "Tento program pocita teziste kulove usece se stredem v pocatku \n"
		<< "Pocet pokusu: " << pocet_pokusu << "\n"
		<< "Pocet bodu v jednom pokusu: " << pocet_bodu << "\n"
		<< "Presnost: " << presnost << "\n\n"; 

	for (int j = 0; j < 4; j++){
		std::cout << "Vyska = " << vysky[j] << "\n";
		Vektor3 Vysledek =  validovaneTeziste(vysky[j],pocet_pokusu, pocet_bodu, presnost);
		
		std::cout 
			<< "x = "<< Vysledek.x 
			<< ", y = " << Vysledek.y 
			<< ", z = " << Vysledek.z 
			<< "\n\n";
	}
	return 0;
}
