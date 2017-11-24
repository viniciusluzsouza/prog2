#ifndef INTERFACEUSUARIO_H
#define INTERFACEUSUARIO_H

#include <string>
#include "shell.h"
#include "macro.h"
 
using namespace std;
 
class InterfaceUsuario {
 private:
  Shell cmd; // o executor de comandos
  string prompt; // o prompt a ser mostrado na linha que espera comando
  Macro v_macro[10];
  int cont_macro;

 public:
  InterfaceUsuario(const string & umPrompt); // construtor
  ~InterfaceUsuario(); // destrutor
 
  // aguarda um comando, executa-o e retorna o status de terminação
  int leUmComando();
 
  // lê e executa comandos indefinidamente ...
  void serve_comandos();

  void adiciona_macro(const Macro & macro);

};

#endif
