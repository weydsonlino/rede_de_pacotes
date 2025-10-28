#pragma once
#include <string>
#include <vector>

class Pacote {
private:
    // === Camada de Enlace (Cabeçaleta) ===
    std::string enderecoMACOrigem;
    std::string enderecoMACDestino;
    unsigned short tipoEthernet; // ex: 0x0800 para IPv4
    // === Camada de Rede (Cabeçalho IP simplificado) ===
    std::string enderecoIPOrigem;
    std::string enderecoIPDestino;
    unsigned char versaoIP;      // normalmente 4
    unsigned char ttl;           // Time To Live
    unsigned short protocolo;    // 6 = TCP, 17 = UDP, etc.

    // === Dados (Payload) ===
    std::vector<unsigned char> dados;

public:
    Pacote(
        const std::string& macOrigem,
        const std::string& macDestino,
        //unsigned short tipo,
        const std::string& ipOrigem,
        const std::string& ipDestino
        //unsigned char versao = 4,
        //unsigned char ttl = 64,
        //unsigned short protocolo = 6
    );

    std::string getEnderecoMACOrigem() const;
    std::string getEnderecoMACDestino() const;
    std::string getEnderecoIPOrigem() const;
    std::string getEnderecoIPDestino() const;
    void setDados(const std::vector<unsigned char>& novosDados);
    std::vector<unsigned char> getDados() const;
    void enviar() const;
    void exibirCabecalhos() const;
};

