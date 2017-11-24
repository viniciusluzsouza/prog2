/* 
 * File:   Pacote.cpp
 * Author: msobral
 * 
 * Created on 5 de Abril de 2016, 14:54
 */

#include <cstdlib>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <iomanip>

using namespace std;

#include "Pacote.h"
#include "pcap/pcap.h"

Pacote::Pacote(u_char* pkt, int tamanho, int capturados) {
    payload = pkt;
    header_len = 0;
    size = tamanho;
    captured = capturados;    
}

void Pacote::dump(ostream& out, u_char* ptr, int len) const {
    int m = 0, line = 0;

    out << hex;
    while (m < len) {
        out << setfill('0') << setw(2) << line*16 << ": ";

        for (int n=0; n < 16 and m < len; n++, m++) {
            out << setfill('0') << setw(2) << (int)ptr[m] << " ";
        }
        out << endl;
        line++;
    }    
}

void Pacote::show_raw(ostream & out) const {
    dump(out, get_data(), captured);
}

void Pacote::show_data(ostream & out) const {
    Pacote * pac = get_payload();
    if (not pac) {
        dump(out, get_data(), captured-header_len);
    } else {
        pac->show_data(out);
        delete pac;
    }
}


//Ethernet::Ethernet(u_char* pkt, int tamanho, int capturados, timeval ts) : Pacote(pkt, tamanho, capturados) {
Ethernet::Ethernet(u_char* pkt, const pcap_pkthdr * a_header) : Pacote(pkt, a_header->len, a_header->caplen) {
    header = *a_header;
    packet = new u_char[header.caplen];
    memcpy(packet, pkt, header.caplen);    
    eth = (sniff_ethernet*)packet;
    payload = (u_char*)(packet + SIZE_ETHERNET);
    header_len = SIZE_ETHERNET;   
}

u_short Ethernet::get_type() const {
    u_short type = ntohs(eth->ether_type);
    return type;
}

u_char * Ethernet::get_src() const {
    return eth->ether_shost;
}

u_char * Ethernet::get_dest() const {
    return eth->ether_dhost;
}

Pacote * Ethernet::get_payload() const {   
    Pacote * p = NULL;
    
    switch (get_type()) {
        case 0x800: // IPv4
            p = (Pacote*) new IP(payload, size-header_len, captured-header_len);
            break;
        case 0x806: // ARP
            p = (Pacote*) new ARP(payload, size-header_len, captured-header_len);
            break;
        case 0x86dd: // IPv6
            p = (Pacote*) new IPv6(payload, size-header_len, captured-header_len);
            break;
    }
    return p;       
}

string Ethernet::mac2string(u_char* addr) {
    int n;
    char byte[4];
    string mac;
    bool started = false;
    
    for (n=0; n < ETHER_ADDR_LEN; n++) {
        snprintf(byte, 4, "%02hhX", addr[n]);
        if (started) mac += ":";
        else started = true;
        mac += byte;
    }
    return mac;
}

u_short Ethernet::get_size() const {
    return size;
}

void Ethernet::show(ostream& out) const {   
    out << dec << header.ts.tv_sec << "." << header.ts.tv_usec << ": ";
    out << mac2string(eth->ether_shost) << " > ";
    out << mac2string(eth->ether_dhost);
    out << ", type=" << hex << get_type();
    Pacote * pac = get_payload();
    if (pac) {
        out << ", ";
        pac->show(out);
    }
}

IP::IP(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    ip = (sniff_ip*)pkt;
    header_len = IP_HL(ip)*4;
    payload = (pkt + header_len);
}

Pacote * IP::get_payload() const {
    Pacote * p = NULL;
    
    switch (ip->ip_p) {
        case proto_TCP: // TCP
            p = (Pacote*) new TCP(payload, size-header_len, captured-header_len);
            break;
        case proto_UDP: // UDP
            p = (Pacote*) new UDP(payload, size-header_len, captured-header_len);
            break;
        case proto_ICMP: // ICMP
            p = (Pacote*) new ICMP(payload, size-header_len, captured-header_len);
            break;
    }
    
    return p;    
}

in_addr IP::get_src() const {
    return ip->ip_src;
}

in_addr IP::get_dest() const {
    return ip->ip_dst;
}

u_char IP::get_proto() const {
    return ip->ip_p;
}

u_short IP::get_size() const {
    return ip->ip_len;
}

string IP::addr2string(in_addr addr) {
    string addr4;
    char buffer[INET_ADDRSTRLEN];
    
    inet_ntop(AF_INET, (void*)&addr, buffer, INET_ADDRSTRLEN);
    addr4 = buffer;
    
    return addr4;
}

void IP::show(ostream& out) const {
    out << "IP: " << addr2string(ip->ip_src) << " > ";
    out << addr2string(ip->ip_dst) << ", ";
    out << "proto " << dec << (int)get_proto();
    Pacote * pac = get_payload();
    if (pac) {
        out << ", ";
        pac->show(out);
    }
}

