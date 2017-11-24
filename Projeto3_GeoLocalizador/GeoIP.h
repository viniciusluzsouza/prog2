#ifndef GEOIP_H
#define GEOIP_H
#include <string>

using namespace std;

// classe para representar uma localização da base GeoIP ...
class GeoIPLocation {
public:
    GeoIPLocation(const string & linha);
    GeoIPLocation();
    GeoIPLocation(const GeoIPLocation & orig);
    GeoIPLocation(unsigned int dado);
    virtual ~GeoIPLocation();
    // faltam aqui os métodos para obter os valores dos atributos ...
    
    bool operator>(const GeoIPLocation & outro){
        return id > outro.id;
    }
    
    bool operator<(const GeoIPLocation & outro){
        return id < outro.id;
    }
    
    bool operator==(const GeoIPLocation & outro){
        return id == outro.id;
    }
    
    bool operator=(const GeoIPLocation & orig){
        id = orig.id;
        pais = orig.pais;
        cidade = orig.cidade;
        zipcode = orig.zipcode;
        longitude = orig.longitude;
        latitude = orig.latitude;
        metro = orig.metro;
        area = orig.area;
    }
    
    friend ostream& operator<<(ostream & saida, const GeoIPLocation & outro){
        saida << outro.id << ",\"" << outro.pais << "\",\"" << outro.regiao << "\",\"" << outro.cidade << "\",\"" << outro.zipcode << "\"," << outro.latitude << "," << outro.longitude << "," << outro.metro << "," << outro.area;
        return saida;
    }
    
    // retorna o pais
    string obtem_pais() { return pais; }
    // retorna regiao
    string obtem_regiao() { return regiao; }
    // retorna cidade
    string obtem_cidade() { return cidade;}
    // retorna o id
    unsigned int obtem_id() { return id; }
    
    //metodo para mostrar conteudo
    void escrevaSe(ostream& out) const;
    
private:
    unsigned int id;
    string pais;
    string regiao;
    string cidade;
    string zipcode;
    double latitude, longitude;
    unsigned int metro, area;
};
 
// classe para representar uma faixa de endereços da base GeoIP ...
class GeoIP {
public:
    GeoIP(const string & linha);
    GeoIP() {}
    GeoIP(const GeoIP& orig);
    GeoIP(const string & ip1, const string & ip2, unsigned int loc);
    GeoIP(unsigned int ip);
    virtual ~GeoIP();
    unsigned int get_addr1() { return addr1;}
    unsigned int get_addr2() { return addr2;}
    unsigned int get_location() { return locid;}
    //devolve addr1 no formato string
    string get_addr1_str();
    //devolve addr2 no formato string
    string get_addr2_str();
    
    bool operator==(const GeoIP & outro){
        bool ok = (addr1 <= outro.addr1) and (addr2 >= outro.addr2);
        if (not ok) {
            ok = (addr1>= outro.addr1) and (addr2 <= outro.addr2);
        }
        return ok;
    }
    
    bool operator>(const GeoIP & outro){
        return addr1 > outro.addr2;
    }
    
    bool operator<(const GeoIP & outro){
        return addr2 < outro.addr1;
    }
    
    bool operator=(const GeoIP & outro){
        addr1 = outro.addr1;
        addr2 = outro.addr2;
        locid = outro.locid;
    }
    
    friend ostream& operator<<(ostream & saida, const GeoIP & outro){
        saida << '"' << outro.addr1 << "\",\"" << outro.addr2 << "\",\"" << outro.locid << '"';
        return saida;
    }

    
private:
    unsigned int addr1, addr2;
    unsigned int locid;
    
    unsigned int str2ip(const string & ip) const;
    string ip2str(unsigned int addr) const;
};

#endif /* GEOIP_H */