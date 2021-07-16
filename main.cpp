#include <iostream>
#include <cstring>
#include <windows.h>
#include <time.h>
#include <cstdlib>
#include <fstream>

#define GET_CURSOR_POS GetConsoleCursorPosition()
#define FLUSH fflush(stdin) // Flushes the standard input
#define T "\n\t\t\t\t" // Prints 4 tabs with 1 newline
#define CLS system("cls") // Clears everything on the console

#define BOARD \
{\
    CLS;\
    showBoard();\
    GET_CURSOR_POS;\
    gotoxy(100, 5);\
    cout << char(219) << " - " << name1;\
    gotoxy(100, 7);\
    cout << char(178) << " - " << name2;\
    gotoxy(100, 11);\
    cout << "S - Start";\
    gotoxy(100, 13);\
    cout << "F - Finish";\
    gotoxy(100, 15);\
    cout << char(202) << " - Ladder Bottom";\
    gotoxy(100, 17);\
    cout << char(203) << " - Ladder Top";\
    gotoxy(100, 19);\
    cout << char(245) << " - Snake Mouth";\
    gotoxy(100, 21);\
    cout << char(244) << " - Snake Tail";\
    gotoxy(x, y);\
}

using namespace std;

class Board
{
private:
    char board[31][82];
public:
    Board(); // Initializes the board
    void showBoard(); // Prints the board
    void editBoard(int, char, int); // Edits the board by changing each player's position after die is thrown

    // Class containing elements of the board
    class elements
    {
    private:
        int *ladders, *snakes;
    public:
        elements(); // Initializes snakes and ladders on the board
        void setNumbers(char[31][82]); // Prints the numbers on the board
        void setLaddersAndSnakes(char[31][82]); // Prints ladders and snakes on the board

        friend class Game;
    };
    elements e;
};

// Class containing player information
class Player
{
private:
    char name[21];
    static int i;
    int position;
public:
    Player(); // Initializes the default name
    void setName(const char*); // Sets the player name
    char* getName(); // Gets the player name
    void loadName(const char*);
    void setPosition(int);

    friend class Game;
};

int Player :: i = 0;

// Class containing actual game mechanism :D
class Game : public Board
{
private:
    int roll;
public:
    void play(Player*, int = 0); // Main part of the game
    int die(); // Returns the number on the die
    int checkSnakeOrLadder(int, char); /* Returns the new position if either snake or ladder is found,
                                    otherwise returns original position.
                                    Also returns 100 if position > 100 */
    int checkSnakeOrLadder(int);
    int PROMPT(int, int, int);
    int PROMPT(int);
};

COORD destCoord;

int x, y;
char name1[21], name2[21];

void gotoxy(int, int);
void GetConsoleCursorPosition();
int wherex();
int wherey();
int row(int); // Converts the number on the board to the corresponding row of the board
int column(int); // Converts the number on the board to the corresponding column of the board
void saveGame(int, int, int);
int* loadGame();
int menu(int);

int main()
{
    while(menu(0)); // 0 indicates default main menu
    return 0;
}

