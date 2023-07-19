#pragma warning(disable:4996) 
#define SHOW_CONSOLE

#include <graphics.h>
#include <ege/sys_edit.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int N, M, C;       // 列x，行y，地雷数
int mine = 0;      // 地雷数
int openCount = 0; // 已经打开的格子数
PIMAGE Image[15];

#define row 25
#define col 31
char Map[row][col];     // 实际地图
char ShowMap[row][col]; // 显示的地图

// 函数声明
int mainProcess();
void startGame();

void intmap(int level) // 根据游戏模式生成地图
{
    switch (level)
    {
    case 1:
        N = 9, M = 9;
        mine = 10;
        break; // 初级
    case 2:
        N = 16, M = 16;
        mine = 40;
        break; // 中级
    case 3:
        // N = 16, M = 30;
        N = 30, M = 16;
        mine = 99;
        break; // 高级
    case 4:
        //   自定义地图
         // 关闭当前窗口
        closegraph();

        // 新建一个窗口
        initgraph(300, 300, 0);
        setcaption("custom setting");
        // 输出文字
        setcolor(WHITE);
        setfont(20, 0, "宋体");
        xyprintf(40, 50, "rows: ");
        xyprintf(40, 100, "columns: ");
        xyprintf(40, 150, "mines: ");
        setbkcolor(LIGHTGRAY);
        xyprintf(100, 200, "Start Game");

        // 创建输入框 
        sys_edit editBoxM;
        // 单行模式
        editBoxM.create(false);
        // 位置
        editBoxM.move(150, 50);
        // 宽高 
        int editTextHeight = 16;
        editBoxM.size(editTextHeight * 5, editTextHeight + 10);
        // 颜色字体
        editBoxM.setbgcolor(YELLOW);
        editBoxM.setcolor(BLACK);
        editBoxM.setfont(editTextHeight, 0, "宋体");
        // 可见
        editBoxM.visible(true);
        editBoxM.settext("12");

        // 创建输入框 
        sys_edit editBoxN;
        // 单行模式
        editBoxN.create(false);
        // 位置
        editBoxN.move(150, 100);
        // 宽高 
        editTextHeight = 16;
        editBoxN.size(editTextHeight * 5, editTextHeight + 10);
        // 颜色字体
        editBoxN.setbgcolor(YELLOW);
        editBoxN.setcolor(BLACK);
        editBoxN.setfont(editTextHeight, 0, "宋体");
        // 可见
        editBoxN.visible(true);
        editBoxN.settext("24");

        // 创建输入框 
        sys_edit editBoxMine;
        // 单行模式
        editBoxMine.create(false);
        // 位置
        editBoxMine.move(150, 150);
        // 宽高 
        editTextHeight = 16;
        editBoxMine.size(editTextHeight * 5, editTextHeight + 10);
        // 颜色字体
        editBoxMine.setbgcolor(YELLOW);
        editBoxMine.setcolor(BLACK);
        editBoxMine.setfont(editTextHeight, 0, "宋体");
        // 可见
        editBoxMine.visible(true);
        editBoxMine.settext("50");

        const int buffSize = 100;
        char strBuffer[100];
        int buffLen = 0;

        for (; is_run(); delay_fps(60))
        {

            while (1)
            {

                mouse_msg msg = getmouse();
                if (msg.is_down() && msg.is_left())
                {
                    if ((100 < msg.x && msg.x < 234) && (200 < msg.y && msg.y < 226))
                    {

                        editBoxM.gettext(buffSize, strBuffer);
                        sscanf(strBuffer, "%d", &M);

                        //获取输入框内容字符串
                        editBoxN.gettext(buffSize, strBuffer);
                        // 从字符串中提取数字
                        sscanf(strBuffer, "%d", &N);

                        editBoxMine.gettext(buffSize, strBuffer);
                        sscanf(strBuffer, "%d", &mine);

                        if (N < row && M < col && N >= 9 && M >= 9 && mine >= 10 && mine <= 668) {
                            // 符合条件，则退出循环，关闭窗口
                            break;
                        }
                    }
                }
            }
            break;
        }

        // 隐藏输入框
        editBoxM.visible(false);
        editBoxN.visible(false);
        editBoxMine.visible(false);
        // 关闭窗口
        closegraph();


        break;
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            // Map[i][j] = 0;
            // 初始化为没有雷
            Map[i][j] = '.';
        }
        // printf("/n");
    }

    srand((unsigned)time(NULL));
    int n = 0;
    while (n < mine)
    {
        int r = rand() % N;
        int c = rand() % M;
        //   如果不是雷，则设置为雷
        if (Map[r][c] != '*')
        {
            Map[r][c] = '*';
            n++;
        }
    }

    return;
}
void LoadPic()
{
    for (int i = 0; i < 13; i++)
    {
        char str[20];
        sprintf(str, "images/%d.jpg", i);
        Image[i] = newimage(30, 30);
        getimage(Image[i], str);
    }
    return;
}
void PutPic()
{
    cleardevice();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (ShowMap[i][j] == '.')
                putimage(30 * i, 30 * j, Image[10]);
            else if (ShowMap[i][j] == '?')
            {
                // ?
                putimage(30 * i, 30 * j, Image[12]);
            }
            else if (ShowMap[i][j] == '!')
            {
                // red flag
                putimage(30 * i, 30 * j, Image[11]);
            }
            else if (ShowMap[i][j] == '*')
            {
                // mine
                putimage(30 * i, 30 * j, Image[9]);
            }

            else
            {
                putimage(30 * i, 30 * j, Image[ShowMap[i][j] - '0']);
            }
        }
    }
    return;
}

