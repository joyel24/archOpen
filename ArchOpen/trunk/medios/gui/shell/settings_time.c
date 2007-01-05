/*
*   kernel/gfx/gui/shell/settings/clock.c
*
*   MediOS project
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#include <kernel/kernel.h>
#include <kernel/evt.h>

#include <gfx/kfont.h>

#include <lib/string.h>

#include <gui/widgetlist.h>
#include <gui/spinbox.h>
#include <gui/button.h>
#include <gui/checkbox.h>
#include <gui/chooser.h>
#include <gui/icons.h>

#include <driver/rtc.h>

#include <fs/cfg_file.h>

//#define TIME_DATE_GUIFONT RADONWIDE
#define TIME_DATE_GUIFONT RADONWIDE

int isPm=0;
int PmPos;
int curH;
SPINBOX time[3];
SPINBOX date[3];

CHECKBOX hourFormat;
CHOOSER dateFormat;

WIDGETLIST menuList;  

void checkbox_12_24_onChange(CHECKBOX t)
{
    if(t->checked)
    {
        /*12h*/
        curH=time[0]->getValue(time[0]);
        if(curH==0)
            curH=24;
        if(curH>12)
        {
            isPm=1;
            curH-=12;
            time[0]->setValue(time[0],curH);
        }
        else
        {
            isPm=0;
        }
        time[0]->setParam(time[0],1,12,1,2);
        time[0]->paint(time[0]);
    }
    else
    {
        /*24h*/
        int H,W;
        if(isPm)
        {           
            time[0]->setParam(time[0],0,23,1,2);
            time[0]->setValue(time[0],time[0]->getValue(time[0])+12);            
            time[0]->paint(time[0]);
        }
        gfx_getStringSize("AM",&W,&H);
        gfx_fillRect(time[0]->backColor,PmPos,80+2,W,H);
    }
}

void hour_onChange(SPINBOX t)
{
    if(hourFormat->checked)
    {
        int newH=time[0]->getValue(time[0]);
        if((newH == 12 && curH == 1)|| (newH == 1 && curH == 12))
            isPm=isPm?0:1;
        curH=newH;
        gfx_putS(time[1]->foreColor,time[1]->backColor,PmPos,80+2,isPm?"PM":"AM");
    }
}

void chooser_date_onChange(CHOOSER t)
{
    if(t->index==1)
    {
        /* MM/DD/YYYY */
        int x= date[1]->x;
        int y= date[1]->y;
        date[1]->x=date[0]->x;
        date[1]->y=date[0]->y;
        date[0]->x=x;
        date[0]->y=y;
        date[0]->paint(date[0]);
        date[1]->paint(date[1]);
    }
    else
    {
        /* DD/MM/YYYY */
        int x= date[1]->x;
        int y= date[1]->y;
        date[1]->x=date[0]->x;
        date[1]->y=date[0]->y;
        date[0]->x=x;
        date[0]->y=y;
        date[0]->paint(date[0]);
        date[1]->paint(date[1]);
    }
}

void okBtn_click(BUTTON b)
{
    struct med_tm now;
    CFG_DATA * cfg;
    now.tm_hour=time[0]->value;
    now.tm_min=time[1]->value;
    now.tm_sec=time[2]->value;
    if(dateFormat->index==1) /* MM/DD/YYYY */
    {
        now.tm_mday=date[1]->value;
        now.tm_mon=date[0]->value;
    }
    else
    {
        now.tm_mday=date[0]->value;
        now.tm_mon=date[1]->value;
    }
    now.tm_year=date[2]->value;
    rtc_setTime(&now);
    
    /* saving to cfg file */
    cfg=cfg_readFile("/medios/medios.cfg");
    if(!cfg)
    {
        printk("Can't open cfg file\n");
        cfg=cfg_newFile();
        if(!cfg)
        {
            printk("Can't create new cfg file\n");
            return;
        }
    }
    
    cfg_writeInt(cfg,"is12H",hourFormat->checked?1:0);
    cfg_writeInt(cfg,"isMMDD",dateFormat->index==1?1:0);
    
    cfg_writeFile(cfg,"/medios/medios.cfg");
    
}   