int menu(int type)
{
    int ch, *pos;
    CLS;
    cout << "\n\t      #######################################################################";
    cout << "\n\t      #\t\t\t\t\t\t\t\t\t    #\n\t      #    ----------------WELCOME TO SNAKES AND LADDERS----------------    #" << endl;
    cout << "\t      #\t\t\t\t\t\t\t\t\t    #\n\t      #\t\t\t\t\t\t\tMade by: Ajay Phade #";
    cout << "\n\t      #\t\t\t\t\t\t\t\t\t    #\n\t      #######################################################################";
    if(type == 0)
        cout << "\n\n\t\t\t   -------MAIN MENU-------\n" << T << "1. New Game" << T << "2. Load Game" <<  T << "3. Exit" << T << "Choose: ";
    else
        cout << "\n\n\t\t\t   -------MAIN MENU-------\n" << T << "1. New Game (You will lost current progress)" << T << "2. Continue" << T << "3. Exit" << T << "Choose: ";
    cin >> ch;
    if(type == 1)
    {
        if(ch == 1)
            return 1;
        else if(ch == 2)
        {
            FLUSH;
            return 0;
        }
        else
            exit(0);
    }
    switch(ch)
    {
    case 1:
    {
        CLS;
        Game g;
        Player p[2];
        cout << T << "Enter Player 1 Name (Max. 20 characters): ";
        p[0].setName("----");
        strcpy(name1, p[0].getName());
        cout << T <<"Name: " << p[0].getName() << endl;
        cout << T << "Enter Player 2 Name (Max. 20 characters): ";
        p[1].setName(p[0].getName());
        strcpy(name2, p[1].getName());
        cout << T <<"Name: " << p[1].getName() << endl;
        g.play(p);
    }
    break;

    case 2:
    {
        pos = loadGame();
        if(pos == 0)
            return 1;
        CLS;
        Game g;
        Player p[2];
        p[0].loadName(name1);
        p[1].loadName(name2);
        p[0].setPosition(pos[0]);
        p[1].setPosition(pos[1]);
        g.editBoard(1, char(219), 1);
        g.editBoard(1, char(178), 1);
        g.editBoard(pos[0], char(219), 0);
        g.editBoard(pos[1], char(178), 0);
        g.play(p, pos[2]);
    }
    break;

    case 3:
    default:
        exit(0);
    }
    return 1;
}

Board :: Board()
{
    int i, j, row = 0, column = 0;

    // Upper Part
    board[row][column] = char(201);
    column++;
    for(i = 1; i < 80; i++)
    {
        if(i % 8 == 0)
            board[row][column] = char(203);
        else
            board[row][column] = char(205);
        column++;
    }
    board[row][column] = char(187);
    column++;
    board[row][column] = '\n';
    column++;
    board[row][column] = '\0';
    column = 0;
    row++;

    // Middle Part
    for(i = 1; i < 30; i++)
    {
        if(i % 3 == 0)
        {
            board[row][column] = char(204);
            column++;
            for(j = 1; j < 80; j++)
            {
                if(j % 8 == 0)
                    board[row][column] = char(206);
                else
                    board[row][column] = char(205);
                column++;
            }
            board[row][column] = char(185);
            column++;
            board[row][column] = '\n';
            column++;
            board[row][column] = '\0';
            column = 0;
            row++;
        }
        else
        {
            board[row][column] = char(186);
            column++;
            for(j = 1; j < 80; j++)
            {
                if(j % 8 == 0)
                    board[row][column] = char(186);
                else
                    board[row][column] = ' ';
                column++;
            }
            board[row][column] = char(186);
            column++;
            board[row][column] = '\n';
            column++;
            board[row][column] = '\0';
            column = 0;
            row++;
        }
    }

    // Lower Part
    board[row][column] = char(200);
    column++;
    for(i = 1; i < 80; i++)
    {
        if(i % 8 == 0)
            board[row][column] = char(202);
        else
            board[row][column] = char(205);
        column++;
    }
    board[row][column] = char(188);
    column++;
    board[row][column] = '\n';
    column++;
    board[row][column] = '\0';
    board[row - 1][4] = 'S';
    board[2][4] = 'F';
    board[29][3] = char(219);
    board[28][3] = char(178);
    board[3][0] = '1';
    board[3][1] = '0';
    board[3][2] = '0';
    e.setNumbers(board);
    e.setLaddersAndSnakes(board);
}

void Board :: showBoard()
{
    int i, j;
    for(i = 0; i < 31; i++)
    {
        cout << "\t\t";
        for(j = 0; j < 82; j++)
            cout << board[i][j];
    }
}

void Board :: editBoard(int position, char icon, int previous)
{
    x = column(position) - 2;
    y = row(position);
    if(icon == char(178))
        y -= 1;
    if(previous)
        board[y][x] = ' ';
    else
        board[y][x] = icon;
}

Board :: elements :: elements()
{
    static int l[14] = {4, 7, 24, 33, 74, 68, 80, 22, 44, 57, 51, 90, 82, 94};
    static int s[14] = {40, 25, 50, 60, 81, 92, 95, 20, 8, 16, 23, 78, 52, 36};
    ladders = l;
    snakes = s;
}

