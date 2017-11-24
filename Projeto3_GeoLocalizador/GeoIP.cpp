/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include "GeoIP.h"

using namespace std;

GeoIP::GeoIP(const string & linha) {
    
    istringstream inp(linha);
    inp.ignore(1);
    inp >> addr1;
    inp.ignore(3);
    inp >> addr2;
    inp.ignore(3);
    inp >> locid;
    
    if (inp.fail()) throw -1;
    
}
GeoIP::~GeoIP(){
    
}

GeoIP::GeoIP(const string & ip1, const string & ip2, unsigned int id){
    addr1 = str2ip(ip1);
    addr2 = str2ip(ip2);
    locid = id;
}

GeoIP::GeoIP(unsigned int ip){
        
    addr1 = addr2 = ip;
    locid = 0;

}

GeoIP::GeoIP(const GeoIP& orig){
    addr1 = orig.addr1;
    addr2 = orig.addr2;
    locid = orig.locid;
}

string GeoIP::get_addr1_str(){
    string IP = ip2str(addr1);
    return IP;
}

string GeoIP::get_addr2_str(){
    string IP = ip2str(addr2);
    return IP;
} 

unsigned int GeoIP::str2ip(const string& ip) const {
    istringstream inp(ip);
    unsigned int o1, o2, o3, o4;
    
    unsigned int addr = 0;
    for (int n=4;n>0;n--){
        unsigned int oct;
                
        inp >> oct;
        if ((oct > 255) or (inp.fail()) ) throw -1;
        addr += (oct << (8*(n-1)));
        inp.ignore(1);
    }
    return addr;
}

string GeoIP::ip2str(unsigned int addr) const {
    ostringstream out;
    
    for (int n=4;n>0;n--){
        unsigned int oct;
        
        oct = (addr >> 8*(n-1)) & 0xff;
        out << oct;
        if (n>1) out << '.';
    }
    return out.str();
}

GeoIPLocation::GeoIPLocation(const string& linha){
    
    char buffer[32];
    
    istringstream inp(linha);
    inp >> id;
    inp.ignore(2);
    inp.get(buffer, 32, '"');
    if (inp.fail()) inp.clear();
    pais = buffer;
    inp.ignore(3);
    inp.get(buffer, 32, '"');
    if (inp.fail()) inp.clear();
    regiao = buffer;
    inp.ignore(3);
    inp.get(buffer, 32, '"');
    if ( inp.fail() ) inp.clear();
    cidade = buffer;
    inp.ignore(3);
    inp.get(buffer, 32, '"');
    if ( inp.fail() ) inp.clear();
    zipcode = buffer;
    if ( inp.fail() ){
        inp.clear();
        zipcode = "0";
    }
    inp.ignore(2);
    inp >> latitude;
    if ( inp.fail() ) inp.clear();
    inp.ignore(1);
    inp >> longitude;
    if ( inp.fail() ) inp.clear();
    inp.ignore(1);
    inp >> metro;
    if ( inp.fail() ) {
        inp.clear();
        metro = 0;
    }
    inp.ignore(1);
    inp >> area;
    if ( inp.fail() ) {
        inp.clear();
        area = 0;
    }
    
    if (inp.fail()) throw -1;
    
}

GeoIPLocation::~GeoIPLocation(){
    
}

GeoIPLocation::GeoIPLocation(){
    
}
    
GeoIPLocation::GeoIPLocation(const GeoIPLocation& orig){
    id = orig.id;
    pais = orig.pais;
    cidade = orig.cidade;
    zipcode = orig.zipcode;
    longitude = orig.longitude;
    latitude = orig.latitude;
    metro = orig.metro;
    area = orig.area;
    
}

GeoIPLocation::GeoIPLocation(unsigned int dado){
    id = dado;
}

void GeoIPLocation::escrevaSe(ostream& out) const {
    //out << "ID: " << id << endl;
    out << "LOCAL:" << endl;
    out << "Pais: " << pais << endl;
    out << "Região: " << regiao << endl;
    out << "Cidade: " << cidade << endl;
    out << "Código Postal: " << zipcode << endl;
    out << "Latitude: " << latitude << endl;
    out << "Longitude: " << longitude << endl;
    out << "Código Metropolitano: " << metro << endl;
    out << "Código de Área: " << area << endl;
}