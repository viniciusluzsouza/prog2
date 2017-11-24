#ifndef FILA_H
#define	FILA_H
 
template <typename T> class Fila {
public:
    // cria uma fila com capacidade "N"
    Fila(unsigned int N);
 
    // cria uma fila que é cópia de outra fila
    Fila(const Fila<T>& orig);
 
    // destrutor da fila
    virtual ~Fila();
 
    // enfileira "algo" no fim da fila
    void enfileira(const T & algo);
 
    // desenfileira um dado do início da fila
    T desenfileira();

 
 
    // retorna uma referência ao dado que está no início 
    // da fila, sem removê-lo da fila
    T & frente() const;

    // retorna comprimento da fila (capacidade)
    int comprimento() { return capacidade; };

    // atribuicao de fila
    Fila& operator=( const Fila& outra ); 
 
    bool vazia() const {return itens == 0; }
    bool cheia() const { return itens == capacidade;}
    unsigned int tamanho() const { return itens;}
private:
    int capacidade;
    int itens, inicio, fim;
 
    // a área de armazenamento da fila
    T* buffer;
};
 
// Abaixo seguem os esqueletos das implementações dos métodos da Fila
 
template <typename T> Fila<T>::Fila(unsigned int N) {
    // inicia os atributos da fila: inicialmente
    // ela está vazia
    capacidade = N;
    inicio = 0;
    fim = 0;
    itens = 0;
 
    // aloca memória para guardar "capacidade" dados do tipo "T"
    buffer = new T[capacidade];
}
 
template <typename T> Fila<T>::Fila(const Fila& orig) {
}
 
template <typename T> Fila<T>::~Fila() {
}
 
template <typename T> void Fila<T>::enfileira(const T& algo) {
  
  if ( cheia() ) throw -1;

  buffer[fim] = algo;
  itens++;
  fim++;
  if ( fim == capacidade )
    fim = 0;
}
 
template <typename T> T Fila<T>::desenfileira() {
  
  if ( vazia() ) throw -1;

  T valor = buffer[inicio];
  inicio++;
  itens--;
  if ( inicio == capacidade )
    inicio = 0;
  return valor;

}
 
template <typename T> T & Fila<T>::frente() const {
  return buffer[inicio];
}

template <typename T> Fila<T>& Fila<T>::operator=( const Fila<T>& outra ){
 
  delete[] buffer;
  capacidade = outra.capacidade;
  itens = outra.itens;
  inicio = outra.inicio;
  fim = outra.fim;
  buffer = new T[capacidade];
  for (int n=0; n < capacidade; n++) buffer[n] = outra.buffer[n];
  return *this;
}

//template <typename T> Fila<T>::Fila(const Fila<T> & outra) {
//  buffer = NULL;
//  *this = outra;
//}

 
#endif	/* FILA_H */
