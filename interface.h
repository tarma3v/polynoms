#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

Font sans;
Vector2f mousepos;
bool isClick = 0;
char pressButton = 0;
float timefps = 0;
int moveCol = 0;

struct box
{
    float x,y;
    float w,h;

    box(){}
    box(float w,float h):w(w),h(h){x = 0;y = 0;}
    box(float w,float h,float x,float y):w(w),h(h),x(x),y(y){}

    virtual void move(float mx,float my)
    {
        x += mx;
        y += my;
    }

    void drawBox(RenderWindow &window,Color color,Color obv)
    {
        RectangleShape BOX({w,h});
        BOX.move(x,y);
        BOX.setOutlineThickness(3);
        BOX.setOutlineColor(obv);
        BOX.setFillColor(color);
        window.draw(BOX);
    }


    virtual void draw(RenderWindow &window)
    {
        drawBox(window, {255,255,255},{0,0,0});
    }
};

struct design
{
    vector<box*> boxes;

    design(){}

    void add(box *n)
    {
        boxes.push_back(n);
    }

    void draw(RenderWindow &window)
    {
        for(int i=0;i<boxes.size();i++)
            boxes[i]->draw(window);
    }
};

struct textbox : public box
{
    string text;

    textbox(){}
    textbox(float h,string text):box(h*0.4,h),text(text){}

    void drawTextBox(RenderWindow &window,Color color,Color obv)
    {
        Text out(text,sans, h*0.75);
        w = out.getGlobalBounds().width + h*0.4;
        drawBox(window, color,obv);

        out.setPosition(x + h*0.2,y);
        out.setFillColor(obv);
        window.draw(out);

		
    }

    virtual void draw(RenderWindow &window)
    {
        drawTextBox(window, {255,255,255},{0,0,0});

    }
};


bool inMouse(box *a)
{
    return a->x <= mousepos.x && a->y <= mousepos.y && a->x + a->w >= mousepos.x && a->y + a->h >= mousepos.y;
}

struct buttontextbox : public textbox
{
    buttontextbox(){};
    buttontextbox(float h,string text):textbox(h,text){};

    bool pastClick = 0;
    bool outClick = 0;

    void drawWithAlpha(RenderWindow &window, int alpha)
    {
        if(inMouse(this))
        {
            if(isClick)
                drawTextBox(window,{100,255,100,(Uint8)alpha},{0,0,0,(Uint8)alpha});
            else
            {
                if(pastClick)
                    outClick = 1;
                drawTextBox(window,{100,100,255,(Uint8)alpha},{0,0,0,(Uint8)alpha});
            }

        }
        else
        {
            drawTextBox(window,{255,255,255,(Uint8)alpha},{0,0,0,(Uint8)alpha});
        }
        pastClick = isClick;
    }

    virtual void draw(RenderWindow &window)
    {
        if(inMouse(this))
        {
            if(isClick)
                drawTextBox(window,{100,255,100},{0,0,0});
            else
            {
                if(pastClick)
                    outClick = 1;
                drawTextBox(window,{100,100,255},{0,0,0});
            }

        }
        else
        {
            drawTextBox(window,{255,255,255},{0,0,0});
        }
        pastClick = isClick;
    }

    bool click()
    {
        if(outClick)
        {
            outClick = 0;
            return 1;
        }
        return 0;
    }

};

struct inputstring : public textbox
{
    string notin;
    int sost = 0;

    inputstring(){}
    inputstring(float h,string in):textbox(h,in){notin = in;}

    bool pastClick = 0;
    bool outClick = 0;
    bool pastButton = 0;