int isMine(int x, int y) // 判断坐标(x,y)是否是地雷
{
    
    if (Map[x / 30][y / 30] == '*')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int countMine(int x, int y) // 判断坐标周围地雷数;
{
    printf("count mine: (%d %d)\n", x / 30, y / 30);
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            int newX = x + i * 30;
            int newY = y + j * 30;

            // 判断是否超出地图范围
            if (newX < 0 || newX >= N * 30) {
                continue;
            }
            if (newY < 0 || newY >= M * 30) {
                continue;
            }

            printf("\t checkIsMine(%d, %d)", newX / 30, newY / 30);

            if (isMine(newX, newY))
            {
                printf(" isMine.");
                count++;
            }
            printf("\n");
        }
    }
    printf("number is: %d\n", count);
    return count;
}

int leftM(char ShowMap[row][col]) // 剩余地雷数
{
    int ans = mine;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (ShowMap[i][j] == '!')
            {
                ans--;
            }
        }
    }
    return ans;
}

void showResultWindow(bool isWin) {
    // 关闭当前窗口
    // closegraph();
    // char title[30];
    // sprintf(title, "Sweepmine: %3d s", openCount);
    // setcaption(title);
    // 新建一个窗口
    // initgraph(300, 300, 0);
    // setcaption("Game result"); 
    int width = getwidth();
    int height = getheight();
    // 清除之前的键盘输入（如果有的话）
    flushkey();

    int i = 0;
    for (; is_run(); delay_fps(60)) {
        i++;
        // 输出文字
        setbkmode(TRANSPARENT);
        if (i < 30) {
            setcolor(YELLOW);
        }
        else if (i < 60) {
            setcolor(RED);
        }
        else {
            setcolor(BLUE);
        }
        if (i == 100) {
            i = 0;
        }
        setfont(30, 0, "宋体");
        int x = (width - 100) / 2;
        int y = height / 3;
        if (isWin) {
            xyprintf(x, y, "You Win");
        }
        else {
            xyprintf(x, y, "You Lose");
        }
        setfont(20, 0, "宋体");
        x = (width - 230) / 2;
        y = height / 3 * 2;
        xyprintf(x, y, "Press any key to contine");

        if (kbmsg()) {
            break;
        }
    }
    // 关闭当前窗口
    closegraph();
    // 重新初始化整个程序
    mainProcess();
}

