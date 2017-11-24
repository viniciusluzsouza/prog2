#include <iostream>
#include <string>
#include "InterfaceUsuario.h"
#include "shell.h"
#include "pilha.h"
#include "fila.h"
#include "historico.h"
#include <sstream>
#include "macro.h"


using namespace std;
Historico hist(100);

InterfaceUsuario::InterfaceUsuario(const string & umPrompt) {
  prompt = umPrompt;
  cont_macro=0;
}

InterfaceUsuario::~InterfaceUsuario() {
}

int InterfaceUsuario::leUmComando() {
  
  string comando;
  int status, indice=0;

  do {
    cout << prompt;
    getline(cin, comando);
    if ( comando == "historico" ) {
      hist.escreve(comando);
      hist.lista();
    }
  } while ( comando.size() == 0 || (comando == "historico") );
  hist.escreve(comando);

  for (int n=0;n<10;n++) {
    if (comando == v_macro[n].nome_macro()) {
      Fila<string> macro_a_executar(10);
      macro_a_executar = v_macro[n].lista_comandos();
      while ( not macro_a_executar.vazia() ) {
        string macro_comando = macro_a_executar.desenfileira();
        for (int i=0;i<10;i++){
          if ( macro_comando == v_macro[i].nome_macro() ) {
            Fila<string> macro_complementar(10);
            macro_complementar = v_macro[i].lista_comandos();
            while ( not macro_complementar.vazia() ) cmd.execute(macro_complementar.desenfileira());
          }
        }
        cmd.execute(macro_comando);
      }
      return 999;
    }
  }

  if ( comando[0] == '!' ) {
    comando.erase(0, 1);
    istringstream stream(comando);
    stream >> indice;
    if (stream.fail()) {
      cout << "Erro: após ! deve vir um número." << endl;
      return -1;
    }
    if ( indice == 0 ) {
      cout << "Posição inexistente. Utilize a partir de !1." << endl;
      return -1;
    }

    try {
    comando = hist.le(indice);
    } catch (int e) {
      cout << "Erro: Posicao inalcançável." << endl;
      return -1;
    }
    cout << "Executando " << '"' << comando << '"' << " ..." << endl;
    status = cmd.execute(comando);
    return status;
  }
  else if ( comando == "sair" )
    return 1000;
  else {
    status = cmd.execute(comando);
    return status;
  }
 

}

void InterfaceUsuario::serve_comandos(){
}

void InterfaceUsuario::adiciona_macro(const Macro & macro){
  if ( cont_macro < 10 ) {
    v_macro[cont_macro] = macro;
    cont_macro++;
  }
}


