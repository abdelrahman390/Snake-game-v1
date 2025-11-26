#include <iostream>
#include <cstdlib>
#include <ctime> // Needed for time()
#include <windows.h>
#include <conio.h>

using namespace std;

bool stopGame = 0;
int generateRandomNum()
{
    return rand();
}

class Player
{
private:
    int pX = 0;
    int pY = 0;
    char dirs[4] = {'t', 'd', 'r', 'l'};
    char dir; // top: t, down: d, right: r, left: l
    int score = 0;

public:
    Player(int x, int y)
    {
        this->pX = x;
        this->pY = y;
        // generate randome number from 0 to 4;
        dir = dirs[generateRandomNum() % 4];
    }

    void changePlayerPositionBy(int x, int y)
    {
        this->pX += x;
        this->pY += y;
    }

    void changePlayerScore(int n){
        this->score += n;
    }

    void setPlayerDir(char c){
        dir = c;
    }

    int getPlayerXPos()
    {
        return this->pX;
    }

    int getPlayerYPos()
    {
        return this->pY;
    }

    int getPlayerScore()
    {
        return this->score;
    }

    char getPlayerDir()
    {
        return this->dir;
    }
};

void movePlayer(Player *p)
{
    // cout << endl << "Dir: " << p->getPlayerDir() << endl;
    switch (p->getPlayerDir())
    {
    case 't':
        p->changePlayerPositionBy(0, -1);
        break;
    case 'd':
        p->changePlayerPositionBy(0, 1);
        break;
    case 'r':
        p->changePlayerPositionBy(1, 0);
        break;
    case 'l':
        p->changePlayerPositionBy(-1, 0);
        break;
    }
}

void input(Player *p)
{
    if (_kbhit())
    {                      // check if a key was pressed
        int ch = _getch(); // get the key code

        if (ch == 27)
            stopGame = 1; // ESC key to exit

        // Arrow keys send TWO codes: first 0 or 224
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch(); // get the second code

            char c;
            switch (arrow)
            {
            case 72: // up arrow
                p->setPlayerDir('t');
                break;
            case 80: // down arrow
                p->setPlayerDir('d');
                break;
            case 75: // left arrow
                p->setPlayerDir('l');
                break;
            case 77: // right arrow
                p->setPlayerDir('r');
                break;
            }
        }
    }
}

class Screen
{
private:
    int height;
    int width;
    bool endGame = 0;
    int fruitXPos;
    int fruitYPos;
    Player *player;

    void frameDraw()
    {
        int width = this->width;
        int hight = this->height;
        int playerX = player->getPlayerXPos();
        int playerY = player->getPlayerYPos();

        for (int i = 0; i < hight; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == 0 || i == hight - 1)
                {
                    cout << '-';
                }
                else
                {
                    if (j == 0 || j == width - 1)
                    {
                        cout << '|';
                    }
                    else
                    {
                        if (playerX <= 0 || playerX >= width - 1 || playerY <= 0 || playerY >= height - 1)
                        {
                            endGame = 1;
                            cout << " ";
                        }
                        else if (i == playerY && j == playerX)
                        {
                            cout << 'o';
                        }
                        else if (i == fruitYPos && j == fruitXPos)
                        {
                            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Set text color to red
                            cout << '*';
                            SetConsoleTextAttribute(hConsole, 7);
                        }
                        else
                        {
                            cout << " ";
                        }
                    }
                }
            }
            cout << endl;
        }
        cout << "Score: " << player->getPlayerScore() << endl;
    }

    void checkFruit() { 
        if (player->getPlayerXPos() == fruitXPos &&
            player->getPlayerYPos() == fruitYPos) {
            player->changePlayerScore(1);
            fruitXPos = 1 + generateRandomNum() % (width - 2);
            fruitYPos = 1 + generateRandomNum() % (height - 2);
        }
    }

public:
    Screen(int h, int w, Player *p)
    {
        this->height = h;
        this->width = w;
        player = p;

        // init fruit Positions
        int fX = 1 + generateRandomNum() % (w - 2);
        int fY = 1 + generateRandomNum() % (h - 2);
        while (fX == p->getPlayerXPos() && fY == p->getPlayerYPos())
        {
            fX = 1 + generateRandomNum() % (w - 2);
            fY = 1 + generateRandomNum() % (h - 2);
        }
        this->fruitXPos = fX;
        this->fruitYPos = fY;
    }

    void draw()
    {
        for (int i = 0; i < 100; i++)
        {
            if (endGame)
            {
                break;
            } else if (stopGame){
                break;
            }
            system("cls");
            checkFruit();
            frameDraw();
            input(player);
            movePlayer(player);
            Sleep(50);
        }
        if (endGame)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Set text color to red
            cout << "Looser" << endl;
            SetConsoleTextAttribute(hConsole, 7);
        } else if (stopGame){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Set text color to red
            cout << "Game Ended" << endl;
            SetConsoleTextAttribute(hConsole, 7);
        }
    }
};

int main(){
    srand(time(0));
    int h, w;
    h = 15;
    w = 50;
    // player
    int px = 1 + generateRandomNum() % (w - 2);
    int py = 1 + generateRandomNum() % (h - 2);
    Player player1(px, py);

    // screen
    Screen mainScreen(h, w, &player1);
    mainScreen.draw();

    system("pause");
    return 0;
}
