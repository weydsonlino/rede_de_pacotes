#include "mainwindow.h"
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QRandomGenerator>
#include <QMessageBox>
#include "backend/Computador.h"
#include "backend/Roteador.h"
#include "backend/Switch.h"
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QInputDialog>
#include <QPropertyAnimation>


// -------------------------
// NetworkView
// -------------------------
NetworkView::NetworkView(QWidget* parent) : QGraphicsView(parent) {
    setAcceptDrops(true);
    networkScene = new QGraphicsScene(this);
    setScene(networkScene);
    networkScene->setBackgroundBrush(Qt::white);
}


void NetworkView::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

void NetworkView::dragMoveEvent(QDragMoveEvent* event) {
    event->acceptProposedAction();
}

void NetworkView::dropEvent(QDropEvent* event) {
    QString typeQt = event->mimeData()->text(); // Qt precisa QString aqui
    std::string type = typeQt.toStdString();

    QPixmap pixmap;
    if (type == "PC") pixmap.load(":/icons/icons/pc.png");
    else if (type == "Switch") pixmap.load(":/icons/icons/switch.png");
    else if (type == "Router") pixmap.load(":/icons/icons/router.png");

    if (pixmap.isNull()) return;
    pixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // ---------- Obter dados do usuário ----------
    bool ok;
    std::string defaultName = type + "_" + std::to_string(QRandomGenerator::global()->bounded(1000));
    QString nomeQt = QInputDialog::getText(nullptr, "Nome do dispositivo",
                                           "Digite o nome do dispositivo:",
                                           QLineEdit::Normal,
                                           QString::fromStdString(defaultName), &ok);
    std::string nome = nomeQt.toStdString();
    if (!ok || nome.empty()) nome = defaultName;

    std::string ip;
    if (type != "Switch") {
        QString ipQt = QInputDialog::getText(nullptr, "IP do dispositivo",
                                             "Digite o IP (opcional):",
                                             QLineEdit::Normal, "", &ok);
        if (ok) ip = ipQt.toStdString();
    }

    std::string macAddress = generateRandomMAC();

    std::string rotaDefaultIP;
    std::string rotaDefaultMAC;
    if (type != "Switch") {
        QString rotaIPQt = QInputDialog::getText(nullptr, "Gateway padrão",
                                                 "Digite o IP do gateway (opcional):",
                                                 QLineEdit::Normal, "", &ok);
        if (ok) {
            rotaDefaultIP = rotaIPQt.toStdString();
            if (!rotaDefaultIP.empty())
                rotaDefaultMAC = buscarMacDoGateway(rotaDefaultIP);
        }
    }

    // ---------- Criar item gráfico ----------
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
    QString textQt = QString::fromStdString(nome);
    if (!ip.empty()) textQt += "\nIP: " + QString::fromStdString(ip);

    QGraphicsTextItem* textItem = new QGraphicsTextItem(textQt);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setFont(QFont("Arial", 10));
    textItem->setPos(0, pixmap.height() + 2);

    QGraphicsItemGroup* group = new QGraphicsItemGroup();
    group->setData(1, QVariant::fromValue((void*)group));
    group->setAcceptedMouseButtons(Qt::AllButtons);
    group->setAcceptHoverEvents(true);
    group->addToGroup(pixmapItem);
    group->addToGroup(textItem);
    group->setFlag(QGraphicsItem::ItemIsMovable);
    group->setFlag(QGraphicsItem::ItemIsSelectable);
    group->setPos(mapToScene(event->position().toPoint()));

    // ---------- Criar objeto back-end ----------
    Dispositivo* dispositivo = nullptr;
    if (type == "PC") {
        dispositivo = new Computador(nome, ip, rotaDefaultIP, rotaDefaultMAC, macAddress);
    } else if (type == "Switch") {
        dispositivo = new Switch(nome);
    } else if (type == "Router") {
        dispositivo = new Roteador(nome, ip, macAddress);
    }

    if (dispositivo) dispositivos.push_back(dispositivo);
    group->setData(0, QVariant::fromValue((void*)dispositivo));

    scene()->addItem(group);
    dispositivoToItem[dispositivo] = group;

    event->acceptProposedAction();
}

