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
#include <string>
#include <fstream>
#include <limits>
#include <ios>

struct Alien;
struct Zombie;

struct Map
{
    std::vector<std::vector<char>> board;
    int DimX, DimY;
    bool play;
    char MapObjects[13] = {' ', ' ', ' ', ' ', ' ', ' ', '^', 'v', '>', '<', 'h', 'p', 'r'};
    int noOfObjects = 13;

    bool IsInsideMap(int x, int y);

    int SetDimensions();

    void CheckObject();
    void DisplayMap();
    void SpawnVector();
    void UpdateVector(Alien &Player);

    void SetArrow();
    char getObject(int x, int y) const;
    void SetObject(int x, int y, char ch);
};

struct ZombieData
{
    std::string Ztype;
    char Zsymbol;
    int ZposX, ZposY;
    int ZHealth, ZAttack;
    int ZHealthMax;
    int ZRange;
    std::string Zbio;
};

struct Alien
{
    int AlienTypes = 3, AlienCode;
    int posX, posY;
    char Aliensymbol;
    int AlienHealth, AlienAttack;
    int MaxAlienHealth, TempAlienAttack;
    int PodAttack, ArrowAttack;
    std::string AlienName;
    std::string AlienBio, AlienSpec;

    void SpawnAlien(Map &GameMap);
    bool CheckforAlien(Map &GameMap, int x, int y);
    bool IsAlienAlive();
    bool RockoCheck();
    bool SheldaCheck();
    int SheldaCount;

    void Move(Map &GameMap, Zombie &Enemy, char Direction);
    void MoveOneStep(Map &GameMap, char direction);
    void Interact(Map &GameMap, char Environment);
    void PodWork(Map &GameMap, Zombie &Enemy, int PosX, int PosY);

    void ShowExtraStats(int AlienCode);
    void ZombieDamaged(Zombie &Enemy, int Zlabel, int attack);
};

struct Zombie
{
    ZombieData Z[9];

    int ZombieAmount, ZombieStillAlive;
    int ZombieTypes = 5;

