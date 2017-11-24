#include <iostream>
#include <string>
#include <fstream>
#include "Arvore.h"
#include "GeoIP.h"
#include "GeoLocalizador.h"
 
using namespace std;
 
int main() {
    
    GeoLocalizador localiza;
    unsigned int opcao;
    bool sair = false;
    
    cout << "#######################################" << endl;
    cout << "#### Localizador Geográfico de IPs ####" << endl;
    cout << "####          Bem Vindo!!          ####" << endl;
    
    while ( not sair ) {
        cout << "#######################################" << endl;
        cout << "#######################################" << endl;
        cout << "Digite a opcao desejada: " << endl;
        cout << "1) Pesquisa por IP;" << endl;
        cout << "2) Pesquisa por Pais;" << endl;
        cout << "3) Pesquisa por Estado;" << endl;
        cout << "4) Pesquisa por Cidade;" << endl;
        cout << "0 - Para sair." << endl;
        cout << "---------------------------------------" << endl;
        cin >> opcao;
    
        string valor, pais, estado, cidade;
        switch (opcao){
            case 1:
                cout << "Entre com o endereço: ";
                cin >> valor;
                localiza.Busca(valor);
                break;
           
            case 2:
                cout << "A string deve ser exatamente igual ao arquivo." << endl;
                cout << "Entre com o pais: ";
                cin >> pais;
                localiza.BuscaPais(pais);
                break;
                
            case 3:
                cout << "Funcao nao implementada." << endl;
                //cout << "Entre com o pais: ";
                //cin >> pais;
                //cout << "Entre com o estado: ";
                //cin >> estado;
                //localiza.BuscaEstado(pais, estado);
                break;
                
            case 4:
                cout << "Funcao nao implementada." << endl;
                //cout << "Entre com o pais: ";
                //cin >> pais;
                //cout << "Entre com o estado: ";
                //cin >> estado;
                //cout << "Entre com a cidade: ";
                //cin >> cidade;
                //cout << "Obs.: Ao rodar este metodo o programa finaliza (motivo nao descoberto)" << endl;
                //localiza.BuscaCidade(pais, estado, cidade);
                break;
            
            case 0:
                sair = true;
                break;
                
            default:
                cout << "Opcao invalida" << endl;
                break;
            
        }
    
    }
    
    localiza.Finaliza();
    
}