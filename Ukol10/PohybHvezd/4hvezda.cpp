#include <iostream>
#include <iomanip>
#include <array>
#include <cmath>

const double Pi = 4 * atan(1.0);
const double G = 4 * Pi* Pi;
enum I {      // seznam veličin vystupujících v dif. rovnici a jejich celkový počet dim
    X1, Y1, Z1,
    X2, Y2, Z2, 
    X3, Y3, Z3, 
    X4, Y4, Z4, 
    vx1, vy1, vz1, 
    vx2, vy2, vz2, 
    vx3, vy3, vz3, 
    vx4, vy4, vz4, 
    dim };

typedef std::array<double, dim> Vektor;     // typ pro uložení vektorové hodnoty hledané funkce
 
typedef Vektor PravaStrana(Vektor U, double t); // typ argumentu následujcící funkce

const double M [4] = {1,1,1,1}; // hmotnosti planet uskladnim do tohoto pole 

double sq(double a) {
    return a * a;
}

// toto je Eulerova metoda. Konkrétní dif. rovnice se jí podstrčí v prvním argumentu
void step_Euler(PravaStrana PohybRovnice, Vektor &Y, double &t, const double dt) {
    Vektor dYdt = PohybRovnice(Y, t);
    for (int i = 0; i < dim; i++)  Y[i] += dYdt[i]*dt;
    t += dt;
}

void krok_RK4( PravaStrana pohybovaRovnice,  Vektor &Y, double &t, const double dt) {

    Vektor k1_dYdt,k234_dYdt,Ytmp;
    double dt2 = 0.5*dt;

    k1_dYdt = pohybovaRovnice(Y, t);

    t += dt2;
    for (int i=0; i<dim; i++) Ytmp[i] = Y[i] + k1_dYdt[i]*dt2;
    k234_dYdt = pohybovaRovnice(Ytmp, t);

    for (int i=0; i<dim; i++) {
        Ytmp[i] = Y[i] + k234_dYdt[i]*dt2;
        k1_dYdt[i]+=2*k234_dYdt[i];
    }
    k234_dYdt = pohybovaRovnice(Ytmp, t);

    t += dt2;
    for (int i=0; i<dim; i++) {
        Ytmp[i] = Y[i] + k234_dYdt[i]*dt;
        k1_dYdt[i]+=2*k234_dYdt[i];
    }
    k234_dYdt = pohybovaRovnice(Ytmp, t);

    for (int i=0; i<dim; i++) Y[i] += (k1_dYdt[i]+k234_dYdt[i])*dt*0.1666666666666666667;
}


// toto je pohybová rovnice planety převedená na dvojici rovnic prvního řádu
Vektor pohybovaRovnicePlanety(Vektor U, double t) {
	Vektor dUdt;
    // vynuluj (pro jistotu)
    for (int i = 0; i != I::dim; i++) dUdt[i] = 0;

    // Krok pro prostorove slozky
    for (int i = 0; i != I::vx1; i++){
        dUdt[i] = U[I::vx1 + i]; // dx_i = v_i
    }
    
    // Krok pro rychlostni slozky
    
    // projdi dvojice castic, j-ta castice pusobi na i-tou
    for (int i = 0; i < I::vx1; i += 3) {
        for (int j = 0; j < I::vx1; j += 3) {
            if (i == j) continue; // self-interaction není

            double Rkvadrat
                = sq(U[j]-U[i])      // delta x kvadrát
                + sq(U[j+1]-U[i+1])  // delta y kvadrát
                + sq(U[j+2]-U[i+2]); // delta z kvadrát
            
            double Rm3 = pow(sqrt(Rkvadrat), -3);

            // projdi v kazde castici tri slozky
            for (int k = 0; k < 3; k++){
                dUdt[I::vx1 + i + k] // dv_(i+k)
                    += G*M[j/3]* Rm3 *(U[j+k] - U[i+k]);
            } 
        }
    }

	return dUdt;
}

double Energie (Vektor Y){
    double E = 0;

    // kineticka energie
    for (int j = I::vx1; j < I::dim; j+=3){
        E += 0.5 * ( sq(Y[j]) + sq(Y[j+1]) + sq(Y[j+2]) ) ;
    }

    // potencialni energie
    // projdi dvojice castic, j-ta castice pusobi na i-tou
    for (int i = 0; i < I::vx1; i += 3) {
        for (int j = 0; j < I::vx1; j += 3) {
            if (i == j) continue; // self-interaction není

            double Rkvadrat
                = sq(Y[j]-Y[i])      // delta x kvadrát
                + sq(Y[j+1]-Y[i+1])  // delta y kvadrát
                + sq(Y[j+2]-Y[i+2]); // delta z kvadrát
            

            // projdi v kazde castici tri slozky
            for (int k = 0; k < 3; k++){
                E -=  G / sqrt(Rkvadrat); 
            }
        }
    }
    return E;
}


bool jeNasobek(double x, double krok) {
  return fabs(x-krok*round(x/krok)) <= 1E-9*fabs(x); // nepříliš robustní !
}

int main()
{   
    //for( double dt = 0.1; dt>0.9E-6; dt*=0.1) { // zkus různě dlouhý krok
        const double dt = 1E-4;
        const double tmax = 10;

        // počáteční čas
        double t = 0;

        // počáteční polohy a rychlosti
        Vektor Y = {
            // Polohy
            -1,                -0.577384749851298, 0,
            -0.37244812608517, -0.219555937346995, 0,
             0.37244812608517,  0.219555937346995, 0, // x3 = -x2, y3 = -y2
             1,                 0.577384749851298, 0, // x4 = -x1, y4 = -y1

            // Rychlosti
            -0.074759317531738,  0.1998824206274314, 0,
             5.433755356745640, -9.4146817103186980, 0,
            -5.433755356745640,  9.4146817103186980, 0, // vx3 = -vx2, vy3 = -vy2
             0.074759317531738, -0.1998824206274314, 0, // vx4 = -vx1, vy4 = -vy1
        };
      
    // Zacni vypisovat pohyb

    std::cout << "#t \t x1 \t y1 \t z1 \t x2 \t y2 \t z2 \t z1 \t z2 \t x3 \t y3 \t z3 \t x4 \t y4 \t z4 \t E \n"; // Abych se v tom vyznal

    while (t < tmax + 1E-9) {  // postupuj v krocích ...

        if (jeNasobek(t,0.01)){
            std::cout << t;
            for (int i =0; i<dim/2; i++){
                std::cout << "\t" << Y[i];
            }
            std::cout << "\t" << Energie(Y) << "\n";
        }

    

    step_Euler(pohybovaRovnicePlanety, Y, t, dt);

	// krok_RK4(pohybovaRovnicePlanety, Y, t, dt);
      }
      std::cout << "\n\n";       // začni malovat novou čáru pro nové dt

    //}

    return 0;
}