    virtual void draw(RenderWindow &window)
    {
        switch(sost)
        {
        case 0:
                if(inMouse(this))
            {
                if(isClick)
                    drawTextBox(window,{100,255,100},{0,0,0});
                else
                {

                    drawTextBox(window,{100,100,255},{0,0,0});
                    if(pastClick)
                    {
                        sost = 1;
                        if(text == notin)
                        text = "";
                    }

                }

            }
            else
            {
                drawTextBox(window,{255,255,255},{0,0,0});
            }
            pastClick = isClick;
            break;
        case 1:
            drawTextBox(window,{200,100,100},{0,0,0});

            if(isClick)
            {
                sost = 0;
                if(text == "")
                    text = notin;
                break;
            }
            if(pressButton == 13)
            {
                if(text == "")
                {
                    text = notin;
                    sost = 0;
                }
                else
                    sost = 2;
                break;
            }
            if(pressButton != 0 && pastButton == 0)
            {
                if(pressButton == 8)
                {
                    if(text.size())
                    text.pop_back();
                }
                else
                    text.push_back(pressButton);
            }

            pastButton = pressButton;
            break;
        case 2:
            text = notin;
            sost = 0;
            break;
        }

    }

    bool isEnter()
    {
        return sost == 2;
    }

};

bool isTouchSection(float x1,float x2,float y1,float y2)
{
    if(x2 < y1 || y2 < x1)
        return false;
    return true;
}

bool isTouchBoxes(box *a,box *b)
{
    return isTouchSection(a->x,a->x + a->w, b->x,b->x + b->w) && isTouchSection(a->y,a->y + a->h, b->y,b->y + b->h);
}


struct listbox : public box
{
    float r;
    float fx;
    float v;
    float v2;
    int click = -1;

    vector<buttontextbox> texts;
    listbox(float w,float h,float r):box(w,h),r(r){fx = 0; v = h/1000;v2 = r/30;}

    virtual void draw(RenderWindow &window)
    {
        for(int i=0;i<texts.size();i++)
        {
            if(isTouchBoxes(&texts[i],this))
            {
                int a = 255;
                if(texts[i].y < y)
                {
                    a = 255.0 *(r + texts[i].y - y)/r;
                }
                if(texts[i].y + r > y + h)
                {
                    float raz = texts[i].y + r - y - h;
                    a = 255.0 *(r - raz)/r;
                }
                texts[i].drawWithAlpha(window,a);
                if(texts[i].click())
                {
                    click = i;
                }
            }
        }
        float move1 = 0;
        if(texts.size())
        {
            float y1 = texts[0].y;
            float y2 = texts.back().y + r;
            if(y2 - y1 > h)
            {
                if(inMouse(this))
                {
                    move1 = timefps * v * moveCol;
                }
                if(y1 + move1 > y)
                    move1 = y - y1;
                if(y2 + move1 < h + y)
                    move1 = h + y - y2;
            }
            else
            {
                move1 = y - y1;
            }
        }
        for(int i=0;i<texts.size();i++)
        {
            texts[i].y += move1;
            if(texts[i].w > w)
            if(inMouse(&texts[i]) && inMouse(this))
            {
                texts[i].x -=  timefps * v2;
                if(texts[i].x + texts[i].w < x + w)
                {
                    texts[i].x = x + w - texts[i].w;
                }
            }
            else
            {
                texts[i].x +=  timefps * v2;
                if(texts[i].x > x)
                {
                    texts[i].x = x;
                }
            }
        }


    }

    void add(string s)
    {
        float bx = x, by = y;
        if(texts.size())
            by = texts.back().y + texts.back().h + r*0.3;
        texts.push_back({r,s});
        texts.back().move(bx,by);

        float move1 = 0;
        float y1 = texts[0].y;
        float y2 = texts.back().y + r;
        if(y2 - y1 > h)
        {
            move1 = h + y - y2;
        }
        else
        {
            move1 = y - y1;
        }
        for(int i=0;i<texts.size();i++)
            texts[i].y += move1;
    }


    int isClick()
    {
        int h = click;
        click = -1;
        return h;
    }

};


struct vectortextbox : public box
{
    float h;
    vector<buttontextbox> texts;
    vectortextbox(){}
    vectortextbox(float h):h(h){}

    virtual void draw(RenderWindow &window)
    {
        for(int i=0;i<texts.size();i++)
        {
            texts[i].draw(window);
			
        }
		
    }


    void add(string in)
    {
        float nx = x;
        if(texts.size())
        {
            nx = texts.back().x + texts.back().w + h*0.2;
        }
        texts.push_back({h,in});
        texts.back().move(nx,y);
    }

    void del()
    {
        texts.pop_back();
    }

};


#endif // INTERFACE_H_INCLUDED