void Board :: elements :: setNumbers(char board[31][82])
{
    int i, j = 1;
    char s[3];
    x = 0, y = 3;
    x += 8;
    for(i = 99; i >= 1; i--)
    {
        if(i % 10 == 0)
        {
            j *= -1;
            x += 8 * j;
            y += 3;
        }
        sprintf(s, "%d", i);
        board[y][x] = s[0];
        if(i >= 10)
            board[y][x + 1] = s[1];
        x += 8 * j;
    }
}

void Board :: elements :: setLaddersAndSnakes(char board[31][82])
{
    int i;
    for(i = 0; i < 7; i++)
    {
        // Ladder bottom
        x = column(ladders[i]);
        y = row(ladders[i]);
        board[y][x] = char(202);
        board[y][x + 1] = i + 1 + 48;

        // Ladder top
        i += 7;
        x = column(ladders[i]);
        y = row(ladders[i]);
        board[y][x] = char(203);
        board[y][x + 1] = i - 6 + 48;
        i -= 7;

        // Snake mouth
        x = column(snakes[i]);
        y = row(snakes[i]);
        board[y][x] = char(245);
        board[y][x + 1] = i + 1 + 48;

        // Snake tail
        i += 7;
        x = column(snakes[i]);
        y = row(snakes[i]);
        board[y][x] = char(244);
        board[y][x + 1] = i - 6 + 48;
        i -= 7;
    }
}

Player :: Player()
{
    position = 1;
    i++;
    char s[3] = {'_', char(i + 48), '\0'};
    strcpy(name, "Player");
    strcat(name, s);
}

void Player :: setName(const char* s)
{
    char temp[21];
    FLUSH;
    fgets(temp, 21, stdin);
    if(temp[strlen(temp) - 1] == '\n')
        temp[strlen(temp) - 1] = '\0';
    if(!strcmp(temp, ""))
        return;
    else if(!strcmp(temp, s))
    {
        cout << T << "Names must be different" << endl;
        return;
    }
    strcpy(name, temp);
}

char* Player :: getName()
{
    return name;
}

void Player :: loadName(const char* s)
{
    strcpy(name, s);
}

void Player :: setPosition(int p)
{
    position = p;
}

void Game :: play(Player* p, int i)
{
    char icon[2] = {char(219), char(178)};
    cout << T << "Press Enter to Play";
    getchar();
    BOARD;
    while(p[0].position < 100 && p[1].position < 100)
    {
        cout << T << p[i].getName() << "'s Turn: (Press Enter to Roll the Die)";
        getchar();
        cout << T << "Rolling the Die...." << T;
        roll = die();
        editBoard(p[i].position, icon[i], 1); // To remove current position from the board 1 is passed
        p[i].position += roll;
        if(p[i].position == checkSnakeOrLadder(p[i].position) && roll != 6)
        {
            if(PROMPT(p[0].position, p[1].position, i) == 1) // Indicates new game
                return;
        }
        else if(PROMPT(0) == 1)
            return;
        p[i].position = checkSnakeOrLadder(p[i].position, icon[i]); // Updates the position if either snake or ladder is found
        editBoard(p[i].position, icon[i], 0); // To update the player to new position on the board 0 is passed
        BOARD;
        while(roll == 6)
        {
            cout << T << "It's 6 !!!!" << T << "Again " << p[i].getName() << "'s Turn: (Press Enter to Roll the Die)";
            getchar();
            cout << T << "Rolling the Die Again...." << T;
            roll = die();
            editBoard(p[i].position, icon[i], 1); // To remove current position from the board 1 is passed
            p[i].position += roll;
            if(p[i].position == checkSnakeOrLadder(p[i].position) && roll != 6)
            {
                if(PROMPT(p[0].position, p[1].position, i) == 1) // Indicates new game
                    return;
            }
            else if(PROMPT(0) == 1)
                return;
            p[i].position = checkSnakeOrLadder(p[i].position, icon[i]); // Updates the position if either snake or ladder is found
            editBoard(p[i].position, icon[i], 0); // To update the player to new position on the board 0 is passed
            BOARD;
        }
        i = (i + 1) % 2;
    }
    i = (i + 1) % 2;
    cout << T << "Congratulations " << p[i].getName() << char(7) << ". You Won the Game !!!!";
    cout << T T << "Press Enter to go to Main Menu: ";
    getchar();
}

