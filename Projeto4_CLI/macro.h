#ifndef MACRO_H
#define MACRO_H

#include "fila.h"

using namespace std;

class Macro {
  private:
    Fila<string> comandos; // cria fila macro
    string nome;

  public:
    Macro(string umNome);
    Macro(const Macro & outra); // construtor de copia
    Macro();
    ~Macro();

    void escreve(string dado);
    void escreve(Macro & outra);
    Fila<string> lista_comandos();
    string nome_macro();
};


#endif