void LigthClick1(int x, int y) // 左键单击点开
{
    // 范围检查 
    if (x / 30 < 0 || x / 30 >= N) {
        return;
    }
    if (y / 30 < 0 || y / 30 >= M) {
        return;
    }

    printf("left click (%d, %d)\n", x / 30, y / 30);

    if (ShowMap[x / 30][y / 30] == '.' || ShowMap[x / 30][y / 30] == '?')
    {

        if (isMine(x, y))
        {

            ShowMap[x / 30][y / 30] = '*';
            PutPic();
            
            showResultWindow(false);
        }
        else
        {
            int count = countMine(x, y);
            if (count == 0)
            {
                ShowMap[x / 30][y / 30] = '0';
                openCount++;
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        int newX = x + i * 30;
                        int newY = y + j * 30;

                        if (i == 0 && j == 0)
                            continue;

                        // 判断是否超出地图范围
                        if (newX < 0 || newX >= N * 30) {
                            continue;
                        }
                        if (newY < 0 || newY >= M * 30) {
                            continue;
                        }

                        if (Map[newX / 30][newY / 30] == '.')
                        {
                            LigthClick1(newX, newY);
                        }
                    }
                }
            }
            else
            {
                ShowMap[x / 30][y / 30] = count + '0';
                openCount++;
            }
        }
    }
    return;
}
void RightClick(int x, int y) // 2
{
    // 范围检查 
    if (x / 30 < 0 || x / 30 >= N) {
        return;
    }
    if (y / 30 < 0 || y / 30 >= M) {
        return;
    }

    printf("right click (%d, %d)\n", x / 30, y / 30);
    if (ShowMap[x / 30][y / 30] == '.')
    {
        ShowMap[x / 30][y / 30] = '!';
    }
    else if (ShowMap[x / 30][y / 30] == '!')
    {
        ShowMap[x / 30][y / 30] = '?';
    }
    else if (ShowMap[x / 30][y / 30] == '?')
    {
        ShowMap[x / 30][y / 30] = '.';
    }

    return;
}

void DoubleClick(int x, int y) // 9
{
    // 范围检查 
    if (x / 30 < 0 || x / 30 >= N) {
        return;
    }
    if (y / 30 < 0 || y / 30 >= M) {
        return;
    }

    printf("double click (%d, %d)\n", x / 30, y / 30);
    int Markedmine = 0;
    for (int i = x / 30 - 1; i <= x / 30 + 1; i++)
    {
        for (int j = y / 30 - 1; j <= y / 30 + 1; j++)
        {
            // 范围检查 
            if (i < 0 || i >= N) {
                continue;
            }
            if (j < 0 || j >= M) {
                continue;
            }
            if (ShowMap[i][j] == '!')
            {
                Markedmine++;
            }
        }
    }
    printf("Markedmine: %d\n", Markedmine);
    if (Markedmine == (ShowMap[x / 30][y / 30] - '0'))
    {
        for (int i = x / 30 - 1; i <= x / 30 + 1; i++)
        {
            for (int j = y / 30 - 1; j <= y / 30 + 1; j++)
            {
                // 范围检查 
                if (i < 0 || i >= N) {
                    continue;
                }
                if (j < 0 || j >= M) {
                    continue;
                }

                if (ShowMap[i][j] != '!' && Map[i][j] == '*')
                {
                    ShowMap[i][j] = '*';

                    PutPic();
                    
                    showResultWindow(false);
                }
            }
        }
        for (int i = x / 30 - 1; i <= x / 30 + 1; i++)
        {
            for (int j = y / 30 - 1; j <= y / 30 + 1; j++)
            {
                // 范围检查 
                if (i < 0 || i >= N) {
                    continue;
                }
                if (j < 0 || j >= M) {
                    continue;
                }
                if (ShowMap[i][j] == '.' || ShowMap[i][j] == '?')
                {

                    LigthClick1(i * 30, j * 30);
                }
            }
        }
    }

    return;
}


int mainProcess()

{
    initgraph(960, 640, 0);
    setcaption("Sweepmine"); // 窗口命名

    PIMAGE BACKGROUND = newimage();

    getimage(BACKGROUND, "./images/back.png");

    putimage(0, 0, BACKGROUND);
    int level;

    for (; is_run(); delay_fps(60))
    {
        while (1)
        {

            mouse_msg msg = getmouse();
            if (msg.is_down() && msg.is_left())
            {
                if ((652 < msg.x && msg.x < 750) && (60 < msg.y && msg.y < 125))
                {

                    level = 1;
                    break;
                }
                else if ((650 < msg.x && msg.x < 750) && (182 < msg.y && msg.y < 248))
                {
                    level = 2;
                    break;
                }
                else if ((652 < msg.x && msg.x < 750) && (309 < msg.y && msg.y < 374))
                {
                    level = 3;
                    break;
                }
                else if ((625 < msg.x && msg.x < 760) && (428 < msg.y && msg.y < 500)) {
                    //   自定义地图
                    level = 4;
                    break;
                }
            }
        }
        break;
    }

    delimage(BACKGROUND);
    intmap(level);
    closegraph();

    startGame();

    return 0;
}