    char ZombieSymbols[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    int NoOfZombies();
    void SpawnZombies(Alien &Player, Map &GameMap);
    bool CheckforZombies(Map &GameMap, int x, int y);
    bool IsZombieAlive(int Zlabel);

    void MoveZombie(Map &GameMap, Alien &Player, char Fail);
    void MoveOneStep(Map &GameMap, char direction, int Zlabel);
    void ZombieInteract(Map &GameMap, Alien &Player);
};

ZombieData CreateZombie(int Ztype, int N, char Zch);

void Save(Map &GameMap, Alien &Player, Zombie &Enemy);

void Load(Map &GameMap, Alien &Player, Zombie &Enemy);

bool Confirmation()
{
    char Ans;

    std::cout << "\nAre you sure? 'Y/N'" << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> Ans;
    Ans = toupper(Ans);
    if (Ans == 'Y')
    {
        return true;
    }
    else
        return false;
}

void QuitConfirm()
{
    char Ans;

    std::cout << "\nAre you sure? 'Y/N' \n Remember to save your progress." << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> Ans;
    Ans = toupper(Ans);
    if (Ans == 'Y')
    {
        std::cout << "Quitting Game." << std::endl;
        std::cout << "Bye bye <3";
        exit(0);
    }
    else
    {
        return;
    }
}

void myPause()
{
    std::cout << "\nPress ENTER to continue...\n";
    std::cin.clear(); // clears input
    std::cin.sync();  // discard unread characters (0 if none)
    std::cin.get();   // wait for input
}

void displayStartMenu()
{
    std::cout << "*---------------------------------*" << std::endl
              << "   Welcome to Aliens VS Zombies!   " << std::endl
              << "Help the aliens regain their homes!" << std::endl
              << "*---------------------------------*" << std::endl
              << std::endl;
    std::cout << "Let's Get Started! Select by typing:" << std::endl;

    std::cout << "+---------------------------------+" << std::endl
              << "|1: Start game                    |" << std::endl
              << "|2: Map Settings                  |" << std::endl
              << "|3: Alien Selection               |" << std::endl
              << "|4: Zombie Settings               |" << std::endl
              << "|5: Quit game                     |" << std::endl
              << "+---------------------------------+" << std::endl
              << std::endl;

    std::cout << "Input => " << std::endl;
}

void ShowAlienStats(Alien &A) // Show Alien Stats
{
    std::cout << "\nAlien Stats: \n"
              << "Name: " << A.AlienName << ", Health: " << A.AlienHealth << ", Attack: " << A.AlienAttack << " \n"
              << "Pod Damage: " << A.PodAttack << ", Bonus damage from Arrows: " << A.ArrowAttack << " \n";

    A.ShowExtraStats(A.AlienCode);
}

void ShowZombieStats(Zombie &Enemy) // Show Zombie Stats
{

    int i, ans;
    char ch;
    std::cout << "Enter the number of the Zombie => ";
    std::cin.clear();
    std::cin.sync();
    std::cin >> ans;
    i = ans - 1;

    if (ans > 0 && ans <= Enemy.ZombieAmount)
    {
        std::cout << "\nZombie " << Enemy.Z[i].Zsymbol << " Stats: \n"
                  << "Type: " << Enemy.Z[i].Ztype << " \n"
                  << "Health: " << Enemy.Z[i].ZHealth << ", Attack: " << Enemy.Z[i].ZAttack << " \n"
                  << "Range: " << Enemy.Z[i].ZRange << " \n"
                  << "Bio: " << Enemy.Z[i].Zbio << " \n";
    }
    else
    {
        std::cout << "There is no Zombie labeled with that number.\n";
    }
}

void killZombie(Map &GameMap, Zombie &Enemy, int Zlabel)
{
    if (Enemy.IsZombieAlive(Zlabel) == false)
    {
        int RealLabel = Zlabel - 1;
        int DposX, DposY;
        DposX = Enemy.Z[RealLabel].ZposX;
        DposY = Enemy.Z[RealLabel].ZposY;

        Enemy.Z[RealLabel] = CreateZombie(0, RealLabel, ' ');
        std::cout << " Zombie [" << Zlabel << "] is now dead. RIP\n";
        --Enemy.ZombieStillAlive;

        GameMap.SetObject(DposX, DposY, ' ');
    }
}

void PlayerInput(Alien &Player, Map &GameMap, Zombie &Enemy)
{
    char PlayerInput;
    bool Answering = true;

    do
    {
        std::cout << std::endl
                  << "Your input? Type 'H' to see the list of inputs." << std::endl;
        std::cin.clear();
        std::cin.sync();
        std::cin >> PlayerInput;
        PlayerInput = toupper(PlayerInput);
        switch (PlayerInput)
        {
        case 'H':
            std::cout << "\n'H' to see the input list\n"
                      << std::endl
                      << "'D' to Display the Map and your Stats" << std::endl
                      << "'C' to Check Objects" << std::endl
                      << "'A' to Switch Arrows" << std::endl
                      << "'Z' to Check Zombies" << std::endl
                      << std::endl
                      << "'Q' to Quit" << std::endl
                      << "'S' to Save game" << std::endl
                      << "'L' to Load a Save file" << std::endl
                      << std::endl
                      << "'^' to move UP" << std::endl
                      << "'V' to move DOWN" << std::endl
                      << "'<' to move LEFT" << std::endl
                      << "'>' to move RIGHT" << std::endl;
            break;
        case 'D':
            GameMap.DisplayMap();
            ShowAlienStats(Player);
            break;
        case 'C':
            GameMap.CheckObject();
            break;
        case 'A':
            GameMap.SetArrow();
            break;
        case 'Z':
            ShowZombieStats(Enemy);
            break;
        case 'Q':
            QuitConfirm();
            break;
        case 'S':
            Save(GameMap, Player, Enemy);
            break;
        case 'L':
            Load(GameMap, Player, Enemy);
            GameMap.DisplayMap();
            break;
        case '^':
            Player.Move(GameMap, Enemy, '^');
            Answering = false;
            break;
        case 'V':
            Player.Move(GameMap, Enemy, 'v');
            Answering = false;
            break;
        case '<':
            Player.Move(GameMap, Enemy, '<');
            Answering = false;
            break;
        case '>':
            Player.Move(GameMap, Enemy, '>');
            Answering = false;
            break;
        default:
            std::cout << "Invalid Input" << std::endl;
            break;
        }

    } while (Answering);
}

void StartGame(Alien &Player, Map &GameMap, Zombie &Enemy)
{
    bool play = true; // Initialize apa yang patut
    Player.AlienHealth = Player.MaxAlienHealth;
    Player.SheldaCount = 0;
    if (Player.RockoCheck() == true)
    {
        for (int i = 6; i < 13; i++)
        {
            GameMap.MapObjects[i] = 'r';
        }
    }

    GameMap.SpawnVector();
    Player.SpawnAlien(GameMap);
    Enemy.SpawnZombies(Player, GameMap);
    GameMap.DisplayMap();
    ShowAlienStats(Player);
    do
    {
        Player.SheldaCount = 0;              // For Shelda
        PlayerInput(Player, GameMap, Enemy); // Alien Punya
        GameMap.UpdateVector(Player);
        GameMap.DisplayMap();
        if (Enemy.ZombieStillAlive <= 0)
        {
            std::cout << "\nYou have defeated all Zombies. You win!\n";
            myPause();
            play = false;
        }
        else
        {
            Enemy.MoveZombie(GameMap, Player, ' '); // Zombie Punya
            GameMap.UpdateVector(Player);
            Enemy.ZombieInteract(GameMap, Player);
            GameMap.DisplayMap();
        }

        if (Player.IsAlienAlive() == false)
        {
            ShowAlienStats(Player);
            std::cout << "\nYou have died. Game Over.\n";
            myPause();
            play = false;
        }

    } while (play);
}

void Map::CheckObject()
{
    int x, y;
    char obj;

    std::cout << "Enter coordinates of object you want to check." << std::endl;
    std::cout << "X-Coordinate: " << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> x;
    std::cout << "Y-Coordinate: " << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> y;

    DisplayMap();
    if (IsInsideMap(x, y) == true)
    {
        obj = board[DimY - y][x - 1];

        if (obj == ' ') // check if empty
        {
            std::cout << "There is nothing at (" << x << ", "
                      << y << ")" << std::endl;
        }
        else
        {
            std::cout << "The object located at (" << x << ", "
                      << y << ") is " << obj << std::endl;
            switch (obj)
            {
            case '^':
            case 'v':
            case '<':
            case '>':
                std::cout << "Arrows will temporarily add to your attack & move you in the corresponding direction.\n";
                break;
            case 'h':
                std::cout << "Health Packs will heal you for 40 HP.\n";
                break;
            case 'r':
                std::cout << "Rocks usually stop your movement. It will break and may have something underneath it.\n";
                break;
            case 'p':
                std::cout << "Pods will damage the nearest Zombie. Run through it to activate it.\n";
                break;
            case 'A':
                std::cout << "This is you. The Alien. You can check its stats by pressing 'D'.\n";
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                std::cout << "This is a Zombie, your foe. Type 'Z' to check its stats.\n";
                break;
            default:
                break;
            }
        }
    }
    else
    {
        std::cout << "Coordinates (" << x << ", "
                  << y << ") does not exist." << std::endl;
    }
}

void Map::DisplayMap()
{
    std::cout << " --__--__--__--__--__--__--__--" << std::endl; // Header
    std::cout << " = Alien, defeat the Zombies! =" << std::endl;
    std::cout << " __--__--__--__--__--__--__--__" << std::endl;

    // Each Row
    for (int i = 0; i < DimY; ++i)
    {
        // display upper border
        std::cout << "  ";
        for (int j = 0; j < DimX; ++j)
        {
            std::cout << "+-";
        }
        std::cout << "+" << std::endl;

        // display row number
        std::cout << std::setw(2) << (DimY - i);

        // DisplayMap cell content
        for (int j = 0; j < DimX; ++j)
        {
            std::cout << "|" << board[i][j]; // letak content
        }
        std::cout << "|" << std::endl;
    }

    // display lower border
    std::cout << "  ";
    for (int j = 0; j < DimX; ++j)
    {
        std::cout << "+-";
    }
    std::cout << "+" << std::endl;

    // display column number
    std::cout << "  ";
    for (int i = 1; i <= DimX; ++i)
    {
        int digit;
        digit = i / 10;

        if (DimX >= 10)
        {
            std::cout << " ";
            if (digit == 0)
            {
                std::cout << " ";
            }
            else
            {
                std::cout << digit;
            }
        }
    }
    if (DimX >= 10) // to display x-coordinates >= 10
    {
        std::cout << std::endl
                  << "  ";
        for (int j = 1; j <= DimX; ++j)
        {
            std::cout << " " << (j % 10);
        }
    }
    else
    {
        for (int j = 1; j <= DimX; ++j)
        {
            std::cout << " " << (j % 10);
        }
    }
    std::cout << std::endl
              << std::endl;
}

char Map::getObject(int x, int y) const // get char on Map
{
    return board[DimY - y][x - 1];
}

bool Map::IsInsideMap(int x, int y) // Bool Checker
{
    if ((y) <= DimY && (y) > 0 &&
        (x) <= DimX && (x) > 0)
        return true;
    else
        return false;
}

void Map::SetArrow()

{
    char Arrow, Ans;
    int x;
    int y;
    std::cout << "Please enter the x coordinate of the arrow" << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> x;

    std::cout << "Please enter the y coordinate of the arrow" << std::endl;
    std::cin.clear();
    std::cin.sync();
    std::cin >> y;

    if (IsInsideMap(x, y) == true)
    {
        Arrow = getObject(x, y);

        switch (Arrow)
        {
        case '^':
            std::cout << "Arrow is '^'." << std::endl;
            break;

        case '>':
            std::cout << "Arrow is '>'." << std::endl;
            break;

        case 'v':
            std::cout << "Arrow is 'v'." << std::endl;
            break;

        case '<':
            std::cout << "Arrow is '<'." << std::endl;
            break;

        default:
            std::cout << "There are no arrows there..." << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "Those coordinates do not exist.\n";
        return;
    }

    std::cout << "What Arrow would you like to change to?  => ";
    std::cin.clear();
    std::cin.sync();
    std::cin >> Ans;
    switch (Ans)
    {
    case '>':
        SetObject(x, y, '>');
        break;

    case 'v':
    case 'V':
        SetObject(x, y, 'v');
        break;

    case '<':
        SetObject(x, y, '<');
        break;

    case '^':
        SetObject(x, y, '^');
        break;

    default:
        std::cout << "Invalid Input" << std::endl;
        break;
    }

    myPause();
    DisplayMap();
}

int Map::SetDimensions() // Map Settings
{
    int AnsX, AnsY;
    bool Answering = true;

    std::cout << "You may set your board dimensions." << std::endl;
    std::cout << "NOTE: Dimensions must be an odd number greater than 3" << std::endl;
    std::cout << "so the alien can be placed in the middle." << std::endl;
    std::cout << "We will round up your input if it is even." << std::endl;
    std::cout << std::endl;

    do
    {
        std::cout << "X-Dimension = " << std::endl;
        std::cin.clear();
        std::cin.sync();
        std::cin >> AnsX;
        std::cout << "Y-Dimension = " << std::endl;
        std::cin.clear();
        std::cin.sync();
        std::cin >> AnsY;
        if (AnsX > 3 && AnsY > 3)
        {
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
            Answering = false;
        }
        else
        {
            std::cout << "Map Dimensions must be at least 5!\n";
            break;
        }
    } while (Answering);
    std::cout << "Map Dimensions have been set to "
              << DimX << " & " << DimY << std::endl;

    return DimX, DimY;
}

void Map::SetObject(int x, int y, char ch)
{
    board[DimY - y][x - 1] = ch;
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

void Map::UpdateVector(Alien &Player)
{

    for (int i = 0; i < DimY; ++i) // put random Characters inside '.'
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

Alien CreateAlien(int Atype)
{
    Alien A;
    switch (Atype)
    {
    case 1:
    default:
        // Alvee the Alien
        A.AlienName = "Alvee", A.AlienCode = 1;
        A.AlienHealth = 300, A.MaxAlienHealth = 300;
        A.AlienAttack = 50, A.TempAlienAttack = 50;
        A.ArrowAttack = 30, A.PodAttack = 30;
        A.AlienBio;
        A.AlienSpec;
        break;
    case 2:
        // Rocko the Rockcrusher
        A.AlienName = "Rocko", A.AlienCode = 2;
        A.AlienHealth = 400, A.MaxAlienHealth = 400;
        A.AlienAttack = 75, A.TempAlienAttack = 75;
        A.ArrowAttack = 30, A.PodAttack = 30;
        A.AlienBio;
        A.AlienSpec;
        /* Speciality:  - Rocks don't stop movements.
                        - All items are hidden beneath rocks.
                        - Hitting a rock reduces attack by 20.
        */
        break;
    case 3:
        // Shelda the Shieldmaiden
        A.AlienName = "Shelda", A.AlienCode = 3;
        A.AlienHealth = 300, A.MaxAlienHealth = 300;
        A.AlienAttack = 50, A.TempAlienAttack = 50;
        A.ArrowAttack = 40, A.PodAttack = 40;
        A.AlienBio;
        A.AlienSpec;
        /* Speciality:  - Shield after 5 items
                        - Damage depends on health percentage
                        - Items deal more damage
        */
        break;
    }
    return A;
}

Alien ChooseAlien()
{
    Alien A;
    bool choose = true;
    int ans;
    do
    {
        std::cout << "\nPlease choose your Alien to fight the Zombies. \n";
        std::cout << "Choose by typing numbers from 1 to " << A.AlienTypes << " \n";
        std::cout << "Number => ";

        std::cin.clear();
        std::cin.sync();
        std::cin.clear();
        std::cin.sync();
        std::cin >> ans;
        switch (ans)
        {
        case 1:
            A = CreateAlien(1);
            ShowAlienStats(A);
            if (Confirmation() == true)
            {
                choose = false;
            }
            break;
        case 2:
            A = CreateAlien(2);
            ShowAlienStats(A);
            if (Confirmation() == true)
            {
                choose = false;
            }
            break;
        case 3:
            A = CreateAlien(3);
            ShowAlienStats(A);
            if (Confirmation() == true)
            {
                choose = false;
            }
            break;

        default:
            std::cout << "Invalid number. Type in numbers from 1 to " << A.AlienTypes << " \n";
            break;
        }
    } while (choose);
    return A;
}

bool Alien::CheckforAlien(Map &GameMap, int x, int y)
{
    char obj;
    obj = GameMap.board[GameMap.DimY - y][x - 1];

    switch (obj)
    {
    case 'A':
        return true;
        break;
    default:
        return false;
        break;
    }
}

void Alien::Interact(Map &GameMap, char Environment)
{
    switch (Environment)
    {
    case '^':
    case 'v':
    case '<':
    case '>': // Arrow Damage
        TempAlienAttack = TempAlienAttack + ArrowAttack;
        std::cout << std::endl
                  << AlienName << " has consumed an Arrow.\n";
        break;
    case 'h': // health
        if (AlienHealth + 40 >= MaxAlienHealth)
        {
            AlienHealth = MaxAlienHealth;
        }
        else
        {
            AlienHealth = AlienHealth + 40;
        }
        std::cout << std::endl
                  << AlienName << " has healed 40 HP.\n";
        break;
    default:
        std::cout << "No Interaction exists.\n";
        break;
    }
}

bool Alien::IsAlienAlive()
{
    if (AlienHealth > 0)
    {
        return true;
    }
    else
        return false;
}

void Alien::Move(Map &GameMap, Zombie &Enemy, char Direction)
{
    switch (Direction) // To move the alien
    {
    case '^':
    {
        int Temp = posY;
        for (int i = posY; i < GameMap.DimY; ++i)
        {
            char ObjectInFront = GameMap.getObject(posX, posY + 1);
            switch (ObjectInFront)
            {
            case ' ':
            case '.':
            {
                ++posY;
                GameMap.SetObject(posX, posY, Aliensymbol);
                GameMap.SetObject(posX, Temp, '.');
                ++Temp;
                GameMap.DisplayMap();
                std::cout << AlienName << " is moving North." << std::endl;
                myPause();
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    case 'v':
    {
        int Temp = posY;
        for (int i = posY; i > 1; --i)
        {
            char ObjectInFront = GameMap.getObject(posX, posY - 1);
            switch (ObjectInFront)
            {
            case ' ':
            case '.':
            {
                --posY;
                GameMap.SetObject(posX, posY, Aliensymbol);
                GameMap.SetObject(posX, Temp, '.');
                --Temp;
                GameMap.DisplayMap();
                std::cout << AlienName << " is moving South." << std::endl;
                myPause();
            }
            default:
                break;
            }
        }
        break;
    }
    case '<':
    {
        int Temp = posX;
        for (int i = posX; i > 1; --i)
        {
            char ObjectInFront = GameMap.getObject(posX - 1, posY);
            switch (ObjectInFront)
            {
            case ' ':
            case '.':
            {
                --posX;
                GameMap.SetObject(posX, posY, Aliensymbol);
                GameMap.SetObject(Temp, posY, '.');
                --Temp;
                GameMap.DisplayMap();
                std::cout << AlienName << " is moving West." << std::endl;
                myPause();
            }
            default:
                break;
            }
        }
        break;
    }
    case '>':
    {
        int Temp = posX;
        for (int i = posX; i < GameMap.DimX; ++i)
        {
            char ObjectInFront = GameMap.getObject(posX + 1, posY);
            switch (ObjectInFront)
            {
            case ' ':
            case '.':
            {
                ++posX;
                GameMap.SetObject(posX, posY, Aliensymbol);
                GameMap.SetObject(Temp, posY, '.');
                ++Temp;
                GameMap.DisplayMap();
                std::cout << AlienName << " is moving East." << std::endl;
                myPause();
            }
            default:
                break;
            }
        }
        break;
    }
    }

    char ObjekDepan, ObjekBelakang;
    int ObjDX, ObjDY, ObjBX, ObjBY;
    switch (Direction) // To check position of ObjekDepan
    {
    case '^':
        ObjDX = posX, ObjDY = posY + 1;
        break;
    case 'v':
        ObjDX = posX, ObjDY = posY - 1;
        break;
    case '<':
        ObjDX = posX - 1, ObjDY = posY;
        break;
    case '>':
        ObjDX = posX + 1, ObjDY = posY;
    default:
        break;
    }
    ObjBX = posX, ObjBY = posY;
    char SpecialObjects[6] = {'^', 'v', '<', '>', 'p', 'h'};
    int noOfPrizes = 6;
    int PrizeNumber = rand() % noOfPrizes;
    char Prize = SpecialObjects[PrizeNumber];

    if (GameMap.IsInsideMap(ObjDX, ObjDY) == true) // Interaction
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
    switch (ObjekDepan)
    {
    case 'r':
        if (RockoCheck() == true)
        {
            MoveOneStep(GameMap, Direction);
            GameMap.SetObject(ObjBX, ObjBY, Prize);
            if (TempAlienAttack - 20 <= 5)
            {
                TempAlienAttack = 5;
            }
            else
            {
                TempAlienAttack = TempAlienAttack - 20;
            }
            std::cout << AlienName << " has smashed through a rock, his damage is reduced by 20.\n";
            myPause();
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
        {
            GameMap.SetObject(ObjDX, ObjDY, Prize);
            std::cout << AlienName << " has break a rock, revealing '" << Prize << "' underneath it.\n";
            myPause();
            break;
        }
    case '^':
        // Arrow;
        MoveOneStep(GameMap, Direction);
        Interact(GameMap, '^');
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, '^');
        break;
    case 'v':
        // Arrow;
        MoveOneStep(GameMap, Direction);
        Interact(GameMap, 'v');
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, 'v');
        break;
    case '<':
        // Arrow;
        MoveOneStep(GameMap, Direction);
        Interact(GameMap, '<');
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, '<');
        break;
    case '>':
        // Arrow;
        MoveOneStep(GameMap, Direction);
        Interact(GameMap, '>');
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, '>');
        break;
    case 'h':
        // Health
        MoveOneStep(GameMap, Direction);
        Interact(GameMap, 'h');
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, Direction);
        break;
    case 'p':
        // Pods
        MoveOneStep(GameMap, Direction);
        PodWork(GameMap, Enemy, ObjDX, ObjDY);
        SheldaCount++; // For Shelda
        myPause();
        Move(GameMap, Enemy, Direction);
        break;

    case '1': // Zombie Cases
        ZombieDamaged(Enemy, 1, TempAlienAttack);
        killZombie(GameMap, Enemy, 1);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '2':
        ZombieDamaged(Enemy, 2, TempAlienAttack);
        killZombie(GameMap, Enemy, 2);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '3':
        ZombieDamaged(Enemy, 3, TempAlienAttack);
        killZombie(GameMap, Enemy, 3);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '4':
        ZombieDamaged(Enemy, 4, TempAlienAttack);
        killZombie(GameMap, Enemy, 4);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '5':
        ZombieDamaged(Enemy, 5, TempAlienAttack);
        killZombie(GameMap, Enemy, 5);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '6':
        ZombieDamaged(Enemy, 6, TempAlienAttack);
        killZombie(GameMap, Enemy, 6);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '7':
        ZombieDamaged(Enemy, 7, TempAlienAttack);
        killZombie(GameMap, Enemy, 7);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '8':
        ZombieDamaged(Enemy, 8, TempAlienAttack);
        killZombie(GameMap, Enemy, 8);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    case '9':
        ZombieDamaged(Enemy, 9, TempAlienAttack);
        killZombie(GameMap, Enemy, 9);
        ObjekDepan = GameMap.board[GameMap.DimY - ObjDY][ObjDX - 1];
        myPause();
        if (ObjekDepan == ' ')
        {
            Move(GameMap, Enemy, Direction);
            break;
        }
        else
            break;
    default:
        // std::cout << "Default case.";
        break;
    }
    TempAlienAttack = AlienAttack;
}

void Alien::MoveOneStep(Map &GameMap, char direction)
{
    Alien A;
    switch (direction)
    {
    case '^':
    {
        {
            int Temp = posY;
            ++posY;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(posX, Temp, '.');
            GameMap.DisplayMap();

            std::cout << AlienName << " is moving North." << std::endl;
            break;
        }
    }
    case 'v':
    {
        {
            int Temp = posY;
            --posY;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(posX, Temp, '.');
            GameMap.DisplayMap();

            std::cout << AlienName << " is moving South." << std::endl;
            break;
        }
    }
    case '<':
    {
        {
            int Temp = posX;
            --posX;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(Temp, posY, '.');
            GameMap.DisplayMap();

            std::cout << AlienName << " is moving West." << std::endl;
            break;
        }
    }
    case '>':
    {
        {
            int Temp = posX;
            ++posX;
            GameMap.SetObject(posX, posY, Aliensymbol);
            GameMap.SetObject(Temp, posY, '.');
            GameMap.DisplayMap();

            std::cout << AlienName << " is moving East." << std::endl;
            break;
        }

    default:
        std::cout << "Alien has stopped moving.\n";
        break;
    }
    }
}

void Alien::PodWork(Map &GameMap, Zombie &Enemy, int PosX, int PosY)
{
    bool Pod = true;
    int r = 0;
    if (Enemy.ZombieStillAlive > 0)
    {
        while (Pod == true)
        {
            ++r;
            if (Pod == true)
            {
                for (int i = PosY - r; i <= PosY + r; i++)
                {
                    if (Pod == true)
                    {
                        for (int j = PosX - r; j <= PosX + r; ++j)
                        {
                            if (Pod == true)
                            {
                                if (GameMap.IsInsideMap(j, i) == true)
                                {
                                    char Target = GameMap.board[GameMap.DimY - i][j - 1];
                                    switch (Target)
                                    {
                                    case '1':
                                        Enemy.Z[0].ZHealth = Enemy.Z[0].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[0].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 1);
                                        Pod = false;
                                        break;
                                    case '2':
                                        Enemy.Z[1].ZHealth = Enemy.Z[1].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[1].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 2);
                                        Pod = false;
                                        break;
                                    case '3':
                                        Enemy.Z[2].ZHealth = Enemy.Z[2].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[2].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 3);
                                        Pod = false;
                                        break;
                                    case '4':
                                        Enemy.Z[3].ZHealth = Enemy.Z[3].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[3].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 4);
                                        Pod = false;
                                        break;
                                    case '5':
                                        Enemy.Z[4].ZHealth = Enemy.Z[4].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[4].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 5);
                                        Pod = false;
                                        break;
                                    case '6':
                                        Enemy.Z[5].ZHealth = Enemy.Z[5].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[5].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 6);
                                        Pod = false;
                                        break;
                                    case '7':
                                        Enemy.Z[6].ZHealth = Enemy.Z[6].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[6].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 7);
                                        Pod = false;
                                        break;
                                    case '8':
                                        Enemy.Z[7].ZHealth = Enemy.Z[7].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[7].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 8);
                                        Pod = false;
                                        break;
                                    case '9':
                                        Enemy.Z[8].ZHealth = Enemy.Z[8].ZHealth - PodAttack;
                                        std::cout << "The Pod has hit " << Enemy.Z[8].Ztype << ", ["
                                                  << Target << "] for " << PodAttack << " damage.\n";

                                        killZombie(GameMap, Enemy, 9);
                                        Pod = false;
                                        break;
                                    default:
                                        // std::cout << "Pod is searching.\n";
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                        break;
                }
            }
            else
                break;
        }
    }
}

bool Alien::RockoCheck()
{
    if (AlienName == "Rocko")
    {
        return true;
    }
    else
        return false;
}

bool Alien::SheldaCheck()
{
    if (AlienName == "Shelda")
    {
        return true;
    }
    else
        return false;
}

void Alien::ShowExtraStats(int AlienCode)
{
    switch (AlienCode)
    {
    case 1:
        AlienBio = "She is an ordinary organism with an extraordinary cause.";
        AlienSpec = "Speciality: \n- None.";
        break;
    case 2:
        AlienBio = "Rocks. He's a big fan of 'em. He'll consume 'em on the run even if it slows him down.";
        AlienSpec = "Speciality: \n- Every item spawns as a rock.\n- Rock won't stop him and items inside rocks will spawn behind him.\n- Hitting a rock reduces attack by 20, to a minimum of 5.";
        break;
    case 3:
        AlienBio = "Shelda makes shields after consuming stuff, because she is lazy when hungry.";
        AlienSpec = "Speciality: \n- She makes a sheild if 5 items are consumed in a turn.\n- Shields negate all damage for a turn.\n- Items deal more damage.\n- Her damage depends on her health percentage.";
        break;
    default:
        break;
    }

    std::cout << "Bio: " << AlienBio << " \n"
              << AlienSpec << " \n";
}

void Alien::SpawnAlien(Map &GameMap) // Spawn the Alien
{
    posX = ((GameMap.DimX + 1) / 2);
    posY = ((GameMap.DimY + 1) / 2);
    Aliensymbol = 'A';

    GameMap.SetObject(posX, posY, Aliensymbol);
}

void Alien::ZombieDamaged(Zombie &Enemy, int Zlabel, int Attack) // Zombie gets attacked
{
    int RealLabel = Zlabel - 1;

    if (SheldaCheck() == true)
    {
        int percent = (float(AlienHealth) / MaxAlienHealth) * 100;
        int Dpercent;
        if (percent > 50)
        {
            Dpercent = percent;
            Attack = TempAlienAttack * (float(AlienHealth) / MaxAlienHealth);
        }
        else
        {
            Dpercent = 50;
            Attack = TempAlienAttack * 0.5;
        }
        std::cout << "Shelda's health is currently " << percent << "%, she will deal "
                  << Dpercent << "% Damage.\n";
    }

    Enemy.Z[RealLabel].ZHealth = Enemy.Z[RealLabel].ZHealth - Attack;

    std::cout << "You attacked " << Enemy.Z[RealLabel].Ztype << ", ["
              << Zlabel << "] for " << Attack << " damage.\n";
}

ZombieData CreateZombie(int Ztype, int N, char Zch)
{
    ZombieData Z[9];
    switch (Ztype)
    {
    case 0:
        // Dead Zombie
        Z[N].Ztype = "Dead", Z[N].Zsymbol = ' ';
        Z[N].ZHealth = 0, Z[N].ZHealthMax = 0, Z[N].ZAttack = 0, Z[N].ZRange = 0;
        Z[N].Zbio = "This Zombie is dead. Congrats...";
        break;

    case 1:
    case 5:
        // Reggie Regular
        Z[N].Ztype = "Regular";
        Z[N].ZHealth = 200, Z[N].ZHealthMax = 200;
        Z[N].ZAttack = 25;
        Z[N].ZRange = 2;
        Z[N].Zbio = "Just your standard zombie. Aliens call them Reggies.";
        Z[N].Zsymbol = Zch;
        break;

    case 2:
        // Coney Conehead
        Z[N].Ztype = "Conehead";
        Z[N].ZHealth = 350, Z[N].ZHealthMax = 350;
        Z[N].ZAttack = 25;
        Z[N].ZRange = 2;
        Z[N].Zbio = "A Reggie with a cone. They apparently got this idea from their penpals at Earth who are fighting plants. Crazy...";
        Z[N].Zsymbol = Zch;
        break;
    case 3:
        // Muhammad Tyson
        Z[N].Ztype = "Puncher";
        Z[N].ZHealth = 250, Z[N].ZHealthMax = 250;
        Z[N].ZAttack = 70;
        Z[N].ZRange = 1;
        Z[N].Zbio = "This fella packs a punch. Don't get near him!";
        Z[N].Zsymbol = Zch;
        break;
    case 4:
        // Sniper Spitter
        Z[N].Ztype = "Spitter";
        Z[N].ZHealth = 150, Z[N].ZHealthMax = 150;
        Z[N].ZAttack = 30;
        Z[N].ZRange = 4;
        Z[N].Zbio = "They will attack from afar by spitting at you. The spit isn't poisonous or anything, the Aliens are just hygienic.";
        Z[N].Zsymbol = Zch;
        break;
    }
    return Z[N];
}

ZombieData AssignStats(int j, int Amount, int Types)
{
    ZombieData Z[9];

    int Znum = rand() % Types + 1;
    char ch = '1' + j;

    Z[j] = CreateZombie(Znum, j, ch);
    return Z[j];
}

bool Zombie::CheckforZombies(Map &GameMap, int x, int y)
{
    char obj;
    obj = GameMap.board[GameMap.DimY - y][x - 1];

    switch (obj)
    {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool Zombie::IsZombieAlive(int Zlabel)
{
    int Rlabel = Zlabel - 1;
    if (Z[Rlabel].ZHealth > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Zombie::MoveOneStep(Map &GameMap, char direction, int Zlabel)
{

    switch (direction)
    {
    case '^':
    {

        int Temp = Z[Zlabel].ZposY;
        char ObjInFront = GameMap.getObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY + 1);

        switch (ObjInFront)
        {
        case ' ':
        case '.':
        case '^':
        case 'v':
        case '<':
        case '>':
        case 'h':
        case 'r':
        case 'p':
        {
            ++Z[Zlabel].ZposY;
            GameMap.SetObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY, Z[Zlabel].Zsymbol);
            GameMap.SetObject(Z[Zlabel].ZposX, Temp, '.');
            GameMap.DisplayMap();

            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1 << "]"
                      << " is moving North." << std::endl;
            myPause();
            break;
        }
        default:
            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1
                      << "] is blocked from moving '" << direction << "'.\n";
            myPause();
            break;
        }
        break;
    }
    case 'v':
    {

        int Temp = Z[Zlabel].ZposY;
        char ObjInFront = GameMap.getObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY - 1);

        switch (ObjInFront)
        {
        case ' ':
        case '.':
        case '^':
        case 'v':
        case '<':
        case '>':
        case 'h':
        case 'r':
        case 'p':
        {
            --Z[Zlabel].ZposY;
            GameMap.SetObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY, Z[Zlabel].Zsymbol);
            GameMap.SetObject(Z[Zlabel].ZposX, Temp, '.');
            GameMap.DisplayMap();

            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1 << "]"
                      << " is moving South." << std::endl;
            myPause();
            break;
        }
        default:
            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1
                      << "] is blocked from moving '" << direction << "'.\n";
            myPause();
            break;
        }
        break;
    }
    case '<':
    {

        int Temp = Z[Zlabel].ZposX;
        char ObjInFront = GameMap.getObject(Z[Zlabel].ZposX - 1, Z[Zlabel].ZposY);

        switch (ObjInFront)
        {
        case ' ':
        case '.':
        case '^':
        case 'v':
        case '<':
        case '>':
        case 'h':
        case 'r':
        case 'p':
        {
            --Z[Zlabel].ZposX;
            GameMap.SetObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY, Z[Zlabel].Zsymbol);
            GameMap.SetObject(Temp, Z[Zlabel].ZposY, '.');
            GameMap.DisplayMap();

            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1 << "]"
                      << " is moving West." << std::endl;
            myPause();
            break;
        }
        default:
            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1
                      << "] is blocked from moving '" << direction << "'.\n";
            myPause();
            break;
        }
        break;
    }
    case '>':
    {

        int Temp = Z[Zlabel].ZposX;
        char ObjInFront = GameMap.getObject(Z[Zlabel].ZposX + 1, Z[Zlabel].ZposY);

        switch (ObjInFront)
        {
        case ' ':
        case '.':
        case '^':
        case 'v':
        case '<':
        case '>':
        case 'h':
        case 'r':
        case 'p':
        {
            ++Z[Zlabel].ZposX;
            GameMap.SetObject(Z[Zlabel].ZposX, Z[Zlabel].ZposY, Z[Zlabel].Zsymbol);
            GameMap.SetObject(Temp, Z[Zlabel].ZposY, '.');
            GameMap.DisplayMap();

            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1 << "]"
                      << " is moving East." << std::endl;
            myPause();
            break;
        }
        default:
            std::cout << Z[Zlabel].Ztype << ", [" << Zlabel + 1
                      << "] is blocked from moving '" << direction << "'.\n";
            myPause();
            break;
        }
        break;
    }
    default:
        std::cout << "\nThis Zombie has stopped moving.\n";
        break;
    }
}

