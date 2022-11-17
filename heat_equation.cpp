#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef struct celdas
{
  int filas;
  int columnas;
  double *elementos;
} celdas;

double Uinicial(int);
double UinicialVel(int);

void imprimirMatriz(double *elem, int n, int m)
{
  for (int t = 0; t < n; t++)
  {
    for (int i = 0; i < m; i++)
    {
      cout << elem[t * m + i] << ' '; // a ceros, para una prueba inicial
    }
    cout << endl;
  }
}

int main(int argc, char *argv[])
{
  int n = atoi(argv[1]) * atoi(argv[1]); // número de filas
  int m = atoi(argv[2]);                 // número de columnas
  // lo escribo así porque después mejoraré el código

  // Variables para manejar los tiempos
  double t0, t1;

  if (n * m == 4194304)
  {
    cout << "Matriz demasiado grande." << endl;
    return 1;
  }
  celdas U, d_U, V;
  U.columnas = m;
  U.filas = n;
  V.columnas = m;
  V.filas = n;
  size_t memSize = n * m * sizeof(double);
  U.elementos = (double *)malloc(memSize);
  V.elementos = (double *)malloc(memSize);
  // condiciones iniciales y de frontera
  const double UbordeInicial = 100.0;
  const double UbordeFinal = 0.0;
  // propiedades físicas de la barra, acero
  const double k = 1;
  const double deltaT = 0.01; // cada décima de segundo
  const double deltaX = 1;    // la barra es m veces este valor
  const double C = (k * deltaT) / deltaX;
  // i, j, k para las dimensiones espaciales, t para el tiempo
  //  en este caso, i va con las x.
  for (int t = 0; t < n; t++)
  {
    for (int i = 0; i < m; i++)
    {
      U.elementos[t * m + i] = 0.0; // a ceros, para una prueba inicial
    }
  }
  // condiciones iniciales y de frontera
  for (int i = 0; i < m; i++)
  {
    U.elementos[0 * m + i] = Uinicial(i);
  }
  for (int t = 0; t < n; t++)
  {
    U.elementos[t * m + 0] = UbordeInicial;
    U.elementos[t * m + (m - 1)] = UbordeFinal;
  }
  cout << endl;
  // imprimirMatriz(U.elementos, n, m);
  //  copia a GPU
  //  ciclo principal en cpu
  cout << "CPU:" << endl;
  t0 = clock();
  int t, i;

  for (t = 0; t < n - 1; t++)
  {
    for (i = 1; i < m - 1; i++)
    {
      // ecuación principal.
      U.elementos[(t + 1) * m + i] = U.elementos[t * m + i] + C * (U.elementos[t * m + (i + 1)] - 2 * U.elementos[t * m + i] + U.elementos[t * m + (i - 1)]);
    }
    cout << "Paso de tiempo " << t << endl;
    // cout << ".";
    imprimirMatriz(U.elementos, n, m);
  }

  t1 = clock();
  double time = (double(t1 - t0) / CLOCKS_PER_SEC);
  cout << '\n'
       << "El tiempo en CPU es: " << time << " El tiempo en CPU con";
  cout << endl;
}

double Uinicial(int i)
{
  // esto para facilitar cambiar las condiciones iniciales en el futuro
  return 0.0;
}

double UinicialVel(int i)
{
  // esto para facilitar cambiar las condiciones iniciales en el futuro
  return 0.0;
}