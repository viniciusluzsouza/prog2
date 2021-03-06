#ifndef HISTORICO_H
#define HISTORICO_H

#include "pilha.h"
#include <string>

using namespace std;

class Historico {
  private:
    Pilha<string> history; // cria historico
    int tam;

  public:
    Historico(int tamanho) ; // construtor

    ~Historico(); // destrutor
 
    void escreve(string dado);
    void lista();
    string le(int indice);
};

Historico::Historico(int tamanho) : history(tamanho) {
  tam = tamanho;
}

Historico::~Historico(){
}


void Historico::escreve(string dado){
  history.push(dado);
}

void Historico::lista(){
  Pilha<string> copia_history(tam);
  Pilha<string> copia_final(tam);
  copia_history = history;

  while ( not copia_history.vazia() )
    copia_final.push(copia_history.pop());

  while ( not copia_final.vazia() )
    cout <<  copia_final.pop() << endl;
}

string Historico::le(int indice){
  Pilha<string> copia_history(10);
  copia_history = history;
  string dado;

  if (indice > history.tamanho() || indice < 0) throw -1;

  for (int i=0;i<indice-1;i++)
    copia_history.pop();

  dado = copia_history.pop();

  return dado;

}


#endif