void Zombie::MoveZombie(Map &GameMap, Alien &Player, char Fail)
{
    int Zlabel;
    int FposX, FposY;
    for (int i = 0; i < ZombieAmount; i++)
    {
        if (Z[i].Ztype != "Dead")
        {
            char Directions[4] = {'^', 'v', '<', '>'};

            if (Z[i].ZposX == GameMap.DimX || Fail == '>')
            {
                Directions[3] = '<';
            }
            if (Z[i].ZposX == 1 || Fail == '<')
            {
                Directions[2] = '>';
            }
            if (Z[i].ZposY == GameMap.DimY || Fail == '^')
            {
                Directions[0] = 'v';
            }
            if (Z[i].ZposY == 1 || Fail == 'v')
            {
                Directions[1] = '^';
            }

            int DirNumber = rand() % 4;
            char zArrow = Directions[DirNumber];
            MoveOneStep(GameMap, zArrow, i);
        }
    }
}

int Zombie::NoOfZombies() // Zombie Settings
{
    int AnsZ;
    bool Z = true;

    std::cout << "\nSelect the number of Zombies you would like to fight.\n"
              << "You may choose between 1 to 9.\n";

    do
    {
        std::cin.clear();
        std::cin.sync();
        std::cin >> AnsZ;

        if (AnsZ >= 1 && AnsZ <= 9)
        {
            ZombieAmount = AnsZ;
            ZombieStillAlive = ZombieAmount;
            Z = false;
        }
        else
        {
            std::cout << "Select a number between 1 to 9.\n";
        }
    } while (Z);

    if (ZombieAmount == 1)
    {
        std::cout << "\nYou will be facing 1 Zombie. Good Luck!\n";
    }
    else
    {
        std::cout << "\nYou will be facing " << AnsZ << " Zombies. Good Luck!\n";
    }
    return ZombieAmount;
    return ZombieStillAlive;
}