int Game :: die()
{
    int i;
    srand(time(0));
    roll = rand() % 6;
    if(roll == 0)
        roll = 6;
    for(i = 0; i < 9; i++)
        cout << char(220);
    cout << T << char(219)<< "       " << char(219);
    cout << T << char(219)<< "   " << roll << "   " << char(219);
    cout << T << char(219)<< "       " << char(219) << T;
    for(i = 0; i < 9; i++)
        cout << char(223);
    return roll;
}

int Game :: checkSnakeOrLadder(int position, char icon)
{
    if(position > 99)
        return 100;
    int new_position = position, i;
    for(i = 0; i < 7; i++)
    {
        if(e.ladders[i] == position)
        {
            new_position = e.ladders[i + 7];
            editBoard(position, icon, 0);
            BOARD;
            cout << T << "Congratulations !!!! You Climbed Up the Ladder to Position: " << new_position << endl;
            PROMPT(0);
            editBoard(position, icon, 1);
            break;
        }
        if(e.snakes[i] == position)
        {
            new_position = e.snakes[i + 7];
            editBoard(position, icon, 0);
            BOARD;
            cout << T << "Oops !!!! Snake Ate You to Position: " << new_position << endl;
            PROMPT(0);
            editBoard(position, icon, 1);
            break;
        }
    }
    return new_position;
}

int Game :: checkSnakeOrLadder(int position)
{
    if(position > 99)
        return 100;
    int new_position = position, i;
    for(i = 0; i < 7; i++)
    {
        if(e.ladders[i] == position)
        {
            new_position = e.ladders[i + 7];
            break;
        }
        if(e.snakes[i] == position)
        {
            new_position = e.snakes[i + 7];
            break;
        }
    }
    return new_position;
}

int Game :: PROMPT(int position1, int position2, int i)
{
    i = (i + 1) % 2; // Indicates next player
    cout << T << "1. Back" << T << "2. Save Game and Exit" << T << "Choose OR Just Press Enter to Continue: ";
    switch(getchar())
    {
    case '1':
        return(menu(1));
        break;

    case '2':
        saveGame(position1, position2, i);
        exit(0);
    }
    return 0;
}

int Game :: PROMPT(int)
{
    cout << T << "1. Back" << T << "Choose OR Just Press Enter to Continue: ";
    switch(getchar())
    {
    case '1':
        return(menu(1));
        break;
    }
    return 0;
}

int row(int number)
{
    int c;
    c = number / 10;
    if(number % 10 == 0)
        c--;
    return 3 * (10 - c) - 1;
}

int column(int number)
{
    int r;
    if(number % 20 > 10)
        r = 11 - number % 10;
    else
        r = number % 10 == 0 ? 1 : number % 10;
    if(number % 20 == 10)
        return 82 - 5 + 8 * (r - 1);
    return 5 + 8 * (r - 1);
}

void saveGame(int p1, int p2, int i)
{
    ofstream save_file;
    save_file.open("save.txt");
    save_file << name1 << " " << name2 << " " << p1 << " " << p2 << " " << i << endl;
}

int* loadGame()
{
    ifstream save_file;
    static int data[3];
    save_file.open("save.txt");
    save_file >> name1 >> name2 >> data[0] >> data[1] >> data[2];
    try
    {
        if(!save_file.is_open() || !strcmp(name1, ""))
            throw "No Saved Game Present !!!!";
    }
    catch(const char* s)
    {
        cout << T << s;
        Sleep(2000);
        return 0;
    }
    return data;
}

void gotoxy(int x, int y)
{
    destCoord.X = x;
    destCoord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), destCoord);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return -1;
    return csbi.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return -1;
    return csbi.dwCursorPosition.Y;
}

void GetConsoleCursorPosition()
{
    x = wherex();
    y = wherey();
}
