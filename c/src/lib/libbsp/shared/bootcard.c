/**
 * @file
 *
 * @ingroup bsp_bootcard
 *
 * @brief Standard system startup.
 *
 *  This is the C entry point for ALL RTEMS BSPs.  It is invoked
 *  from the assembly language initialization file usually called
 *  start.S.  It provides the framework for the BSP initialization
 *  sequence.  The basic flow of initialization is:
 *
 *  + start.S: basic CPU setup (stack, zero BSS)
 *    + boot_card
 *      + bspstart.c: bsp_start - more advanced initialization
 *      + obtain information on BSP memory and allocate RTEMS Workspace
 *      + rtems_initialize_data_structures
 *      + allocate memory to C Program Heap
 *      + initialize C Library and C Program Heap
 *      + bsp_pretasking_hook
 *      + if defined( RTEMS_DEBUG )
 *        - rtems_debug_enable( RTEMS_DEBUG_ALL_MASK );
 *      + rtems_initialize_before_drivers
 *      + bsp_predriver_hook
 *      + rtems_initialize_device_drivers
 *        - all device drivers
 *      + bsp_postdriver_hook
 *      + rtems_initialize_start_multitasking
 *        - 1st task executes C++ global constructors
 *          .... appplication runs ...
 *          - exit
 *      + will not return to here
 *
 *  This style of initialization ensures that the C++ global
 *  constructors are executed after RTEMS is initialized.
 *  Thanks to Chris Johns <cjohns@plessey.com.au> for the idea
 *  to move C++ global constructors into the first task.
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <bsp/bootcard.h>

#include <rtems.h>

#if 0
/*GPBREGISTER*/
#define rGPBCON    (*(volatile unsigned *)0x56000010)	//Port B control
#define rGPBDAT    (*(volatile unsigned *)0x56000014)	//Port B data

/*GPBCON_CONF*/

#define GPB5_msk (3<<5*2)
#define GPB6_msk (3<<6*2)
#define GPB7_msk (3<<7*2)
#define GPB8_msk (3<<8*2)

#define GPB5_out (1<<5*2)
#define GPB6_out (1<<6*2)
#define GPB7_out (1<<7*2)
#define GPB8_out (1<<8*2)

/*GPBDATE*/
unsigned int code[]={0xFDF,0xFBF,0xF7F,0xEFF};
#endif
/*
 *  At most a single pointer to the cmdline for those target
 *  short on memory and not supporting a command line.
 */
const char *bsp_boot_cmdline;

/*
 *  This is the initialization framework routine that weaves together
 *  calls to RTEMS and the BSP in the proper sequence to initialize
 *  the system while maximizing shared code and keeping BSP code in C
 *  as much as possible.
 */
#if 0
static void delay1(void)
{
    int i,j;

    for(i = 0; i < 1000; i++)
    {
        for(j=0; j < 1000; j++);
     }
     return;
}

static void my_Display1(void)
{
  rGPBCON &= ~((GPB5_msk)|(GPB6_msk)|(GPB7_msk)|(GPB8_msk));
  rGPBCON |= ((GPB5_out)|(GPB6_out)|(GPB7_out)|(GPB8_out));
    int i;
   for(i = 0; i < 4; i++)
     {
        rGPBDAT = code[i];

        delay1();
     }

    return;
}
#endif

void boot_card(
  const char *cmdline
)
{
  //my_Display1(); //------------- -2
  rtems_interrupt_level  bsp_isr_level;

  /*
   *  Make sure interrupts are disabled.
   */
  (void) bsp_isr_level;
  rtems_interrupt_disable( bsp_isr_level );

  //my_Display(); // ------ -1

  bsp_boot_cmdline = cmdline;

  /*
   * Invoke Board Support Package initialization routine written in C.
   */
  bsp_start();

  /*
   *  Initialize the RTEMS Workspace and the C Program Heap.
   */
  bsp_work_area_initialize();

  //my_Display(); // ----- 1
  /*
   *  Initialize RTEMS data structures
   */
  rtems_initialize_data_structures();

  /*
   *  Initialize the C library for those BSPs using the shared
   *  framework.
   */
  bsp_libc_init();

  /*
   *  Let the BSP do any required initialization now that RTEMS
   *  data structures are initialized.  In older BSPs or those
   *  which do not use the shared framework, this is the typical
   *  time when the C Library is initialized so malloc()
   *  can be called by device drivers.  For BSPs using the shared
   *  framework, this routine can be empty.
   */
  bsp_pretasking_hook();

  /*
   *  If debug is enabled, then enable all dynamic RTEMS debug
   *  capabilities.
   *
   *  NOTE: Most debug features are conditionally compiled in
   *        or enabled via configure time plugins.
   */
  #ifdef RTEMS_DEBUG
    rtems_debug_enable( RTEMS_DEBUG_ALL_MASK );
  #endif

  //my_Display(); // ------2
  /*
   *  Let RTEMS perform initialization it requires before drivers
   *  are allowed to be initialized.
   */
  rtems_initialize_before_drivers();

  /*
   *  Execute BSP specific pre-driver hook. Drivers haven't gotten
   *  to initialize yet so this is a good chance to initialize
   *  buses, spurious interrupt handlers, etc..
   *
   *  NOTE: Many BSPs do not require this handler and use the
   *        shared stub.
   */
  bsp_predriver_hook();

  /*
   *  Initialize all device drivers.
   */
   //my_Display(); // ------3
  rtems_initialize_device_drivers();

  //my_Display(); // ------4
  /*
   *  Invoke the postdriver hook.  This normally opens /dev/console
   *  for use as stdin, stdout, and stderr.
   */
  bsp_postdriver_hook();

  //my_Display(); // ------5
  /*
   *  Complete initialization of RTEMS and switch to the first task.
   *  Global C++ constructors will be executed in the context of that task.
   */
  rtems_initialize_start_multitasking();

  /***************************************************************
   ***************************************************************
   *  APPLICATION RUNS NOW!!!  We will not return to here!!!     *
   ***************************************************************
   ***************************************************************/
}
