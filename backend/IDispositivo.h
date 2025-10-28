#pragma once
#include <string>
#include "Pacote.h"
#include "InterfaceDoDispositivo.h"
#include <iostream>
#include <sstream>
#include <array>

class InterfaceDoDispositivo;
class Pacote;

class Dispositivo {

public:
    std::string rotaDefaultIP;
    std::string rotaDefaultMAC;

    Dispositivo(const std::string& rotaDefaultIP,
        const std::string& rotaDefaultMAC)
        : rotaDefaultIP(rotaDefaultIP),
        rotaDefaultMAC(rotaDefaultMAC) {
    }


    // M?todos que cada dispositivo implementa
    virtual std::string getNome() const = 0;
    virtual void setNome(const std::string& novoNome) = 0;
    virtual std::array<InterfaceDoDispositivo*, 5> getInterfaces() const = 0;


    // M?todos comuns (implementa??o base)

    virtual void conectarDispositivo(InterfaceDoDispositivo* interface_1, InterfaceDoDispositivo* interface_2) {
        if (!interface_1 || !interface_2) {
            std::cerr << "Erro: ponteiro de interface nulo ao conectar dispositivos." << std::endl;
            return;
        }
        interface_1->setInterfaceConectado(interface_2);
        interface_2->setInterfaceConectado(interface_1);
        std::cout << "Interfaces conectadas com sucesso." << std::endl;
    }

    virtual bool receberPacote(Pacote* pacote) const {
        if (!pacote) {
            std::cerr << "Erro: pacote nulo." << std::endl;
            return false;
        }

        std::cout << "[DEBUG] Pacote recebido no dispositivo " << getNome() << ":" << std::endl;
        pacote->exibirCabecalhos();

        auto interfaces = this->getInterfaces();

        // Se o IP de destino ? deste dispositivo, aceita
        for (auto* iface : interfaces) {
            if (iface && iface->getIP() == pacote->getEnderecoIPDestino()) {
                std::cout << "Pacote destinado a este dispositivo (iface " << iface->getMacAddress() << ")." << std::endl;
                return true;
            }
        }

        const std::string destMac = pacote->getEnderecoMACDestino();
        std::cout << "Pacote n?o destinado a este dispositivo. Tentando encaminhar..." << std::endl;

        for (auto* iface : interfaces) {
            if (!iface || !iface->getStatus()) continue;

            auto* peer = iface->getInterfaceConectado();
            if (!peer) continue;

            std::cout << "[DEBUG] vizinho=" << peer->getMacAddress()
                        << " destino=" << destMac << std::endl;

            if (peer->getMacAddress() == destMac) {
                std::cout << "Encaminhando pela interface (" << iface->getMacAddress() << ")." << std::endl;
                iface->encaminharPacote(pacote);
                return true;
            }
        }

        std::cout << "Nenhuma interface conectada corresponde ao MAC de destino do pacote." << std::endl;

        return false;

    }
    virtual void enviarPacote(InterfaceDoDispositivo* interface_1, InterfaceDoDispositivo* interface_2) const {
        if (interface_1->getStatus() && interface_2->getStatus()) {
            std::cout << "[DEBUG] Enviar pacote de "
                << interface_1->getIP() << " para "
                << interface_2->getIP() << std::endl;
            if (checarRede(interface_1, interface_2)) {
                interface_1->encaminharPacote(new Pacote(interface_1->getMacAddress(), interface_2->getMacAddress(), interface_1->getIP(), interface_2->getIP()));
                std::cout << "Enviando o Ping" << std::endl;
            }
            else {
                auto rotaDefaultMAC = interface_1->dispositivoVinculado->rotaDefaultMAC;
                if (rotaDefaultMAC.empty()) {
                    std::cout << "Falha no ping: Nenhuma rota padr?o configurada." << std::endl;
                }
                else {
                    interface_1->encaminharPacote(new Pacote(interface_1->getMacAddress(), rotaDefaultMAC, interface_1->getIP(), interface_2->getIP()));
                };
            }
        }
        else {
            std::cout << "Voc? n?o est? conectado a nenhuma rede." << std::endl;
        }
    }
    virtual bool checarRede(InterfaceDoDispositivo* interface_1, InterfaceDoDispositivo* interface_2) const {
        std::array<std::string, 4> partes1;
        std::array<std::string, 4> partes2;

        std::stringstream ss1(interface_2->getIP());
        std::stringstream ss2(interface_1->getIP());
        std::string parte;

        int i = 0, j = 0;
        while (std::getline(ss1, parte, '.') && i < 4) {
            partes1[i++] = parte;
        }

        while (std::getline(ss2, parte, '.') && j < 4) {
            partes2[j++] = parte;
        }

        if (i != 4 || j != 4) {
            std::cerr << "IP inv?lido!" << std::endl;
            return false;
        }

        for (int k = 0; k < 3; ++k) {
            if (partes1[k] != partes2[k]) {
                std::cerr << "Octeto diferente" << std::endl;
                return false;
            }
            std::cerr << "Check" << std::endl;
        }

        return true;
    }

};
