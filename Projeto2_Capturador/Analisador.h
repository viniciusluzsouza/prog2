/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Analisador.h
 * Author: vinicius
 *
 * Created on 26 de Maio de 2016, 15:52
 */

#ifndef ANALISADOR_H
#define ANALISADOR_H

#include "Lista.h"
#include <string>
#include "Capturador.h"

using namespace std;

/*Requisita ao Capturador que capture pacotes, e implementa as regras de análise dos pacotes capturados */
class Analisador {
    
public:
    //Construtor
    Analisador(const char * inet);
    
    //Destrutor
    ~Analisador();
    
    // Primeiro requisito - apresentar de forma crescente e decrescente:
    void capturaCrescente();
    void capturaDecrescente();
    
    // Segundo requisito - contar pacotes transferidos classificando-os de 100 em 100 bytes
    void capturaContarTransferidos();
    
    // Terceiro requisito - apresentar taxa de bits de cada fluxo
    void capturaTaxaFluxo();
    
    // Quarto requisito - Contar quantos fuxos existem na rede durante a captura
    void contabilizaFluxo();
    
    // Quinto requisito - Identificar os fluxos que mais transferiram durante a captura
    void maioresFluxos();
    
    // Sexto requisito - Apresentar periodicamente a taxa de bits ou bytes capturados
    void capturaPeriodica();
    
private:
    Capturador cap;
    
};

#endif /* ANALISADOR_H */