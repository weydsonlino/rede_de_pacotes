#pragma once
#include "IDispositivo.h"
#include "InterfaceDoDispositivo.h"

class Switch : public Dispositivo {
private:
    std::string nome;
    std::array<InterfaceDoDispositivo*, 5> interfaces;

public:
    explicit Switch(const std::string& nome);

    std::string getNome() const override { return nome; };
    void setNome(const std::string& novoNome) override;

    void adicionarPorta(InterfaceDoDispositivo* novaPorta);
    InterfaceDoDispositivo* getPorta(std::string& mac);
    std::array<InterfaceDoDispositivo*, 5> getInterfaces() const override;
};
