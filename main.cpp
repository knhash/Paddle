#define _WIN32_WINNT 0x0500
#include <iostream>
#include<windows.h>
#include<string.h>
#include<conio.h>
#include <limits.h>
#include<time.h>

using namespace std;

//GLOBAL VARIABLES
int chnc=5;int dir=1;int h=77;int scr=0;char ch;int speed=60;string pad;
int pads;int dif;int mx,my;


int main();


//CLASSES AND OBJECTS
class location
{
        public:

                short x;
                short y;

                void Pinit(int a,int b)
                {
                        x=a;
                        y=b;
                }
};
location ball,paddle,oppo;


//FUNCTIONS
void gotoxy(int x, int y)               //Changing cursor co-ordinates
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setcursor(bool visible,DWORD size) //Removes blinking cursor
{
        HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
        if(size==0)
        {
                size=20;
        }
        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible=visible;
        lpCursor.dwSize=size;
        SetConsoleCursorInfo(console,&lpCursor);
}

void mousepos()
{
    // Get screen frame rate, as there's no point going any faster
    DWORD frameRate = 0;

    DEVMODEA devMode = {0};
    devMode.dmSize = sizeof(DEVMODE);
    if(EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &devMode))
        frameRate = devMode.dmDisplayFrequency;
    else
        frameRate = 60; // default value if fail to get actual value

    // get our window
    HWND hWnd = GetConsoleWindow();

    // to remember where we were last time
    POINT posLast = {LONG_MIN, LONG_MIN};

    while(1)
    {
        // get current mouse position in screen coords
        POINT pos = {0, 0};
        if (GetCursorPos(&pos))
        {
            // convert position to client window coords
            if (ScreenToClient(hWnd, &pos))
            {
                // get window's client rect
                RECT rcClient = {0};
                GetClientRect(hWnd, &rcClient);

                // if mouse cursor is inside rect
                if (PtInRect(&rcClient, pos))
                {
                    // and if we've moved since last time
                    if ((posLast.x != pos.x) || (posLast.y != pos.y))
                    {
                        // get size and position of console window/buffer in rows and columns
                        CONSOLE_SCREEN_BUFFER_INFO info = {0};
                        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
                        const SMALL_RECT& smRect = info.srWindow;

                        // calculate row and column corresponding to mouse position
                        // (note this takes accounts of console buffer scrolling)
                        mx = smRect.Left + (pos.x * (smRect.Right  - smRect.Left))
                                / (rcClient.right  - rcClient.left);
                        my = smRect.Top  + (pos.y * (smRect.Bottom - smRect.Top ))
                                / (rcClient.bottom - rcClient.top );

                        // move to new position and output 'o'
                        /*gotoxy(60,20);
                        cout << mx <<':'<< my <<" ";*/

                        //paddle x
                        if((mx-pads/2)>0 && (mx+pads/2)<79)
                        paddle.x=mx-pads/2;
                        break;

                        // update title bar
                        char title[64] = "";
                        wsprintfA(title, "Shank Test - (%d, %d)", mx, my);
                        SetConsoleTitleA(title);

                        // and last position
                        posLast = pos;
                    }
                }
                else
                {
                    // update title bar (for testing purposes) to know we don't care
                    // where the mouse is at the moment
                    SetConsoleTitleA("(out of window)");
                }

                // wait a short while
                Sleep(1000 / frameRate);
            }
        }
    }
}

void Pborder()                           //Custom Border
{
    int c,r;
    //HORIZONTAL
    for(c=0;c<80;c++)
    {
        gotoxy(c,0);
        cout<<'%';
        gotoxy(c,2);
        cout<<'%';
        gotoxy(c,24);
        cout<<'%';
    }

    //VERTICAL
    for(r=0;r<24;r++)
    {
        gotoxy(0,r);
        cout<<'%';
        gotoxy(79,r);
        cout<<'%';
    }
}

void Pclear(int a=0)                     //Custom Clear Screen
{
    if(a==0)
    {
        //CLEAR EVERYTHING EXCEPT BORDER
        for(int i=1;i<79;++i)
            for(int k=1;k<24;++k)
            {
                if(k==2)
                    continue;
                gotoxy(i,k);
                cout<<"K";
                Sleep(.5);
            }

        for(int i=1;i<79;++i)
            for(int k=1;k<24;++k)
            {
                if(k==2)
                    continue;
                gotoxy(i,k);
                cout<<" ";
                Sleep(.5);
            }
    }
}

