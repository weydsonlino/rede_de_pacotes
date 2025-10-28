#include "Switch.h"
#include "InterfaceDoDispositivo.h"

Switch::Switch(const std::string& nome)
    : Dispositivo("", ""),
    nome(nome),
    interfaces{ nullptr, nullptr, nullptr, nullptr, nullptr }
{
    std::cout << "[Switch] Criado switch " << nome
        << " com 5 portas." << std::endl;
}

InterfaceDoDispositivo* Switch::getPorta(std::string& mac)
{
    for (int i = 0; i < 5; ++i) {
        if (interfaces[i] != nullptr && interfaces[i]->getMacAddress() == mac) {
            return interfaces[i];
        }
    }
    return nullptr;
}

std::array<InterfaceDoDispositivo*, 5> Switch::getInterfaces() const
{
    return interfaces;
}

void Switch::setNome(const std::string& novoNome) {
    nome = novoNome;
}

void Switch::adicionarPorta(InterfaceDoDispositivo* novaPorta) {
    for (int i = 0; i < 5; ++i) {
        if (interfaces[i] == nullptr) {
            interfaces[i] = novaPorta;
            std::cout << "[Switch] Porta adicionada: " << novaPorta->getMacAddress() << std::endl;
            return;
        }
    }
    std::cout << "[Switch] Todas as portas estão ocupadas!" << std::endl;
}
