#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <unistd.h>

using namespace std;

// A classe Shell implementa um tipo de executor de comandos.
// Objetos dessa classe são capazes de executar uma linha de comando.

class Shell {
 private:
  pid_t pid; // PID do processo atualmente em execução
  int status; // status de terminação do último processo executado

 public:
  Shell(); // construtor: não precisa de parâmetros
  ~Shell(); // destrutor: termina o processo eventualmente em execução

  // executa um comando em primeiro plano
  // Retorna o status de terminação do comando
  int execute(const string & comando);

  // executa um comando em primeiro plano (se fg==true) ou segundo plano (fg==false)
  // Retorna o status de terminação do comando (se fg==true), ou -1 caso contrário.
  int execute(const string & comando, bool fg);

  // obtem o status de terminação do último comando executado
  // se comando ainda em execução, retorna imediatamente com valor -1
  int obtem_status();

  // obtem o status de terminação do último comando executado
  // se comando ainda em execução, retorna imediatamente -1 se wait==false
  // caso contrário, espera até o comando terminar e então retorna seu status
  int obtem_status(bool wait);

  // retorna o pid do comando em execução, ou 0 se não houver
  // comando em execução.
  pid_t obtem_pid() { return pid;}

 private:
  char ** cmd2argv(const string & comando);
};

#endif