void Zombie::SpawnZombies(Alien &Player, Map &GameMap)
{
    char ch;
    bool Zbool, Abool;

    for (int i = 0; i < ZombieAmount; i++) // Spawn Zombie at coordinates
    {
        int Znum = rand() % ZombieTypes + 1;
        ch = '1' + i;

        // Z = CreateZombie(Znum, i, ch);
        Z[i] = AssignStats(i, ZombieAmount, ZombieTypes);
        Z[i].ZposX = rand() % GameMap.DimX + 1;
        Z[i].ZposY = rand() % GameMap.DimY + 1;

        Zbool = CheckforZombies(GameMap, Z[i].ZposX, Z[i].ZposY);
        Abool = Player.CheckforAlien(GameMap, Z[i].ZposX, Z[i].ZposY);
        if (Zbool == false && Abool == false)
        {
            GameMap.SetObject(Z[i].ZposX, Z[i].ZposY, Z[i].Zsymbol);
        }
        else
        {
            --i;
        }
    }
}

void Zombie::ZombieInteract(Map &GameMap, Alien &Player)
{
    for (int z = 0; z < ZombieAmount; z++)
    {
        if (Z[z].Ztype != "Dead")
        {
            for (int i = Z[z].ZposY - Z[z].ZRange;
                 i <= Z[z].ZposY + Z[z].ZRange; i++)
            {
                for (int j = Z[z].ZposX - Z[z].ZRange;
                     j <= Z[z].ZposX + Z[z].ZRange; j++)
                {
                    if (GameMap.IsInsideMap(j, i) == true)
                    {
                        if (GameMap.board[GameMap.DimY - i][j - 1] == Player.Aliensymbol)
                        {
                            if (Player.SheldaCheck() == true && Player.SheldaCount >= 5)
                            {
                                std::cout << "Shelda has gained a shield for this turn. \nNo damage will be recieved. Yeay!\n";
                                myPause();
                            }
                            else
                            {
                                Player.AlienHealth = Player.AlienHealth - Z[z].ZAttack;
                                std::cout << Z[z].Ztype << ", [" << z + 1
                                          << "] attacked you for " << Z[z].ZAttack
                                          << " damage.\n";
                                myPause();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Save(Map &GameMap, Alien &Player, Zombie &Enemy)
{
    std::ofstream SaveFile;
    std::string SaveName;

    std::cout << "Enter a name for your save file => ";
    std::getline(std::cin >> std::ws, SaveName);

    SaveFile.open(SaveName);
    if (SaveFile.fail())
    {
        perror("Save Error");
        return;
    }

    // int Data
    SaveFile << Player.AlienHealth << " " << Player.AlienAttack << " "
             << Player.MaxAlienHealth << " " << Player.TempAlienAttack << " "
             << Player.PodAttack << " " << Player.ArrowAttack << " "
             << Player.posX << " " << Player.posY << " " << Player.AlienCode << " ";

    SaveFile << Enemy.ZombieAmount << " " << Enemy.ZombieStillAlive << "\n";
    for (int i = 0; i < Enemy.ZombieAmount; i++)
    {
        SaveFile << Enemy.Z[i].ZHealth << " " << Enemy.Z[i].ZAttack << " "
                 << Enemy.Z[i].ZHealthMax << " " << Enemy.Z[i].ZRange << " ";
        if (Enemy.Z[i].ZHealthMax <= 0)
        {
            Enemy.Z[i].ZposX = 0, Enemy.Z[i].ZposY = 0;
            Enemy.Z[i].Zsymbol = '#';
            SaveFile << Enemy.Z[i].ZposX << " " << Enemy.Z[i].ZposY << " "
                     << Enemy.Z[i].Zsymbol << " \n";
        }
        else
        {
            SaveFile << Enemy.Z[i].ZposX << " " << Enemy.Z[i].ZposY << " "
                     << Enemy.Z[i].Zsymbol << " \n";
        }
    }

    // Save Board
    SaveFile << GameMap.DimY << " " << GameMap.DimX << '\n';
    for (int i = 0; i < GameMap.DimY; i++)
    {
        for (int j = 0; j < GameMap.DimX; j++)
        {
            if (GameMap.board[i][j] == ' ')
            {
                SaveFile << "#"
                         << " ";
            }
            else
            {
                SaveFile << GameMap.board[i][j] << " ";
            }
        }
        SaveFile << "\n";
    }

    // Save Alien

    SaveFile << Player.AlienName << "\n";

    // Save Zombies
    for (int z = 0; z < Enemy.ZombieAmount; z++)
    {
        SaveFile << Enemy.Z[z].Ztype << "\n"
                 << Enemy.Z[z].Zbio << "\n";
    }

    SaveFile.close();
    std::cout << "Data has been saved to file " << SaveName << std::endl;
}

void Load(Map &GameMap, Alien &Player, Zombie &Enemy)
{
    std::ifstream LoadFile;
    std::string LoadName;

    std::cout << "Enter the name of your save file => ";
    std::getline(std::cin >> std::ws, LoadName);

    LoadFile.open(LoadName);
    if (LoadFile.fail())
    {
        perror("Load Error");
        return;
    }

    // int Data
    LoadFile >> Player.AlienHealth >> Player.AlienAttack >> Player.MaxAlienHealth >> Player.TempAlienAttack >> Player.PodAttack >> Player.ArrowAttack >> Player.posX >> Player.posY >> Player.AlienCode;
    LoadFile >> Enemy.ZombieAmount >> Enemy.ZombieStillAlive;
    for (int i = 0; i < Enemy.ZombieAmount; i++)
    {
        LoadFile >> Enemy.Z[i].ZHealth >> Enemy.Z[i].ZAttack >> Enemy.Z[i].ZHealthMax >> Enemy.Z[i].ZRange >> Enemy.Z[i].ZposX >> Enemy.Z[i].ZposY;
        char ch;
        LoadFile >> ch;
        if (ch == '#')
        {
            Enemy.Z[i].Zsymbol = ' ';
        }
        else
        {
            Enemy.Z[i].Zsymbol = ch;
        }
    }

    // Load Board
    LoadFile >> GameMap.DimY >> GameMap.DimX;
    GameMap.board.resize(GameMap.DimY);
    for (int i = 0; i < GameMap.DimY; ++i)
    {
        GameMap.board[i].resize(GameMap.DimX);
        for (int j = 0; j < GameMap.DimX; ++j)
        {
            char ch;
            LoadFile >> ch;
            if (ch == '#')
            {
                GameMap.board[i][j] = ' ';
            }
            else
            {
                GameMap.board[i][j] = ch;
            }
        }
    }

    // Load Alien
    Player.AlienName.clear();

    LoadFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(LoadFile, Player.AlienName);

    for (int z = 0; z < Enemy.ZombieAmount; z++)
    {
        Enemy.Z[z].Ztype.clear();
        Enemy.Z[z].Zbio.clear();
        std::getline(LoadFile, Enemy.Z[z].Ztype);
        std::getline(LoadFile, Enemy.Z[z].Zbio);
    }

    LoadFile.close();
    std::cout << "Save file " << LoadName << " has been loaded." << std::endl;
}

int main()
{
    // srand(64);
    srand(time(NULL));

    std::ofstream Savefile;
    std::ifstream LoadFile;
    Map map;
    Alien alien;
    Zombie zombie;
    char Input;

    map.DimX = 9, map.DimY = 5;
    zombie.ZombieAmount = 3;
    zombie.ZombieStillAlive = zombie.ZombieAmount;
    alien = CreateAlien(1); // default values

    bool done = true;
    do
    {
        displayStartMenu();

        std::cin.clear();
        std::cin.sync();
        std::cin >> Input;
        switch (Input)
        {
        case '1':
            StartGame(alien, map, zombie);
            break;
        case '2':
            map.SetDimensions();
            break;
        case '3':
            alien = ChooseAlien();
            break;
        case '4':
            zombie.NoOfZombies();
            break;
        case '5':
            QuitConfirm();
            break;
        default:
            std::cout << "Invalid Input.\n"
                      << std::endl;
            break;
        }
        std::cout << std::endl;
        myPause();
    } while (done);
}






// We're done.