void resetBtn_click(BUTTON b)
{
    struct med_tm now;
    rtc_getTime(&now);
    time[0]->value=now.tm_hour;
    time[1]->value=now.tm_min;
    time[2]->value=now.tm_sec;
    if(dateFormat->index==1) /* MM/DD/YYYY */
    {
        date[1]->value=now.tm_mday;
        date[0]->value=now.tm_mon;
    }
    else
    {
        date[0]->value=now.tm_mday;
        date[1]->value=now.tm_mon;
    }
    date[2]->value=now.tm_year;
    menuList->paint(menuList);
}

void clock_setting(void)
{
    struct med_tm now;
    ICON logo;
    CFG_DATA * cfg;
    
    char * dateFormStr[2]={"DD/MM/YYYY","MM/DD/YYYY"};
    
    BUTTON mib;
            
    FONT f=fnt_fontFromId(TIME_DATE_GUIFONT);
    
    int evtHandle;
    int event;
    int sepH,sepW,w,h;
    int is12H,isMMDD;
    
    int x=100,y=50;
    
    rtc_getTime(&now);
    
    printk("Cur time: %02d:%02d:%02d %02d/%02d/%04d\n",now.tm_hour,now.tm_min,now.tm_sec,
        now.tm_mday,now.tm_mon,now.tm_year);
   
    if(now.tm_hour>12)
        isPm=1;
    
    cfg=cfg_readFile("/medios/medios.cfg");
    
    if(!cfg)
    {
        printk("Can't open cfg file\n");        
        is12H=0;
        isMMDD=0;
    }
    else
    {
        is12H=cfg_readInt(cfg,"is12H");    
        isMMDD=cfg_readInt(cfg,"isMMDDYYYY");
    }
    
    printk("get: %d %d\n",is12H,isMMDD);
            
    gfx_clearScreen(COLOR_WHITE);
    gfx_fontSet(TIME_DATE_GUIFONT);
           
    evtHandle = evt_getHandler(BTN_CLASS|GUI_CLASS);
    
       
    logo=icon_load("clock_icon.ico");
    gfx_drawBitmap(&logo->bmap_data,10,10);
    
    // menuList
    menuList=widgetList_create();
    menuList->ownWidgets=true;

    // standardMenu
    
    time[0]=spinbox_create();
    time[0]->setFont(time[0],TIME_DATE_GUIFONT);
    time[0]->setParam(time[0],0,23,1,2);
    time[0]->setPos(time[0],x,y);
    time[0]->setValue(time[0],now.tm_hour);
    time[0]->onChange=(SPINBOX_CHANGEEVENT)hour_onChange;
    menuList->addWidget(menuList,time[0]);    
        
    gfx_getStringSize(":",&sepW,&sepH);
    
    x+=time[0]->width+1;
    gfx_putS(time[0]->foreColor,time[0]->backColor,x,y+2,":");
    x+=(sepW+1);    
        
    time[1]=spinbox_create();
    time[1]->setFont(time[1],TIME_DATE_GUIFONT);
    time[1]->setParam(time[1],0,59,1,2);
    time[1]->setPos(time[1],x,y);
    time[1]->setValue(time[1],now.tm_min);
    menuList->addWidget(menuList,time[1]);
    
    x+=time[1]->width+1;
    gfx_putS(time[1]->foreColor,time[1]->backColor,x,y+2,":");
    x+=(sepW+1);    
        
    time[2]=spinbox_create();
    time[2]->setFont(time[2],TIME_DATE_GUIFONT);
    time[2]->setParam(time[2],0,59,1,2);
    time[2]->setPos(time[2],x,y);
    time[2]->setValue(time[2],now.tm_sec);
    menuList->addWidget(menuList,time[2]);  
    
    PmPos=x+time[2]->width+6;       
    
    y += 2*time[2]->height;
    x= 100;
    gfx_getStringSize("/",&sepW,&sepH);
    
    date[0]=spinbox_create();
    date[0]->setFont(date[0],TIME_DATE_GUIFONT);
    date[0]->setParam(date[0],1,31,1,2);
    date[0]->setPos(date[0],x,y);
    date[0]->setValue(date[0],now.tm_mday);
    menuList->addWidget(menuList,date[0]);    
        
    x+=date[0]->width+2;
    gfx_putS(date[0]->foreColor,date[0]->backColor,x,y+2,"/");
    x+=(sepW+1);   
        
    date[1]=spinbox_create();
    date[1]->setFont(date[1],TIME_DATE_GUIFONT);
    date[1]->setParam(date[1],1,12,1,2);
    date[1]->setPos(date[1],x,y);
    date[1]->setValue(date[1],now.tm_mon);
    menuList->addWidget(menuList,date[1]);    
    
    x+=date[1]->width+2;
    gfx_putS(date[1]->foreColor,date[1]->backColor,x,y+2,"/");    
    x+=(sepW+1);   
        
    date[2]=spinbox_create();
    date[2]->setFont(date[2],TIME_DATE_GUIFONT);
    date[2]->setParam(date[2],1900,2100,1,4);
    date[2]->setPos(date[2],x,y);
    date[2]->setValue(date[2],now.tm_year);
    menuList->addWidget(menuList,date[2]);    
    
    y += 2*time[2]->height;
    x= 100;
    
    hourFormat=checkbox_create();
    hourFormat->caption="12h format";
    hourFormat->font=TIME_DATE_GUIFONT;
    hourFormat->setRect(hourFormat,x,y,8,8);
    hourFormat->onChange=(CHECKBOX_CHANGEEVENT)checkbox_12_24_onChange;
    hourFormat->checked=is12H;
    menuList->addWidget(menuList,hourFormat);
    
    y += 2*time[2]->height;
    x= 100;
    
    dateFormat=chooser_create();
    dateFormat->items=dateFormStr;
    dateFormat->itemCount=2;
    dateFormat->index=isMMDD?1:0;
    dateFormat->font=TIME_DATE_GUIFONT;
    gfx_getStringSize(dateFormStr[0],&w,&h);
    dateFormat->setRect(dateFormat,x,y,w+29,h+1);    
    dateFormat->onChange=(CHOOSER_CHANGEEVENT)chooser_date_onChange;
    dateFormat->evt_prev=BTN_DOWN;
    dateFormat->evt_nxt=BTN_UP;
    dateFormat->wrap=WIDGET_WRAP_ON;
    dateFormat->orientation=WIDGET_ORIENTATION_VERT;
    menuList->addWidget(menuList,dateFormat);
    
    y += 2*MAX(0,f->height)+4;
    x= 100+4;
    
    gfx_getStringSize("OK",&sepW,&sepH);
    
    mib=button_create();
    mib->caption="OK"; 
    mib->font=TIME_DATE_GUIFONT;
    mib->setRect(mib,x,y,sepW+2,sepH+2);
    mib->onClick=(BUTTON_CLICKEVENT)okBtn_click;
    menuList->addWidget(menuList,mib);
        
    gfx_getStringSize("RESET",&sepW,&sepH);
    x+=mib->width+4;
    
    mib=button_create();
    mib->caption="Reset"; 
    mib->font=TIME_DATE_GUIFONT;
    mib->setRect(mib,x,y,sepW+2,sepH+2);
    mib->onClick=(BUTTON_CLICKEVENT)resetBtn_click;
    menuList->addWidget(menuList,mib);
    
    // intial paint
    menuList->setFocusedWidget(menuList,time[0]);
    menuList->paint(menuList);
    
    do{
        event=evt_getStatus(evtHandle);
        if (!event) continue; // no new events
        switch(event)
        {
            case BTN_LEFT:
                menuList->changeFocus(menuList,WLD_PREVIOUS);
                break;
            case BTN_RIGHT:
                menuList->changeFocus(menuList,WLD_NEXT);
                break;    
            default:
                menuList->handleEvent(menuList,event);
                break;
        }
    }while(event!=BTN_OFF);    
}