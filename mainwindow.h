#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <vector>
#include <string>
#include <QMap>
#include <QGraphicsItemGroup>
#include "backend/IDispositivo.h"

class NetworkView : public QGraphicsView {
    Q_OBJECT
public:
    explicit NetworkView(QWidget* parent = nullptr);

    void addDispositivo(Dispositivo* d, QGraphicsItem* item);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QGraphicsScene* networkScene;
    std::vector<Dispositivo*> dispositivos;
    QMap<Dispositivo*, QGraphicsItem*> dispositivoToItem;
    QList<QGraphicsLineItem*> linhasConectadas;
    QMap<QGraphicsLineItem*, QPair<Dispositivo*, Dispositivo*>> linhaParaDispositivos;

    std::string buscarMacDoGateway(const std::string& rotaDefaultIP);
    std::string generateRandomMAC();
};

class DeviceButton : public QPushButton {
    Q_OBJECT
public:
    explicit DeviceButton(const std::string& type, const QIcon& icon, QWidget* parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    std::string deviceType;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    NetworkView* networkView;
    QGraphicsScene* networkScene;
    QTabWidget* bottomTabs;

    QWidget* pcTab;
    QWidget* switchTab;
    QWidget* routerTab;

    QHBoxLayout* pcLayout;
    QHBoxLayout* switchLayout;
    QHBoxLayout* routerLayout;

    void setupUI();
    void populateTabs();
};