void Ppadsize()                           //Different for different difficulties
{
    switch(dif)
    {
        case 1: pad="<===========>";
        pads=13;
        break;

        case 2: pad="<=======>";
        pads=9;
        break;

        case 3: pad="<===>";
        pads=5;
        break;

    }
}

void Pdifficulty(void)                    //Difficulty Selection
{
    do
    {
      Pclear(); // Custom Clear Screen

      gotoxy(0, 0);

      gotoxy(35,6);cout << "Difficulty : " << endl << endl;
      gotoxy(35,8);cout << "(1)... Easy " << endl;
      gotoxy(35,9);cout << "(2)... Medium " << endl;
      gotoxy(35,10);cout << "(3)... Hard " << endl << endl;
      gotoxy(35,12);

      if (!(cin >> dif)) // If player enters something besides a number, cin.clear() & cin.ignore() will be called
      {
         cin.clear();               //Clear Error Flag
         cin.ignore(10000, '\n');   //Ignore next 10000 char or till '\n' is reached
      }
    }
    while(dif != 1 && dif != 2 && dif != 3 && dif != 9);

    Pclear();

}

void Pinit()                              //Initialisation
{
    Pborder();
    gotoxy(35,12);
    cout<<"PADDLE!!!...";
    Sleep(1000);
    Pdifficulty();
    Ppadsize();

    switch(dif)
    {
    case 1:
        ball.Pinit(31,22);
        break;
    case 2:
        ball.Pinit(29,22);
        break;
    case 3:
        ball.Pinit(27,22);
        break;
    }

    paddle.Pinit(25,23);
    oppo.Pinit(29,3);

    // DISPLAY JUST NIL SCORE IN THE START OF THE GAME
    gotoxy(1,1);
    cout<<"              ";
    gotoxy(1,1);
    cout<<"SCORE :";
    gotoxy(9,1);
    cout<<"000000";
    Sleep(250);

    //CHANCE LEFT
    gotoxy(37,1);
    cout<<"               ";
    gotoxy(37,1);
    cout<<"CHANCE : "<<chnc;
    Sleep(250);

    gotoxy(63,1);
    cout<<"'DOWN' FOR PAUSE";
    Sleep(500);

    //clear();

    //DISPLAY JUST PADDLE AND BALL AND OPPO IN STARTING OF THE GAME
    gotoxy(ball.x,ball.y);
    cout<<'o';
    gotoxy(paddle.x,paddle.y);
    cout<<pad;
    gotoxy(oppo.x,oppo.y);
    cout<<"<===>";


    ch=getch();
    if(GetKeyState(0x25)<-100)
        dir=2;
    if(GetKeyState(0x27)<-100)
        dir=1;

}

void Phitoppo()                            //Opponent Collision
{

    ball.y=4;
    if(dir==2)
        dir=3;
    if(dir==1)
        dir=4;
}

void Prebound()                            //Rebound conditions for walls
{
    //REBOUND
        if(ball.x==0)//LEFT WALL
        {
            ball.x=1;
            if(dir==2)
                dir=1;
            if(dir==3)
                dir=4;
        }

        if(ball.x==79)//RIGHT WALL
        {
            ball.x=78;
            if(dir==1)
                dir=2;
            if(dir==4)
                dir=3;
        }

        if((ball.y==3)&&(ball.x>=oppo.x)&&(ball.x<=oppo.x+9))
            Phitoppo();//CONDITION IF BALL HITS OPPO

        /*if(ball.y==1)//TOP WALL
        {
            ball.y=2;
            if(dir==2)
                dir=3;
            if(dir==1)
                dir=4;
        }*/
}

void Phitpaddle()                          //Paddle Collision and direction changes
{
        ball.y=23;
        if(dir==3)
            dir=2;
        if(dir==4)
            dir=1;
}

void Pgameover()                           //Gameover
{
    Pclear();
    gotoxy(35,15);
    cout<<"!GAME OVER!";
    gotoxy(30,18);
    cout<<"YOUR TOTAL SCORE : "<<scr-1;
    ch=getch();
    gotoxy(27,21);
    system("PAUSE");
    exit(0);
}

