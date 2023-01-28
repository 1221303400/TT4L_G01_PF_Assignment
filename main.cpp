/* *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TT4L
// Names: Mohd Farhan Bin Mohd Fairusham | Najjwa Farhanis Binti Mahayadin | Alvy Chang Kae Lyn
// IDs: 1221303400 | 1221303465 | 1221303500
// Emails: 1221303400@student.mmu.edu.my | 1221303465@student.mmu.edu.my | 1221303500@student.mmu.edu.my
// Phones: +60-1162980150 | +60-176610562 | +60-166318787
// *********************************************************/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
using namespace std;

struct Map
{
    vector<vector<char>> board;
    int DimX, DimY;
    bool play;
    char MapObjects[6] = {' ', ' ', ' ', ' ', ' ', ' '};
    //, '^', 'v', '>', '<'};
    //, 'h', 'h', 'p', 'p', 'r'};
    int noOfObjects = 6;

    bool IsInsideMap(int x, int y);

    int SetDimensions();

    void CheckObject();
    void DisplayMap();
    void SpawnVector();
    void UpdateVector();

    char getObject(int x, int y) const;
    void SetObject(int x, int y, char ch);
};

struct Alien
{
    int posX, posY;
    char Aliensymbol; // 'A', 'Á', 'Ä', 'Â', 'À', 'Ã',
    int health, attack;
    int PodAttack, ArrowAttack;

    void SpawnAlien(Map &GameMap);
    void MoveUp(Map &GameMap);
    void MoveDown(Map &GameMap);
    void MoveLeft(Map &GameMap);
    void MoveRight(Map &GameMap);
};

void QuitConfirm()
{
    char Ans;

    cout << "Are you sure? 'Y/N'" << endl;
    cin >> Ans;
    Ans = toupper(Ans);
    if (Ans == 'Y')
    {
        cout << "Quitting Game." << endl;
        cout << "Bye bye <3";
        exit(0);
    }
    else
    {
        return;
    }
}

void myPause()
{
    cout << "\nPress ENTER to continue...\n";
    cin.clear(); // clears input
    cin.sync();  // discard unread characters (0 if none)
    cin.get();   // wait for input
}

void displayStartMenu()
{
    cout << "*---------------------------------*" << endl
         << "   Welcome to Aliens VS Zombies!   " << endl
         << "Help the aliens regain their homes!" << endl
         << "*---------------------------------*" << endl
         << endl;
    cout << "Let's Get Started! Select by typing:" << endl;

    cout << "1: Start game " << endl
         << "2: Map Settings " << endl
         << "3: Quit game " << endl
         << endl;

    cout << "Input => " << endl;
}

void PlayerInput(Alien &Player, Map &GameMap)
{
    char PlayerInput;
    bool Answering = true;

    do
    {
        cout << endl
             << "Your input? Type 'H' to see the list of inputs." << endl;
        cin >> PlayerInput;
        PlayerInput = toupper(PlayerInput);
        switch (PlayerInput)
        {
        case 'H':
            cout << "'H' to see the input list" << endl
                 << "'C' to Check Objects" << endl
                 << "'A' to Switch Arrows" << endl
                 << endl
                 << "'Q' to Quit" << endl
                 << "'S' to Save" << endl
                 << endl
                 << "'^' to move UP" << endl
                 << "'V' to move DOWN" << endl
                 << "'<' to move LEFT" << endl
                 << "'>' to move RIGHT" << endl;
            break;

        case 'C':
            GameMap.CheckObject();
            break;
        case 'A':
            cout << "Feature has not been implemented yet." << endl;
            break;
        case 'Q':
            QuitConfirm();
            break;
        case 'S':
            cout << "Feature has not been implemented yet." << endl;
            break;
        case '^':
            Player.MoveUp(GameMap);
            // cout << "Feature has not been implemented yet." << endl;
            Answering = false;
            break;
        case 'V':
            Player.MoveDown(GameMap);
            // cout << "Feature has not been implemented yet." << endl;
            Answering = false;
            break;
        case '<':
            Player.MoveLeft(GameMap);
            // cout << "Feature has not been implemented yet." << endl;
            Answering = false;
            break;
        case '>':
            Player.MoveRight(GameMap);
            // cout << "Feature has not been implemented yet." << endl;
            Answering = false;
            break;
        default:
            cout << "Invalid Input" << endl;
            break;
        }

    } while (Answering);
}