void startGame() {

    // 初始化 为 0
    openCount = 0;

    initgraph(N * 30, M * 30, 0);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            ShowMap[i][j] = '.';
        }
    }
    LoadPic();
    PutPic();

    // 开始时间
    double startTime = fclock();

    for (; is_run(); delay_fps(60))
    {

        DWORD lastClickTime = 0;        // 上一次鼠标点击的时间戳
        bool isLeftButtonDown = false;  // 左键是否按下
        bool isRightButtonDown = false; // 右键是否按下

        for (; is_run(); delay_fps(60))
        {
            // 把时间显示到标题上...
            double endTime = fclock();
            double timeElapsed = endTime - startTime;
            char title[100];
            sprintf(title, "Sweepmine: %3d s", (int)timeElapsed);
            setcaption(title);


            // 检测鼠标事件
            while (mousemsg())
            {
                MOUSEMSG msg = GetMouseMsg();

                if (msg.uMsg == WM_LBUTTONDOWN) // 左键按下
                {
                    isLeftButtonDown = true;
                    lastClickTime = GetTickCount(); // 记录当前时间戳
                }
                else if (msg.uMsg == WM_LBUTTONUP) // 左键松开
                {
                    isLeftButtonDown = false;
                    DWORD timeInterval = GetTickCount() - lastClickTime; // 计算时间间隔

                    if (timeInterval < 300) // 判断是否为左键单击
                    {
                        printf("left click\n");
                        LigthClick1(msg.x, msg.y);
                        printf("openCount: %d, total: %d*%d-%d=%d\n", openCount, N, M, mine, N * M - mine);
                        if (openCount == N * M - mine)
                        {
                            PutPic();
                            //xyprintf(0, 0, "Game over, you win");
                            //break;
                            //  
                            showResultWindow(true);
                        }
                        else
                        {
                            PutPic();
                        }
                    }
                }
                else if (msg.uMsg == WM_RBUTTONDOWN) // 右键按下
                {
                    isRightButtonDown = true;
                    lastClickTime = GetTickCount(); // 记录当前时间戳
                }
                else if (msg.uMsg == WM_RBUTTONUP) // 右键松开
                {
                    isRightButtonDown = false;
                    DWORD timeInterval = GetTickCount() - lastClickTime; // 计算时间间隔

                    if (timeInterval < 300) // 判断是否为右键单击
                    {
                        printf("right click\n");
                        RightClick(msg.x, msg.y);
                        printf("openCount: %d, total: %d*%d-%d=%d\n", openCount, N, M, mine, N * M - mine);
                        if (openCount == N * M - mine)
                        {
                            // cleardevice();
                            PutPic();
                            // xyprintf(0, 0, "Game over, you win");
                            // exit(0);
                            //   展示结果窗口
                            showResultWindow(true);
                        }
                        else
                        {
                            PutPic();
                        }
                    }
                }

                // 检测左右键是否同时按下
               
                if (keystate(key_mouse_l) && keystate(key_mouse_r))
                {
                    
                    {
                        printf("double click\n");
                        DoubleClick(msg.x, msg.y);
                        printf("openCount: %d, total: %d*%d-%d=%d\n", openCount, N, M, mine, N * M - mine);
                        if (openCount == N * M - mine)
                        {
                            // cleardevice();
                            PutPic();
                            
                            showResultWindow(true);
                        }
                        else
                        {
                            // cleardevice();
                            PutPic();
                        }
                    }

                    isLeftButtonDown = true;
                    isRightButtonDown = true;
                }
                else
                {
                    isLeftButtonDown = false;
                    isRightButtonDown = false;
                }
            }
        }
    }

    getch();
    closegraph();
}

int main() {
    mainProcess();
    return 0;
}