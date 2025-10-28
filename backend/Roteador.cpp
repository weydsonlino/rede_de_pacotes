#include "Roteador.h"
#include <iostream>
#include "InterfaceDoDispositivo.h"

Roteador::Roteador(const std::string& nome, const std::string& ip, const std::string& macAddress)
    : Dispositivo("", ""),
    nome(nome),
    interfaces{ nullptr, nullptr, nullptr, nullptr, nullptr }
{
    interfaces[0] = new InterfaceDoDispositivo(ip, macAddress, this);
}

std::string Roteador::getNome() const {
    return nome;
}

void Roteador::setNome(const std::string& novoNome) {
    nome = novoNome;
}

std::array<InterfaceDoDispositivo*, 5> Roteador::getInterfaces() const
{
    return interfaces;
}

void Roteador::adicionarPorta(InterfaceDoDispositivo* novaPorta) {
    for (int i = 0; i < 5; ++i) {
        if (interfaces[i] == nullptr) {
            interfaces[i] = novaPorta;
            std::cout << "[Roteador] Porta adicionada: " << novaPorta->getMacAddress() << std::endl;
            return;
        }
    }
    std::cout << "[Roteador] Todas as portas estão ocupadas!" << std::endl;
}

InterfaceDoDispositivo* Roteador::getPorta(std::string& mac)
{
    for (int i = 0; i < 5; ++i) {
        if (interfaces[i] != nullptr && interfaces[i]->getMacAddress() == mac) {
            return interfaces[i];
        }
    }
    return nullptr;
}

bool Roteador::receberPacote(Pacote* pacote) const {
    if (!pacote) {
        std::cerr << "Erro: pacote nulo." << std::endl;
        return false;
    }

    const std::string dstIP  = pacote->getEnderecoIPDestino();
    const std::string dstMac = pacote->getEnderecoMACDestino();

    std::cout << "[DEBUG][Roteador " << getNome() << "] Pacote recebido:" << std::endl;
    pacote->exibirCabecalhos();

    auto ifaces = getInterfaces();

    bool ownsDstIP  = false;
    bool ownsDstMAC = false;

    for (auto* iface : ifaces) {
        if (!iface || !iface->getStatus()) continue;
        if (iface->getIP() == dstIP)        ownsDstIP  = true;
        if (iface->getMacAddress() == dstMac) ownsDstMAC = true;
    }

    // Regra 1: possui IP e MAC de destino -> aceita
    if (ownsDstIP && ownsDstMAC) {
        std::cout << "[Roteador] Pacote destinado ao próprio roteador. Aceito." << std::endl;
        return true;
    }

    // A partir daqui, só roteia se o quadro veio para um MAC do roteador
    if (!ownsDstMAC) {
        std::cout << "[Roteador] Quadro não endereçado a este roteador (MAC destino desconhecido). Descartado." << std::endl;
        return false;
    }

    // Regra 2: possui só o MAC -> encaminhar para a interface cujo vizinho tem o IP de destino
    for (auto* iface : ifaces) {
        if (!iface || !iface->getStatus()) continue;
        auto* peer = iface->getInterfaceConectado();
        if (!peer) continue;

        if (peer->getIP() == dstIP) {
            std::cout << "[Roteador] Encaminhando para a interface " << iface->getMacAddress()
                      << " (vizinho com IP " << dstIP << ")." << std::endl;
            iface->encaminharPacote(pacote);
            return true;
        }
    }

    // Regra 3: não há vizinho direto com o IP de destino -> flood para todas as portas ativas
    std::cout << "[Roteador] Nenhum vizinho com IP " << dstIP << ". Encaminhando para todas as interfaces ativas." << std::endl;
    bool enviou = false;
    for (auto* iface : ifaces) {
        if (!iface || !iface->getStatus()) continue;
        if (iface->getInterfaceConectado() == nullptr) continue;
        iface->encaminharPacote(pacote);
        enviou = true;
    }
    if (!enviou) {
        std::cout << "[Roteador] Nenhuma interface apta para encaminhar." << std::endl;
        return false;
    }

    return true;
}