std::string NetworkView::buscarMacDoGateway(const std::string& rotaDefaultIP) {
    if (rotaDefaultIP.empty()) return "";

    for (Dispositivo* d : dispositivos) {
        Roteador* r = dynamic_cast<Roteador*>(d);
        if (!r) continue;

        auto interfaces = r->getInterfaces();
        for (InterfaceDoDispositivo* iface : interfaces) {
            if (!iface) continue;
            if (iface->getIP() == rotaDefaultIP)
                qDebug() << "[NetworkView] MAC do gateway encontrado para IP:" << QString::fromStdString(rotaDefaultIP);
                return iface->getMacAddress();
        }
    }

    qDebug() << "[NetworkView] MAC do gateway não encontrado para IP:" << QString::fromStdString(rotaDefaultIP);
    return "";
}

std::string NetworkView::generateRandomMAC() {
    std::string mac;
    for (int i = 0; i < 6; ++i) {
        int byte = QRandomGenerator::global()->bounded(256);
        char buf[3];
        sprintf(buf, "%02X", byte);
        mac += buf;
        if (i != 5) mac += ":";
    }
    return mac;
}

// -------------------------
// Clique do mouse
// -------------------------
void NetworkView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem* item = scene()->itemAt(scenePos, QTransform());
    if (!item) return;

    // Encontra o item com o data(0) que contém o dispositivo
    QGraphicsItem* current = item;
    while (current && !current->data(0).isValid())
        current = current->parentItem();
    if (!current || !current->data(0).isValid()) return;

    Dispositivo* dispositivoClicado = static_cast<Dispositivo*>(current->data(0).value<void*>());
    if (!dispositivoClicado) return;

    qDebug() << "Menu aberto para:" << QString::fromStdString(dispositivoClicado->getNome());

    // ---------------- MENU PRINCIPAL ----------------
    QMenu menu(this);
    QAction* alterarNomeAction = menu.addAction("Alterar nome do dispositivo");

    // Só para Computador
    if (Computador* pc = dynamic_cast<Computador*>(dispositivoClicado)) {
        QAction* alterarGatewayAction = menu.addAction("Alterar gateway");
        connect(alterarGatewayAction, &QAction::triggered, [this, pc]() {
            bool ok;
            QString novoGatewayIP = QInputDialog::getText(nullptr,
                                                          "Alterar gateway",
                                                          "Digite o IP do gateway:",
                                                          QLineEdit::Normal,
                                                          QString::fromStdString(pc->rotaDefaultIP),
                                                          &ok);
            if (ok && !novoGatewayIP.isEmpty()) {
                // Atualiza MAC do gateway automaticamente
                QString gatewayMAC = QString::fromStdString(buscarMacDoGateway(novoGatewayIP.toStdString()));

                pc->setGateway(novoGatewayIP.toStdString(), gatewayMAC.toStdString());
                qDebug() << "[NetworkView] Gateway alterado para:" << novoGatewayIP << "MAC:" << gatewayMAC;
            }
        });
    }




    QAction* removerAction = menu.addAction("Remover dispositivo");
    connect(removerAction, &QAction::triggered, [this, dispositivoClicado, current]() {
        // Remove do back-end
        auto it = std::find(dispositivos.begin(), dispositivos.end(), dispositivoClicado);
        if (it != dispositivos.end()) dispositivos.erase(it);

        // Remove do mapa
        dispositivoToItem.remove(dispositivoClicado);

        // Remove da cena
        scene()->removeItem(current);
        delete current;

        qDebug() << "[NetworkView] Dispositivo removido:" << QString::fromStdString(dispositivoClicado->getNome());
    });

    // Submenu de interfaces
    QMenu* interfacesMenu = menu.addMenu("Interfaces de rede");
    auto interfaces = dispositivoClicado->getInterfaces();
    int ifaceIndex = 0;
    for (auto* iface : interfaces)
    {
        if (!iface) { ifaceIndex++; continue; }

        QString status = iface->getStatus() ? "Ativa" : "Desligada";
        QString ipText = iface->getIP().empty() ? "Sem IP" : QString::fromStdString(iface->getIP());
        QString label = QString("Interface %1 (%2, %3)").arg(ifaceIndex + 1).arg(ipText).arg(status);

        QMenu* ifaceMenu = interfacesMenu->addMenu(label);

        // --- Alterar IP ---
        QAction* alterarIPAction = ifaceMenu->addAction("Alterar IP");
        connect(alterarIPAction, &QAction::triggered, [this, iface]() {
            bool ok;
            QString novoIP = QInputDialog::getText(this,
                                                   "Alterar IP",
                                                   "Digite o novo IP:",
                                                   QLineEdit::Normal,
                                                   QString::fromStdString(iface->getIP()),
                                                   &ok);
            if (ok && !novoIP.isEmpty()) iface->setIP(novoIP.toStdString());
        });

        // --- Conectar com outra interface ---
        QAction* conectarAction = ifaceMenu->addAction("Conectar com outra interface");
        connect(conectarAction, &QAction::triggered, [this, iface, dispositivoClicado]() {
            QMenu dispositivosMenu("Selecione o dispositivo");

            for (Dispositivo* outroDispositivo : dispositivos) {
                if (outroDispositivo == dispositivoClicado) continue;

                QAction* dispositivoAction = dispositivosMenu.addAction(QString::fromStdString(outroDispositivo->getNome()));
                connect(dispositivoAction, &QAction::triggered, [this, iface, dispositivoClicado, outroDispositivo]() {
                    QMenu ifaceMenu("Selecione a interface");
                    auto interfacesOutro = outroDispositivo->getInterfaces();
                    int idx = 0;

                    for (auto* outraIface : interfacesOutro) {
                        if (!outraIface) { idx++; continue; }

                        QString label = QString("Interface %1 (%2)")
                                            .arg(idx + 1)
                                            .arg(outraIface->getStatus() ? "Ativa" : "Desligada");
                        QAction* ifaceAction = ifaceMenu.addAction(label);
                        if (outraIface->getInterfaceConectado()) ifaceAction->setDisabled(true);

                        connect(ifaceAction, &QAction::triggered, [this, iface, outraIface, dispositivoClicado, outroDispositivo]() {
                            dispositivoClicado->conectarDispositivo(iface, outraIface);
                            qDebug() << "Interfaces conectadas: "
                                     << QString::fromStdString(iface->getIP())
                                     << "<->" << QString::fromStdString(outraIface->getIP());

                            // Desenha linha
                            if (!dispositivoToItem.contains(dispositivoClicado) || !dispositivoToItem.contains(outroDispositivo)) return;
                            QGraphicsItem* item1 = dispositivoToItem[dispositivoClicado];
                            QGraphicsItem* item2 = dispositivoToItem[outroDispositivo];

                            QGraphicsLineItem* linha = new QGraphicsLineItem(QLineF(item1->sceneBoundingRect().center(),
                                                                                    item2->sceneBoundingRect().center()));
                            linha->setPen(QPen(Qt::black, 2));
                            scene()->addItem(linha);
                            linhaParaDispositivos[linha] = qMakePair(dispositivoClicado, outroDispositivo);
                            linhasConectadas.append(linha);
                        });

                        idx++;
                    }

                    ifaceMenu.exec(QCursor::pos());
                });
            }

            dispositivosMenu.exec(QCursor::pos());
        });

        // --- Ativar/Desativar interface ---
        QString toggleLabel = iface->getStatus() ? "Desativar interface" : "Ativar interface";
        QAction* toggleStatusAction = ifaceMenu->addAction(toggleLabel);
        connect(toggleStatusAction, &QAction::triggered, [iface]() {
            iface->setStatus(!iface->getStatus());
        });

        // --- Enviar pacote ---
        QAction* enviarPacoteAction = ifaceMenu->addAction("Enviar pacote");
        connect(enviarPacoteAction, &QAction::triggered, [this, dispositivoClicado, iface]() {
            QMenu dispositivosMenu("Selecione o dispositivo destino");
            for (Dispositivo* outroDispositivo : dispositivos) {
                if (outroDispositivo == dispositivoClicado) continue;

                QAction* dispositivoAction = dispositivosMenu.addAction(QString::fromStdString(outroDispositivo->getNome()));
                connect(dispositivoAction, &QAction::triggered, [this, dispositivoClicado, iface, outroDispositivo]() {
                    QMenu ifaceDestinoMenu("Selecione a interface destino");
                    auto interfacesOutro = outroDispositivo->getInterfaces();
                    int idx2 = 0;

                    for (auto* outraIface : interfacesOutro) {
                        if (!outraIface) { idx2++; continue; }

                        QString label = QString("Interface %1 (%2)").arg(idx2 + 1).arg(outraIface->getStatus() ? "Ativa" : "Desligada");
                        QAction* ifaceDestAction = ifaceDestinoMenu.addAction(label);
                        if (!outraIface->getStatus()) ifaceDestAction->setDisabled(true);

                        connect(ifaceDestAction, &QAction::triggered, [dispositivoClicado, iface, outroDispositivo, outraIface]() {
                            dispositivoClicado->enviarPacote(iface, outraIface);
                            qDebug() << "Pacote enviado de "
                                     << QString::fromStdString(iface->getIP())
                                     << " para "
                                     << QString::fromStdString(outraIface->getIP());
                        });
                        idx2++;
                    }
                    ifaceDestinoMenu.exec(QCursor::pos());
                });
            }
            dispositivosMenu.exec(QCursor::pos());
        });

        ifaceIndex++;
    }


    // --- Adicionar nova interface (fora do loop) ---
    QAction* adicionarIfaceAction = interfacesMenu->addAction("Adicionar nova interface");
    connect(adicionarIfaceAction, &QAction::triggered, [this, dispositivoClicado]() {
        // Pergunta IP (somente se não for switch)
        std::string ip = "";
        if (!dynamic_cast<Switch*>(dispositivoClicado)) {
            bool ok;
            QString ipQt = QInputDialog::getText(this,
                                                 "Nova Interface",
                                                 "Digite o IP da nova interface:",
                                                 QLineEdit::Normal,
                                                 "", &ok);
            if (ok) ip = ipQt.toStdString();
        }

        // MAC sempre gerado automaticamente
        std::string mac = generateRandomMAC();

        // Cria a interface
        InterfaceDoDispositivo* novaIface = new InterfaceDoDispositivo(ip, mac, dispositivoClicado);

        // Adiciona usando o método correto
        if (Switch* sw = dynamic_cast<Switch*>(dispositivoClicado)) {
            sw->adicionarPorta(novaIface);
        } else if (Roteador* r = dynamic_cast<Roteador*>(dispositivoClicado)) {
            r->adicionarPorta(novaIface);
        } else {
            qDebug() << "[NetworkView] Dispositivo não suporta múltiplas interfaces.";
            delete novaIface;
            return;
        }

        qDebug() << "Nova interface adicionada no dispositivo: "
                 << QString::fromStdString(dispositivoClicado->getNome())
                 << " IP: " << QString::fromStdString(ip)
                 << " MAC: " << QString::fromStdString(mac);
    });


    menu.exec(event->globalPos());
}


