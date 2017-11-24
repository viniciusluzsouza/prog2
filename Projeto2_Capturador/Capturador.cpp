/* 
 * File:   Capturador.cpp
 * Author: msobral
 * 
 * Created on 1 de Abril de 2016, 15:00
 */

#include "Capturador.h"
#include "Timer.h"
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <unistd.h>

Capturador::Capturador(int bytes) {
    char errbuf[PCAP_ERRBUF_SIZE];

    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) throw errbuf;
    inicia(bytes);
}

Capturador::Capturador(const char * pathname) {
    dev = strdup(pathname);
    inicia(0);
}

Capturador::Capturador(int bytes, const char * iface) {
    dev = strdup(iface);
    inicia(bytes);
}

Capturador::Capturador(const Capturador& orig) {
    dev = NULL;
    handle = NULL;
    *this = orig;
}

Capturador::Capturador() {
    dev = NULL;
    snaplen = 0;
    handle = NULL;
    fd = -1;
}

void Capturador::inicia(int bytes) {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    snaplen = bytes;
    if (snaplen == 0) {
        handle = pcap_open_offline(dev, errbuf);
    } else {
        handle = pcap_open_live(dev, bytes, 0, 0, errbuf);
    }
    if (handle == NULL)  {
        throw errbuf;
    }
    
    fd = pcap_get_selectable_fd(handle);
}

Capturador::~Capturador() {
    if (handle) pcap_close(handle);
    if (dev and not snaplen) delete dev;
}

Capturador& Capturador::operator=(const Capturador & orig) {
    if (handle) pcap_close(handle);
    if (dev) delete dev;
    fd = orig.fd;
    dev = strdup(orig.dev);
    inicia(orig.snaplen);
}

void Capturador::muda_filtro(const char* filtro) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    bpf_u_int32 net;
    bpf_u_int32 mask;

    if (snaplen == 0) return; // ignora se for arquivo de captura
    
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        throw errbuf;        
    }
    if (pcap_compile(handle, &fp, (const char*)filtro, 0, net) == -1) {
        throw "filtro invalido";
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        throw "filtro inaplicavel";
    }
}

Frame Capturador::capture() {
    const u_char * pkt;
    struct pcap_pkthdr header;
    Ethernet * p;
    
    if (not handle) throw "captura não ativada";
    
    pkt = pcap_next(handle, &header);
    if (not pkt) throw "erro na captura de pacote";

    p = new Ethernet((u_char*)pkt, &header);
    return Frame(p);
}

/*list<Ethernet*> Capturador::capture(int count) {
    list<Ethernet*> pkts;
    
    while (count > 0) {
        pkts.push_back(capture());
        count--;
    }
    return pkts;
}*/

Lista<Frame>* Capturador::capture(unsigned int count) {
    if (not handle) throw "captura não ativada";

    Lista<Frame>* pkts = new Lista<Frame>;    
    pcap_loop(handle, count, Capturador::handle_pkt, (u_char*)pkts);
    return pkts;
}

void Capturador::timer_handler(int tid, void* obj) {
    Capturador * cap = (Capturador*)obj;
    pcap_breakloop(cap->handle);
}

Lista<Frame>* Capturador::capture(unsigned int count, unsigned long segundos) {
    if (segundos > 0) {
        Timer clock(1000*segundos, (TimerHandler)Capturador::timer_handler, 0, (void*)this);
        clock.start();
        return capture(count);
    } else if (count == 0) throw "erro: segundos e count não podem ser ambos nulos";
    
    return capture(count);
}

NetInterface Capturador::obtem_iface() const {
    if (snaplen == 0) throw -1; // captura feita em arquivo
    return NetInterface(dev);
}

void Capturador::handle_pkt(u_char *args, 
        const struct pcap_pkthdr *header,
	const u_char *pkt) {

    Lista<Frame> * pkts = (Lista<Frame> *) args;
    Ethernet * p;
    
    p = new Ethernet((u_char*)pkt, header);
    pkts->anexa(Frame(p));    
}

