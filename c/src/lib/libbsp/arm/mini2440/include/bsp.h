/**
 * @file
 * @ingroup arm_gp32
 * @brief Global BSP definitons.
 */

/*
 *  Copyright (c) Canon Research France SA.]
 *  Emmanuel Raguet, mailto:raguet@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _BSP_H
#define _BSP_H

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#include <rtems.h>
#include <rtems/iosupp.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>
//#include <usr/rtems-4.11/rtems-4.10.99-mini2440/c/src/lib/libcpu/arm/s3c24xx/include/s3c24xx.h>

#if 1
#include <s3c24xx.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_FEATURE_IRQ_EXTENSION

#define gp32_initButtons() {rPBCON=0x0;}
#define gp32_getButtons() \
    ( (((~rPEDAT >> 6) & 0x3 )<<8) | (((~rPBDAT >> 8) & 0xFF)<<0) )

/**
 * @defgroup arm_gp32 GP32 Support
 * @ingroup bsp_arm
 * @brief GP32 Support Pacakge
 * @{
 */

/**
 * @brief functions to get the differents s3c2400 clks
 * @{
 */

uint32_t get_FCLK(void);
uint32_t get_HCLK(void);
uint32_t get_PCLK(void);
uint32_t get_UCLK(void);

/** @} */

void gp32_setPalette( unsigned char pos, uint16_t color);

/* What is the input clock freq in hertz? */
/** @brief 12 MHz oscillator */

/* * FCLK:HCLK:PCLK = 1:4:8 
** FCLK = 405MHz 
**/
#define BSP_OSC_FREQ  12000000

#if 0
/** @brief FCLK=133Mhz */
#define M_MDIV 81
#define M_PDIV 2
#define M_SDIV 1
/** @brief HCLK=FCLK/2, PCLK=FCLK/2 */
#define M_CLKDIVN 2
/** @brief enable refresh */
#define REFEN	0x1
/** @brief CBR(CAS before RAS)/auto refresh */
#define TREFMD	0x0
/** @brief 2 clk */
#define Trp	0x0
/** @brief 7 clk */
#define Trc	0x3
/** @brief 3 clk */
#define Tchr	0x2
#endif

#define M_MDIV 		0x7f    /* FCLK=405Mhz, origin: 81, to: 0x7f*/</SPAN>  
#define M_PDIV 		2       
#define M_SDIV 		1   
#define M_CLKDIVN 	5 		/* HCLK=FCLK/4, PCLK=FCLK/2 */   
            
  
#define REFEN   0x1 	/* enable refresh */   
#define TREFMD  0x0 	/* CBR(CAS before RAS)/auto refresh */   
#define Trp 	0x0 	/* 2 clk */   
#define Trc 	0x3 	/* 7 clk */   
#define Tchr    0x2 	/* 3 clk */

/**
 * @brief This BSP provides its own IDLE thread to override the RTEMS one.
 *
 *  So we prototype it and define the constant confdefs.h expects
 *  to configure a BSP specific one.
 */
void *bsp_idle_thread(uintptr_t ignored);

/** @} */

#define BSP_IDLE_TASK_BODY bsp_idle_thread

#ifdef __cplusplus
}
#endif

#endif /* _BSP_H */

