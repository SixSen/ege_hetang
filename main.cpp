#include <stdio.h>
#include <graphics.h>
#include <math.h>
/*******************************************************************************/
#define SCR_WIDTH      800
#define SCR_HEIGHT     600
//窗口大小800*600
#define WATER_HEIGHT 400
#define RAIN_MIN_RADIUS 15
//水位最高400px，雨滴最小15px
/************************/

const    float sp = 10;                   //雨滴下落速度

const    int  w=5;                       //风速

/*************************/
POINT point;
HWND hwnd;
/***********************************************/
struct ripple
{
//涟漪结构体
    float water_level;
    float max_radius;
    float radius;
};
/***********************************************/
struct Rain
{
//雨滴结构体
    float x,y;                             //位置坐标
    float dx,dy;                       //速度方向
    float radius;                     //雨滴半径
    color_t color;                   //雨滴颜色
    int alpha;                           //雨滴透明度
    PIMAGE img;
    ripple rp;
    void init();
    void init_ripple(float r);
} rain;
/***********************************************/
void Rain::init_ripple(float r)
{
//重设涟漪
    this->rp.radius = r;
    this->rp.water_level = WATER_HEIGHT +(SCR_HEIGHT - WATER_HEIGHT) * randomf();
    this->rp.max_radius = (int)r * random(10)% RAIN_MIN_RADIUS + RAIN_MIN_RADIUS;
}
/***********************************************/
void mBall(Rain ball[])
{
//初始化雨滴
    for(int i = 0; i != 500; ++i)
    {

        ball[i].x = randomf() *SCR_WIDTH ;
        ball[i].y = randomf() *SCR_HEIGHT/2;
        randomize();
        ball[i].dy = sp;
        ball[i].radius = randomf() *3;
        ball[i].color = random(0xff0000) + 0xffff;

        ball[i].img = newimage(ball[i].radius *2,ball[i].radius*6 );
        ball[i].alpha = random(255);

        setcolor(ball[i].color, ball[i].img);
        setfillcolor(ball[i].color, ball[i].img);
        fillellipse(ball[i].radius,ball[i].radius*3,ball[i].radius,ball[i].radius*3,ball[i].img);


        ball[i].init_ripple(ball[i].radius);
    }
}
/***********************************************/
int wind()
{
//鼠标位置判断风向
    GetCursorPos(&point);
    ScreenToClient(hwnd,&point);
    if (point.y>0&&point.x>0&&point.x<SCR_WIDTH&&point.y<SCR_HEIGHT)
    {
        if(point.x<SCR_WIDTH/2)//风向向左
            return -1;
        else                        //风向向右
            return 1;
    }
    else return 0;        //无风
}
/***********************************************/
void raining(int n,Rain ball[])
{
//雨滴与涟漪
    for(int i = 0; i != n; ++i)
    {
        if(ball[i].y < ball[i].rp.water_level)//当雨滴未落至湖面
        {
            putimage_alphatransparent(NULL,ball[i].img,ball[i].x,ball[i].y,BLACK,ball[i].alpha);

            ball[i].x += ball[i].dx+(wind()*w);
            ball[i].y += ball[i].dy;
        }
        else//雨滴落入湖面
        {
            if(ball[i].rp.radius > ball[i].rp.max_radius) ball[i].init();
            else
            {
                setcolor(ball[i].color);
                ellipsef(ball[i].x,ball[i].y,0,360,ball[i].rp.radius * 2,ball[i].rp.radius);
                ball[i].rp.radius = 1.25 * ball[i].rp.radius;
            }
        }
    }
}
/***********************************************/
void Rain::init()
{
//重设雨滴位置
    this->x = randomf() *SCR_WIDTH ;
    this->y = randomf() *SCR_HEIGHT/2;
    this->rp.radius = this->radius;
}

