#pragma once
#include <string>
#include <array>
#include "IDispositivo.h"
#include "InterfaceDoDispositivo.h"

class Roteador : public Dispositivo {
private:
    std::string nome;
    std::array<InterfaceDoDispositivo*, 5> interfaces;

public:
    Roteador(const std::string& nome, const std::string& ip, const std::string& macAddress);

    std::string getNome() const override;
    void setNome(const std::string& novoNome) override;
    std::array<InterfaceDoDispositivo*, 5> getInterfaces() const override;
    InterfaceDoDispositivo* getPorta(std::string& mac);
    void adicionarPorta(InterfaceDoDispositivo* novaPorta);

    bool receberPacote(Pacote* pacote) const override; // <= adicionado
};
