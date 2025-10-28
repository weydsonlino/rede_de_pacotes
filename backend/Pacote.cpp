#include "Pacote.h"
#include <iostream>

// === Construtor ===
Pacote::Pacote(
    const std::string& enderecoMACOrigem,
    const std::string& enderecoMACDestino,
    const std::string& enderecoIPOrigem,
    const std::string& enderecoIPDestino
) : enderecoMACOrigem(enderecoMACOrigem),
enderecoMACDestino(enderecoMACDestino),
tipoEthernet(0x0800),
enderecoIPOrigem(enderecoIPOrigem),
enderecoIPDestino(enderecoIPDestino),
versaoIP(4),
ttl(64),
protocolo(6) {
}


std::string Pacote::getEnderecoMACOrigem() const { return enderecoMACOrigem; }
std::string Pacote::getEnderecoMACDestino() const { return enderecoMACDestino; }
std::string Pacote::getEnderecoIPOrigem() const { return enderecoIPOrigem; }
std::string Pacote::getEnderecoIPDestino() const { return enderecoIPDestino; }

void Pacote::setDados(const std::vector<unsigned char>& novosDados) {
    dados = novosDados;
}

std::vector<unsigned char> Pacote::getDados() const {
    return dados;
}

// === Métodos de simulação ===
void Pacote::enviar() const {
    std::cout << "Enviando pacote de " << enderecoIPOrigem
        << " para " << enderecoIPDestino << "..." << std::endl;
}

void Pacote::exibirCabecalhos() const {
    std::cout << "=== Cabeçaleta (Enlace) ===" << std::endl;
    std::cout << "MAC Origem:  " << enderecoMACOrigem << std::endl;
    std::cout << "MAC Destino: " << enderecoMACDestino << std::endl;
    std::cout << "Tipo:        0x" << std::hex << tipoEthernet << std::dec << std::endl;

    std::cout << "\n=== Cabeçalho IP (Rede) ===" << std::endl;
    std::cout << "IP Origem:   " << enderecoIPOrigem << std::endl;
    std::cout << "IP Destino:  " << enderecoIPDestino << std::endl;
    std::cout << "Versão IP:   " << (int)versaoIP << std::endl;
    std::cout << "TTL:         " << (int)ttl << std::endl;
    std::cout << "Protocolo:   " << protocolo << std::endl;
}