// -------------------------
// DeviceButton
// -------------------------
DeviceButton::DeviceButton(const std::string& type, const QIcon& icon, QWidget* parent)
    : QPushButton(icon, "", parent), deviceType(type) {
    setIconSize(QSize(48,48));
    setFixedSize(64,64);
}

void DeviceButton::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(QString::fromStdString(deviceType));
    drag->setMimeData(mimeData);
    drag->setPixmap(icon().pixmap(iconSize()));
    drag->exec(Qt::CopyAction);
}

// -------------------------
// MainWindow
// -------------------------
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUI();
    populateTabs();
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    setCentralWidget(central);

    networkView = new NetworkView();
    networkScene = networkView->scene();
    networkView->setMinimumSize(800, 500);
    mainLayout->addWidget(networkView, 1);

    bottomTabs = new QTabWidget();
    bottomTabs->setTabPosition(QTabWidget::South);

    pcTab = new QWidget();
    switchTab = new QWidget();
    routerTab = new QWidget();

    pcLayout = new QHBoxLayout(pcTab);
    switchLayout = new QHBoxLayout(switchTab);
    routerLayout = new QHBoxLayout(routerTab);

    bottomTabs->addTab(pcTab, "Computadores");
    bottomTabs->addTab(switchTab, "Switches");
    bottomTabs->addTab(routerTab, "Roteadores");

    mainLayout->addWidget(bottomTabs);
}

void MainWindow::populateTabs() {
    QIcon pcIcon(":/icons/icons/pc.png");
    QIcon swIcon(":/icons/icons/switch.png");
    QIcon rtIcon(":/icons/icons/router.png");

    for (int i = 0; i < 3; ++i) pcLayout->addWidget(new DeviceButton("PC", pcIcon));
    for (int i = 0; i < 2; ++i) switchLayout->addWidget(new DeviceButton("Switch", swIcon));
    for (int i = 0; i < 2; ++i) routerLayout->addWidget(new DeviceButton("Router", rtIcon));
}
