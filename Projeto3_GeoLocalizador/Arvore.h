#ifndef ARVORE_H
#define ARVORE_H

#include <string>
#include <fstream>
#include "Pilha.h"
#include "Lista.h"

using namespace std;

// Uma áore de pesquisa bináa: na verdade, um objeto da classe Arvore
// representa um nodo de uma áore ...

template <class T> class Arvore {
 protected:
  T valor; // valor guardado neste nodo da áore
  Arvore<T> * esq, * dir; // ramos esquerdo e direito
  // Operação que calcula o fator de balanceamento
  //Pilha<Arvore<T> *> iterador; // pilha para o iterador
  //T * atual;  

  
 public:
  Arvore(const T& data);
  Arvore(const Arvore<T> & outra);
  Arvore();
  ~Arvore() {};
  
  // rotação esquerda
  Arvore<T> * rotacionaL();
  
  int fatorB() const;
 
  // rotação direita
  Arvore<T> * rotacionaR();

  // adiciona um dado àrvore
  void adiciona(const T & dado);
  
  // Operação que balanceia a árvore
  Arvore<T> * balanceia();

  // "obtem" e "operator[]" sãidêicos:
  // obtéum dado que seja igual a "algo"
  T & obtem(const T & algo);
  T & operator[](const T & algo);

  // remove um valor da áore... deixemos isto por úo ;-)
  T remove(const T & algo);

  // algo mais simples: remove os ramos esquerdo e direito
  void esvazia();

  // altura da folha mais profunda
  unsigned int altura() const;

  // quantidade de nodos da áore
  unsigned int tamanho() const;

  void escrevaSe(ostream & out) const;

  // retorna a subáore esquerda ou direita
  Arvore<T> * esquerda() const {
      if (esq) return esq;
      else return NULL;
      }
  Arvore<T> * direita() const {
      if (dir) return dir;
      else return NULL;
    }

  // grava uma cópia dos valores na lista fornecida
  void valores(Lista<T> & lista) const;
  
  // copia na ordem: esquerda, raiz, direita
  void listeInOrder(Lista<T> & lista) const;
  
  // copia na ordem: raiz, esquerda, direita
  void listePreOrder(Lista<T> & lista) const;
  
  // copia na ordem: esquerda, direita, raiz
  void listePosOrder(Lista<T> & lista) const;

  // retorna uma referêia ao valor da raiz
  T & obtemValor() {return valor;}

  // verifica se "algo" existe na áore
  bool existe(const T & algo) const;
  
  //Escreva se pre-order
  void escrevaSePreOrder(ostream & out) const;
  
  //Iterador
  //Inicia iterador
  //void inicia();
  //Retorna proximo dado
  //T & proximo();
  //Rertorna se o iterador finalizou
  //bool fim();
  
};

template <typename T>Arvore<T>::Arvore(const T & data){
        esq = NULL;
        dir = NULL;
        valor = data;
}

/*Adiciona recursivo*/
/*template <typename T> void Arvore<T>::adiciona(const T & dado){
   if (dado < valor){
     if (esq == NULL) esq = new Arvore<T>(dado);
     else esq->adiciona(dado);
  }
   else if (dado > valor){
    if (dir == NULL) dir = new Arvore<T>(dado);
          else dir->adiciona(dado);
  }
   else valor = dado;
}*/

/*Adiciona não recursivo*/
template <typename T> void Arvore<T>::adiciona(const T& dado) {
    Arvore<T> * ptr = this;
    
    while (true){
        if (ptr->valor == dado) {
            ptr->valor = dado;
            break;
        }
        if ( ptr->valor > dado ){
            if (ptr->esq == NULL){
                ptr->esq = new Arvore<T>(dado);
                break;
            }
            ptr = ptr->esq;
        } else {
            if (ptr->dir == NULL) {
                ptr->dir = new Arvore<T>(dado);
                break;
            }
            ptr = ptr->dir;
        }
    }
}

/*obtem recursivo*/
/*template <typename T> T& Arvore<T>::obtem(const T& algo) const{
    if (valor == algo)
        return valor;
    
    if (algo < valor) {
        if (esq) return esq->obtem(algo);
        throw -1;
    }
    if (algo > valor) {
        if (dir) return dir->obtem(algo);
        throw -1;
    }
}*/

