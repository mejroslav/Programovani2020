#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <fstream>


/**********************************************************
/ ***   Klicovy datovy typ pro ulozeni informaci o puntiku
/ **********************************************************/

typedef struct {
       double x,y;   // 2D poloha
       double vx,vy; // 2D rychlost
       double r;     // polomer kruznice
       double m;     // hmotnost (dulezita pri srazce)
}  tPuntik;

// pomocna funkce/makro oznacujici v dalsim kodu mista, kde mate kod doplnit
//#define zdeChybi(s) ((std::cout << "Zde chybi " << s << "\n"), assert(false), 0)

double sqr(double x) {
    return x*x;
}

/**********************************************************
/ ***   modifikace rychlosti pri odrazu dotykajicich se puntiku
/ **********************************************************/

void odraz(tPuntik& a, tPuntik& b)                  // {radek c. 31}
// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
{
  assert(fabs( sqr(a.x-b.x)+sqr(a.y-b.y)-sqr(a.r+b.r) ) < 1E-10 && "Kruznice se nedotykaji!" ); 

  double skal_soucin = (a.vx-b.vx)*(a.x-b.x)+(a.vy-b.vy)*(a.y-b.y);
  double faktor = 2/((a.m+b.m)*sqr(a.r+b.r) )* skal_soucin ;// "hodnota faktoru Q podle (3)")

  a.vx = a.vx - b.m * faktor*( a.x-b.x) ;//("x-slozka podle (1)")
  a.vy = a.vy - b.m * faktor*( a.y-b.y) ;

  b.vx = b.vx + a.m * faktor*( a.x-b.x) ;
  b.vy = b.vy + a.m * faktor*( a.y-b.y);
}


// **********************************************************
// **  vypocet casu zbyvajiciho do srazky dvou puntiku
// **********************************************************

double cas_do_srazky(tPuntik const &a, tPuntik const &b)
{ 
  const double nesrazi_se = -1.0;  // pokud se v budoucnu nepotkaji vraci funkce zaporne cislo

  double d2 = sqr(a.r+b.r);
  double x2 = sqr(a.x-b.x ) + sqr(a.y-b.y) ;// "|r1-r2|^2 vystupujici v (5)")
  
  assert( x2-d2 > -1E-10 && "Disky se prekryvaji!" ); 
  
  double v2 = sqr(a.vx-b.vx ) + sqr(a.vy-b.vy) ; //("|v1-v2|^2 vystupujici v (5)");
  if (v2==0) return nesrazi_se;    // disky se vuci sobe nepohybuji

  double skal_soucin = (a.vx-b.vx)*(a.x-b.x)+(a.vy-b.vy)*(a.y-b.y) ; // ("(v1-v2).(r1-r2) vystupujici v (4-5)");
  double diskriminant = sqr(skal_soucin) - v2*(x2-d2) ;//("hodnota D podle (5)");
  
  if (diskriminant<=0) return nesrazi_se;

  return (-skal_soucin-sqrt(diskriminant))/v2;
}       


/**********************************************************
/ ***  rychly test spravnosti funkci cas_do_srazky a Odraz
/ **********************************************************/
void testSrazeni()
{                    //   x     y    vx  vy   r   m
    tPuntik  a = { 10.0, 71.6,  7,  1,  4,  1 };
    tPuntik  b = { 41.2, 20.0,  1,  9,  6,  2 };
    const double tpq = 5.2;
    const double eps = 1E-11;

    // 1. kontrola, ze se spravne pocita cas do srazky
    double cas = cas_do_srazky(a,b);
    a.x+= a.vx*cas;     a.y+= a.vy*cas;
    b.x+= b.vx*cas;     b.y+= b.vy*cas;
    if (fabs( tpq - cas ) > eps) {
       std::cout << "Test funkce cas_do_srazky nedopadnul dobre.\n"
                 << "Misto " << tpq << " vratila " << cas <<"\n";
       // assert(false);
    }
   // 2. kontrola, ze se pri srazce zachovava hybnost a energie
   double px = a.m*a.vx+b.m*b.vx; 
   double py = a.m*a.vy+b.m*b.vy;
   double Ek = 0.5*a.m*(sqr(a.vx)+sqr(a.vy))+0.5*b.m*(sqr(b.vx)+sqr(b.vy));
   
   odraz(a,b);
   
   double qx = a.m*a.vx+b.m*b.vx; 
   double qy = a.m*a.vy+b.m*b.vy;
   double Fk = 0.5*a.m*(sqr(a.vx)+sqr(a.vy))+0.5*b.m*(sqr(b.vx)+sqr(b.vy));
   if (fabs( px-qx ) + fabs( py-qy ) > eps) {
       std::cout << "Test funkce odraz nedopadnul dobre.\n"
                 << "Misto [" << px<< "," << py << "] vratila po srazce hybnost ["
                 << qx << "," << qy << "]\n"; 
       assert(false);
   }
   if (fabs( Ek-Fk ) > eps) {
       std::cout << "Test funkce odraz nedopadnul dobre.\n"
                 << "Misto " << Ek << " vratila po srazce energii " << Fk << "\n";
       assert(false);
   }
}  


