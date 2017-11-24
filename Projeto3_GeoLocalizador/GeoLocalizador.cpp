/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeoLocalizador.cpp
 * Author: vinicius
 * 
 * Created on 10 de Julho de 2016, 16:52
 */

#include <string>
#include "Arvore.h"
#include "GeoIP.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Lista.h"
#include "GeoLocalizador.h"

using namespace std;

GeoLocalizador::GeoLocalizador() {
    
    string line;
    tamanho_locais = 0;
    
    ifstream blocks_prontos("blocks_prontos.csv");
    if ( blocks_prontos.is_open() ){
        cout << "Lendo dados de blocos de endereços ... " << endl;
        while (true){
            try {
                getline(blocks_prontos, line);
                GeoIP block(line);
                blocos = new Arvore<GeoIP>(block);
                tamanho_blocos++;
                break;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        while (true){
            try {
                getline(blocks_prontos, line);
                if ( blocks_prontos.eof() ) break;
                GeoIP block2(line);
                blocos->adiciona(block2);
                tamanho_blocos++;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        blocks_prontos.close();
    } else {
        //cout << "Preparando dados de blocos para leitura." << endl;
        //system("sort -R GeoLiteCity-Blocks.csv > Blocks_Aleatorios.csv");
        cout << "Lendo dados de blocos de endereços ... " << endl;
        ifstream blocks("Blocks_Aleatorios.csv");
        while (true){
            try {
                getline(blocks, line);
                GeoIP block(line);
                blocos = new Arvore<GeoIP>(block);
                tamanho_blocos++;
                break;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        while (true){
            try {
                getline(blocks, line);
                if ( blocks.eof() ) break;
                GeoIP block2(line);
                blocos->adiciona(block2);
                tamanho_blocos++;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        blocks.close();
        //system("rm -rf Blocks_Aleatorios.csv");
        cout << "Balanceando ... " << endl;
        blocos = blocos->balanceia();
        blocos = blocos->balanceia();
    }
    cout << "Altura blocos: " << blocos->altura() << endl;
    
    cout << "Lendo base de endereços ... " << endl;
    
    ifstream locs_prontos("location_prontos.csv");
    
    if ( locs_prontos.is_open() ){
        cout << "Lendo dados de localidades ... " << endl;
        while (true){
            try {
                getline(locs_prontos, line);
                GeoIPLocation local(line);
                locais = new Arvore<GeoIPLocation>(local);
                tamanho_locais++;
                break;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        while (true) {
            try {
                getline(locs_prontos, line);
                if ( locs_prontos.eof() ) break;
                GeoIPLocation local2(line);
                locais->adiciona(local2);
                tamanho_locais++;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
    } else {
        //cout << "Preparando dados de blocos para leitura." << endl;
        //system("sort -R GeoLiteCity-Location.csv > Location_Aleatorios.csv");
        //cout << "Lendo dados de localidades ... " << endl;
        ifstream locs("Location_Aleatorios.csv");
        while (true){
            try {
                getline(locs, line);
                GeoIPLocation local(line);
                locais = new Arvore<GeoIPLocation>(local);
                tamanho_locais++;
                break;
            } catch (int e){
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        while (true){
            try {
                getline(locs, line);
                if ( locs.eof() ) break;
                GeoIPLocation local2(line);
                locais->adiciona(local2);
                tamanho_locais++;
            } catch (int e) {
                cerr << "ignorando - " << e << ": " << line << endl;
            }
        }
        locs.close();
        //system("rm -rf Location_Aleatorios.csv");
        cout << "Balanceando ... " << endl;
        locais = locais->balanceia();
        locais = locais->balanceia();
    }
    cout << "Altura locais: " << locais->altura() << endl;
    
    cout << "Leitura concluida" << endl;
    
}

GeoLocalizador::GeoLocalizador(const GeoLocalizador& orig) {
}

GeoLocalizador::~GeoLocalizador() {
}


void GeoLocalizador::Busca(string& ip) {
    try {
        GeoIP gip(ip,ip,0);
        gip = blocos->obtem(gip);
        cout << "Bloco: " << gip.get_addr1_str() << " a " << gip.get_addr2_str() << endl;
        GeoIPLocation local(gip.get_location());
        local = locais->obtem(local);
        local.escrevaSe(cout);
    } catch (int e){
        cerr << "IP nao encontrado" << endl;
    }
}


void GeoLocalizador::Finaliza(){
    cout << "Salvando arquivos de blocos ... " << endl;
    ofstream arq("blocks_prontos.csv");
    blocos->escrevaSePreOrder(arq);
    arq.close();
    cout << "Salvando arquivos de localidades ... " << endl;
    ofstream arq2("location_prontos.csv");
    locais->escrevaSePreOrder(arq2);
    arq2.close();
}

void GeoLocalizador::BuscaPais(string & pais){
    
    Lista<GeoIP> lgip, lgip_final;
    Lista<GeoIPLocation> lloc;
    
    int n;
    Arvore<unsigned int> * arv;
    
    locais->listePreOrder(lloc);
    
    
    lloc.inicia();
    while (true){
        GeoIPLocation local = lloc.proximo();
        if ( local.obtem_pais() == pais ) {
            unsigned int id = local.obtem_id();
            arv = new Arvore<unsigned int>(id);
            break;
        }
    }
    
    while (not lloc.fim()){
        GeoIPLocation local = lloc.proximo();
        if ( local.obtem_pais() == pais ){
            unsigned int id = local.obtem_id();
            arv->adiciona(id);
        }
    }
    
    if ( not arv ) {
        cout << "Não encontrado." << endl;
        return;
    }

    arv = arv->balanceia();
    blocos->listeInOrder(lgip);
    lgip.inicia();
    while (not lgip.fim()){
        GeoIP gip = lgip.proximo();
        unsigned int id = gip.get_location();
        try {
            if ( arv->obtem(id) ) {
                lgip_final.anexa(gip);
            }
        } catch (int e) {
        }
    } 
    
    lgip_final.inicia();
    while (not lgip_final.fim()) {
        GeoIP gip = lgip_final.proximo();
        cout << "Bloco: " << gip.get_addr1_str() << " a " << gip.get_addr2_str() << endl;
    }
    
    
    delete arv;
    
}

void GeoLocalizador::BuscaEstado(string& pais, string& estado){
    
    Lista<GeoIP> lgip, lgip_final;
    Lista<GeoIPLocation> lloc;
    
    int n;
    Arvore<unsigned int> * arv;
    
    locais->listePreOrder(lloc);
    
    
    lloc.inicia();
    while (true){
        GeoIPLocation local = lloc.proximo();
        string ok = local.obtem_regiao();
        if ( ok.size() > 0 ){
            if ( local.obtem_pais() == pais and local.obtem_regiao() == estado) {
                unsigned int id = local.obtem_id();
                arv = new Arvore<unsigned int>(id);
                break;
            }
        }
    }
    
    while (not lloc.fim()){
        GeoIPLocation local = lloc.proximo();
        string ok = local.obtem_regiao();
        if ( ok.size() > 0 ){
            if ( local.obtem_pais() == pais and local.obtem_regiao() == estado){
                unsigned int id = local.obtem_id();
                arv->adiciona(id);
            }
        }
    }
    
    if ( not arv ) {
        cout << "Não encontrado." << endl;
        return;
    }

    arv = arv->balanceia();
    blocos->listeInOrder(lgip);
    lgip.inicia();
    while (not lgip.fim()){
        GeoIP gip = lgip.proximo();
        unsigned int id = gip.get_location();
        try {
            if ( arv->obtem(id) ) {
                lgip_final.anexa(gip);
            }
        } catch (int e) {
        }
    } 
    
    lgip_final.inicia();
    while (not lgip_final.fim()) {
        GeoIP gip = lgip_final.proximo();
        cout << "Bloco: " << gip.get_addr1_str() << " a " << gip.get_addr2_str() << endl;
    }
            
    delete arv;
    
}

void GeoLocalizador::BuscaCidade(string& pais, string& estado, string& cidade){

    Lista<GeoIP> lgip, lgip_final;
    Lista<GeoIPLocation> lloc;
    
    int n;
    Arvore<unsigned int> * arv;
    
    locais->listePreOrder(lloc);
    
    
    lloc.inicia();
    while (true){
        GeoIPLocation local = lloc.proximo();
        string ok = local.obtem_cidade();
        if ( ok.size() > 0 ){
            if ( local.obtem_pais() == pais and local.obtem_cidade() == cidade) {
                unsigned int id = local.obtem_id();
                arv = new Arvore<unsigned int>(id);
                break;
            }
        }
    }

    while (not lloc.fim()){
        GeoIPLocation local = lloc.proximo();
        string ok = local.obtem_cidade();
        if ( ok.size() ) {
            if ( local.obtem_pais() == pais and local.obtem_cidade() == cidade){
                unsigned int id = local.obtem_id();
                arv->adiciona(id);
            }
        }
    }
    
    if ( not arv ) {
        cout << "Não encontrado." << endl;
        return;
    }

    arv = arv->balanceia();
    blocos->listeInOrder(lgip);
    lgip.inicia();
    while (not lgip.fim()){
        GeoIP gip = lgip.proximo();
        unsigned int id = gip.get_location();
        try {
            if ( arv->obtem(id) ) {
                lgip_final.anexa(gip);
            }
        } catch (int e) {
        }
    } 

    lgip_final.inicia();
    while (not lgip_final.fim()) {
        GeoIP gip = lgip_final.proximo();
        cout << "Bloco: " << gip.get_addr1_str() << " a " << gip.get_addr2_str() << endl;
    }
    
    delete arv; 
    
}