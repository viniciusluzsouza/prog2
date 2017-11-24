/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Lista.h
 * Author: vinicius
 *
 * Created on 6 de Maio de 2016, 10:09
 */

#ifndef LISTA_H
#define LISTA_H

#include <iostream>

using namespace std;

// uma classe template para nodos da lista ...

template <class T> struct Nodo {
    T dado;
    Nodo<T> * proximo;

    // um construtor para criar um Nodo facilmente ...
    Nodo(const T & umDado);
};

// a classe template para a lista ...

template <class T> class Lista {
private:
    // ponteiros para o primeiro e último nodos da lista
    Nodo<T> * inicio, * ultimo;

    // a quantidade de nodos existente na lista
    int comp;
    
    Nodo<T> * atual;
public:
    //construtor: não precisa de parâmetros para criar uma nova lista
    Lista();

    // construtor de cópia
    Lista(const Lista &outra);

    // destrutor
    ~Lista();

    // adiciona "algo" no inicio da lista
    void adiciona(const T & algo);

    // adiciona "algo" no final da lista
    void anexa(const T & algo);

    // adiciona "algo" em uma posição específica da lista  
    void insere(const T & algo, int posicao);

    // remove o dado que está na "posicao" na lista, e retorna 
    // uma cópia desse dado
    T remove(int posicao);

    // remove o dado que for equivalente a "algo", e retorna 
    // uma cópia desse dado
    T retira(const T & algo);

    // estas duas operações são idênticas: retorna
    // uma referência ao dado que está na "posicao" na lista
    T& obtem(int posicao) const;
    T& operator[](int pos) const;

    // estes operadores podem ser implementados depois
    Lista& operator=(const Lista<T> & outra);
    bool operator==(const Lista<T> & outra) const;


    // Retorna uma referência a um dado que seja equivalente a "algo"
    Lista<T> * procura(const T &algo) const;

    // Procura todos os dados equivalentes a "algo", e os
    // anexa a "lista". Retorna uma referência a "lista".
    Lista<T> & procuraMuitos(const T &algo, Lista<T> & lista) const;
    
    // Procura por um dado e retorna quantas vezes ele aparece na lista
    int procuraContabiliza(const T &algo) const;

    // retorna a quantidade de dados armazenados na lista
    int comprimento() const;

    // retorna true se lista estiver vazia
    bool vazia() const;

    // Esvazia a lista
    void esvazia();

    // apresenta o conteúdo da lista no stream "out"
    void escrevaSe(ostream & out) const;
    void escrevaSe(ostream & out, char delim) const;

    // ordena a lista
    void ordena();
    
    // ordena bolha
    void ordenaBolha();
    void ordenaBolhaDecrescente();
    void ordenaSelecao();
    void insereOrdenado(const T & algo);
    
    //iterador
    void inicia();
    T& proximo();
    bool fim() const;

};

template <typename T> Nodo<T>::Nodo(const T & umDado) {
    proximo = NULL;
    dado = umDado;
}

template <typename T> Lista<T>::Lista() {
    inicio = NULL;
    ultimo = NULL;
    comp = 0;
}

template <typename T> Lista<T>::Lista(const Lista<T> & outra) {
}

template <typename T> Lista<T>::~Lista() {
}

template <typename T> void Lista<T>::adiciona(const T& algo) {
    Nodo<T> * nodo = new Nodo<T>(algo);
    nodo->proximo = inicio;
    if (inicio == NULL) ultimo = nodo;
    inicio = nodo;
    comp++;
}

template <typename T> void Lista<T>::anexa(const T& algo) {
    Nodo<T> * nodo = new Nodo<T>(algo);


    if (comp == 0) {
        inicio = nodo;
    } else {
        ultimo->proximo = nodo;
    }
    ultimo = nodo;
    comp++;
}

template <typename T> int Lista<T>::comprimento() const {
    return comp;
}

template <typename T> void Lista<T>::esvazia() {
    Nodo<T> * p = inicio;
    Nodo<T> * prox = p->proximo;
    while (prox != NULL){
        delete p;
        p = prox;
        prox = p->proximo;
    }
    inicio = ultimo = NULL;
    comp = 0;    
}

template <typename T> void Lista<T>::insere(const T& algo, int posicao) {

    if (posicao >= comp) {
        anexa(algo);
        comp++;
        return;
    }

    if (posicao == 0) {
        adiciona(algo);
        comp++;
        return;
    }

    Nodo<T> * nodo = new Nodo<T>(algo);
    Nodo<T> *p = inicio;
    for (int n = posicao - 1; n > 0; n--, p = p->proximo);
    nodo->proximo = p->proximo;
    p->proximo = nodo;
    
    comp++;

}

