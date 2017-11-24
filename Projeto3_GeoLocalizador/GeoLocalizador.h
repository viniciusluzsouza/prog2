/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeoLocalizador.h
 * Author: vinicius
 *
 * Created on 10 de Julho de 2016, 16:52
 */

#ifndef GEOLOCALIZADOR_H
#define GEOLOCALIZADOR_H
#include <string>
#include "Arvore.h"
#include "GeoIP.h"

using namespace std;

class GeoLocalizador {
public:
    GeoLocalizador();
    GeoLocalizador(const GeoLocalizador& orig);
    virtual ~GeoLocalizador();
    
    //busca por ip
    void Busca(string& ip);
    
    //busca por pais
    void BuscaPais(string & pais);
    
    //busca por estado
    void BuscaEstado(string & pais, string & estado);
    
    //busca por cidade
    void BuscaCidade(string & pais, string& estado, string& cidade);
    
    //finaliza o programa
    void Finaliza();
    
private:
    Arvore<GeoIP> * blocos;
    Arvore<GeoIPLocation> * locais; 
    unsigned int tamanho_locais, tamanho_blocos;
};

#endif /* GEOLOCALIZADOR_H */