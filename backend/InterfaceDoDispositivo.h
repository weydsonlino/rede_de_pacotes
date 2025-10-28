#pragma once
#include <string>

class Dispositivo;
class Pacote;

class InterfaceDoDispositivo
{
private:
	bool status;
	std::string ip;
	InterfaceDoDispositivo* interfaceConectada;
	std::string macAddress;

public:
	Dispositivo* dispositivoVinculado;
	InterfaceDoDispositivo(std::string ip, std::string macAddress, Dispositivo* dispositivoVinculado);
	bool getStatus() const;
	std::string getIP() const;
	std::string getMacAddress() const;
	void setStatus(bool status);
	void setInterfaceConectado(InterfaceDoDispositivo* interface);
	InterfaceDoDispositivo* getInterfaceConectado() const;
	void encaminharPacote(Pacote* pacote) const;
	void receberPacote(Pacote* pacote) const;

	// Novo: permitir a GUI atualizar o IP
	void setIP(const std::string& novoIP);
};
