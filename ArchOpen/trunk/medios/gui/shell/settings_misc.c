/*
*   kernel/gfx/gui/shell/settings_misc.c
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
#include <kernel/lang.h>

#include <gfx/kfont.h>

#include <lib/string.h>

#include <gui/widgetlist.h>
#include <gui/widgetmenu.h>
#include <gui/settings_screens.h>
#include <gui/icons.h>
#include <gui/msgBox.h>
#include <gui/virtKbd.h>

#include <gui/settings_misc.h>

#include <driver/lcd.h>
#include <driver/speaker.h>
#include <driver/fm_remote.h>

#include <fs/cfg_file.h>

#define MISC_GUIFONT RADONWIDE

static WIDGETMENU widgetMenu;
static WIDGETMENU_CHECKBOX FmRemote;
static WIDGETMENU_CHECKBOX ExtSpkr;
static WIDGETMENU_CHECKBOX develFct;
static WIDGETMENU_CHECKBOX virtKbdLY;

int has_develFct;

void miscSet_sav(void)
{
    int needSave=0;
    int fmSate=FM_getState();
    int spkrState;
    CFG_DATA * cfg;
    /* opening config file */
    
    TEST_VAR(fmSate,FmRemote->checkbox->checked,needSave);
    TEST_VAR(has_develFct,develFct->checkbox->checked,needSave);
    if(SPKR_AVAILABLE())
    {
        spkrState=SPKR_STATE();
        TEST_VAR(spkrState,ExtSpkr->checkbox->checked,needSave);
    }
    TEST_VAR(paramVirtKbd,virtKbdLY->checkbox->checked,needSave);
    
    if(!needSave)
    {
        printk("No change ==> no save\n");
        return;
    }
    
    msgBox_info(getLangStr(STRLNG_SAVE_SETTINGS));
    
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
    
    /* setting the config */
    
    if(FmRemote->checkbox->checked != FM_getState())
    {
        if(FmRemote->checkbox->checked)
            FM_enable();
        else
            FM_disable();
        cfg_writeInt(cfg,"fmRemote",FmRemote->checkbox->checked);
    }
    
    if(develFct->checkbox->checked != has_develFct)
    {
        has_develFct=develFct->checkbox->checked;
        cfg_writeInt(cfg,"develFct",has_develFct);
    }
    
    if(SPKR_AVAILABLE() && ExtSpkr->checkbox->checked!=SPKR_STATE())
    {
        if(ExtSpkr->checkbox->checked)
        {
            SPKR_ON();
        }
        else
        {
            SPKR_OFF();
        }
        cfg_writeInt(cfg,"ExtSpkr",ExtSpkr->checkbox->checked);
    }
    
    if(virtKbdLY->checkbox->checked != paramVirtKbd)
    {
        paramVirtKbd=virtKbdLY->checkbox->checked;
        cfg_writeInt(cfg,"VkbdLY",paramVirtKbd);
    }
    
    cfg_writeFile(cfg,"/medios/medios.cfg");
    cfg_clear(cfg);
}

void misc_setting(void)
{
    ICON logo;
    
    int minX,minY;
    
    logo=icon_get("misc");
    if(!logo)
        icon_load("misc.ico");
    
    settings_initScreen(getLangStr(STRLNG_MISC_SETTINGS),logo,&minX,&minY);
    
    // menuList
    widgetMenu=widgetMenu_create();
    widgetMenu->setRect(widgetMenu,minX,minY,LCD_WIDTH-minX,LCD_HEIGHT-minY);
    widgetMenu->ownItems=true; // the menu will handle items destroy
    
    // standardMenu
    FmRemote=widgetMenuCheckbox_create();
    FmRemote->caption=NULL;
    FmRemote->checkbox->caption=getLangStr(STRLNG_MISC_REMOTE);
    FmRemote->checkbox->checked=FM_getState();
    FmRemote->doAutoSize=true;
    widgetMenu->addItem(widgetMenu,FmRemote);
    
    develFct=widgetMenuCheckbox_create();
    develFct->caption=NULL;
    develFct->checkbox->caption=getLangStr(STRLNG_MISC_DEV);
    develFct->checkbox->checked=has_develFct;
    develFct->doAutoSize=true;
    widgetMenu->addItem(widgetMenu,develFct);
        
    if(SPKR_AVAILABLE())
    {
        ExtSpkr=widgetMenuCheckbox_create();
        ExtSpkr->caption=NULL;
        ExtSpkr->checkbox->caption=getLangStr(STRLNG_MISC_SPEAKER);
        ExtSpkr->checkbox->checked=SPKR_STATE();
        ExtSpkr->doAutoSize=true;
        widgetMenu->addItem(widgetMenu,ExtSpkr);
    }
    
    virtKbdLY=widgetMenuCheckbox_create();
    virtKbdLY->caption=NULL;
    virtKbdLY->checkbox->caption="Kbd, text at top";
    virtKbdLY->checkbox->checked=paramVirtKbd;
    virtKbdLY->doAutoSize=true;
    widgetMenu->addItem(widgetMenu,virtKbdLY);
            
    // intial paint
    // set focus
    widgetMenu->setFocus(widgetMenu,FmRemote);    
    widgetMenu->paint(widgetMenu);
    
    settings_evtLoop(widgetMenu,miscSet_sav,-1);
       
    widgetMenu->destroy(widgetMenu);
}

void misc_loadPref(void)
{
    CFG_DATA * cfg;
    int needWrite=0;
           
    cfg=cfg_readFile("/medios/medios.cfg");
               
    if(!cfg)
    {
        printk("Can't open cfg file\n");
        /* creating default */
        cfg=cfg_newFile();
        if(!cfg)
        {
            printk("Can't create new cfg file\n");
            return;
        }
        cfg_writeInt(cfg,"fmRemote",1);
        if(SPKR_AVAILABLE())
            cfg_writeInt(cfg,"ExtSpkr",0);
        cfg_writeInt(cfg,"develFct",0);
        paramVirtKbd=defaultVirtKbdLY();
        cfg_writeInt(cfg,"VkbdLY",paramVirtKbd);        
        needWrite=1;   
    }
    else
    {
        if(cfg_itemExists(cfg,"fmRemote"))
        {
            if(cfg_readInt(cfg,"fmRemote"))
                FM_enable();
            else
                FM_disable();
        }
        else
        {
            cfg_writeInt(cfg,"fmRemote",1);
            FM_enable();
            needWrite=1;
        }
        
        if(cfg_itemExists(cfg,"develFct"))
        {
            has_develFct=cfg_readInt(cfg,"develFct");
        }
        else
        {
            cfg_writeInt(cfg,"develFct",0);
            has_develFct=0;
            needWrite=1;
        }
        
        if(SPKR_AVAILABLE())
        {
            if(cfg_itemExists(cfg,"ExtSpkr"))
            {
                if(cfg_readInt(cfg,"ExtSpkr"))
                {
                    SPKR_ON();
                }
                else
                {
                    SPKR_OFF();
                }
            }
            else
            {
                cfg_writeInt(cfg,"ExtSpkr",0);
                SPKR_OFF();
                needWrite=1;
            }
        }
        
        if(cfg_itemExists(cfg,"VkbdLY"))
        {
            paramVirtKbd=cfg_readInt(cfg,"VkbdLY");
        }
        else
        {
            paramVirtKbd=defaultVirtKbdLY();
            cfg_writeInt(cfg,"VkbdLY",paramVirtKbd);            
            needWrite=1;
        }
        
        
    }
    if(needWrite) cfg_writeFile(cfg,"/medios/medios.cfg");
    cfg_clear(cfg);
}
