#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <qevent.h>
#include "ship.h"
#include <QFileDialog>
#include "fire.h"

enum TypeRect {
    T_WATER,
    T_SHIP,
    T_WATER_NEAR_SHIP
};

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    View(QGraphicsScene *scene);

    bool playerTurn = true;
    bool building = true;
    bool beforeStart = true;
    bool holdingShip = false;
    bool endOfGame = false;

    bool enableViewConsole = true;

    const static int sirkaGrid = 500;//šířka hracího pole
    const static int mezeraGrid = 50;//mezera mezi hracími poli

    QPoint currentMouse;
    QPoint currentMouseLong;

    int gridId = 0;//0 - leva, 1 - prava, 2 - mezera

    int rotation = 1;//0 - none, 1 - svisle, 2 - vodorovne
    int shipEquiped = 0;//0 - none, 1 - 1x1, 2 - 2x1, 3 - 3x1, 4 - 4x1

    int pocetShip1x1 = 4;//pocty jednotlivych typu lodi
    int pocetShip2x1 = 3;
    int pocetShip3x1 = 2;
    int pocetShip4x1 = 1;

    int soucetShipPolicek = 20;//soucet policek ktera zabiraji lode celkem

    int saveIndex = 0;

    QString enemyInfo = "localComputer";

    QString playerName = "Player";

    QString latestConsoleText = "Application started";

public slots:
    void StartNewGame();

    void StartGame(QString enemyInfo);

    void UpdateHoverShipPublicSlots();

protected:
    void CreateGrid(int startX, int startY);

    int listGridPlayer[10][10];//umisteni jednotlivych lodi hrace //0 - none, 1 - 1x1 svisle, 2 - 2x1 svisle, 3 - 3x1 svisle, 4 - 4x1 svisle, 5 - 1x1 vodorovne, 6 - 2x1 vodorovne, 7 - 3x1 vodorovne, 8 - 4x1 vodorovne
    int listShipPlayer[10][10];//obsazenost policek hrace //0 - none, 1 - lod, 2 - voda v okoli lodi
    int listFirePlayer[10][10];//oznaceni jiz pouzitych policek na ktera strilel nepritel //0 - nestrileno, 1 - trefena voda, 2 - trefena lod
    int countSuccesfulFirePlayer = 0;//pocet trefenych policek kde nepritel trefil lod

    int listGridEnemy[10][10];//umisteni jednotlivych lodi nepritele //0 - none, 1 - 1x1 svisle, 2 - 2x1 svisle, 3 - 3x1 svisle, 4 - 4x1 svisle, 5 - 1x1 vodorovne, 6 - 2x1 vodorovne, 7 - 3x1 vodorovne, 8 - 4x1 vodorovne
    int listShipEnemy[10][10];//obsazenost policek nepritele //0 - voda, 1 - lod, 2 - voda v okoli lodi
    int listFireEnemy[10][10];//oznaceni jiz pouzitych policek na ktera strilel hrac //0 - nestrileno, 1 - trefena voda, 2 - trefena lod
    int countSuccesfulFireEnemy = 0;//pocet trefenych policek kde hrac trefil lod

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseWheelEvent(QGraphicsSceneWheelEvent *event);

    QGraphicsItem *ship1x1;
    QGraphicsItem *ship2x1;
    QGraphicsItem *ship3x1;
    QGraphicsItem *ship4x1;
    QGraphicsItem *ship1x1hover;
    QGraphicsItem *ship2x1hover;
    QGraphicsItem *ship3x1hover;
    QGraphicsItem *ship4x1hover;
    QGraphicsItem *fireSuccesful;
    QGraphicsItem *fireWatter;

    void LeftMouseClicked();

    void PlaceMyShipWhenBuilding();
    bool CanPlaceThereWhenBuilding(int startX, int startY, int shipEquiped, int rotation);

    void DisplayHoverShip(QPoint currentMouse, int shipEquiped, int rotation);
    void UpdateHoverShip(QPoint currentMouse, int shipEquiped, int rotation);
    bool CheckShipCount(int shipEquiped);
    void RemoveHoverShip(int shipEquiped);

    void CreateGameSaveFile();
    void ReadGameSaveFile();

    void FillListShip(QPoint currentMouse, int shipEquiped, int rotation);
    void PrepareListShip();

    void PlayerPlayTurn();
    void PlayerFireAtGrid(QPoint currentMouse);

    void EnemyIsOnTurn();

    bool CheckGameWin();
    void GameWon(QString winner);

    void LocalComputerBuilShips();
    void LocalComputerPlaceShip();
    bool LocalComputerCanPlaceThere(int startX, int startY, int shipEquiped, int rotation);
    void LocalComputerPlayTurn();
    void LocalComputerFireAtGrid();
    int LocalComputerLastPlayedX = 0;//kam strilel naposledy
    int LocalComputerLastPlayedY = 0;//kam strilel naposledy
    int LocalComputerLastPlayedGridType = 0;//co trefil naposledy
    int randomIntX = 0;
    int randomIntY = 0;
    int randomGridX = 0;
    int randomGridY = 0;
    int randomDirection = 0;//0 - nahoru, 1 - doprava, 2 - dolu, 3 - doleva

    int wheelRotation = 0;

    void ViewLargeText(QString text);
    void ViewConsole(QString text);
};

#endif // VIEW_H
