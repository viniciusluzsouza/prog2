/* 
 * File:   Pacote.h
 * Author: msobral
 *
 * Created on 5 de Abril de 2016, 14:54
 */

#ifndef PACOTE_H
#define	PACOTE_H

#include <string>
#include <fstream>
#include <pcap.h>

using namespace std;

#include "headers.h"

class Pacote {
public:
  Pacote(u_char * pac, int tamanho, int capturados);
  virtual ~Pacote() {}
  
  virtual Pacote * get_payload() const { return NULL;}
  u_char * get_data() const { return payload;}
  virtual u_short get_size() {return size;}
  
  virtual void show(ostream & out) const { }
  virtual void show_data(ostream & out) const;
  virtual void show_raw(ostream & out) const;
private:
    void dump(ostream & out, u_char * ptr, int len) const;
 protected:
    u_char * payload;
    u_short header_len;
    u_short size;
    u_short captured;
};

class Ethernet : public Pacote{
public:
    //Ethernet(u_char * pkt, int tamanho, int capturados, timeval ts);
    Ethernet(u_char * pkt, const pcap_pkthdr * header);
    virtual ~Ethernet() { delete packet;}

    virtual Pacote * get_payload() const;
    
    u_short get_type() const;
    u_char* get_src() const;
    u_char* get_dest() const;
    virtual u_short get_size() const;
    
    static string mac2string(u_char*);
    virtual void show(ostream & out) const;

    pcap_pkthdr get_pcap_header() const { return header;}
    timeval get_timestamp() const { return header.ts;}

    u_char * get_raw() const { return packet;}

    enum Types {type_IP=0x800, type_IPv6=0x86dd, type_ARP=0x806};
private:    
    sniff_ethernet * eth;
    pcap_pkthdr header;
    u_char * packet;
};

class IP : public Pacote{
public:
    IP(u_char * pkt, int tamanho, int capturados);
    virtual ~IP() {}
    
    virtual Pacote * get_payload() const;
    
    u_char get_proto() const;
    in_addr get_src() const;
    in_addr get_dest() const;
    virtual u_short get_size() const;
    static string addr2string(in_addr addr);
    
    virtual void show(ostream & out) const;
    
    enum Protocols {proto_TCP=6, proto_UDP=17, proto_ICMP=1, proto_IPv4=4};
 private:
    sniff_ip* ip;
};

class TCP : public Pacote{
public:
    TCP(u_char * pkt, int tamanho, int capturados);
    virtual ~TCP() {}
    
    u_short get_sport() const;
    u_short get_dport() const;
    u_short get_window() const;
    u_char get_flags() const;
    u_int get_sequence() const;
    u_int get_ack() const;
    virtual u_short get_size() const;
    
    virtual void show(ostream & out) const;
    
 private:
    sniff_tcp* tcp;
};

class UDP : public Pacote{
public:
    UDP(u_char * pkt, int tamanho, int capturados);
    virtual ~UDP() {}
    
    u_short get_sport() const;
    u_short get_dport() const;
    virtual u_short get_size() const;
    
    virtual void show(ostream & out) const;
 private:
    sniff_udp* udp;
};

class ICMP : public Pacote{
public:
    ICMP(u_char * pkt, int tamanho, int capturados);
    virtual ~ICMP() {}
    
    u_char get_type() const;
    u_char get_code() const;
    virtual u_short get_size() const;
    
    virtual void show(ostream & out) const;
 private:
    sniff_icmp* icmp;
};

class ARP : public Pacote {
public:
    ARP(u_char * pkt, int tamanho, int capturados);
    virtual ~ARP() {}
    
    virtual u_short get_size() const;
    
    u_short get_hardware_type() const;
    u_short get_protocol() const;
    u_char get_hlen() const;
    u_char get_proto_len() const;
    u_short get_opcode() const;
    u_char * get_sha() const;
    u_char * get_tha() const;
    in_addr get_spa() const;
    in_addr get_tpa() const;
    
    virtual void show(ostream & out) const;
private:
    sniff_arp * arp;
};

class IPv6 : public Pacote {
public:
    IPv6(u_char * pkt, int tamanho, int capturados);
    virtual ~IPv6() {}
    
    virtual u_short get_size() const {return 0;}

    virtual Pacote * get_payload() const;
    u_char get_version() const;
    u_char get_class() const;
    u_int get_flowlabel() const;
    u_short get_len() const;
    u_char get_next() const;
    u_char get_proto() const;
    u_char get_limit() const;
    u_char * get_src() const;
    u_char * get_dest() const;
    
    string addr2string(u_char * addr) const;
    
    virtual void show(ostream & out) const;
    enum Protocols {proto_TCP=6, proto_UDP=17, proto_ICMP=1, proto_IPv4=4};
private:
    sniff_ipv6 * ipv6;
};

ostream& operator<<(ostream & out, const Pacote * pkt);
ostream& operator<<(ostream & out, const Pacote & pkt);

#endif	/* PACOTE_H */