void StartGame(Alien &Player, Map &GameMap)
{
    bool play = true;
    GameMap.SpawnVector();
    Player.SpawnAlien(GameMap);
    GameMap.DisplayMap();

    do
    {
        PlayerInput(Player, GameMap);
        GameMap.UpdateVector();
        GameMap.DisplayMap();
    } while (play);
}

void Alien::SpawnAlien(Map &GameMap) // Spawn the Alien
{
    posX = ((GameMap.DimX + 1) / 2);
    posY = ((GameMap.DimY + 1) / 2);
    Aliensymbol = 'A';

    GameMap.SetObject(posX, posY, Aliensymbol);
}

void Alien::MoveUp(Map &GameMap)
{
    int Temp = posY;
    for (int i = posY; i < GameMap.DimY; ++i)
    {
        char objekDepan = GameMap.getObject(posX, posY + 1);
        switch (objekDepan)
        {
        case ' ':
            ++posY;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(posX, Temp, '.');
            ++Temp;
            GameMap.DisplayMap();
            cout << "Alien is moving North." << endl;
            myPause();
            break;
        default:
            break;
        }
    }
}

void Alien::MoveDown(Map &GameMap)
{
    int Temp = posY;
    for (int i = posY; i > 1; --i)
    {
        char objekDepan = GameMap.getObject(posX, posY - 1);
        switch (objekDepan)
        {
        case ' ':
            --posY;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(posX, Temp, '.');
            --Temp;
            GameMap.DisplayMap();
            cout << "Alien is moving South." << endl;
            myPause();
            break;
        default:
            break;
        }
    }
}

void Alien::MoveLeft(Map &GameMap)
{
    int Temp = posX;
    for (int i = posX; i > 1; --i)
    {
        char objekDepan = GameMap.getObject(posX - 1, posY);
        switch (objekDepan)
        {
        case ' ':
            --posX;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(Temp, posY, '.');
            --Temp;
            GameMap.DisplayMap();
            cout << "Alien is moving East." << endl;
            myPause();
            break;
        default:
            break;
        }
    }
}

void Alien::MoveRight(Map &GameMap)
{
    int Temp = posX;
    for (int i = posX; i < GameMap.DimX; ++i)
    {
        char objekDepan = GameMap.getObject(posX + 1, posY);
        switch (objekDepan)
        {
        case ' ':
            ++posX;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(Temp, posY, '.');
            ++Temp;
            GameMap.DisplayMap();
            cout << "Alien is moving West." << endl;
            myPause();
            break;
        default:
            break;
        }
    }
}

bool Map::IsInsideMap(int x, int y)
{
    if ((DimY - y) <= DimY &&
        (DimY - y) >= 0 &&
        (x - 1) <= DimX &&
        (x - 1) >= 0)
        return true;
    else
        return false;
}

void Map::SetObject(int x, int y, char ch)
{
    board[DimY - y][x - 1] = ch;
}

char Map::getObject(int x, int y) const
{
    return board[DimY - y][x - 1];
}

void Map::SpawnVector()
{
    board.resize(DimY); // Create empty vector
    for (int i = 0; i < DimY; ++i)
    {
        board[i].resize(DimX);
    }

    for (int i = 0; i < DimY; ++i) // put random Characters inside vector
    {
        for (int j = 0; j < DimX; ++j)
        {
            int ObjectNumber = rand() % noOfObjects;
            board[i][j] = MapObjects[ObjectNumber];
        }
    }
}