void Pmisspaddle()                         //End of Game Screen
{
    //DEDUCTION OF CHANCES
    //CONDITION IF CHANCE LEFT = 0
    if(chnc==0)
        Pgameover();

    chnc--;


    loopchance:
    //LOOP FOR ANOTHER CHANCE OR QUIT
    gotoxy(14,10);
    cout<<"FOR ANOTHER CHANCE PRESS 'R'  OR TO QUIT PRESS'Q'";
    ch=getch();

    // IF THE USER TYPE 'Q'AT TIME OF CHOICE
    if((ch=='q')||(ch=='Q'))
    //ERASING AND QUITTING
    {
        gotoxy(33,11);
        cout<<"               ";
        Pgameover();
    }
    //IF THE USER TYPE 'R' AT TIME OF CHOICE
    if((ch=='r')||(ch=='R'))
    //RESTART THE GAME
    {
        Pclear();
        gotoxy(14,10);
        cout<<"                                                 ";
        gotoxy(33,11);
        cout<<"               ";
        gotoxy(1,1);
        cout<<"              ";
        gotoxy(1,1);
        cout<<"SCORE :";
        gotoxy(9,1);
        cout<<"000000";
        gotoxy(37,1);
        cout<<"               ";
        gotoxy(37,1);
        cout<<"CHANCE : "<<chnc;
        gotoxy(63,1);
        cout<<"'DOWN' FOR PAUSE";

        //RE-ASSIGN ALL THE VALUES
        ball.x=30;ball.y=22;
        paddle.x=25;
        oppo.x=29;

        //WHEN ANOTHER CHANCE STARTS DISPLAY BAT AND BALL AND OPPO
        gotoxy(ball.x,ball.y);
        cout<<'o';
        gotoxy(paddle.x,23);
        cout<<pad;
        gotoxy(oppo.x,oppo.y);
        cout<<"<===>";

        ch=getch();
        if(GetKeyState(0x25)<-100)
            dir=2;
        if(GetKeyState(0x27)<-100)
            dir=1;
    }

    else
        goto loopchance;

}

void Pstop()                               //Pause screen
{

    while(ch!='r'&&ch!='R')
    {

            gotoxy(63,20);
            cout<<"                 ";
            gotoxy(63,20);
            cout<<"RESUME...'UP' ";
            if(GetKeyState(0x26)<-100)
            {
                gotoxy(63,20);
                cout<<"'DOWN' FOR PAUSE";
                break;
            }
            else if(GetKeyState(0x1B)<-100)
            {
                Pgameover();
                break;
            }
    }
}

void Paddle()                               //Main game loop
{
    INPUT_RECORD InRec;
    while(!(GetKeyState(0x1B)<-100))
    {
        while((ball.x>0)&&(ball.x<79)&&(ball.y>3)&&(ball.y<24))
        {
            //OPPO MOVEMENT AI
            if((ball.x-2)<75&&(ball.x-2)>0)
                oppo.x=ball.x-2;

            //SHOW BALL
            gotoxy(ball.x,ball.y);
            cout<<'o';

            //SCORE++
            gotoxy(14,1);
            cout<<scr;
            scr++;

            //SHOW BAT
            gotoxy(paddle.x,paddle.y);
            cout<<pad;

            //SHOW OPPO
            gotoxy(oppo.x,oppo.y);
            cout<<"<===>";
            Sleep(speed);

            //REMOVE BALL
            gotoxy(ball.x,ball.y);
            cout<<' ';

            switch(dif)//REMOVE BAT//REMOVE OPPO
            {
            case 1: gotoxy(paddle.x,paddle.y);
                    cout<<"             ";
                    gotoxy(oppo.x,oppo.y);
                    cout<<"     ";
                    break;
            case 2: gotoxy(paddle.x,paddle.y);
                    cout<<"         ";
                    gotoxy(oppo.x,oppo.y);
                    cout<<"     ";
                    break;
            case 3: gotoxy(paddle.x,paddle.y);
                    cout<<"     ";
                    gotoxy(oppo.x,oppo.y);
                    cout<<"     ";
                    break;
            }

            if(GetKeyState(0x28)<-100)
                Pstop();


            //PADDLE DIRECTION
            if(GetKeyState(0x25)<-100)
                if(paddle.x>1)
                    paddle.x-=3;
            if (GetKeyState(0x27)<-100)
            {
                if(dif==1)
                if(paddle.x<66)
                    paddle.x+=3;

                if(dif==2)
                if(paddle.x<70)
                    paddle.x+=3;

                if(dif==3)
                if(paddle.x<72)
                    paddle.x+=3;
            }
            //MOUSE CONTROLS
            if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
            {
                mousepos();

            }

            //SCORE POS
            if(scr==10)
                h--;
            if(scr==100)
                h--;
            if(scr==1000)
                h--;
            if(scr==10000)
                h--;

            if(scr%150==0)
                speed-=5;    //INCREASE PADDLE SPEED

            // DIRECTIONS
            if(dir==1)
                {ball.x++;ball.y--;}//UP RIGHT
            if(dir==2)
                {ball.x--;ball.y--;}//UP LEFT
            if(dir==3)
                {ball.x--;ball.y++;}//DOWN LEFT
            if(dir==4)
                {ball.x++;ball.y++;}//DOWN RIGHT
        }

        Prebound();

        switch(dif)
        {
            case 1: if((ball.y==24)&&(ball.x>=paddle.x)&&(ball.x<=paddle.x+13))
                    Phitpaddle();
                    break;
            case 2: if((ball.y==24)&&(ball.x>=paddle.x)&&(ball.x<=paddle.x+9))
                    Phitpaddle();
                    break;
            case 3: if((ball.y==24)&&(ball.x>=paddle.x)&&(ball.x<=paddle.x+5))
                    Phitpaddle();
                    break;
        }

        switch(dif)
        {
            case 1: if((ball.y==24)&&((ball.x<paddle.x)||(ball.x>paddle.x+13)))
                    Pmisspaddle();//CONDITION IF BALL DOESN'T HIT THE BAT
                    break;
            case 2: if((ball.y==24)&&((ball.x<paddle.x)||(ball.x>paddle.x+9)))
                    Pmisspaddle();//CONDITION IF BALL DOESN'T HIT THE BAT
                    break;
            case 3: if((ball.y==24)&&((ball.x<paddle.x)||(ball.x>paddle.x+5)))
                    Pmisspaddle();//CONDITION IF BALL DOESN'T HIT THE BAT
                    break;
        }
    }
}

