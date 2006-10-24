/*
*   HW_timer.h
*
*   AV3XX emulator
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#ifndef __HW_TIMER_H
#define __HW_TIMER_H

#include <emu.h>
#include <core/HW_access.h>

class HW_timer;

#include <HW/HW_TI.h>


class HW_timer : public HW_access {
    public:
        HW_timer(int timer_num,HW_TI * HW_ti);

        uint32_t read(uint32_t addr,int size);
        void write(uint32_t addr,uint32_t val,int size);

        void nxtStep(void);

    private:
        int timer_num;
        int tm_mode;
        int tm_clk;
        int tm_p_scaler;
        int tm_max;
        int tm_trigger;
        int tm_cnt;
        int tm_ps;

        HW_TI * HW_ti;
};

#endif // __HW_TIMER_H
