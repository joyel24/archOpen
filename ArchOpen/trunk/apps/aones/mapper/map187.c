#include "datatypes.h"
#include "unes_mapper.h"
#include "unes.h"
#include "unes_ppu.h"

extern int32 num_1k_VROM_banks;
extern int32 num_8k_ROM_banks;

static uint8  regs[8];
static uint8  bregs[8];
static uint32 ex_bank_enabled,ex_bank_mode;
static uint32 prg0,prg1,ex_prg0,ex_prg1;
static uint32 chr01,chr23,chr4,chr5,chr6,chr7;
static uint32 chr_swap() { return regs[0] & 0x80; }
static uint32 prg_swap() { return regs[0] & 0x40; }
static uint8 irq_enabled; // IRQs enabled
static uint8 irq_counter; // IRQ scanline counter, decreasing
static uint8 irq_latch;   // IRQ scanline counter latch
/////////////////////////////////////////////////////////////////////
// Mapper 187

static void map187_MMC3_set_CPU_banks()
{
#if 1
	if(prg_swap())
	{
		mmc_bankromALL4(num_8k_ROM_banks-2,prg1,prg0,num_8k_ROM_banks-1);
	}
	else
	{
		mmc_bankromALL4(prg0,prg1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
	}
#else
	if(ex_bank_mode & 0x80){
		if(ex_bank_mode & 0x20){
			uint32 bn = (ex_bank_mode&0x1f)<<1;
			mmc_bankromALL4(bn,bn+1,bn+2,bn+3);
		}else{
			uint32 bn = (ex_bank_mode&0x1F)<<1;
			mmc_bankromALL4(bregs[6],bregs[7],bn,bn+1);
		}
	}
	else{
		mmc_bankromALL4(bregs[6],bregs[7],num_8k_ROM_banks-2,num_8k_ROM_banks-1);
	}

#endif
}

static void map187_MMC3_set_PPU_banks()
{
	if(num_1k_VROM_banks)
	{
		if(chr_swap())
		{
			mmc_bankvromALL8(chr4,chr5,chr6,chr7,chr01,chr01+1,chr23,chr23+1);
		}
		else
		{
			mmc_bankvromALL8(chr01,chr01+1,chr23,chr23+1,chr4,chr5,chr6,chr7);
		}
	}
}


static void map187_Reset()
{
	int i;
	// clear registers FIRST!!!
	for(i = 0; i < 8; i++){ regs[i] = 0x00; bregs[i] =0;}

	// set CPU bank pointers
	prg0 = 0;
	prg1 = 1;

	// set VROM banks
	if(num_1k_VROM_banks)
	{
		chr01 = 0;
		chr23 = 2;
		chr4  = 4;
		chr5  = 5;
		chr6  = 6;
		chr7  = 7;
		map187_MMC3_set_PPU_banks();
	}
	else
	{
		chr01 = chr23 = chr4 = chr5 = chr6 = chr7 = 0;
	}

	ex_prg0 = 0;
	ex_prg1 = 1;
	ex_bank_enabled = ex_bank_mode = 0;
	map187_MMC3_set_CPU_banks();
//	set_CPU_banks(0,1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);

	irq_enabled = 0;
	irq_counter = 0;
	irq_latch = 0;
}

static void map187_MemoryWriteLow(uint32 addr, uint8 data)
{
	//LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
	if(addr == 0x5000)
	{
//		map187_MMC3_set_CPU_banks();

		if(data & 0x80)
		{
			if(data & 0x20)
			{
				mmc_bankrom4((data & 0x1E)*2+0);
				mmc_bankrom5((data & 0x1E)*2+1);
				mmc_bankrom6((data & 0x1E)*2+2);
				mmc_bankrom7((data & 0x1E)*2+3);
			}
			else
			{
				mmc_bankrom5((data & 0x1F)*2+0);
				mmc_bankrom6((data & 0x1F)*2+1);
			}
		}
		else
		{
			mmc_bankromALL4(ex_prg0,ex_prg1,num_8k_ROM_banks-2,num_8k_ROM_banks-1);
		}
		ex_bank_mode = data;
	}
}

static void map187_MemoryWrite(uint32 addr, uint8 data)
{
	//LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
	switch(addr/*&0xF003*/)
	{
#if 1
	case 0x8000:
		{
			ex_bank_enabled = 0;
			regs[0] = data;
			map187_MMC3_set_PPU_banks();
			//map187_MMC3_set_CPU_banks();
		}
		break;

	case 0x8001:
		{
			uint32 bank_num;
			regs[1] = data;
			bank_num = regs[1];

			if(ex_bank_enabled)
			{
				if(regs[0] == 0x2A)
				{
					mmc_bankrom5(0x0F);
				}
				if(regs[0] == 0x28)
				{
					mmc_bankrom6(0x17);
				}
			}
			else
			{
				switch(regs[0] & 0x07)
				{
				case 0x00:
					{
						if(num_1k_VROM_banks)
						{
							bank_num &= 0xfe;
							chr01 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x01:
					{
						if(num_1k_VROM_banks)
						{
							bank_num &= 0xfe;
							chr23 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x02:
					{
						if(num_1k_VROM_banks)
						{
							chr4 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x03:
					{
						if(num_1k_VROM_banks)
						{
							chr5 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x04:
					{
						if(num_1k_VROM_banks)
						{
							chr6 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x05:
					{
						if(num_1k_VROM_banks)
						{
							chr7 = bank_num;
							map187_MMC3_set_PPU_banks();
						}
					}
					break;

				case 0x06:
					{
						if((ex_bank_mode & 0xA0) != 0xA0)
						{
							prg0 = bank_num;
							map187_MMC3_set_CPU_banks();
						}
						ex_prg0 = data;
					}
					break;

				case 0x07:
					{
						if((ex_bank_mode & 0xA0) != 0xA0)
						{
							prg1 = bank_num;
							map187_MMC3_set_CPU_banks();
						}
						ex_prg1 = data;
					}
					break;
				}
			}
//			if((regs[0] & 0x07) == 0x06) ex_prg0 = data;
//			if((regs[0] & 0x07) == 0x07) ex_prg1 = data;
		}
		break;

	case 0x8003:
		{
			ex_bank_enabled = 1;
			if(!(data & 0xF0))
			{
				mmc_bankrom6(num_8k_ROM_banks-2);
			}
		}
		break;
#else
	case 0x8000:
		ex_bank_enabled = 0;
		regs[0] = data;
		break;
	case 0x8001:
		bregs[regs[0]&7] = data;
		map187_MMC3_set_CPU_banks();
		break;
	case 0x8003:
		ex_bank_enabled = 1;
		break;
#endif
	case 0xA000:
		{
			regs[2] = data;

			//if(parent_NES->ROM->get_mirroring() != NES_PPU::MIRROR_FOUR_SCREEN)
			if (mmc_getinfo()->var.mirroring!=5)
			{
				if(data & 0x01)
				{
					ppu_mirror(0,0,1,1);
				}
				else
				{
					ppu_mirror(0,1,0,1);
				}
			}
		}
		break;

	case 0xA001:
		{
			regs[3] = data;
		}
		break;

	case 0xC000:
		regs[4] = data;
		irq_counter = regs[4];
		break;

	case 0xC001:
		regs[5] = data;
		irq_latch = regs[5];
		break;

	case 0xE000:
	case 0xE002:
		regs[6] = data;
		irq_enabled = 0;
		break;

	case 0xE001:
	case 0xE003:
		regs[7] = data;
		irq_enabled = 1;
		break;
	}
}

static void map187_HSync(uint32 scanline)
{
	if(irq_enabled)
	{
		if((scanline >= 0) && (scanline <= 239))
		{
			//if(parent_NES->ppu->spr_enabled() || parent_NES->ppu->bg_enabled())
			if (ppu_enabled())
			{
				if(!(irq_counter--))
				{
					irq_counter = irq_latch;
					//parent_NES->cpu->DoIRQ();
					doIRQ();
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////

static void map187_getstate(SnssMapperBlock *state)
{


}


static void map187_setstate(SnssMapperBlock *state)
{

}


mapintf_t map187_intf =
{
   187, /* mapper number */
   "map187", /* mapper name */
   map187_Reset, /* init routine */
   NULL, /* vblank callback */
   map187_HSync, /* hblank callback */
   map187_getstate, /* get state (snss) */
   map187_setstate, /* set state (snss) */   
   NULL,  /*Read low*/
   NULL,  /*Read saveram or rom*/
   NULL,  /*Write high regs 0x4000-0x4017*/
   map187_MemoryWriteLow,  /*Write low   0x4018-0x5FFF*/
   NULL,  /*Write saveram*/
   map187_MemoryWrite,  /*Write rom*/
   NULL,   /*Latch FDFE*/
   NULL,   /*Latch renderscreen*/
   NULL,   /*Latch Address*/
   NULL    /*SetBarcodeValue*/         
};

