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
Historico hist(10);

InterfaceUsuario::InterfaceUsuario(const string & umPrompt) : variaveis(100), macros(100){
  prompt = umPrompt;
  //cont_macro=0;
}

InterfaceUsuario::~InterfaceUsuario() {
}

int InterfaceUsuario::leUmComando() {
  
  string comando;
  int status, indice=0;

  do {
    cout << prompt;
    getline(cin, comando);
    
    //tratamento de alguns comandos
    trata_comando(comando);
    
    //executador macro
    try {
        Lista<string> macro = macros.obtem(comando);
	macro.inicia();
	while ( not macro.fim() ) {
            string aux = macro.proximo();
            cmd.execute(aux);
            hist.escreve(aux);
            
	}
        comando.erase(0);
    } catch (int e) {
    }
    
    int pos = comando.find(' ');
    string umComando = comando.substr(0,pos);
    
    //definicao de variavel
    if ( umComando == "defvar" ) {
        hist.escreve(comando);
        comando = comando.substr(pos+1);
        try {
            criaVar(comando);
        } catch (int e) {
            cout << "Formato invalido. Utilize o formato \"defvar nome=variavel\"" << endl;
            comando.erase(0);
        }
    }
    
    //definicao de macro
    if ( umComando == "defmacro" ) {
        hist.escreve(comando);
	comando = comando.substr(pos+1);
	pos = comando.find('=');
	string aux = comando.substr(0,pos);
	Lista<string> l;
	criaMacro(comando, l);
        while (aux[0] == ' ') aux.erase(0,1);
        for (int j=aux.size()-1;aux[j] == ' ';j=aux.size()-1) aux.erase(j);
        int i = aux.find(' ');
        if ( i > 0 ) cout << "Formato invalido. Utilize o formato \"defmacro nome=comando1;comando2 ...\"" << endl;
	else macros.adiciona(aux, l);
	comando.erase(0);	
    }
    
  } while ( comando.size() == 0 );
  hist.escreve(comando);

  if ( comando == "sair" )
    return 1000;
  
  //o executador  
  status = cmd.execute(comando);
  return status;

}

void InterfaceUsuario::serve_comandos(){
}

void InterfaceUsuario::criaMacro(string& var, Lista<string>& l1){
    
    string valor;
    int n = var.find('=');
    var = var.substr(n+1);
    
    for ( n=var.find(';');n>0;n=var.find(';') ){
	valor = var.substr(0,n);
        while ( valor[0] == ' ' ) valor.erase(0,1);
        for (int j=valor.size()-1;valor[j] == ' ';j=valor.size()-1) valor.erase(j);
	l1.anexa(valor);
	var = var.substr(n+1);
    }
    while (var[0] == ' ') var.erase(0,1);
    for (int j=var.size()-1;var[j] == ' ';j=var.size()-1) var.erase(j);
    n = var.size();
    if ( n > 0 ) l1.anexa(var);

}

void InterfaceUsuario::criaVar(string& var) { 
    
    string nome, valor;
    int n = var.find('=');
    nome = var.substr(0,n);
    while ( nome[0] == ' ' ) nome.erase(0,1);
    for (int j=nome.size()-1;nome[j] == ' ';j=nome.size()-1) nome.erase(j);
    int i = nome.find(' ');
    if ( i > 0 ) throw -1;
    valor = var.substr(n+1);
    while ( valor[0] == ' ' ) valor.erase(0,1);
    for (int j=valor.size()-1;valor[j] == ' ';j=valor.size()-1) valor.erase(j);
    variaveis.adiciona(nome, valor);
    var.erase(0);
}

string InterfaceUsuario::verificaVar(string& var){
    
    //int i = var.find_last_of('$');
    //string final = var.substr(i+1);
    //i = final.find(' ');
    //if ( i >= 0 ) final.erase(0,i+1);
    //else final.erase(0);
    int p = var.find('$');
    string nova = var.substr(0,p);
    string resto = var.substr(p);
    
    p = resto.find('$');
    
    while ( p >= 0 ){
        int n = resto.find(' '); //|grep aula
        if (n > 0) {
            p++;
            string busca = resto.substr(p,n-p);
            busca = variaveis.obtem(busca);
            nova = nova + busca + ' ';   
        } else {
            resto = resto.erase(0,1);
            resto = variaveis.obtem(resto);
            nova += resto;
            break;
        }
        resto = resto.substr(n+1);
        p = resto.find('$');
    }
 
    //if (final.size()>0) nova = nova + final;
    
    //cout << "nova: " << nova << endl;
    
    return nova;
    
}

void InterfaceUsuario::trata_comando(string& comando){
    
    int status, indice=0;
    
  if ( comando[0] == '!' ) {
    comando.erase(0, 1);
    istringstream stream(comando);
    stream >> indice;
    if (stream.fail()) {
      cout << "Erro: após ! deve vir um número." << endl;
      comando.erase(0);
      return;
    }
    if ( indice == 0 ) {
      cout << "Posição inexistente. Utilize a partir de !1." << endl;
      comando.erase(0);
      return;
    }

    try {
    comando = hist.le(indice);
    } catch (int e) {
      cout << "Erro: Posicao inalcançável." << endl;
      comando.erase(0);
      return;
    }
    cout << "Executando " << '"' << comando << '"' << " ..." << endl;
  }
    
    if ( comando == "historico" ) {
      hist.escreve(comando);
      hist.lista();
      comando.erase(0);
    }
     
    int pos = comando.find('$');
    if ( pos > 0 ) {
	try {
            comando = verificaVar(comando);
            hist.escreve(comando);
	} catch (int e) {
	}
    }
   
       
}