template <typename T> T& Lista<T>::obtem(int posicao) const {

    if (posicao >= comp) throw -1;

    Nodo<T> *p = inicio;
    //for (; posicao > 0; posicao--, p = p->proximo);
    for (int n = 0; n < posicao; n++)
        p = p->proximo;
    return p->dado;
}

template <typename T> T& Lista<T>::operator[](int pos) const {

}

template <typename T> void Lista<T>::escrevaSe(ostream& out) const {
    for (Nodo<T> *p = inicio; p != NULL; p = p->proximo) out << p->dado << endl;
}

template <typename T> void Lista<T>::escrevaSe(ostream& out, char delim) const {
    for (Nodo<T> *p = inicio; p != NULL; p = p->proximo) out << p->dado << delim;
}

template <typename T> T Lista<T>::remove(int posicao) {

    if (comp == 0) throw -1;
    if (posicao >= comp) throw -2;

    Nodo<T> *p = inicio;
    T dado;

    if (posicao == 0) {
        dado = p->dado;
        inicio = p->proximo;
        delete p;
        comp--;
        return dado;
    }

    Nodo<T> *p_anterior = inicio;
    for (int n = posicao - 1; n > 0; n--, p_anterior = p_anterior->proximo);
    for (; posicao > 0; posicao--, p = p->proximo);
    p_anterior->proximo = p->proximo;
    dado = p->dado;
    delete p;
    comp--;
    return dado;

}

template <typename T> T Lista<T>::retira(const T& algo) {

}

template <typename T> bool Lista<T>::vazia() const {
    return comp == 0;
}

template <typename T> void Lista<T>::ordena() {

}

template <typename T> void Lista<T>::ordenaBolha() {
    
    int k;
    Nodo<T> *p;
    Nodo<T> *next;
    
    for (k=comp;k>0;k--){
        p=inicio;
        for (int j=k-1;j>0;j--){
            next = p->proximo;
            if (p->dado > next->dado){
                T dado = p->dado;
                p->dado = next->dado;
                next->dado = dado;
            }
            p=p->proximo;
        }
    }
    
}

/*Nao utilizar este metodo*/
template <typename T> void Lista<T>::ordenaBolhaDecrescente() {
    
    int k;
    Nodo<T> *p;
    Nodo<T> *next;
    
    for (k=comp;k>0;k--){
        p=inicio;
        for (int j=k-1;j>0;j--){
            next = p->proximo;
            if (p->dado < next->dado){
                T dado = p->dado;
                p->dado = next->dado;
                next->dado = dado;
            }
            p=p->proximo;
        }
    }
    
}

template <typename T> void Lista<T>::ordenaSelecao(){
    for (Nodo<T> *j = inicio; j->proximo != NULL; j = j->proximo){
        Nodo<T> *menor = j;
        for (Nodo<T> *k = j->proximo; k != NULL; k = k->proximo){
            if (k->dado < menor->dado) menor = k;
        }
        T aux = menor->dado;
        menor->dado = j->dado;
        j->dado = aux;
    }
}

template <typename T> void Lista<T>::insereOrdenado(const T & algo){
    
    Nodo<T> * nodo = new Nodo<T>(algo);
    
    if ( algo < inicio->dado ){
        adiciona(algo);
        return;
    } else {
        Nodo<T> * p;
        Nodo<T> * p_anterior = inicio;
        for (p=inicio;p->proximo!=NULL;p_anterior = p_anterior->proximo){
            p=p->proximo;
            if ( p->dado > algo ) {
                nodo->proximo = p;
                p_anterior->proximo = nodo;
                return;
            }
        }
        anexa(algo);
    }
    
}

template <typename T> Lista<T> * Lista<T>::procura(const T& algo) const {
    Lista<T> *aux = new Lista<T>;
    for (Nodo<T> *p = inicio;p!=NULL;p=p->proximo){
        if (p->dado == algo){
            aux->anexa(p->dado);
        }
    }
    return aux;
}

template <typename T> Lista<T>& Lista<T>::procuraMuitos(const T& algo, Lista<T>& lista) const {

}

template <typename T> Lista<T>& Lista<T>::operator=(const Lista<T>& outra) {
}

template <typename T> bool Lista<T>::operator==(const Lista<T>& outra) const {
}

template <typename T> void Lista<T>::inicia() {
    atual = inicio;
}

template <typename T> T & Lista<T>::proximo() {
    
    if (atual == NULL) throw -1;
    
    T & dado = atual->dado;
    atual = atual->proximo;
    return dado;
}

template <typename T> bool Lista<T>::fim() const {
    return atual == NULL;
}

#endif /* LISTA_H */