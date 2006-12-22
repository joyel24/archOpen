/*
*   include/kernel/target/arch_AV3XX/dma.h
*
*   MediOS project
*   Copyright (c) 2005 by Christophe THOMAS (oxygen77 at free.fr)
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express of implied.
*/

#ifndef __DMA_DSC21_H
#define __DMA_DSC21_H

#define DMA_BASE           0x30b80

#define DMA_SET_DEV(src,dest) {if(src==1) outw(1,DMA_DEV_SEL); else outw(0,DMA_DEV_SEL);}

#define DMA_SRC_HI         (DMA_BASE+0x08)
#define DMA_SRC_LO         (DMA_BASE+0x06)
#define DMA_DEST_HI        (DMA_BASE+0x0C)
#define DMA_DEST_LO        (DMA_BASE+0x0A)
#define DMA_SIZE           (DMA_BASE+0x04)
#define DMA_DEV_SEL        (DMA_BASE+0x02)
#define DMA_START          (DMA_BASE+0x00)
#define DMA_STATE          (DMA_BASE+0x0E)

#define DMA_SDRAM          0x1

#endif