/***********************************************/
color_t alphasingle(color_t a, color_t b, color_t alpha)
{
// alpha 混合计算函数
    return (a * (0xFF - alpha) + b * alpha) >> 8;
}
/***********************************************/
color_t alphacol(color_t d, color_t s, color_t alpha)
{
// 颜色alpha混合计算函数
    return (alphasingle(d&0xFF00FF, s&0xFF00FF, alpha) & 0xFF00FF)
           | (alphasingle(d&0xFF00, s&0xFF00, alpha) >> 8 << 8);
}
/***********************************************/
void thunder()
{
//随机闪电
    setfillcolor(RGB(245, 255, 250));
    int x= randomf() *SCR_WIDTH;
    int y=randomf() *SCR_HEIGHT/4;
    int a=max(7,randomf()*10);
    int b=max(100,randomf()*60);
    int c=min(100,randomf()*50);
    int d=max(200,randomf()*120);
    setcolor(RGB(245, 255, 250));
    int  pt[ ]=
    {
        x,  y,
        x-a,  y+b,
        x,  y+b,
        x-8,  y+d,
        x+a,  y+c,
        x, y+c,
    };
    fillpoly(6,pt);
}
/***********************************************/
void sky()
{
//天空渐变
    int c;
    for (c=0; c<256 ; ++c)
    {
        color_t a = RGB(112,25,50), b = BLACK;
        setcolor(alphacol(a, b, c));
        line(0, c, SCR_WIDTH, c);
    }
}
/***********************************************/
void hello()
{
//欢迎界面
    int q=SCR_HEIGHT;

    for(; is_run(); delay_fps(30))
    {
        initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
        sky();

        setcolor(0x87CEFA);

        LOGFONTA f;
        getfont(&f);
        f.lfHeight = 100;
        f.lfWidth = 60;
        strcpy(f.lfFaceName, "微软雅黑");
        f.lfQuality = ANTIALIASED_QUALITY;
        setfont(&f);
        setbkmode(TRANSPARENT);
        outtextxy(150,q,"池塘夜雨");
        if(q>270)q-=3;
        if(q==270)
        {
            Sleep(1200);
            return;
        }
    }
}
/***********************************************/
void guide()
{
//指导教程
    for(; is_run(); delay_fps(60))
    {
        initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
        PIMAGE img=newimage(800,600);
        getimage(img,".\\timg (1).jpg",800,600);
        putimage(0, 0,img,SRCCOPY);

        setcolor(0x006699);

        LOGFONTA f;
        getfont(&f);
        f.lfHeight = 100;
        f.lfWidth = 50;
        strcpy(f.lfFaceName, "楷体");
        f.lfQuality = ANTIALIASED_QUALITY;
        setfont(&f);
        setbkmode(TRANSPARENT);
        outtextxy(80,100,"↑键增大雨量");
        outtextxy(80,200,"↓键减小雨量");
        outtextxy(80,300," TAB按键打雷");
        outtextxy(80,400,"鼠标控制风向");
        Sleep(2500);
        delimage(img);

        return;
    }
}
/***********************************************/
void lotus()
{
//绘制荷叶
    setcolor(RGB(0,0,0));
    setfillcolor(0x308014);
    fillellipse(100,450,60,20);
    fillellipse(200,550,60,20);
    fillellipse(500,440,58,20);
    fillellipse(600,500,60,20);
    fillellipse(700,460,60,18);
}

/***********************************************/
int main()
{
//主函数
    /*****导入音乐*****/
    MUSIC mus;
    mus.OpenFile(".\\rain.mp3");
    mus.SetVolume(0.05f);
    /*****导入雷声*****/
    MUSIC thun;
    thun.OpenFile(".\\thunder.mp3");
    thun.SetVolume(0.5f);
    /**创建无边窗口***/
    setinitmode(INIT_NOBORDER);
    initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
    /*****创建雨滴*****/
    Rain ball[500];
    mBall(ball);
    /****定义变量******/
    int n = 1;
    double t = fclock();
    /*****欢迎界面*****/
    hello();
    guide();
    /***主动画循环*****/
    for(; is_run(); delay_fps(60))
    {
        /***雨声背景********/
        if (mus.GetPlayStatus ()== MUSIC_MODE_STOP)
        {
            //当背景雨声结束时再次播放
            mus.Play(0);
        }
        /****清屏************/
        cleardevice();
        /***绘制天空*********/
        sky();
        /***键盘控制*********/
        key_msg k = {0};
        if(kbmsg())
        {
            k = getkey();
            if(k.key == key_esc)
            {
                //ESC键退出程序
                break;
            }
            else if(k.key == VK_TAB)
            {
                //Tab键控制打雷
                thunder();
                Sleep(50);//雷声晚于闪电
                thun.Play(0);
            }
            else if(k.key == VK_UP)
            {
                //↑键增大雨量
                n = min(500,n +31);
            }
            else if(k.key == VK_DOWN)
            {
                //↓键减小雨量
                n = max(0,n - 31);
            }
        }
        /***雨量自动增加**/
        if(fclock() - t > 0.2)
        {
            t = fclock();
            if (n < 500) ++n;
            //通过量大小调节下雨音量
            mus.SetVolume(n/1000.0f);
        }
        /*****雨滴涟漪*****/
        raining(n,ball);
        /*****荷叶**********/
        lotus();
    }
    /***清除雨滴图像*****/
    for(int i = 0; i <500; i ++) delimage(ball[i].img);
    //关闭图像
    closegraph();
    return 0;
}
