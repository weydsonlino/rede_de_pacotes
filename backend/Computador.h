#pragma once
#include <string>
#include "IDispositivo.h"

class InterfaceDoDispositivo;
class Pacote;

class Computador : public Dispositivo
{
	private:
	std::string nome;
	InterfaceDoDispositivo* interfaceDispositivo;

	public:
	Computador(std::string nome, std::string ip, std::string rotaDefaultIP, std::string rotaDefaultMAC,std::string macAddress);
	std::string getNome() const override;
    void setNome(const std::string& novoNome) override;
	std::array<InterfaceDoDispositivo*, 5> getInterfaces() const override;
    void setGateway(std::string novoGateway, std::string novoGatewayMac);
};

