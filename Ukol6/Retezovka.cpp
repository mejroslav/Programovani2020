#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

// Hyperbolicke funkce
double sinh(double x) {
    double ex = exp(x);
    return 0.5 * (ex - 1 / ex);
}
double cosh(double x) {
    double ex = exp(x);
    return 0.5 * (ex + 1 / ex);
}
// Rovnice pro retezovku
double bod_retezovky(double x, double a) {
    return a * (cosh(x / a) - 1);
}
// Delka retezovky
double delka_retezovky(double d, double a) {
    return 2 * a * sinh(0.5 * d / a);
}
// Proveseni spocita rozdil bodu na kraji (maxima retezovky) a v nule (minima retezovky)
double proveseni(double sirka, double a) {
    return bod_retezovky(sirka / 2, a) - bod_retezovky(0, a);
}



void vypisTabulku(double x_min, double x_max, double a, double sirka,int pocetBodu = 100) {
    std::ofstream soubor;

    static bool vytvor_novy_soubor = true;  // prvni krivku do noveho souboru, 
                                            // dalsi pak pripojit (angl. append)
                                            // append == ofstream::app 

    if (vytvor_novy_soubor)
        soubor.open("retezovka.txt");
    else
        soubor.open("retezovka.txt", std::ofstream::app);

    double const dx = (x_max - x_min) / pocetBodu;

    soubor << std::setprecision(9);

    for (double x = x_min; x < x_max + dx / 2; x += dx) {
        soubor << std::setw(12) << x << " "
            << std::setw(12) << bod_retezovky(x, a) - proveseni(sirka, a) << "\n"; // Chci, aby sly retezovky od nuly dolu, proto je posunu o "proveseni"
    }

    soubor << "\n";            // oddeluje nove krivky
    vytvor_novy_soubor = false;
    soubor.close();
}

double hodnota_a(double sirka, double delka) {
    double a1 = sirka / sqrt(24 * (delka / sirka - 1)); // krajni hodnota intervalu a1
    double a2 = sirka / (2 * log(delka / sirka)); // krajni hodnota intervalu a2
    double const epsilon = 1e-11; // pozadovana presnost: zvolil jsem 10 na minus jedenactou
    double old ; // dulezita promenna k ukonceni procesu
    double c = 0; 
    do {
        old = c; //ulozi c z minuleho cyklu
        c = (a1 + a2) / 2; //spocte novou hodnotu
        
        if (delka_retezovky(sirka, c) - delka < 0) a2 = c;
        else a1 = c; // zmeni jeden krajni bod intervalu
    } while (fabs(old - c) > epsilon * c); // podminka pro ukonceni programu

    return c;
}


double const sirka_zavesu = 3;

int main(int argc, char** argv) {
    std::cout << std::setprecision(12);

    for (double delka_lana = 4; delka_lana <= 7; delka_lana++) {
        double a = hodnota_a(sirka_zavesu, delka_lana);
        std::cout << std::setw(5) << sirka_zavesu << " "
            << std::setw(5) << delka_lana << " "
            << std::setw(15) << a << " "
            << std::setw(15) << delka_retezovky(sirka_zavesu, a) << " " 
            // delka retezovky by mela byt stejna jako delka lana na pozadovanou presnost
            << std::setw(15) << proveseni(sirka_zavesu, a) // pridan parametr proveseni
            << "\n";

        vypisTabulku(-sirka_zavesu / 2, sirka_zavesu / 2, a, sirka_zavesu);
    }

    return 0;
}