TCP::TCP(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    tcp = (sniff_tcp*)pkt;
    header_len = TH_OFF(tcp)*4; 
    payload = (pkt + header_len);
}

u_short TCP::get_dport() const {
    return ntohs(tcp->th_dport);
}
    
u_short TCP::get_sport() const {
    return ntohs(tcp->th_sport);
}

u_short TCP::get_window() const {
    return ntohs(tcp->th_win);
}

u_int TCP::get_ack() const {
    return ntohl(tcp->th_ack);
}

u_int TCP::get_sequence() const {
    return ntohl(tcp->th_seq);
}

u_char TCP::get_flags() const {
    return tcp->th_flags;
}

u_short TCP::get_size() const {
    return size;
}

void TCP::show(ostream& out) const {
    out << "TCP: sport=" << dec << get_sport();
    out << ", dport=" << get_dport();
}

UDP::UDP(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    udp = (sniff_udp*)pkt;
    header_len = SIZE_UDP;
    payload = udp->data;
}

u_short UDP::get_dport() const {
    return ntohs(udp->dport);
}

u_short UDP::get_sport() const {
    return ntohs(udp->sport);
}

u_short UDP::get_size() const {
    return size;
}

void UDP::show(ostream& out) const {
    out << "UDP: sport=" << dec << get_sport();
    out << ", dport=" << get_dport();
}

ICMP::ICMP(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    icmp = (sniff_icmp*)pkt;
    header_len = sizeof(sniff_icmp);
    payload = icmp->data;
}

u_char ICMP::get_code() const {
    return icmp->code;
}

u_char ICMP::get_type() const {
    return icmp->type;
}

u_short ICMP::get_size() const {
    return size;
}

void ICMP::show(ostream& out) const {
    out << "ICMP: type=" << dec << (int)get_type();
    out << ", code=" << (int)get_code();
}

ARP::ARP(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    arp = (sniff_arp*)pkt;
    header_len = sizeof(sniff_arp);
    payload = arp->data;
}

u_short ARP::get_hardware_type() const {
    return ntohs(arp->hrd);
}

u_short ARP::get_protocol() const {
    return ntohs(arp->pro);
}

u_char ARP::get_hlen() const {
    return arp->hln;
}

u_char ARP::get_proto_len() const {
    return arp->pln;
}

u_short ARP::get_opcode() const {
    return ntohs(arp->op);
}

u_char * ARP::get_sha() const {
    return arp->sha;
}

u_char * ARP::get_tha() const {
    return arp->tha;
}

in_addr ARP::get_spa() const {
    return arp->spa;
}

in_addr ARP::get_tpa() const {
    return arp->tpa;
}

u_short ARP::get_size() const {
    return size;
}

void ARP::show(ostream& out) const {
    out << "ARP: source IP=" << IP::addr2string(arp->spa);
    out << ", dest IP=" << IP::addr2string(arp->tpa);
}

IPv6::IPv6(u_char* pkt, int tamanho, int capturados) : Pacote(pkt, tamanho, capturados) {
    ipv6 = (sniff_ipv6*)pkt;
    header_len = sizeof(sniff_ipv6);
    payload = (pkt + header_len); // errado !!! corrigir ...
}

u_char IPv6::get_version() const {
    return ipv6->vtf & 0xf;
}

u_char IPv6::get_class() const {
    return (ipv6->vtf >> 4) & 0xff;
}

u_int IPv6::get_flowlabel() const {
    return (ipv6->vtf >> 12);
}

u_short IPv6::get_len() const {
    return ntohs(ipv6->len);
}

u_char IPv6::get_next() const {
    return ipv6->next;
}

u_char IPv6::get_proto() const {
    return ipv6->next;
}

u_char IPv6::get_limit() const {
    return ipv6->limit;
}

u_char * IPv6::get_src() const {
    return ipv6->src;
}

u_char * IPv6::get_dest() const {
    return ipv6->dest;
}

string IPv6::addr2string(u_char* addr) const {
    string addr6;
    char buffer[INET6_ADDRSTRLEN];
    
    inet_ntop(AF_INET6, (void*)addr, buffer, INET6_ADDRSTRLEN);
    addr6 = buffer;
    
    return addr6;
}

Pacote * IPv6::get_payload() const {
    Pacote * p = NULL;
    
    switch (ipv6->next) {
        case proto_TCP: // TCP
            p = (Pacote*) new TCP(payload, size-header_len, captured-header_len);
            break;
        case proto_UDP: // UDP
            p = (Pacote*) new UDP(payload, size-header_len, captured-header_len);
            break;
        case proto_ICMP: // ICMP
            p = (Pacote*) new ICMP(payload, size-header_len, captured-header_len);
            break;
    }
    
    return p;    
}

void IPv6::show(ostream& out) const {
    
}

ostream& operator<<(ostream & out, const Pacote & pkt) {
    pkt.show(out);
    return out;
}

ostream& operator<<(ostream & out, const Pacote * pkt) {
    pkt->show(out);
    return out;
}
