#include "InterfaceDoDispositivo.h"
#include "IDispositivo.h"
#include "Pacote.h"
#include "Computador.h"
#include "Switch.h"
#include "Roteador.h"


InterfaceDoDispositivo::InterfaceDoDispositivo(std::string ip, std::string macAddress, Dispositivo* dispositivoVinculado)
    : status(true), ip(ip), interfaceConectada(nullptr), macAddress(macAddress), dispositivoVinculado(dispositivoVinculado) {
}
bool InterfaceDoDispositivo::getStatus() const {
    return status;
}

std::string InterfaceDoDispositivo::getIP() const {
    return ip;
}

std::string InterfaceDoDispositivo::getMacAddress() const {
    return macAddress;
}

void InterfaceDoDispositivo::setStatus(bool status) {
    this->status = status;
}

void InterfaceDoDispositivo::setInterfaceConectado(InterfaceDoDispositivo* interface) {
    this->interfaceConectada = interface;
}

InterfaceDoDispositivo* InterfaceDoDispositivo::getInterfaceConectado() const {
    return interfaceConectada;
}

void InterfaceDoDispositivo::encaminharPacote(Pacote* pacote) const {
    if (status && interfaceConectada) {
        interfaceConectada->receberPacote(pacote);
        std::cout << "Pacote encaminhado de " << pacote->getEnderecoIPOrigem() << " para " << pacote->getEnderecoIPDestino() << "." << std::endl;
    }
    else {
        std::cout << "Interface inativa ou nenhum dispositivo conectado." << std::endl;
    }
}
void InterfaceDoDispositivo::receberPacote(Pacote* pacote) const {
    if (!status) {
        std::cout << "Interface inativa. Pacote n?o pode ser recebido." << std::endl;
        return;
    }

    if (pacote->getEnderecoIPDestino() == this->ip) {
        dispositivoVinculado->receberPacote(pacote);
        std::cout << "Pacote recebido no dispositivo " << dispositivoVinculado->getNome()
                  << " com IP " << this->ip << "." << std::endl;
        return;
    }

    if (dynamic_cast<Switch*>(dispositivoVinculado) || dynamic_cast<Roteador*>(dispositivoVinculado)) {
        dispositivoVinculado->receberPacote(pacote);
        std::cout << "Pacote recebido no dispositivo " << dispositivoVinculado->getNome() << "." << std::endl;
        return;
    }

    std::cout << "Pacote descartado. Endere?o IP de destino n?o corresponde ao dispositivo vinculado." << std::endl;
}

// Novo
void InterfaceDoDispositivo::setIP(const std::string& novoIP) {
    this->ip = novoIP;
}
