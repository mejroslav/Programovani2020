//Sirpinskeho trojuhelnik
#include <iostream>
#include <cmath>

void VypisObvod(double xa, double ya,
    double xb, double yb,
    double xc, double yc,
    int n) {

    std::cout << xa << " " << ya << " " << n << "\n";
    std::cout << xb << " " << yb << " " << n << "\n";
    std::cout << xc << " " << yc << " " << n << "\n";
    std::cout << xa << " " << ya << " " << n << "\n";

    std::cout << "\n";
 
    double xab = (xa + xb) / 2;
    double yab = (ya + yb) / 2;
    double xac = (xa + xc) / 2;
    double yac = (ya + yc) / 2;
    double xbc = (xb + xc) / 2;
    double ybc = (yb + yc) / 2;

    if (n > 0) {
        VypisObvod(xa, ya, xab, yab, xac, yac , n - 1);
        VypisObvod(xab, yab, xb, yb, xbc, ybc, n - 1);
        VypisObvod(xac, yac, xbc, ybc, xc, yc, n - 1);
    }
}

int main()
{
    // Pocatecni podminky
    double xa =0 , ya = 0, xb = 2, yb = 0, xc = 1, yc = sqrt(3), n = 5; 
    
    VypisObvod(xa, ya, xb, yb, xc, yc, n);
    return(0);
}