// **********************************************************
// *** Seznam puntiku (glob. promenna)
// ***   -- blok velkych puntiku
// ***   -- pocatecni poloha a rychlost maleho puntiku
// **********************************************************
                     //   x    y   vx   vy     r    m
const tPuntik malyP  = { -119, 0, 239,   2,  0.5,   1};
const tPuntik velkyP = {   0,  0,   0,   0,    4, 100};

const int npx = 6;
const int npy = 6;
const int pocet_puntiku = npx*npy+1;
const double rozestup = 10;

  
tPuntik puntiky[pocet_puntiku];

void provedRozmisteni() {
   puntiky[0] = malyP;
   int n = 1;
   for (int i = 0; i<npx; i++) 
     for (int j = 0; j<npy; j++) {
       tPuntik a = velkyP;
       a.x = (i+0.5-npx/2.0)*rozestup;
       a.y = (j+0.5-npy/2.0)*rozestup;
       puntiky[n] = a;
       n = n+1;                          // {radek c. 141}
     }
}



// **********************************************************
// ***   Vystup do souboru
// **********************************************************

std::ofstream vystup;

double  cas  = 0;
double  cas_vypisu = 0;
const double dt_vypis = 1.0/16;

// Vypis polohy bodu posunute o cas dt za prepokladu pohybu bez srazek
void vypisPolohyPuntiku(double dt) 
{
   for (int n=0; n<pocet_puntiku; n++) 
     vystup << puntiky[n].x+puntiky[n].vx*dt << " "
            << puntiky[n].y+puntiky[n].vy*dt << " "
            << puntiky[n].r << "\n";
 
   vystup << "\n\n";
}


// **********************************************************
// ***  Evoluce
// **********************************************************

void posunVCase(double dt)
{
   for (int n=0; n<pocet_puntiku; n++) {
     puntiky[n].x += puntiky[n].vx*dt;
     puntiky[n].y += puntiky[n].vy*dt;
   }
   cas += dt;
}
   

bool dalsiSrazka()
{
const double nikdy = 1E20;

   // 1. najdi dvojici disku, ktere se srazi nejdrive
   int m1,n1; 
   double dt_min = nikdy;
   for (int m=0; m<pocet_puntiku; m++)
     for (int n=m+1; n<pocet_puntiku; n++) {        // {radek c. 191}
           double dt = cas_do_srazky(puntiky[m],puntiky[n]);
           if (dt>0 && dt<dt_min) {
               dt_min = dt;
               m1 = m;
               n1 = n;
           }
     }
   
   if (dt_min == nikdy) return false;

   //2. Pokud k takove srazce dojde
   //   - posun vsechny disky do casu srazky
   //   - otoc rychlosti srazejicich se disku
   //   - vypis take data potrebna k vykresleni animace

   
    std::cout << std::setw(4) << m1 << std::setw(4) << n1 << std::setw(12) << dt_min << "\n";
    while (cas+dt_min > cas_vypisu) {
        vypisPolohyPuntiku(cas_vypisu-cas);
        cas_vypisu += dt_vypis;
    }
    
    posunVCase(dt_min);
    odraz(puntiky[m1],puntiky[n1]);
    return true;
}        

int main(int argc, char**argv) {
    testSrazeni();

    provedRozmisteni();   

    vystup.open("puntiky.txt");

    while (dalsiSrazka()) ;     
    
    vystup.close();
   
    return 0;
}


/* ************************************************************
 *   instrukce pro pouzit GNUPLOT
 * ************************************************************
# Prikazy pro vykresleni souboru puntiky.txt
# nejprve nastavime zpusob zobrazeni a barvy

set size ratio -1
set palette model RGB defined ( 0 'red', 4 'blue' )
set cbrange [0:4]
unset colorbox

# opakovane muzeme pak prikazem (ano, dlouhym) spustit animaci

stats 'puntiky.txt' nooutput; do for [k=0:STATS_blocks-2] { plot [-80:80][-80:80] 'puntiky.txt' index k using 1:2:3:3 with circles palette fs solid; pause 0.1 }

# az bude dobre vypadat, vyrobime animovany gif nasledujicimi prikazy (chvili to trva...)

set term gif size 800,800 animate; set output 'puntiky.gif'; do for [k=0:STATS_blocks-2] { plot [-80:80][-80:80] 'puntiky.txt' index k using 1:2:3:3 with circles palette fs solid } ; unset term

# komu se nelibi jeden dlouhy radek, lze jej rozdelit pouzitim \

set term gif size 800,800 animate; \
set output 'puntiky.gif'; \
do for [k=0:STATS_blocks-2] { \
  plot [-80:80][-80:80] 'puntiky.txt' index k using 1:2:3:3 with circles palette fs solid; \
  pause 0.1 \
} ;\
unset term


*/
