#ifndef INTERFACEUSUARIO_H
#define INTERFACEUSUARIO_H

#include <string>
#include "shell.h"
//#include "macro.h"
#include "hash.h"
 
using namespace std;
 
class InterfaceUsuario {
 private:
  Shell cmd; // o executor de comandos
  string prompt; // o prompt a ser mostrado na linha que espera comando
  //Macro v_macro[10];
  //int cont_macro;
  TabelaHash<string> variaveis;
  TabelaHash<Lista<string> > macros;

 public:
  InterfaceUsuario(const string & umPrompt); // construtor
  ~InterfaceUsuario(); // destrutor
 
  // aguarda um comando, executa-o e retorna o status de terminação
  int leUmComando();
 
  // lê e executa comandos indefinidamente ...
  void serve_comandos();

  void criaMacro(string& var, Lista<string>& l1);
  
  //cria variavel
  void criaVar(string & var);
  
  //veririca se variavel existe e retorna o valor contido
  string verificaVar(string & var);
  
  //faz o tratamento de comandos locais
  void trata_comando(string & comando);


};

#endif