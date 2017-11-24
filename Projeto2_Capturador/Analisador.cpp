/* 
 * File:   Analisador.cpp
 * Author: Vinicius e Thiago
 * 
 * Created on 25 de Maio de 2016, 16:00
 */


#include <iostream>
#include <string>
#include "Lista.h"
#include "Capturador.h"
#include "Analisador.h"
#include "Pacote.h"

using namespace std;

struct Fluxo {
    in_addr src_ip;
    in_addr dst_ip;
    u_char proto;
    u_short src_port;
    u_short dst_port;
    unsigned int contador;
    
    
    inline bool operator==(const Fluxo& outro){
        
        if ( (src_ip.s_addr==outro.src_ip.s_addr ) && (dst_ip.s_addr==outro.dst_ip.s_addr) && (proto==outro.proto) && (src_port==outro.src_port) && (dst_port==outro.dst_port) ) {
            return true;
        } else {
            return false;
        }
    } 
    
    inline bool operator<(const Fluxo& outro){
        return contador<outro.contador;
    }
          
};

//CONSTRUTOR
Analisador::Analisador(const char * inet) : cap(1518, inet) {
}

//DESTRUTOR
Analisador::~Analisador(){
    
}

//Captura pacotes e retorna em ordem crescente de tamanho
void Analisador::capturaCrescente(){
    Lista<Frame> * captura = cap.capture(0,10);
    captura->ordenaSelecao();
    captura->inicia();
    while (not captura->fim() ){
        Frame & pacote = captura->proximo();
        cout << pacote << endl;
    }
    
    
}

//Captura pacotes e retorna em ordem decrescente de tamanho
void Analisador::capturaDecrescente(){
    Lista<Frame> * captura = cap.capture(0,10);
    captura->ordenaSelecao();
    for (int n=captura->comprimento() - 2;n>=0;n--){
        captura->anexa(captura->remove(n));
    }
    captura->inicia();
    while ( not captura->fim() ) {
        Frame & pacote = captura->proximo();
        cout << pacote << endl;
    }  
}

//Captura pacotes e contabiliza por faixa de tamanhos. De 100 em 100 bytes.
void Analisador::capturaContarTransferidos(){
    Lista<Frame> * captura = cap.capture(0,10);
    captura->ordenaSelecao();
    int contador=0;
    int n=100;
    captura->inicia();
    while ( not captura->fim() ) {
        Frame & pacote = captura->proximo();
        if ( pacote->get_size() <= n ) {
            contador++;
        } else if ( pacote->get_size() > n ) {
            cout << (int)contador << " pacotes ate " << n << " bytes." << endl;
            if ( pacote->get_size() < (n+100) )
                contador=1;
            else contador=0;
            n=n+100;
        }
    }

}

//Captura pacotes e retorna a quantidade de bytes por fluxo
void Analisador::capturaTaxaFluxo(){
    cap.muda_filtro("ip");
    Lista<Frame> * captura = cap.capture(0,10);
    Lista<Fluxo> LFluxo;
    
    captura->inicia();
    while ( not captura->fim() ){
        struct Fluxo umFluxo;
        Frame & pacote = captura->proximo();
        IP * ip = (IP*)pacote->get_payload();
        umFluxo.src_ip=ip->get_src();
        umFluxo.dst_ip=ip->get_dest();
        umFluxo.proto=ip->get_proto();
        if ( ip->get_proto() == IP::proto_TCP ){
            TCP * tcp = (TCP*) ip->get_payload();
            umFluxo.src_port=tcp->get_sport();
            umFluxo.dst_port=tcp->get_dport();
        } else if ( ip->get_proto() == IP::proto_UDP ){
            UDP * udp = (UDP*) ip->get_payload();
            umFluxo.src_port=udp->get_sport();
            umFluxo.dst_port=udp->get_dport();
        }
        umFluxo.contador=pacote->get_size();
        LFluxo.inicia();
        bool existe=0;
        while ( not LFluxo.fim() ){
            int n=0;
            struct Fluxo & outro = LFluxo.proximo();
            if ( umFluxo == outro ){
                outro.contador += umFluxo.contador;
                existe=1;
                break;
            }
            n++;
        }
        if ( not existe ) LFluxo.anexa(umFluxo);        
    }
    
    LFluxo.inicia();
    int n=1;
    while (not LFluxo.fim()){
        cout << "Fluxo " << n << " :" << endl;
        struct Fluxo mostraFluxo = LFluxo.proximo();
        in_addr src = mostraFluxo.src_ip;
        in_addr dest = mostraFluxo.dst_ip;
        cout << "IP Origem: " << IP::addr2string(src) << " ; " << "IP Destino: " << IP::addr2string(dest) << " ; " << "Protocolo: " << (int)mostraFluxo.proto << " ; " << "Porta Origem: " << mostraFluxo.src_port << " ; " << "Porta Destino: " << mostraFluxo.dst_port << endl;
        unsigned int bits = mostraFluxo.contador*8;
        cout << "Quantidade de bits do fluxo: " << bits<< endl;
        cout << endl;
        n++;
    }
}

