#include <iostream>
#include <string>
#include "fila.h"
#include "macro.h"

using namespace std;

Macro::Macro(string umNome) : comandos(10) {
  nome = umNome;
}

Macro::Macro() : comandos(10) {
}

Macro::Macro(const Macro & outra) : comandos(1) {
  *this = outra;
}

void Macro::escreve(string dado){
  comandos.enfileira(dado);
}

Macro::~Macro(){
}

string Macro::nome_macro() {
  return nome;
};

Fila<string> Macro::lista_comandos() {
  Fila<string> umMacro(10);
  umMacro = comandos;
  return umMacro;
}

