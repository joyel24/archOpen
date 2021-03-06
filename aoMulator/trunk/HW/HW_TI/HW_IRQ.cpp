
/*
*   HW_IRQ.cpp
*
*   AV3XX emulator
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/
#include <stdlib.h>
#include <stdio.h>

#include <HW/HW_IRQ.h>



extern char ** str_irq_fiq;

#include "irq_cmd_line_fct.h"

HW_IRQ::HW_IRQ(void):HW_access(IRQ_START,IRQ_END,"IRQ/FIQ")
{
    init_irq_static_fct(this);
    for(int i=0;i<NB_OF_REG;i++)
    {
        status[i]=0xFFFF;
    }

    for(int i=0;i<NB_IRQ;i++)
    {

        enable[i]=0;
        fsel[i]=0;
    }

    for(int i=0;i<NB_INT;i++)
        prio[i]=i;
    
    entry[0]=0;
    entry[1]=0;

    /*for(int i=0;i<NB_IRQ;i++)
        prio[i]=i;*/
    have_int_IRQ = false;
    have_int_FIQ = false;
    eabase = 0x0;
    bloc_size=0x0;
    raw=id=0;
}

void HW_IRQ::do_INT(int num)
{
#ifndef DSC21
    int type=((0x1<<REAL_NUM(num))&fsel[REG_NUM(num)])?FIQ:IRQ;
#else
    int type=(num==WDT_FIQ)?FIQ:IRQ;
    num=(num==WDT_FIQ)?0:num;
#endif
    status[(type==FIQ?0:IRQ_OFFSET)+REG_NUM(num)] &= ((~(0x1<<REAL_NUM(num))) & 0xFFFF);

    calcEntry();

    DEBUG_HW(IRQ_HW_DEBUG,"%s Doing %s num = %x (%d)",
        name,type==FIQ?"FIQ":"IRQ",num,
        (type==FIQ?0:NB_FIQ)+REG_NUM(num)  );
    if(type==FIQ)
    {
        have_int_FIQ = true;
        if(chkFiqFlag())
        {
            DEBUG_HW(IRQ_HW_DEBUG,"with fiq handler");
            cur_irq_fct = cpu_do_fiq;
        }
    }
    else
    {
        have_int_IRQ = true;
        
        if(chkIrqFlag())
        {
            DEBUG_HW(IRQ_HW_DEBUG,"with irq handler");
            cur_irq_fct = cpu_do_irq;
        }
        /*if(num==15)
            CHG_RUN_MODE(STEP)*/
    }
    DEBUG_HW(IRQ_HW_DEBUG,"\n");
}
