#include "view.h"
#include "mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include <qdockwidget.h>
#include <QGraphicsSceneMouseEvent>
#include "ship.h"
#include <time.h>

View::View()
{

}

View::View(QGraphicsScene *scene)
{
    setScene(scene);
    this->scene()->setBackgroundBrush(QBrush(QColor(230,230,230)));
}

void View::CreateGrid(int startX, int startY)
{
    this->scene()->addRect(startX,startY,sirkaGrid,sirkaGrid,QPen(Qt::black),QBrush(QColor(230,230,230)));

    for(int i = 1;i < 10;++i){
        this->scene()->addLine(startX + (sirkaGrid / 10 * i) - 1,startY,startX + (sirkaGrid / 10 * i) - 1,sirkaGrid,QPen(Qt::black));
    }

    for(int i = 1;i < 10;++i){
        this->scene()->addLine(startX,startY + (sirkaGrid / 10 * i) - 1,startX + sirkaGrid,startY + (sirkaGrid / 10 * i) - 1,QPen(Qt::black));
    }
}

void View::StartNewGame()
{
//    MainWindow::LogWriter();//nefunguje
//    latestConsoleText = "Started new game, now with console output";//nefunguje
    qDebug() << "Started new game";

    this->scene()->clear();
    CreateGrid(0,0);
    CreateGrid(sirkaGrid + mezeraGrid,0);

    setMouseTracking(true);

    beforeStart = false;
    building = true;
    endOfGame = false;

    PrepareListShip();//vynulovani vsech seznamu

    ship1x1 = new ship("ship1x1",Qt::red,mezeraGrid,mezeraGrid);
    this->scene()->addItem(ship1x1);
    ship1x1->setPos(sirkaGrid,0);

    ship2x1 = new ship("ship2x1",Qt::red,mezeraGrid,mezeraGrid * 2);
    this->scene()->addItem(ship2x1);
    ship2x1->setPos(sirkaGrid,50);

    ship3x1 = new ship("ship3x1",Qt::red,mezeraGrid,mezeraGrid * 3);
    this->scene()->addItem(ship3x1);
    ship3x1->setPos(sirkaGrid,150);

    ship4x1 = new ship("ship4x1",Qt::red,mezeraGrid,mezeraGrid * 4);
    this->scene()->addItem(ship4x1);
    ship4x1->setPos(sirkaGrid,300);
}

void View::UpdateHoverShipPublicSlots()
{
    UpdateHoverShip(currentMouse, shipEquiped, rotation);
}

void View::StartGame(QString enemyInfo)
{
    qDebug() << "Starting game with oponent " << enemyInfo;
    this->scene()->addRect(sirkaGrid,0,mezeraGrid,sirkaGrid,QPen(Qt::black),QBrush(QColor(230,230,230)));
    building = false;
    holdingShip = false;
    playerTurn = true;
    beforeStart = false;
    if(enemyInfo == "localComputer"){
        LocalComputerBuilShips();
    }
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if(event->pos().x() > (sirkaGrid + mezeraGrid)){
        currentMouse = QPoint((qFloor(event->pos().x()/50.0)) - 11,qFloor(event->pos().y())/50.0);
        gridId = 1;
    }
    else if(event->pos().x() < sirkaGrid){
        currentMouse = QPoint(qFloor(event->pos().x()/50.0),qFloor(event->pos().y())/50.0);
        gridId = 0;
        if(holdingShip){
            UpdateHoverShip(currentMouse, shipEquiped, rotation);
        }
    }
    else{
        currentMouse = QPoint(qFloor(event->pos().x()/50.0) - 10,qFloor(event->pos().y())/50.0);
        gridId = 2;
    }
    currentMouseLong = event->pos();
}

void View::mousePressEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        qDebug() << "Left mouse button was pressed";
        LeftMouseClicked();
    }
    if(event->buttons() == Qt::RightButton){
        qDebug() << "Right mouse button was pressed";
    }
    if(event->buttons() == Qt::MiddleButton){
        qDebug() << "Middle mouse button was pressed";
    }
}

void View::mouseWheelEvent(/*QWheelEvent*/QGraphicsSceneWheelEvent *event)//nefunguje
{
    if(event->delta() != wheelRotation){
        wheelRotation = event->delta();
        qDebug() << "Mouse wheel moved";
        if(rotation == 1){
            rotation = 2;
            qDebug() << "Rotation changed to vertical";
        }
        else{
            rotation = 1;
            qDebug() << "Rotation changed to horizontal";
        }
        UpdateHoverShip(currentMouse, shipEquiped, rotation);
    }
}

