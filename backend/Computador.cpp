#include <array>
#include "Computador.h"
#include "InterfaceDoDispositivo.h"

// Corrigindo chamada do construtor base Dispositivo
Computador::Computador(std::string nome, std::string ip, std::string rotaDefaultIP, std::string rotaDefaultMAC, std::string macAddress)
	: Dispositivo(rotaDefaultIP, rotaDefaultMAC), nome(nome), interfaceDispositivo(new InterfaceDoDispositivo(ip, macAddress, this)) {
}

std::string Computador::getNome() const {
	return nome;
}
void Computador::setNome(const std::string& novoNome) {
    nome = novoNome;
}

void Computador::setGateway(std::string novoGateway, std::string novoGatewayMac){
    rotaDefaultIP = novoGateway;
    rotaDefaultMAC = novoGatewayMac;
};
std::array<InterfaceDoDispositivo*, 5> Computador::getInterfaces() const
{
	std::array<InterfaceDoDispositivo*, 5> interfaces{};
	interfaces.fill(nullptr);         // Inicializa tudo com nullptr
	interfaces[0] = interfaceDispositivo;   // A interface única do computador fica no índice 0
	return interfaces;
}