void Pmenu()                                 //MENU INTERFACE
{
    int x=0,    y=0;
    do
    {
        if((GetKeyState(VK_LBUTTON) & 0x80) != 0)
            {

                    mousepos();
                    switch(mx)
                    {
                        case 35: x=mx-34; break;
                        case 36: x=mx-35; break;
                        case 37: x=mx-36; break;
                        case 38: x=mx-37; break;
                        case 39: x=mx-38; break;
                        case 40: x=mx-38; break;
                        case 41: x=mx-39; break;
                        case 42: x=mx-40; break;
                        case 43: x=mx-41; break;
                        case 44: x=mx-42; break;
                        default: x=0;
                    }

                    switch(my)
                    {
                        case 10: y=my-9; break;
                        case 11: y=my-10; break;
                        case 12: y=my-10; break;
                        case 13: y=my-11; break;
                        case 14: y=my-11; break;
                        case 15: y=my-12; break;
                        case 16: y=my-12; break;
                        case 17: y=my-13; break;
                        default: y=0;
                    }

            }

    }while((x<1|| x>3)||(y<1 || y>4));

    if(y==2 && x<3)
    {
        system("cls");
        Pinit();
        Paddle();
    }

    else if(y==3 && x<2)
    {
        system("cls");
        gotoxy(30,9);
        cout<<"Instructions:\n\n"
            <<"\t\t\tThe rules of Paddle are simple:\n\n"
            <<"\t\t* Bounce the Ball.....\n\n"
            <<"\t\t* Use Arrow Keys to move (or) Use Mouse to move.\n\n"
            <<"\t\t* Press Down to Pause, Up to Resume\n";
        Sleep(6000);
    }

    else if(y==4 && x<2)
    {
        system("cls");
        gotoxy(30,10);
        cout<<"Thank You for playing!!!"<<endl;
        Sleep(1000);
        exit(0);
    }
}


//MAIN FUNCTION
int main()
{
    setcursor(0,0);
    SetConsoleTitle("Paddle v2");
    srand(time(NULL));
    Sleep(150);

    while(1)
    {
        system("cls");
        gotoxy(27,7);
        cout<<"\tWelcome to Paddle!!!!"<<endl;
        cout<<"\n\n\n\t\t\t\t    Bug"<<endl;
        cout<<"\n\t\t\t\t    Play"<<endl;
        cout<<"\n\t\t\t\t    Help"<<endl;
        cout<<"\n\t\t\t\t    Exit"<<endl;
        Pmenu();
    }

    Pinit();
    Paddle();

    getch();
    system("PAUSE");
    return 0;
}