ArquivoCaptura * Capturador::gera_arquivo_captura(const char* pathname) const {
    if (not handle) return NULL;
    ArquivoCaptura * arq = new ArquivoCaptura(pathname, handle);
    return arq;
}

ArquivoCaptura::ArquivoCaptura(const char* pathname, pcap_t* handle) {
   dumper = pcap_dump_open(handle, pathname); 
   if (not dumper) throw -1;
}

ArquivoCaptura::~ArquivoCaptura() {
    pcap_dump_close(dumper);
}

void ArquivoCaptura::adicione(const Ethernet* pkt) {
    pcap_pkthdr header = pkt->get_pcap_header();
    pcap_dump((u_char*)dumper, &header, pkt->get_raw());    
}

void ArquivoCaptura::adicione(Lista<Ethernet*>* pkts) {
    int n = pkts->comprimento();
    while (n > 0) {
        Ethernet * pkt = pkts->remove(0);
        pkts->anexa(pkt);
        adicione(pkt);
        n--;
    }    
}

NetInterface::NetInterface(const string & ifname) : name(ifname) {}

NetInterface::NetInterface() {}

NetInterface::NetInterface(const NetInterface & outra) : name(outra.name) {}

NetInterface::~NetInterface() {}

NetInterface & NetInterface::operator=(const NetInterface & outra) { 
    name = outra.name;
}

u_char * NetInterface::obtem_mac_addr() const {
    static u_char mac_addr[ETHER_ADDR_LEN];
    ifreq ifr;

    do_ioctl(ifr, SIOCGIFHWADDR);
    
    memcpy(mac_addr, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
    
    return mac_addr;    
}

in_addr NetInterface::obtem_ip_addr() const {
    ifreq ifr;

    do_ioctl(ifr, SIOCGIFADDR);
    
    sockaddr_in * addr = (sockaddr_in*)&(ifr.ifr_ifru.ifru_addr);
    return addr->sin_addr;    
}

void NetInterface::do_ioctl(ifreq & ifr, int cmd) const {
    if (name.size() == 0) throw -1;
    
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) throw -1;
    
    bzero(&ifr, sizeof(ifreq));
    strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ); 
    
    if (ioctl(sd, cmd, &ifr) < 0) {
        close(sd);
        throw -1;
    }
    
    close(sd);
}

in_addr NetInterface::obtem_netmask() const {
    ifreq ifr;

    do_ioctl(ifr, SIOCGIFNETMASK);
    
    sockaddr_in * addr = (sockaddr_in*)&(ifr.ifr_ifru.ifru_addr);
    return addr->sin_addr;   
}

in_addr NetInterface::obtem_prefixo() const {
    in_addr addr = obtem_ip_addr();
    in_addr mask = obtem_netmask();
    
    addr.s_addr = addr.s_addr & mask.s_addr;
    return addr;
}

in_addr NetInterface::obtem_broadcast() const {
    ifreq ifr;

    do_ioctl(ifr, SIOCGIFBRDADDR);
    
    sockaddr_in * addr = (sockaddr_in*)&(ifr.ifr_ifru.ifru_addr);
    return addr->sin_addr;   
}

u_int NetInterface::obtem_mtu() const {
    ifreq ifr;

    do_ioctl(ifr, SIOCGIFMTU);
    
    return ifr.ifr_mtu;   
}

void remove_pktlist(Lista<Frame> * lista) {
    while (not lista->vazia()) {
        Frame f = lista->remove(0);
        f.release();
    }
    delete lista;
}

ostream & operator<<(ostream & out, const Frame & f) {
    out << f.get_frame();
    return out;
}

bool Frame::operator<(const Frame & o) {
    Ethernet * p = o.get_frame();
    
    return pkt->get_size() < p->get_size();
}