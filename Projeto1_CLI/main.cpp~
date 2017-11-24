#include <iostream>
#include "InterfaceUsuario.h"
#include "pilha.h"
#include "fila.h"
#include "macro.h"
 
using namespace std;
 
int main() {
  // cria um objeto InterfaceUsuario, com prompt "comando> "
  InterfaceUsuario interface("root@equipeT&V#: ");
  Macro macro1("m1");
  macro1.escreve("ls");
  macro1.escreve("pwd");
  macro1.escreve("date");
  interface.adiciona_macro(macro1);

  Macro macro2("m2");
  macro2.escreve("date");
  macro2.escreve("m1");
  macro2.escreve("df");
  interface.adiciona_macro(macro2);

  Macro macro3("m3");
  macro3.escreve("ls");
  macro3.escreve("date");
  macro3.escreve("pwd");
  macro3.escreve("date");
  macro3.escreve("df");
  macro3.escreve("date");
  interface.adiciona_macro(macro3);
 
  while (true) {
    int status = interface.leUmComando();
    if ( status == 1000 ) break;
    else if ( status == 2 ) {
      cout << "Comando nao encontrado" << endl;
      cout << "Executou com status: " << status << endl;
    }
    else if ( status == 999 ) cout << "Macro executada com sucesso" << endl;
    else
      cout << "Executou com status: " << status << endl;
  }
}
