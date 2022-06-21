#include <iostream>
#include <cstring>
#include <windows.h>
#include <cstdlib>

// Карта отрисовывается             ++
// Змея движется                    ++
// Хвостт змеи отрисовывается       ++
// Хвост змеи движется за змеёй     ++
// Хвост змеи увеличивается         ++


#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#define mLength 25 //y
#define mWidth 80 //x
#define sLen 200

struct TMap{
    char map[mLength][mWidth+1];
    void ClearMap();
    void Show ();
};

enum TDirection{dirLeft, dirRigth, dirUp, dirDown};
enum TResult {reOk, reKill};

class TSnake{
    int x, y;
    TMap *mp;
    TDirection dir;
    POINT tail[sLen];
    int len;
public:
    void AddTail(int _x, int _y);
    void AddTailCnt(int _x, int _y, int cnt);
    void MoveTail(int _x, int _y);

    TSnake(TMap *_mp);
    void InitSnake(int _x, int _y,TDirection _dir);
    void PutSnake();
    TResult UserControl(char w, char s, char a, char d);
};


int main (){

    HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE); //спрятать курсор
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);

    int userCnt = 2;
    TMap m1;
    TSnake snake1(&m1);
    TSnake snake2(&m1);
    snake1.InitSnake(5,5,dirRigth);//x,y
    snake2.InitSnake(60,5,dirLeft);
    do{
        if (snake1.UserControl('W', 'S', 'A', 'D') == reKill || 
        ( (userCnt >= 2) && (snake2.UserControl(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT)) == reKill)){
            snake1.InitSnake(5,5,dirRigth);//x,y
            snake2.InitSnake(60,5,dirLeft);
        }
        m1.ClearMap();
        snake1.PutSnake();
        if (userCnt >= 2) 
            snake2.PutSnake();
        m1.Show();
        Sleep(100);
    }
    while(GetKeyState (VK_ESCAPE) >= 0);
    system("pause");
}

void MoveXY(int x, int y){
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void TMap::ClearMap(){
    for (int i = 0; i < mWidth; i++)
        map[0][i] = ' ';
    map[0][mWidth] = '\0';
    for(int i = 1; i < mLength; i++)
        strncpy(map[i], map[0], mWidth+1); 

    
}

void TMap::Show(){
    map[mLength-1][mWidth-1] = '\0';

    map[12][20] = '*';
    map[8][20] = '*';

    for (int i = 0; i < mLength; i++)
        cout << map[i]<< endl;  
    MoveXY(0,0);
}

TSnake::TSnake(TMap *_mp){
    mp = _mp;
    InitSnake(0, 0, dirRigth);
}

void TSnake::InitSnake(int _x, int _y,TDirection _dir){
    x = _x;
    y = _y;
    dir = _dir;
    len = 0;
}

void TSnake::PutSnake(){
    mp->map[y][x] = '@';
    for (int i = 0; i < len; i++)
        mp->map[tail[i].y][tail[i].x] = '+';
}

TResult TSnake::UserControl(char w, char s, char a, char d){
    POINT old;
    old.x = x;
    old.y = y;
    if (dir == dirUp){
        if (GetKeyState(a) < 0) dir = dirLeft;
        if (GetKeyState(d) < 0) dir = dirRigth;
    }
    if (dir == dirDown){ 
        if (GetKeyState(a) < 0) dir = dirLeft;
        if (GetKeyState(d) < 0) dir = dirRigth;
    }
    if (dir == dirLeft){
        if (GetKeyState(w) < 0) dir = dirUp;
        if (GetKeyState(s) < 0) dir = dirDown;
    }
    if (dir == dirRigth){
        if (GetKeyState(w) < 0) dir = dirUp;
        if (GetKeyState(s) < 0) dir = dirDown;
    }


    if (dir == dirUp) y--;
    if (dir == dirDown) y++;
    if (dir == dirLeft) x--;
    if (dir == dirRigth) x++;

    if (x >= mWidth || x<0 || y<0 || y>= mLength || mp->map[y][x] =='+')
        return reKill;

    if (mp->map[y][x] == '*')   
        AddTailCnt(old.x, old.y, 5);
    else 
        MoveTail(old.x, old.y);

    return reOk;
}

void TSnake::MoveTail(int _x, int _y){
    for(int i = len - 1; i >= 0; i--)
        tail[i+1]=tail[i];
    tail[0].x = _x;
    tail[0].y = _y;
}   
void TSnake::AddTail(int _x, int _y){
    MoveTail(_x, _y);
    len++;
    if(len >= sLen) len = sLen - 1;
}
void TSnake::AddTailCnt(int _x, int _y, int cnt){
    for (int i = 0; i < cnt; i++){
        AddTail(_x,_y);
    }
}
