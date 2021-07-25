// TODO: jak spocitat odpor??

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

typedef std::vector<double> tVektor;
typedef std::vector<tVektor> tMatice;

void vypisVektor(const tVektor x) {
    for (int i =0 ; i<x.size(); i++)
        std::cout << x[i] << " ";
    std::cout << "\n";
}

void vypisMatici(const tMatice m) {
    for (int r = 0 ; r<m.size(); r++) {
        for (int s = 0 ; s<m[r].size(); s++) {
            std::cout.width(6);
            std::cout.precision(3);
            std::cout << m[r][s] << " ";
        }    
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Pro vypsani soustavy ve tvaru Gaussovy eliminace
void vypisSoustavu(const tMatice m, const tVektor v) {
    for (int r = 0 ; r<m.size(); r++) {
        for (int s = 0 ; s<m[r].size(); s++) {
            std::cout.width(6);
            std::cout.precision(3);
            std::cout << m[r][s] << " ";
        }    
        std::cout << "| " << v[r] << "\n";
    }
    std::cout << "\n";
}

// Kod ze cviceni pro reseni soustavy Ax=b
tVektor LinearSolve(tMatice A, tVektor b) {

int n = A.size();    
for (int r = 0 ; r < n; r++) {
    // 1. zaridit vhodny prvek na diagonale
    //   prohozenim radku r s nekterym z intervalu r .. A.size()-1
    //   nezapomeneme na prislusny prvek vektoru b  
    int rMax = r;
    for (int r1 = r+1 ; r1 < n; r1++) 
        if ( fabs(A[r1][r]) > fabs(A[rMax][r]) ) rMax = r1;
    if (rMax!=r) {
       tVektor v = A[rMax]; A[rMax] = A[r]; A[r] = v;
       double  d = b[rMax]; b[rMax] = b[r]; b[r] = d;
    }  
    
    // 2. normalizace, tedy vynasobeni radku r hodnotou 1/A[r,r]
    //    nezapomeneme na prislusny prvek vektoru b   
    double q = 1/A[r][r];
    for (int s=r; s<n; s++) A[r][s] *= q; //vynasobit zbyle prvky radku
    b[r] *= q;                            //vynasobit b[r]
    
    // 3. eliminace, od vsech {r1}:r1 != r, odecti vhodny nasobek {r}
    //    nezapomeneme na prislusny (r1) prvek vektoru b
    for (int r1=0; r1<n; r1++) 
        if (r1!=r) {
            double q = A[r1][r];
            for (int s=r; s<n; s++) A[r1][s] -= q*A[r][s];
            b[r1] -= q*b[r];
        }
    //  Pozustatky ladeni
    //vypisMatici(A);
    //vypisVektor(b);
    //assert(false);
}


return b;
    
}

tMatice nulovaMatice (int n) {
    tMatice Matice(n, tVektor(n, 0.0)); 
     for (int r = 0 ; r<n; r++) {
        for (int s = 0 ; s<n; s++) {
            Matice[r][s] = 0 % n;
        }    
    }  
   return Matice;  
}

tVektor nulovyVektor (int n){
    tVektor Vektor;
    for (int i = 0; i<n; i++) Vektor[i] =0;
    return Vektor;
}

tVektor pravaStrana(int n) {
  tVektor v(n, 0.0);  // vektor je slozen z _n_ nul   
  
  for (int r = 0 ; r<n; r++) {
            v[r] = (0.5 - r % 2)*n*n*(n-1);
    }  
   return v;  
}

tMatice maticeSoustavy(int n) {
  tMatice Matice(n, tVektor(n, 0.0));  
  // matice (Matice) se sklada z (n) radku 
  // kazdy radek je slozen z (n) nul   
  for (int r = 0 ; r<n; r++) {
        for (int s = 0 ; s<n; s++) {
            Matice[r][s] = (r+s) % n;
        }    
    }  
   return Matice;  
}



void Obsad (tMatice& Mat, tVektor& Vek, int N) {

    // Obsazovani matice
    const int k = Mat.size(); // pocet radku/sloupcu matice

    for (int i = 0; i < k; i++){
        for (int j = 0; j < k; j++){

            //Postupujeme po radcich
            int p = j; // p cisluje uzel
            int s = p % N; // s cisluje sloupec matice
            int r = (p-s)/N; // r cisluje radek matice

            // Implicitni konverze na cislo
            if (i==j) Mat[i][j] = 2 + (r > 0 && r < N-1) + (s > 0 && s < N-1);
            // Jestlize odpor vede horizontalne, pak se indexy lisi o 1
            // Jestlize odpor vede vertikalne, pak se indexy lisi o n
            // Musime byt ale opatrni a zakazat prechody mezi krajnimi
            // odpory k(N-1) -> kN, kN -> k(N-1)
            else if  ( 
                (j == i + 1  && s != 0)     || 
                (j == i - 1 && s != N-1)    || 
                (abs(i-j) == N )
             )  Mat[i][j] = -1 ;
        }
    }

    // Obsazovani vektoru
     Vek[N*(N-1)] = 1;
}


int main(){

    const int N = 3; // pocet radku a sloupcu

    tMatice B = nulovaMatice(N*N-1);
    tVektor I(N*N-1) ;

    Obsad(B,I,N);
    tVektor Reseni = LinearSolve(B,I);

    std::cout
        << "Tento program resi odpory ctvercove site.\n"
        << "Pocet radku/sloupcu: " << N << "\n\n"
        << "Matice soustavy:\n";
    vypisSoustavu(B,I);
    std::cout << "\n";

    std::cout << "Vektor U:\n";
    vypisVektor(Reseni);
    std::cout 
        << "\n"
        << "Odpor = " << Reseni[N*(N-1)] << " " << "\n";

    return 0;
}