//Contar quantos fuxos existem na rede durante a captura
void Analisador::contabilizaFluxo(){
    cap.muda_filtro("ip");
    Lista<Frame> * captura = cap.capture(0,10);
    Lista<Fluxo> LFluxo;
    
    captura->inicia();
    while ( not captura->fim() ){
        struct Fluxo umFluxo;
        Frame & pacote = captura->proximo();
        IP * ip = (IP*)pacote->get_payload();
        umFluxo.src_ip=ip->get_src();
        umFluxo.dst_ip=ip->get_dest();
        umFluxo.proto=ip->get_proto();
        if ( ip->get_proto() == IP::proto_TCP ){
            TCP * tcp = (TCP*) ip->get_payload();
            umFluxo.src_port=tcp->get_sport();
            umFluxo.dst_port=tcp->get_dport();
        } else if ( ip->get_proto() == IP::proto_UDP ){
            UDP * udp = (UDP*) ip->get_payload();
            umFluxo.src_port=udp->get_sport();
            umFluxo.dst_port=udp->get_dport();
        }
        umFluxo.contador=pacote->get_size();
        LFluxo.inicia();
        bool existe=0;
        while ( not LFluxo.fim() ){
            struct Fluxo outro = LFluxo.proximo();
            if ( umFluxo == outro) {
                existe=1;
                break;
            }
        }
        if ( not existe ) LFluxo.anexa(umFluxo);
    }

    
    cout << "Foram contabilizados " << LFluxo.comprimento() << " fluxos durante a captura." << endl;
    
}

//Identifica os fluxos que mais transferiram durante a captura
void Analisador::maioresFluxos(){
    cap.muda_filtro("ip");
    Lista<Frame> * captura = cap.capture(0,10);
    Lista<Fluxo> LFluxo;
    
    captura->inicia();
    while ( not captura->fim() ){
        struct Fluxo umFluxo;
        Frame & pacote = captura->proximo();
        IP * ip = (IP*)pacote->get_payload();
        umFluxo.src_ip=ip->get_src();
        umFluxo.dst_ip=ip->get_dest();
        umFluxo.proto=ip->get_proto();
        if ( ip->get_proto() == IP::proto_TCP ){
            TCP * tcp = (TCP*) ip->get_payload();
            umFluxo.src_port=tcp->get_sport();
            umFluxo.dst_port=tcp->get_dport();
        } else if ( ip->get_proto() == IP::proto_UDP ){
            UDP * udp = (UDP*) ip->get_payload();
            umFluxo.src_port=udp->get_sport();
            umFluxo.dst_port=udp->get_dport();
        }
        umFluxo.contador=pacote->get_size();
        LFluxo.inicia();
        bool existe=0;
        while ( not LFluxo.fim() ){
            int n=0;
            struct Fluxo & outro = LFluxo.proximo();
            if ( umFluxo == outro ){
                outro.contador += umFluxo.contador;
                existe=1;
                break;
            }
            n++;
        }
        if ( not existe ) LFluxo.anexa(umFluxo);        
    }
    
    LFluxo.ordenaSelecao();
    
    LFluxo.inicia();
    cout << "Os 3 fluxos que mais transferiram dados foram os seguintes: " << endl;
    
    int coletor = LFluxo.comprimento() - 1;
    
    for (int n=1;n<4;n++) {
        cout << "Fluxo " << n << " :" << endl;
        struct Fluxo mostraFluxo = LFluxo.obtem(coletor);
        in_addr src = mostraFluxo.src_ip;
        in_addr dest = mostraFluxo.dst_ip;
        cout << "IP Origem: " << IP::addr2string(src) << " ; " << "IP Destino: " << IP::addr2string(dest) << " ; " << "Protocolo: " << (int)mostraFluxo.proto << " ; " << "Porta Origem: " << mostraFluxo.src_port << " ; " << "Porta Destino: " << mostraFluxo.dst_port << endl;
        cout << "Quantidade de bytes do fluxo: " << mostraFluxo.contador << endl;
        cout << endl;
        coletor--;
    }
}

//Apresentar periodicamente a taxa de bits ou bytes capturados
void Analisador::capturaPeriodica() {
    while ( true ){
        cout << "A captura irá rodar por 5 segundos ..." << endl;
        cout << "Iniciando captura ..." << endl;
        Lista<Frame> * captura = cap.capture(0,5);
        unsigned int contador = 0;
        captura->inicia();
        while (not captura->fim() ){
            Frame & pacote = captura->proximo();
            contador += pacote->get_size();
        }
        string comando;
        unsigned int taxa = contador/5;
        cout << "A taxa de dados foi de " << taxa << "  Bps." << endl;
        cout << "Tecle ENTER para continuar ou SAIR para sair: ";
        getline(cin, comando);
        if ( comando == "sair" || comando == "SAIR" )
            break;
    }
}



