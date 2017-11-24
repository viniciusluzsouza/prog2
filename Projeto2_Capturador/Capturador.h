/* 
 * File:   Capturador.h
 * Author: msobral
 *
 * Created on 1 de Abril de 2016, 15:00
 */

#ifndef CAPTURADOR_H
#define	CAPTURADOR_H

#include <pcap.h>
#include <net/if.h>

#include "Pacote.h"

#include <string>
#include <ostream>
#include "Lista.h"

using namespace std;

class ArquivoCaptura {
public:
    ArquivoCaptura(const char * pathname, pcap_t * handle);
    ~ArquivoCaptura();
    void adicione(const Ethernet * pkt);
    void adicione(Lista<Ethernet*> * pkts);
private:
    pcap_dumper_t * dumper;
};

class NetInterface {
private:
    string name;
public:
    NetInterface(const string & ifname);
    NetInterface();
    NetInterface(const NetInterface & outra);
    virtual ~NetInterface();
    NetInterface & operator=(const NetInterface & outra);
    u_char * obtem_mac_addr() const;
    in_addr obtem_ip_addr() const;
    u_int obtem_mtu() const;
    in_addr obtem_netmask() const;
    in_addr obtem_broadcast() const;
    in_addr obtem_prefixo() const;
private:
    void do_ioctl(ifreq & ifr, int cmd) const;    
};

class Frame {
private:
    Ethernet * pkt;
public:
    Frame(Ethernet * p) : pkt(p) {}
    Frame() : pkt(NULL) {}
    void release() { if (pkt) delete pkt;}
    Ethernet * get_frame() const { return pkt;}   
    Ethernet * operator->() const { return pkt;}
    bool operator<(const Frame & o);    
};

ostream & operator<<(ostream & out, const Frame & f);

class Capturador {
public:
    // construtor: captura de um arquivo de captura existente
    Capturador(const char * pathname);
    
    // construtor: captura da interface default 
    // bytes: limite de bytes capturados por pacote
    Capturador(int bytes);
    
    // construtor: captura da interface "iface"
    // bytes: limite de bytes capturados por pacote
    Capturador(int bytes, const char * iface);
    
    // construtor: cria um capturador igual a outro existente
    Capturador(const Capturador& orig);
    
    // construtor: cria um capturador nulo
    Capturador();    

    virtual ~Capturador();
    
    // operador de atribuição
    Capturador& operator=(const Capturador & orig);
    
    // Captura um pacote, esperando-o indefinidamente
    Frame capture();
    
    // Captura "count" pacotes, esperando-os indefinidamente
    // "count" deve ser > 0, do contrário uma exceção é disparada.
    Lista<Frame>* capture(unsigned int count);
    
    // Captura até "count" pacotes, esperando-os por 
    // no máximo "segundos". Se "count" == 0, captura quantos
    // pacotes aparecerem durante "segundos". Se "segundos" == 0,
    // funciona igual a capture(int count).
    // OBS: se "count" == 0 e "segundos" == 0, dispara uma exceção.
    Lista<Frame>* capture(unsigned int count, unsigned long segundos);
    
    void muda_filtro(const char* filtro);
    NetInterface obtem_iface() const;
    
    // gera um arquivo de captura onde se podem 
    // gravar pacotes capturados
    ArquivoCaptura * gera_arquivo_captura(const char * pathname) const;
private:
    pcap_t * handle;
    char * dev;
    int fd;
    int snaplen;
    
    void inicia(int bytes);
    static void handle_pkt(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet);
    static void timer_handler(int tid, void *obj);
};

void remove_pktlist(Lista<Frame> * lista);

#endif	/* CAPTURADOR_H */

