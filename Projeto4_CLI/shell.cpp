#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <list>

using namespace std;

Shell::Shell() {
  pid = 0;
  status = 0;
}

Shell::~Shell() {
  if (pid) {
    kill(pid, SIGTERM);
    sleep(1);
    kill(pid, SIGKILL);
  }
}

int Shell::execute(const string & comando) {
  return execute(comando, true);
}

int Shell::execute(const string & comando, bool fg) {
  if (pid) return -1; // erro: jah ha um comando em execucao !

  status = -1;
  pid = fork();
  if (not pid) {
    char ** argv = cmd2argv(comando);
    if (not argv) _exit(EINVAL);
    execvp(argv[0], argv);
    _exit(errno);
  }

  if (fg) {
    waitpid(pid, &status, 0);
    status = WEXITSTATUS(status);
    pid = 0;
  }

  return status;
}

int Shell::obtem_status() {
  return obtem_status(false);
}

int Shell::obtem_status(bool wait) {
  int options = 0;

  if (not wait) options = WNOHANG;
  if (pid) {
    waitpid(pid, &status, options);
    if (WIFEXITED(status) or WIFSIGNALED(status)) {
      status = WEXITSTATUS(status);
      pid = 0;
    } else status = -1;
  }
  return status;
}

size_t mfind(const string & s, const string & cars, int pos) {
  ssize_t r = string::npos;

  for (int n=0; n < cars.size() and r == string::npos; n++) {
    r = s.find(cars[n], pos);
  }
  return r;
}

list<string> tokenizer(const string& s) {
  int pos = 0, n;
  list<string> res;

  while ((n = mfind(s, " \t\n", pos)) != string::npos) {
    res.push_back(s.substr(pos, n-pos));
    pos = n+1;
  }
  if (pos < s.size()) {
    res.push_back(s.substr(pos, s.size()-pos));
  }
  return res;
}

char ** Shell::cmd2argv(const string & comando) {
  list<string> args = tokenizer(comando); 

  if (not args.size()) return NULL;

  char ** argv = new char*[args.size()+1];
  char ** ptr = argv;
  for (list<string>::iterator it=args.begin(); it != args.end(); it++, ptr++) {
    *ptr = strdup(it->c_str());
  }
  *ptr = NULL;
  return argv;
}