void Map::UpdateVector()
{
    for (int i = 0; i < DimY; ++i) // put random Characters inside vector
    {
        for (int j = 0; j < DimX; ++j)
        {
            if (board[i][j] == '.')
            {
                int ObjectNumber = rand() % noOfObjects;
                board[i][j] = MapObjects[ObjectNumber];
            }
        }
    }
}

void Map::CheckObject()
{
    int x, y;

    cout << "Enter coordinates of object you want to check." << endl;
    cout << "X-Coordinate: " << endl;
    cin >> x;
    cout << "Y-Coordinate: " << endl;
    cin >> y;

    DisplayMap();
    if (IsInsideMap(x, y) == true)
    {
        if (board[DimY - y][x - 1] == ' ')
        {
            cout << "There is nothing at (" << x << ", "
                 << y << ")" << endl;
        }
        else
        {
            cout << "The object located at (" << x << ", "
                 << y << ") is " << board[DimY - y][x - 1]
                 << endl;
        }
    }
    else
    {
        cout << "Coordinates (" << x << ", "
             << y << ") does not exist." << endl;
    }
}

int Map::SetDimensions()
{
    int AnsX, AnsY;

    cout << "You may set your board dimensions." << endl;
    cout << "NOTE: Dimensions must be an odd number" << endl;
    cout << "so the alien can be placed in the middle." << endl;
    cout << "We will round up your input if it is even." << endl;
    cout << endl;
    cout << "X-Dimension = " << endl;
    cin >> AnsX;
    cout << "Y-Dimension = " << endl;
    cin >> AnsY;

    // odd or even checker
    if ((AnsX % 2) != 0)
    {
        DimX = AnsX;
    }
    else
    {
        DimX = AnsX + 1;
    }
    if ((AnsY % 2) != 0)
    {
        DimY = AnsY;
    }
    else
    {
        DimY = AnsY + 1;
    }

    cout << "Map Dimensions have been set to "
         << DimX << " & " << DimY << endl;

    return DimX, DimY;
}

void Map::DisplayMap()
{
    cout << " --__--__--__--__--__--__--__--" << endl; // Header
    cout << " = Alien, defeat the Zombies! =" << endl;
    cout << " __--__--__--__--__--__--__--__" << endl;

    // Each Row
    for (int i = 0; i < DimY; ++i)
    {
        // display upper border
        cout << "  ";
        for (int j = 0; j < DimX; ++j)
        {
            cout << "+-";
        }
        cout << "+" << endl;

        // display row number
        cout << setw(2) << (DimY - i);

        // DisplayMap cell content
        for (int j = 0; j < DimX; ++j)
        {
            cout << "|" << board[i][j]; // letak content
        }
        cout << "|" << endl;
    }

    // display lower border
    cout << "  ";
    for (int j = 0; j < DimX; ++j)
    {
        cout << "+-";
    }
    cout << "+" << endl;

    // display column number
    cout << "  ";
    for (int i = 1; i <= DimX; ++i)
    {
        int digit;
        digit = i / 10;

        if (DimX >= 10)
        {
            cout << " ";
            if (digit == 0)
            {
                cout << " ";
            }
            else
            {
                cout << digit;
            }
        }
    }
    if (DimX >= 10) // to display x-coordinates >= 10
    {
        cout << endl
             << "  ";
        for (int j = 1; j <= DimX; ++j)
        {
            cout << " " << (j % 10);
        }
    }
    else
    {
        for (int j = 1; j <= DimX; ++j)
        {
            cout << " " << (j % 10);
        }
    }
    cout << endl;
}

int main()
{
    srand(1);
    Map map;
    Alien alien;
    char Input;
    map.DimX = 7, map.DimY = 7; // default values
    bool done = true;
    do
    {
        displayStartMenu();

        cin >> Input;
        switch (Input)
        {
        case '1':
            StartGame(alien, map);
            break;
        case '2':
            map.SetDimensions();
            break;
        case '3':
            QuitConfirm();
            break;
        default:
            cout << "Invalid Input.\n"
                 << endl;
            break;
        }
        cout << endl;
        myPause();
    } while (done);
}
