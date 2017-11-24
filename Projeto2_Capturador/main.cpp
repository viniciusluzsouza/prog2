/* 
 * File:   main.cpp
 * Author: msobral
 *
 * Created on 1 de Abril de 2016, 14:04
 */

#include <cstdlib>
#include <iostream>
#include "Capturador.h"
#include "Lista.h"
#include "Analisador.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    
    bool sair=false;
    string interface;
    cout << endl;
    cout << "*** Entre com a interface a ser capturada: ";
    cin >> interface;

    while ( not sair ){
        
        int opcao=0;
                
        cout << endl;
        cout << "###########################################" << endl;
        cout << "########### CAPTURADOR T&V DUMP ###########" << endl;
        cout << "###########################################" << endl;
        cout << endl;
        cout << "Selecione o tipo de captura:" << endl;
        cout << "1 - Capturar dados em ordem crescente;" << endl;
        cout << "2 - Capturar dados em ordem decrescente;" << endl;
        cout << "3 - Capturar e contar dados transferidos;" << endl;
        cout << "4 - Capturar dados e apresentar taxa de bits por fluxo;" << endl;
        cout << "5 - Contar quantidade de fluxos de uma captura;" << endl;
        cout << "6 - Identificar fluxos com maior transferencia de dados;" << endl;
        cout << "7 - Apresentar periodicamente as taxas de bits por segundo." << endl;
        cout << "0 - Para sair do capturador" << endl;
        cout << "OPCAO: ";
        cin >> opcao;
        
        Analisador analise(interface.c_str());
        
        switch (opcao){
            case 1:    
                cout << "Serão caputardos N pacotes durante 10 segundos." << endl;
                cout << "Iniciando captura ..." << endl;
                analise.capturaCrescente();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
            
            case 2:
                cout << "Serão caputados N pacotes durantes 10 segundos." << endl;
                cout << "Iniciando captura ..." << endl;
                analise.capturaDecrescente();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
                
            case 3:
                cout << "Serão capturados N pacotes durante 10 segundos" << endl;
                cout << "Iniciando captura ..." << endl;
                analise.capturaContarTransferidos();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
                
            case 4:
                cout << "Serão capturados N pacotes durante 10 segundos" << endl;
                cout << "Iniciando captura ..." << endl;
                analise.capturaTaxaFluxo();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
                
            case 5:
                cout << "Serão capturados N pacotes durante 10 segundos" << endl;
                cout << "Iniciando captura ..." << endl;
                analise.contabilizaFluxo();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
                
            case 6:
                cout << "Serão capturados N pacotes durante 10 segundos" << endl;
                cout << "Iniciando captura ..." << endl;
                analise.maioresFluxos();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                
                break;
                
            case 7:
                analise.capturaPeriodica();
                cout << endl;
                cout << "*** Captura finalizada! ***" << endl;
                break;
                
            default:
                cout << endl;
                cout << "BYE! :)" << endl;
                cout << endl;
                sair=true;
                break;
           
        }
    }
    
    
    
    return 0;
}
