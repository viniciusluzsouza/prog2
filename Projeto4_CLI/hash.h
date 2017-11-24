#ifndef HASH_H
#define HASH_H
 
#include <cstdlib>
#include <string>
#include "Lista.h"
 
using namespace std;
 
template <typename T> class TabelaHash {
 public:
  // cria uma tabela hash com num_linhas linhas
  TabelaHash(int num_linhas);
 
  TabelaHash(const TabelaHash &outra); // construtor de copia
 
  // destrutor
  ~TabelaHash();
 
  // adiciona um dado à tabela. Se já existir na tabela um par contendo a chave
  // "chave", o dado desse par deve ser substituído por "algo". Caso contrário,
  // um novo par deve ser adicionado à tabela.
  void adiciona(const string& chave, const T& algo);
 
  // remove a chave "chave" e o dado a ela associado.
  T remove(const string& chave);
 
  // retorna o dado associado a "chave"
  T & operator[](const string& chave) const;
  T & obtem(const string& chave) const;
 
  // retorna uma lista com as chaves existentes na tabela
  Lista<string> * chaves() const;
 
  // retorna uma lista com os dados existentes na tabela
  Lista<T> * valores() const;
 
  // retorna verdadeiro se "chave" existe na tabela
  bool existe(const string& chave) const;
 
  // esvazia a tabela
  void esvazia();
 
  // retorna a quantidade de dados (ou chaves) existentes na tabela
  int tamanho() const;
 
 private:
  // Atributos da tabela
  // esta struct Par serve para associar uma chave e um dado
  // note que a tabela hash armazena pares (chave, dado)
  struct Par {
    string chave;
    T valor;
 
    Par(const string& k, const T& dado) {
      chave = k;
      valor = dado;
    }
    Par() {}
    bool operator==(const Par & outro) {
      return chave == outro.chave;
    }

  };

  Lista<Par> * tab; // vetor de listas ... a tabela propriamente dita !
  int linhas; // a quantidade de linhas da tabela

  // calcula um valor hash para "chave"
  unsigned int hash(const string & chave) const;
};


// Obs: melhores resultados se N for primo !
template <typename T> unsigned int TabelaHash<T>::hash(const string & chave) const {
  int n;
  unsigned int soma = 0;
 
  for (n=0; n < chave.size(); n++) {
    soma = ((soma << 8) + chave[n]) % linhas; 
  }
  return soma % linhas;  
}

template <typename T> TabelaHash<T>::TabelaHash(int num_linhas) {
  linhas = num_linhas;
  tab = new Lista<Par> [linhas];  
}

template <typename T> TabelaHash<T>::~TabelaHash() {
  delete[] tab;
}

template <typename T> void TabelaHash<T>::adiciona(const string& chave, const T& algo) {
  unsigned int linha = hash(chave);
  Par umPar(chave, algo);
  
  Lista<Par> * resultado = tab[linha].procura(umPar);
  if (resultado->comprimento() > 0){
    Par & outroPar = resultado->obtem(0);
    outroPar.valor = algo;
  } else {
    tab[linha].adiciona(umPar);
  }

  delete resultado;
  /*
  try {
    Par & outroPar = tab[linha].procura(umPar);
    outroPar.valor = algo;
  } catch (int e) {
    tab[linha].adiciona(umPar);
  }
  */
} 

template <typename T> T& TabelaHash<T>::operator[](const string& chave) const {
  return obtem(chave);
}

template <typename T> T& TabelaHash<T>::obtem(const string& chave) const {
  unsigned int linha = hash(chave);
  Par umPar;
  umPar.chave = chave;
  
  Lista<Par> * resultado = tab[linha].procura(umPar);
  if (resultado->comprimento() > 0) {
    Par & outroPar = resultado->obtem(0);
    return outroPar.valor;
  } else {
    throw -1;
  }
  /*
  Par & outroPar = tab[linha].procura(umPar);
  return outroPar.valor;
  */
}
 
template <typename T> bool TabelaHash<T>::existe(const string& chave) const {
    unsigned int linha = hash(chave);

    Par umPar;
    umPar.chave = chave;
    Lista<Par> * resultado = tab[linha].procura(umPar);
    if (resultado -> comprimento() > 0 ) return true;
    else return false; 
}

template <typename T> T TabelaHash<T>::remove(const string& chave) {
   unsigned int linha =hash(chave);
   int n;

   Par umPar;
   umPar.chave = chave;
   tab[linha].inicia();
   for (n=0; not tab[linha].fim(); n++){
    if (umPar == tab[linha].proximo() ) break;
    }
   
    umPar = tab[linha].remove(n);
    return umPar.valor;
}

template <typename T> Lista<string> * TabelaHash<T>::chaves() const {
   Lista<string> * umaChaves = new Lista<string>;
  
    for (int n=0;n<linhas;n++){
        tab[n].inicia();
        while (not tab[n].fim()) {
        Par umPar = tab[n].proximo();
        umaChaves->adiciona(umPar.chave);   
        }
    }
    return umaChaves; 
}

template <typename T> Lista<T> * TabelaHash<T>::valores() const {
   Lista<T> * unsValores = new Lista<T>;
  
   for (int n=0;n<linhas;n++){
   tab[n].inicia();
   while (not tab[n].fim()) {
     Par umPar = tab[n].proximo();
     unsValores->adiciona(umPar.valor);   
     }
   }
   return unsValores;
}

template <typename T> int TabelaHash<T>::tamanho() const {
    
   Lista<T> * umaValores = new Lista<T>;
  
   for (int n=0;n<linhas;n++){
   tab[n].inicia();
   while (not tab[n].fim()) {
     Par umPar = tab[n].proximo();
     umaValores->adiciona(umPar.valor);   
     }
   }
   return umaValores->comprimento();
}

#endif
