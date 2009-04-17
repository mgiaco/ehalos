#include <avr32/io.h>

#include <sys/sysregs.h>
#include <stdlib.h>

#include "../inc/init.h"

#include "sdramc_at32ap7000.h"
#include "pm_at32ap7000.h"



void low_level_init() {

	// Reset PM. Makes sure we get the expected clocking after a soft reset (e.g.: JTAG reset)
	pm_reset();

	// Start PLL0 giving 80 MHz clock
	pm_pll_opt_t pll_opt ={
	    .pll_id = 0,
	    .mul = 15,
	    .div = 2,
	    .osc_id = 0,
	    .count = 16,
	    .wait_for_lock = 1,
	};

	pm_start_pll(&pll_opt);

	// Divide HSB by 2, PBB by 2 and PBA by 4 to keep them below maximum ratings
	pm_set_clock_domain_scaler(PM_HSB_DOMAIN, 2);
	pm_set_clock_domain_scaler(PM_PBB_DOMAIN, 2);
	pm_set_clock_domain_scaler(PM_PBA_DOMAIN, 8);

	// TODO: search for the magic numbers
	AVR32_SMC.cs[0].mode  = 0x00031103 ;
	AVR32_SMC.cs[0].cycle = 0x000c000d ;
	AVR32_SMC.cs[0].pulse = 0x0b0a0906 ;
	AVR32_SMC.cs[0].setup = 0x00010002 ;

	pm_set_mclk_source(PM_PLL0);

	// after this, the AP7000 should work above 120MHz.

	sdramc_refresh_rate_update();
}



void  clk_switch_init () {
    unsigned int  mcctrl;

    mcctrl          =  AVR32_PM.mcctrl;
    mcctrl         &= ~AVR32_PM_MCCTRL_PLLSEL_MASK;
    mcctrl         |= (0 << AVR32_PM_MCCTRL_PLLSEL_OFFSET) &
                       AVR32_PM_MCCTRL_PLLSEL_MASK;

    AVR32_PM.mcctrl =  mcctrl;
}