void View::PlaceMyShipWhenBuilding()
{
    if(shipEquiped == 1 && pocetShip1x1 != 0){
        if(CanPlaceThereWhenBuilding(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
            if(rotation == 1){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 1;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship1x1 = new ship("ship1x1",Qt::red,mezeraGrid,mezeraGrid);
                this->scene()->addItem(ship1x1);
                ship1x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip1x1--;
                qDebug() << "Placing ship1x1 verticaly at grid " << currentMouse;
            }
            else if(rotation == 2){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 5;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship1x1 = new ship("ship1x1",Qt::red,mezeraGrid,mezeraGrid);
                this->scene()->addItem(ship1x1);
                ship1x1->setRotation(90);
                ship1x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip1x1--;
                qDebug() << "Placing ship1x1 horizontaly at grid " << currentMouse;
            }
            else{
                qDebug() << "Rotation was not set";
            }
        }
        else{
            qDebug() << "Here you can not place that";
        }
    }
    else if(shipEquiped == 2 && pocetShip2x1 != 0){
        if(CanPlaceThereWhenBuilding(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
            if(rotation == 1){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 2;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship2x1 = new ship("ship2x1",Qt::red,mezeraGrid,mezeraGrid * 2);
                this->scene()->addItem(ship2x1);
                ship2x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip2x1--;
                qDebug() << "Placing ship2x1 verticaly at grid " << currentMouse;
            }
            else if(rotation == 2){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 6;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship2x1 = new ship("ship2x1",Qt::red,mezeraGrid,mezeraGrid * 2);
                this->scene()->addItem(ship2x1);
                ship2x1->setRotation(90);
                ship2x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip2x1--;
                qDebug() << "Placing ship2x1 horizontaly at grid " << currentMouse;
            }
            else{
                qDebug() << "Rotation was not set";
            }
        }
        else{
            qDebug() << "Here you can not place that";
        }
    }
    else if(shipEquiped == 3 && pocetShip3x1 != 0){
        if(CanPlaceThereWhenBuilding(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
            if(rotation == 1){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 3;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship3x1 = new ship("ship3x1",Qt::red,mezeraGrid,mezeraGrid * 3);
                this->scene()->addItem(ship3x1);
                ship3x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip3x1--;
                qDebug() << "Placing ship3x1 verticaly at grid " << currentMouse;
            }
            else if(rotation == 2){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 7;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship3x1 = new ship("ship3x1",Qt::red,mezeraGrid,mezeraGrid * 3);
                this->scene()->addItem(ship3x1);
                ship3x1->setRotation(90);
                ship3x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip3x1--;
                qDebug() << "Placing ship3x1 horizontaly at grid " << currentMouse;
            }
            else{
                qDebug() << "Rotation was not set";
            }
        }
        else{
            qDebug() << "Here you can not place that";
        }
    }
    else if(shipEquiped == 4 && pocetShip4x1 != 0){
        if(CanPlaceThereWhenBuilding(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
            if(rotation == 1){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 4;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship4x1 = new ship("ship4x1",Qt::red,mezeraGrid,mezeraGrid * 4);
                this->scene()->addItem(ship4x1);
                ship4x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip4x1--;
                qDebug() << "Placing ship4x1 verticaly at grid " << currentMouse;
            }
            else if(rotation == 2){
                listGridPlayer[currentMouse.x()][currentMouse.y()] = 8;
                FillListShip(currentMouse, shipEquiped, rotation);
                ship4x1 = new ship("ship4x1",Qt::red,mezeraGrid,mezeraGrid * 4);
                this->scene()->addItem(ship4x1);
                ship4x1->setRotation(90);
                ship4x1->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
                pocetShip4x1--;
                qDebug() << "Placing ship4x1 horizontaly at grid " << currentMouse;
            }
            else{
                qDebug() << "Rotation was not set";
            }
        }
        else{
            qDebug() << "Here you can not place that";
        }
    }
    else{
        RemoveHoverShip(shipEquiped);
        qDebug() << "You do not have another ships to paste";
    }
}

void View::LeftMouseClicked()
{
    if(!beforeStart && !endOfGame){
        if(gridId == 2 && building){
            if(currentMouse.y() == 0 && pocetShip1x1 != 0){
                RemoveHoverShip(shipEquiped);
                shipEquiped = 1;
                holdingShip = true;
                qDebug() << "Equiped ship1x1";
            }
            else if((currentMouse.y() == 1 || currentMouse.y() == 2) && pocetShip2x1 != 0){
                RemoveHoverShip(shipEquiped);
                shipEquiped = 2;
                holdingShip = true;
                qDebug() << "Equiped ship2x1";
            }
            else if((currentMouse.y() == 3 || currentMouse.y() == 4 || currentMouse.y() == 5) && pocetShip3x1 != 0){
                RemoveHoverShip(shipEquiped);
                shipEquiped = 3;
                holdingShip = true;
                qDebug() << "Equiped ship3x1";
            }
            else if((currentMouse.y() == 6 || currentMouse.y() == 7 || currentMouse.y() == 8 || currentMouse.y() == 9) && pocetShip4x1 != 0){
                RemoveHoverShip(shipEquiped);
                shipEquiped = 4;
                holdingShip = true;
                qDebug() << "Equiped ship4x1";
            }
            else{
                shipEquiped = -1;
                holdingShip = false;
                qDebug() << "Equiping nothing left";
            }
            DisplayHoverShip(currentMouse, shipEquiped, rotation);
        }
        if(gridId == 0 && building){
            PlaceMyShipWhenBuilding();
        }
        if(gridId == 1 && !building){
            if(playerTurn){
                qDebug() << "Trying to fire";
                if(listFireEnemy[currentMouse.x()][currentMouse.y()] == 0){//pokud se na toto policko jeste nestrilelo
                    PlayerFireAtGrid(currentMouse);
                }
                else{
                    qDebug() << "Player fired again at " << currentMouse;
                }
            }
            else{
                qDebug() << "It is not your turn now";
            }
        }
    }
}

bool View::CanPlaceThereWhenBuilding(int startX, int startY, int shipEquiped, int rotation)
{
    if(listShipPlayer[startX][startY] != 0){
        return false;
    }
    else if(shipEquiped == 1 && rotation == 1 && listShipPlayer[startX][startY] == 0){
            return true;
    }
    else if(shipEquiped == 1 && rotation == 2 && listShipPlayer[startX][startY] == 0){
            return true;
    }
    else if(shipEquiped == 2 && rotation == 1 && startY < 9){
        if(listShipPlayer[startX][startY + 1] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else if(shipEquiped == 3 && rotation == 1 && startY < 8){
        if(listShipPlayer[startX][startY + 1] != 0 || listShipPlayer[startX][startY + 2] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else if(shipEquiped == 4 && rotation == 1 && startY < 7){
        if(listShipPlayer[startX][startY + 1] != 0 || listShipPlayer[startX][startY + 2] != 0 || listShipPlayer[startX][startY + 3] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else if(shipEquiped == 2 && rotation == 2 && startX > 0){
        if(listShipPlayer[startX - 1][startY] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else if(shipEquiped == 3 && rotation == 2 && startX > 1){
        if(listShipPlayer[startX - 1][startY] != 0 || listShipPlayer[startX - 2][startY] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else if(shipEquiped == 4 && rotation == 2 && startX > 2){
        if(listShipPlayer[startX - 1][startY] != 0 || listShipPlayer[startX - 2][startY] != 0 || listShipPlayer[startX - 3][startY] != 0){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
}

void View::DisplayHoverShip(QPoint currentMouse, int shipEquiped, int rotation)
{
    if(shipEquiped == 1 && rotation == 1){
        ship1x1hover = new ship("ship1x1hover",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(ship1x1hover);
        ship1x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship1x1 hover item";
    }
    else if(shipEquiped == 1 && rotation == 2){
        ship1x1hover = new ship("ship1x1hover",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(ship1x1hover);
        ship1x1hover->setRotation(90);
        ship1x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship1x1 hover item";
    }
    else if(shipEquiped == 2 && rotation == 1){
        ship2x1hover = new ship("ship2x1hover",Qt::red,mezeraGrid,mezeraGrid * 2);
        this->scene()->addItem(ship2x1hover);
        ship2x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship2x1 hover item";
    }
    else if(shipEquiped == 2 && rotation == 2){
        ship2x1hover = new ship("ship2x1hover",Qt::red,mezeraGrid,mezeraGrid * 2);
        this->scene()->addItem(ship2x1hover);
        ship2x1hover->setRotation(90);
        ship2x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship2x1 hover item";
    }
    else if(shipEquiped == 3 && rotation == 1){
        ship3x1hover = new ship("ship3x1hover",Qt::red,mezeraGrid,mezeraGrid * 3);
        this->scene()->addItem(ship3x1hover);
        ship3x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship3x1 hover item";
    }
    else if(shipEquiped == 3 && rotation == 2){
        ship3x1hover = new ship("ship3x1hover",Qt::red,mezeraGrid,mezeraGrid * 3);
        this->scene()->addItem(ship3x1hover);
        ship3x1hover->setRotation(90);
        ship3x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship3x1 hover item";
    }
    else if(shipEquiped == 4 && rotation == 1){
        ship4x1hover = new ship("ship4x1hover",Qt::red,mezeraGrid,mezeraGrid * 4);
        this->scene()->addItem(ship4x1hover);
        ship4x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship4x1 hover item";
    }
    else if(shipEquiped == 4 && rotation == 2){
        ship4x1hover = new ship("ship4x1hover",Qt::red,mezeraGrid,mezeraGrid * 4);
        this->scene()->addItem(ship4x1hover);
        ship4x1hover->setRotation(90);
        ship4x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
        qDebug() << "Added ship4x1 hover item";
    }
    else{
        qDebug() << "Could not add hover for ship with this parameters";
    }
}

void View::UpdateHoverShip(QPoint currentMouse, int shipEquiped, int rotation)
{
    if(CheckShipCount(shipEquiped)){
        shipEquiped = View::shipEquiped;
        if(shipEquiped == 1 && rotation == 1){
            ship1x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship1x1hover->setRotation(0);
        }
        else if(shipEquiped == 2 && rotation == 1){
            ship2x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship2x1hover->setRotation(0);
        }
        else if(shipEquiped == 3 && rotation == 1){
            ship3x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship3x1hover->setRotation(0);
        }
        else if(shipEquiped == 4 && rotation == 1){
            ship4x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship4x1hover->setRotation(0);
        }
        else if(shipEquiped == 1 && rotation == 2){
            ship1x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship1x1hover->setRotation(90);
        }
        else if(shipEquiped == 2 && rotation == 2){
            ship2x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship2x1hover->setRotation(90);
        }
        else if(shipEquiped == 3 && rotation == 2){
            ship3x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship3x1hover->setRotation(90);
        }
        else if(shipEquiped == 4 && rotation == 2){
            ship4x1hover->setPos(currentMouse.x() * (sirkaGrid / 10),currentMouse.y() * (sirkaGrid / 10));
            ship4x1hover->setRotation(90);
        }
        else{
            qDebug() << "Could not update hover of 'undefinied' ship";
        }
    }
    else{
        qDebug() << "You do not have more ships";
    }
}

bool View::CheckShipCount(int shipEquiped)
{
    if(pocetShip1x1 == 0 && pocetShip2x1 == 0 && pocetShip3x1 == 0 && pocetShip4x1 == 0){
        holdingShip = false;
        return false;
    }
    else if(shipEquiped == 1){
        if(pocetShip1x1 != 0){
            return true;
        }
        else{
            View::shipEquiped = 2;
            qDebug() << "Changing equiped ship";
            DisplayHoverShip(currentMouse, View::shipEquiped, rotation);
        }
    }
    else if(shipEquiped == 2){
        if(pocetShip2x1 != 0){
            return true;
        }
        else{
            View::shipEquiped = 3;
            qDebug() << "Changing equiped ship";
            DisplayHoverShip(currentMouse, View::shipEquiped, rotation);
        }
    }
    else if(shipEquiped == 3){
        if(pocetShip3x1 != 0){
            return true;
        }
        else{
            View::shipEquiped = 4;
            qDebug() << "Changing equiped ship";
            DisplayHoverShip(currentMouse, View::shipEquiped, rotation);
        }
    }
    else if(shipEquiped == 4){
        if(pocetShip4x1 != 0){
            return true;
        }
        else{
            View::shipEquiped = 1;
            qDebug() << "Changing equiped ship";
            DisplayHoverShip(currentMouse, View::shipEquiped, rotation);
        }
    }
    else{
        return false;
    }
}

void View::RemoveHoverShip(int shipEquiped)
{
    holdingShip = false;
    if(shipEquiped == 1){
        this->scene()->removeItem(ship1x1hover);
        qDebug() << "Ship hover removed";
    }
    else if(shipEquiped == 2){
        this->scene()->removeItem(ship2x1hover);
        qDebug() << "Ship hover removed";
    }
    else if(shipEquiped == 3){
        this->scene()->removeItem(ship3x1hover);
        qDebug() << "Ship hover removed";
    }
    else if(shipEquiped == 4){
        this->scene()->removeItem(ship4x1hover);
        qDebug() << "Ship hover removed";
    }
    else{
        qDebug() << "Could not remove hover of 'undefinied' ship";
    }
}

void View::CreateGameSaveFile()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    QString dir = fileDialog.getExistingDirectory();
    QFile file(dir + "/save" + saveIndex + ".ship");
    qDebug() << "Trying to save file";
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    //zpracovani souboru zde
}

void View::ReadGameSaveFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    QFile file(fileName);
    qDebug() << "Trying to read file";
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    //zpracovani souboru zde
}

void View::FillListShip(QPoint currentMouse, int shipEquiped, int rotation)
{
    if(shipEquiped == 1 && rotation == 2){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
    }
    else if(shipEquiped == 1 && rotation == 1){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
    }
    else if(shipEquiped == 2 && rotation == 2){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() + 1] = 2;
    }
    else if(shipEquiped == 2 && rotation == 1){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 2] = 2;
    }
    else if(shipEquiped == 3 && rotation == 2){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 2][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 3][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 3][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 3][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
    }
    else if(shipEquiped == 3 && rotation == 1){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 2] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 3] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 3] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 3] = 2;
    }
    else if(shipEquiped == 4 && rotation == 2){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 2][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 3][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x() - 4][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 3][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 4][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 4][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 3][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 2][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
    }
    else if(shipEquiped == 4 && rotation == 1){
        listShipPlayer[currentMouse.x()][currentMouse.y()] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 1] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 2] = 1;//lod
        listShipPlayer[currentMouse.x()][currentMouse.y() + 3] = 1;//lod
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 3] = 2;
        listShipPlayer[currentMouse.x() + 1][currentMouse.y() + 4] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x()][currentMouse.y() + 4] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() - 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y()] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 1] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 2] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 3] = 2;
        listShipPlayer[currentMouse.x() - 1][currentMouse.y() + 4] = 2;
    }
    else{
        qDebug() << "Could not set value in list 'listShipPlayer' from this parameters";
    }
}

void View::PrepareListShip()
{
    qDebug() << "Preparing 'listShipPlayer' and 'listShipEnemy' and 'listGridPlayer' and 'listGridEnemy' for use";
    for(int i = 0;i < 10;++i){
        for(int j = 0;j < 10;++j){
            listShipPlayer[i][j] = 0;
            listShipEnemy[i][j] = 0;
            listGridPlayer[i][j] = 0;
            listGridEnemy[i][j] = 0;
            listFirePlayer[i][j] = 0;
            listFireEnemy[i][j] = 0;
        }
    }
    pocetShip1x1 = 4;
    pocetShip2x1 = 3;
    pocetShip3x1 = 2;
    pocetShip4x1 = 1;
    shipEquiped = 0;
    rotation = 1;
    holdingShip = false;
    soucetShipPolicek = 20;
    LocalComputerLastPlayedX = 0;
    LocalComputerLastPlayedY = 0;
    LocalComputerLastPlayedGridType = 0;
    countSuccesfulFireEnemy = 0;
    countSuccesfulFirePlayer = 0;
    randomIntX = 0;
    randomIntY = 0;
    randomDirection = 0;
}

void View::PlayerPlayTurn()
{
    qDebug() << "Player playing turn";
}

void View::EnemyIsOnTurn()
{
    if(enemyInfo == "localComputer"){
        LocalComputerPlayTurn();
    }//sem dopsat dalsi druhy protihracu
    else{
        qDebug() << "Enemy is not specified corectly";
    }
}

bool View::CheckGameWin()
{
    if(soucetShipPolicek == countSuccesfulFireEnemy){
        qDebug() << "CheckGameWin - player won the game";
        GameWon(playerName);
        return true;
    }
    else if(soucetShipPolicek == countSuccesfulFirePlayer){
        qDebug() << "CheckGameWin - " << enemyInfo << " won the game";
        GameWon(enemyInfo);
        return true;
    }
    else{
        qDebug() << "CheckGameWin - game is not complete";
        return false;
    }
}

void View::GameWon(QString winner)
{
    qDebug() << "**************** " << winner << " ****************";
    playerTurn = false;
    endOfGame = true;
}

void View::LocalComputerBuilShips()
{
    qDebug() << "LocalComputer building ships";
    for(int i = 0;i < 10;++i){//na testovani
        for(int j = 0;j < 10;++j){//na testovani
            listGridEnemy[i][j] = listGridPlayer[i][j];//na testovani
            listShipEnemy[i][j] = listShipPlayer[i][j];//na testovani
        }//na testovani
    }//na testovani
    qsrand(time(0)+qrand());
    randomGridX = (qrand() % (9/*max*/ + 1) - 0/*min*/);
    randomGridY = (qrand() % (9/*max*/ + 1) - 0/*min*/);
}

void View::LocalComputerPlaceShip()//opravit souradnice z mouse na rand
{
//    if(shipEquiped == 1 && pocetShip1x1 != 0){
//        if(LocalComputerCanPlaceThere(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
//            if(rotation == 1){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 1;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip1x1--;
//                qDebug() << "LocalComputer - Placing ship1x1 verticaly at grid " << currentMouse;
//            }
//            else if(rotation == 2){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 5;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip1x1--;
//                qDebug() << "LocalComputer - Placing ship1x1 horizontaly at grid " << currentMouse;
//            }
//            else{
//                qDebug() << "LocalComputer - Rotation was not set";
//            }
//        }
//        else{
//            qDebug() << "LocalComputer - Here you can not place that";
//        }
//    }
//    else if(shipEquiped == 2 && pocetShip2x1 != 0){
//        if(LocalComputerCanPlaceThere(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
//            if(rotation == 1){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 2;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip2x1--;
//                qDebug() << "LocalComputer - Placing ship2x1 verticaly at grid " << currentMouse;
//            }
//            else if(rotation == 2){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 6;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip2x1--;
//                qDebug() << "LocalComputer - Placing ship2x1 horizontaly at grid " << currentMouse;
//            }
//            else{
//                qDebug() << "LocalComputer - Rotation was not set";
//            }
//        }
//        else{
//            qDebug() << "LocalComputer - Here you can not place that";
//        }
//    }
//    else if(shipEquiped == 3 && pocetShip3x1 != 0){
//        if(LocalComputerCanPlaceThere(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
//            if(rotation == 1){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 3;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip3x1--;
//                qDebug() << "LocalComputer - Placing ship3x1 verticaly at grid " << currentMouse;
//            }
//            else if(rotation == 2){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 7;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip3x1--;
//                qDebug() << "LocalComputer - Placing ship3x1 horizontaly at grid " << currentMouse;
//            }
//            else{
//                qDebug() << "LocalComputer - Rotation was not set";
//            }
//        }
//        else{
//            qDebug() << "LocalComputer - Here you can not place that";
//        }
//    }
//    else if(shipEquiped == 4 && pocetShip4x1 != 0){
//        if(LocalComputerCanPlaceThere(currentMouse.x(),currentMouse.y(),shipEquiped,rotation)){
//            if(rotation == 1){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 4;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip4x1--;
//                qDebug() << "LocalComputer - Placing ship4x1 verticaly at grid " << currentMouse;
//            }
//            else if(rotation == 2){
//                listGridPlayer[currentMouse.x()][currentMouse.y()] = 8;
//                LocalComputerFillListShip(currentMouse, shipEquiped, rotation);
//                pocetShip4x1--;
//                qDebug() << "LocalComputer - Placing ship4x1 horizontaly at grid " << currentMouse;
//            }
//            else{
//                qDebug() << "LocalComputer - Rotation was not set";
//            }
//        }
//        else{
//            qDebug() << "LocalComputer - Here you can not place that";
//        }
//    }
//    else{
//        qDebug() << "LocalComputer - You do not have another ships to paste";
//    }
}

bool View::LocalComputerCanPlaceThere(int startX, int startY, int shipEquiped, int rotation)
{

}

void View::LocalComputerPlayTurn()
{
    qDebug() << "LocalComputer playing turn";
    LocalComputerFireAtGrid();
}

void View::LocalComputerFireAtGrid()
{//localComputer "AI"
    if(LocalComputerLastPlayedGridType == 1){//pokud predtim trefil lod
        qsrand(time(0)+qrand());
        randomDirection = (qrand() % (3/*max*/ + 1) - 0/*min*/);
        int i_pocetZmen = 0;
        while(listFirePlayer[randomIntX][randomIntY] != 0 && i_pocetZmen < 4){
            if(i_pocetZmen > 0){//vraceni predchoziho propoctu - pouze pokud se prvni pokus s dopocitanymi souradnicemi nepovedl
                if(randomDirection == 1){
                    randomIntY++;
                }
                else if(randomDirection == 2){
                    randomIntX--;
                }
                else if(randomDirection == 3){
                    randomIntY--;
                }
                else if(randomDirection == 4){
                    randomIntX++;
                }
            }
            if(randomDirection == 0/*nahoru*/){
                randomIntY--;
            }
            else if(randomDirection == 1/*doprava*/){
                randomIntX++;
            }
            else if(randomDirection == 2/*dolu*/){
                randomIntY++;
            }
            else if(randomDirection == 3/*doleva*/){
                randomIntX--;
            }
            if(randomDirection < 3){
                randomDirection++;
            }
            else{
                randomDirection = 0;
            }
            ++i_pocetZmen;
        }
        qDebug() << "LocalComputer trying to fire at " << QPoint(randomIntX, randomIntY) << " based on previous fire";
    }
    else{//pokud predtim trefil vodu
        while(listFirePlayer[randomIntX][randomIntY] != 0){
            qsrand(time(0)+qrand());
            randomIntX = (qrand() % (9/*max*/ + 1) - 0/*min*/);
            randomIntY = (qrand() % (9/*max*/ + 1) - 0/*min*/);
            qDebug() << "LocalComputer trying to fire at " << QPoint(randomIntX, randomIntY);
        }
    }
    if(listShipEnemy[randomIntX][randomIntY] == 1){
        fireSuccesful = new Fire("fireSuccesful",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(fireSuccesful);
        fireSuccesful->setPos(randomIntX * (sirkaGrid / 10),randomIntY * (sirkaGrid / 10));
        countSuccesfulFirePlayer++;//docasne pak se bude pocitat z pole
        if(CheckGameWin()){
            return;
        }
        listFirePlayer[randomIntX][randomIntY] = 2/*pouzite policko - lod*/;
        qDebug() << "LocalComputer fired at " << QPoint(randomIntX, randomIntY) << " and shoted a ship";
        LocalComputerLastPlayedGridType = 1/*trefil lod*/;
        LocalComputerFireAtGrid();
    }
    else{
        fireWatter = new Fire("fireWatter",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(fireWatter);
        fireWatter->setPos(randomIntX * (sirkaGrid / 10),randomIntY * (sirkaGrid / 10));
        listFirePlayer[randomIntX][randomIntY] = 1/*pouzite policko - voda*/;
        qDebug() << "LocalComputer fired at " << QPoint(randomIntX, randomIntY) << " and shoted the water";
        LocalComputerLastPlayedGridType = 0/*trefil vodu*/;
        playerTurn = true;
        PlayerPlayTurn();
    }
}

void View::ViewLargeText(QString text)
{

}

void View::ViewConsole(QString text)
{
    if(enableViewConsole){
        qDebug() << text;
    }
}

void View::PlayerFireAtGrid(QPoint currentMouse)
{
    if(listShipEnemy[currentMouse.x()][currentMouse.y()] == 1){//pokud hrac trefil nepritelovu lod
        fireSuccesful = new Fire("fireSuccesful",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(fireSuccesful);
        fireSuccesful->setPos((currentMouse.x() * (sirkaGrid / 10)) + mezeraGrid + sirkaGrid,currentMouse.y() * (sirkaGrid / 10));
        countSuccesfulFireEnemy++;//docasne pak se bude pocitat z pole
        if(CheckGameWin()){
            return;
        }
        listFireEnemy[currentMouse.x()][currentMouse.y()] = 2;
        qDebug() << "Player fired at " << currentMouse << " and shoted a ship";
    }
    else{
        listFireEnemy[currentMouse.x()][currentMouse.y()] = 1;
        fireWatter = new Fire("fireWatter",Qt::red,mezeraGrid,mezeraGrid);
        this->scene()->addItem(fireWatter);
        fireWatter->setPos((currentMouse.x() * (sirkaGrid / 10)) + mezeraGrid + sirkaGrid,currentMouse.y() * (sirkaGrid / 10));
        playerTurn = false;
        qDebug() << "Player fired at " << currentMouse << " and shoted the water";
        EnemyIsOnTurn();
    }
}