/*obtem não recursivo*/
template <typename T> T& Arvore<T>::obtem(const T& algo) {
    Arvore<T> * ptr = this;
    
    while (ptr){
        if ( ptr->valor == algo  ) return ptr->valor;
        if ( ptr->valor > algo ) ptr = ptr->esq;
        else ptr = ptr->dir;
    }
    throw -1; //exceção
}

template <typename T> void Arvore<T>::escrevaSe(ostream& out) const {
    
    static int nivel = -1;
    string prefixo;
 
    nivel++;
    prefixo.append(nivel, ' ');
 
    if (dir) dir->escrevaSe(out);
    out << prefixo << valor << endl;
    if (esq) esq->escrevaSe(out);
    nivel--;
    
    /*
    if (esq) esq->escrevaSe(out);
    out << valor << endl;
    if (dir) dir->escrevaSe(out);
    */
}

template <typename T> void Arvore<T>::escrevaSePreOrder(ostream& out) const {
    
    out << valor << endl;
    if (esq) esq->escrevaSePreOrder(out);    
    if (dir) dir->escrevaSePreOrder(out);
    
}

template <typename T> unsigned int Arvore<T>::altura() const {
    unsigned int cont_left=0,cont_right=0;
    
    if (esq==NULL and dir==NULL) return 0;
    if (esq) cont_left = 1 + esq->altura();
    if (dir) cont_right = 1 + dir->altura();
    if (cont_left > cont_right) return cont_left;
    else return cont_right;
    
}

template <typename T> int Arvore<T>::fatorB() const {
    int cont1=0, cont2=0;
    if (esq) cont1 = 1 + esq->altura();
    if (dir) cont2 = 1 + dir->altura();
    return cont1 - cont2;
}

template <typename T> Arvore<T> * Arvore<T>::rotacionaR(){
    Arvore<T> *p1=esq, *p2=this, *p3=p1->dir;
    p1->dir=p2;
    p2->esq=p3;
    //if (not p3) {
    //    p1->dir=p2;
    //    p2->dir = p2->esq = NULL;
    //}
    //else {
    //    p2->esq=p3;
    //    p2->dir=NULL;
    //    p1->dir=p2;
    //}
    return p1;  
}

template <typename T> Arvore<T> * Arvore<T>::rotacionaL(){
    Arvore<T> *p1=dir, *p2=this, *p3=p1->esq;
    p1->esq=p2;
    p2->dir=p3;
    //if (not p3) {
    //   p1->esq=p2;
    //    p2->dir = p2->esq = NULL;
    //}
    //else {
    //    p2->dir=p3;
    //    p2->esq=NULL;
    //    p1->esq=p2;
    //} 
    return p1;
}

template <typename T> Arvore<T> * Arvore<T>::balanceia(){
    Arvore<T> * p=this;
    int fb=0;
    
    if ( not (esq or dir)) return this;
    if (esq) esq=esq->balanceia();
    if (dir) dir=dir->balanceia();
    
    fb = p->fatorB();
    
    while ( fb < -1 ){
        if ( p->dir->fatorB() > 0  ) p->dir = p->dir->rotacionaR();
        p = p->rotacionaL();
        fb = p->fatorB();
    }
    
    while ( fb > 1 ){
        if ( p->esq->fatorB() < 0 ) p->esq = p->esq->rotacionaL();
        p = p->rotacionaR();
        fb = p->fatorB();
    }
    
    return p;
    
}

/*
template <typename T> void Arvore<T>::inicia() {
    Arvore<T> * p = this;
    iterador.push(p);
    atual = p->
    
    while (p->esq){
        atual = atual->esq;
        iterador.push(atual);
    }
    
}

template <typename T> void Arvore<T>::proximo() {
    atual = iterador.pop();
    
    if ( atual
}
*/

template <typename T> void Arvore<T>::listeInOrder(Lista<T>& lista) const {

    if (esq) esq->listeInOrder(lista);
    lista.anexa(valor);
    if (dir) dir->listeInOrder(lista);

   /*
    if (esq) esq->escrevaSe(out);
    out << valor << endl;
    if (dir) dir->escrevaSe(out);
    */
    
}

template <typename T> void Arvore<T>::listePreOrder(Lista<T>& lista) const {
    
    lista.anexa(valor);
    if (esq) esq->listePreOrder(lista);
    if (dir) dir->listePreOrder(lista);
            
    /*        
    out << valor << endl;
    if (esq) esq->escrevaSePreOrder(out);    
    if (dir) dir->escrevaSePreOrder(out);
    */
}

#endif