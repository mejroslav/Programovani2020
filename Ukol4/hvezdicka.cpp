#include <iostream>
#include <cmath>

const double Pi = 4 * atan(1.0);

const int M = 8; // Pocet vrcholu mensiho mnohouhelniku
const double aM = 0.5; // Polomer kruznice opsane mensimu mnohouhelniku

const int V = 24; // Pocet vrcholu vetsiho mnohouhelniku
const double aV = 1.0; // Polomer kruznice opsane vetsimu mnohouhelniku

int main()
    {
        std::cout << "#Hvezdice z maleho a velkeho mnohouhelniku \n";
        std::cout << "#Maly mnohouhelnik: pocet vrcholu = " << M << ", polomer = " << aM << "\n";
        std::cout << "#Velky mnohouhelnik: pocet vrcholu = " << V << ", polomer = " << aV << "\n";
        
        // For-cyklus pres maly mnohouhelnik
        for (int i = 0; i < M; i++) {
            double phiM = 2 * Pi * i/ M; // uhel mezi osou x a vrcholem mensiho
            double xM = aM * cos(phiM);
            double yM = aM * sin(phiM);

            // For-cyklus pres velky mnohouhelnik
            for (int j = 0; j < V; j++) {
                double phiV = 2 * Pi * j / V; // uhel mezi osou x a vrcholem vetsiho
                double xV= aV * cos(phiV);
                double yV = aV * sin(phiV);

            // Lezi usecky mimo kruznici? To zjisti hodnota skalarniho soucinu. 
            // Pro jistotu ho odrazime od nuly nejakym malym cislem.
                if (xM * (xV - xM) + yM * (yV - yM) > 0.0001) {
                    // Pokud ano, vypis jejich souradnice
                    std::cout << "\n" << xM << "\t" << yM << "\n"
                    << xV << "\t" << yV << "\n" ;
                }
            }
        }
        return 0;
}
