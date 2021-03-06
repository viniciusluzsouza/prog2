#ifndef PILHA_H
#define PILHA_H
template <typename T> class Pilha {
 public:
  // construtor: deve-se informar a capacidade da pilha
  Pilha(int umaCapacidade);
 
  // construtor de cópia: cria uma pilha que é cópia de outra
  Pilha(const Pilha& outra);
 
  // destrutor da pilha
  ~Pilha();
 
  // operador de atribuição: torna esta pilha uma cópia de outra pilha
  Pilha<T> & operator=(const Pilha<T> & outra);
 
  // operações da pilha
 
  void push(const T & dado); // empilha um dado
 
  T pop(); // desempilha um dado
 
  T& top() const; // retorna o dado do topo da pilha, sem retirá-lo
 
  bool vazia() const { return topo == 0;}
  bool cheia() const {return topo == capacidade;}
  void esvazia();
  int tamanho() { return topo; }

 protected:
  // atributos da pilha
  T * buffer; // a área de memória para armazenamento da pilha
  int topo, capacidade;
  // outros atributos ???
 
};
 
template <typename T> Pilha<T>::Pilha(int umaCapacidade) {
  capacidade = umaCapacidade;
  topo = 0;
  buffer = new T[capacidade];
}
 
template <typename T> Pilha<T>::Pilha(const Pilha& outra) {
}

template <typename T> Pilha<T>& Pilha<T>::operator=( const Pilha<T>& outra ){

  delete[] buffer;
  capacidade = outra.capacidade;
  topo = outra.topo;
  buffer = new T[capacidade];
  for (int i=0;i<topo;i++)
    buffer[i] = outra.buffer[i];

}
 
template <typename T> Pilha<T>::~Pilha() {
}
 
template <typename T> void Pilha<T>::esvazia() {
}
 
template <typename T> void Pilha<T>::push(const T & dado) {

  if ( cheia() ) {
  Pilha<T> aux(1);
  aux = *this;
  while ( not aux.vazia() ) aux.pop();
  while ( topo > 1 ) aux.push(pop());
  pop();
  while ( not aux.vazia() ) push(aux.pop());
  }

  buffer[topo] = dado;
  topo++;
}
 
template <typename T> T Pilha<T>::pop() {
  if ( vazia() ) throw -1;
  T valor = buffer[topo-1];
  topo--;
  return valor;
}
 
template <typename T> T& Pilha<T>::top() const{
  return buffer[topo-1];
}



#endif