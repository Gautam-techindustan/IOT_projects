#ifndef _ESP32_SOC_H_
    #define _ESP32_SOC_H_
    
    #include <stdint.h>
    
    //Register Bits{{
    #define BIT31   0x80000000
    #define BIT30   0x40000000
    #define BIT29   0x20000000
    #define BIT28   0x10000000
    #define BIT27   0x08000000
    #define BIT26   0x04000000
    #define BIT25   0x02000000
   #define BIT24   0x01000000
    #define BIT23   0x00800000
    #define BIT22   0x00400000
    #define BIT21   0x00200000
    #define BIT20   0x00100000
    #define BIT19   0x00080000
    #define BIT18   0x00040000
    #define BIT17   0x00020000
    #define BIT16   0x00010000
    #define BIT15   0x00008000
    #define BIT14   0x00004000
    #define BIT13   0x00002000
    #define BIT12   0x00001000
    #define BIT11   0x00000800
    #define BIT10   0x00000400
    #define BIT9     0x00000200
    #define BIT8     0x00000100
    #define BIT7     0x00000080
    #define BIT6     0x00000040
    #define BIT5     0x00000020
    #define BIT4     0x00000010
    #define BIT3     0x00000008
    #define BIT2     0x00000004
    #define BIT1     0x00000002
    #define BIT0     0x00000001
    //}}
    
    #define PRO_CPU_NUM (0)
    #define APP_CPU_NUM (1)
    
    //Registers Operation {{
    #define ETS_UNCACHED_ADDR(addr) (addr)
    #define ETS_CACHED_ADDR(addr) (addr) 
    
    #define BIT(nr)                 (1UL << (nr))
    
    //write value to register
    #define REG_WRITE(_r, _v)    (*(volatile uint32_t *)(_r)) = (_v)
    
    //read value from register
    #define REG_READ(_r) (*(volatile uint32_t *)(_r))
    
    //get bit or get bits from register
    #define REG_GET_BIT(_r, _b)  (*(volatile uint32_t*)(_r) & (_b))
    
    //set bit or set bits to register
    #define REG_SET_BIT(_r, _b)  (*(volatile uint32_t*)(_r) |= (_b))
    
    //clear bit or clear bits of register
    #define REG_CLR_BIT(_r, _b)  (*(volatile uint32_t*)(_r) &= ~(_b))
    
    //set bits of register controlled by mask
    #define REG_SET_BITS(_r, _b, _m) (*(volatile uint32_t*)(_r) = (*(volatile uint32_t*)(_r) & ~(_m)) | ((_b) & (_m)))
    
    //get field from register, used when _f is not left shifted by _f##_S
    #define REG_GET_FIELD(_r, _f) ((REG_READ(_r) >> (_f##_S)) & (_f))
    
    //set field to register, used when _f is not left shifted by _f##_S
    #define REG_SET_FIELD(_r, _f, _v) (REG_WRITE((_r),((REG_READ(_r) & ~((_f) << (_f##_S)))|(((_v) & (_f))<<(_f##_S)))))
    
    //get field value from a variable, used when _f is not left shifted by _f##_S
    #define VALUE_GET_FIELD(_r, _f) (((_r) >> (_f##_S)) & (_f))
    
    //get field value from a variable, used when _f is left shifted by _f##_S
    #define VALUE_GET_FIELD2(_r, _f) (((_r) & (_f))>> (_f##_S))
    
    //set field value to a variable, used when _f is not left shifted by _f##_S
    #define VALUE_SET_FIELD(_r, _f, _v) ((_r)=(((_r) & ~((_f) << (_f##_S)))|((_v)<<(_f##_S))))
    
    //set field value to a variable, used when _f is left shifted by _f##_S
    #define VALUE_SET_FIELD2(_r, _f, _v) ((_r)=(((_r) & ~(_f))|((_v)<<(_f##_S))))
    
   //generate a value from a field value, used when _f is not left shifted by _f##_S
   #define FIELD_TO_VALUE(_f, _v) (((_v)&(_f))<<_f##_S)
   
   //generate a value from a field value, used when _f is left shifted by _f##_S
   #define FIELD_TO_VALUE2(_f, _v) (((_v)<<_f##_S) & (_f))
   
   //read value from register
   #define READ_PERI_REG(addr) (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) 
   
   //write value to register
   #define WRITE_PERI_REG(addr, val) (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val)   
   
   //clear bits of register controlled by mask
   #define CLEAR_PERI_REG_MASK(reg, mask) WRITE_PERI_REG((reg), (READ_PERI_REG(reg)&(~(mask))))
   
   //set bits of register controlled by mask
   #define SET_PERI_REG_MASK(reg, mask)   WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)))
   
   //get bits of register controlled by mask
   #define GET_PERI_REG_MASK(reg, mask)   (READ_PERI_REG(reg) & (mask))
   
   //get bits of register controlled by highest bit and lowest bit
   #define GET_PERI_REG_BITS(reg, hipos,lowpos)      ((READ_PERI_REG(reg)>>(lowpos))&((1<<((hipos)-(lowpos)+1))-1))
   
   //set bits of register controlled by mask and shift
   #define SET_PERI_REG_BITS(reg,bit_map,value,shift) (WRITE_PERI_REG((reg),(READ_PERI_REG(reg)&(~((bit_map)<<(shift))))|(((value) & bit_map)<<(shift)) ))
   
   //get field of register
   #define GET_PERI_REG_BITS2(reg, mask,shift)      ((READ_PERI_REG(reg)>>(shift))&(mask))
   //}}
   
   //Periheral Clock {{
   #define  APB_CLK_FREQ_ROM                            26*1000000
   #define  CPU_CLK_FREQ_ROM                            APB_CLK_FREQ_ROM
   #define  CPU_CLK_FREQ                                APB_CLK_FREQ
   #define  APB_CLK_FREQ                                80*1000000       //unit: Hz
   #define  UART_CLK_FREQ                               APB_CLK_FREQ
   #define  WDT_CLK_FREQ                                APB_CLK_FREQ
   #define  TIMER_CLK_FREQ                              (80000000>>4) //80MHz divided by 16
   #define  SPI_CLK_DIV                                 4
   #define  TICKS_PER_US_ROM                            26              // CPU is 80MHz
   //}}
   
   #define DR_REG_DPORT_BASE                       0x3ff00000
   #define DR_REG_UART_BASE                        0x3ff40000
   #define DR_REG_SPI1_BASE                        0x3ff42000
   #define DR_REG_SPI0_BASE                        0x3ff43000
   #define DR_REG_GPIO_BASE                        0x3ff44000
   #define DR_REG_GPIO_SD_BASE                     0x3ff44f00
   #define DR_REG_FE2_BASE                         0x3ff45000
   #define DR_REG_FE_BASE                          0x3ff46000
   #define DR_REG_RTCCNTL_BASE                     0x3ff48000
   #define DR_REG_RTCIO_BASE                       0x3ff48400
   #define DR_REG_SARADC_BASE                      0x3ff48800
   #define DR_REG_IO_MUX_BASE                      0x3ff49000
   #define DR_REG_RTCMEM0_BASE                     0x3ff61000
   #define DR_REG_RTCMEM1_BASE                     0x3ff62000
   #define DR_REG_RTCMEM2_BASE                     0x3ff63000
   #define DR_REG_HINF_BASE                        0x3ff4B000
   #define DR_REG_UHCI1_BASE                       0x3ff4C000
   #define DR_REG_I2S_BASE                         0x3ff4F000
   #define DR_REG_UART1_BASE                       0x3ff50000
   #define DR_REG_BT_BASE                          0x3ff51000
   #define DR_REG_I2C_EXT_BASE                     0x3ff53000
   #define DR_REG_UHCI0_BASE                       0x3ff54000
   #define DR_REG_SLCHOST_BASE                     0x3ff55000
   #define DR_REG_RMT_BASE                         0x3ff56000
   #define DR_REG_PCNT_BASE                        0x3ff57000
   #define DR_REG_SLC_BASE                         0x3ff58000
   #define DR_REG_LEDC_BASE                        0x3ff59000
   #define DR_REG_EFUSE_BASE                       0x3ff5A000
   #define DR_REG_SPI_ENCRYPT_BASE                 0x3ff5B000
   #define DR_REG_PWM_BASE                         0x3ff5E000
   #define DR_REG_TIMERGROUP0_BASE                 0x3ff5F000
   #define DR_REG_TIMERGROUP1_BASE                 0x3ff60000
   #define DR_REG_SPI2_BASE                        0x3ff64000
   #define DR_REG_SPI3_BASE                        0x3ff65000
   #define DR_REG_I2C1_EXT_BASE                    0x3ff67000
   #define DR_REG_SDMMC_BASE                       0x3ff68000
  #define DR_REG_EMAC_BASE                        0x3ff69000
   #define DR_REG_PWM1_BASE                        0x3ff6C000
   #define DR_REG_I2S1_BASE                        0x3ff6D000
   #define DR_REG_UART2_BASE                       0x3ff6E000
   #define DR_REG_PWM2_BASE                        0x3ff6F000
   #define DR_REG_PWM3_BASE                        0x3ff70000
   #define PERIPHS_SPI_ENCRYPT_BASEADDR      DR_REG_SPI_ENCRYPT_BASE
   
   //Interrupt hardware source table
   //This table is decided by hardware, don't touch this.
   #define ETS_WIFI_MAC_INTR_SOURCE                0
   #define ETS_WIFI_MAC_NMI_SOURCE                 1
   #define ETS_WIFI_BB_INTR_SOURCE                 2
   #define ETS_BT_MAC_INTR_SOURCE                  3
   #define ETS_BT_BB_INTR_SOURCE                   4
   #define ETS_BT_BB_NMI_SOURCE                    5
   #define ETS_RWBT_INTR_SOURCE                    6
   #define ETS_RWBLE_INTR_SOURCE                   7
   #define ETS_RWBT_NMI_SOURCE                     8
   #define ETS_RWBLE_NMI_SOURCE                    9
   #define ETS_SLC0_INTR_SOURCE                    10
   #define ETS_SLC1_INTR_SOURCE                    11
   #define ETS_UHCI0_INTR_SOURCE                   12
   #define ETS_UHCI1_INTR_SOURCE                   13
   #define ETS_TG0_T0_LEVEL_INTR_SOURCE            14
   #define ETS_TG0_T1_LEVEL_INTR_SOURCE            15
   #define ETS_TG0_WDT_LEVEL_INTR_SOURCE           16
   #define ETS_TG0_LACT_LEVEL_INTR_SOURCE          17
   #define ETS_TG1_T0_LEVEL_INTR_SOURCE            18
   #define ETS_TG1_T1_LEVEL_INTR_SOURCE            19
   #define ETS_TG1_WDT_LEVEL_INTR_SOURCE           20
   #define ETS_TG1_LACT_LEVEL_INTR_SOURCE          21
   #define ETS_GPIO_INTR_SOURCE                    22
   #define ETS_GPIO_NMI_SOURCE                     23
   #define ETS_FROM_CPU_INTR0_SOURCE               24
   #define ETS_FROM_CPU_INTR1_SOURCE               25
   #define ETS_FROM_CPU_INTR2_SOURCE               26
   #define ETS_FROM_CPU_INTR3_SOURCE               27
   #define ETS_SPI0_INTR_SOURCE                    28
   #define ETS_SPI1_INTR_SOURCE                    29
   #define ETS_SPI2_INTR_SOURCE                    30
   #define ETS_SPI3_INTR_SOURCE                    31
   #define ETS_I2S0_INTR_SOURCE                    32
   #define ETS_I2S1_INTR_SOURCE                    33
   #define ETS_UART0_INTR_SOURCE                   34
   #define ETS_UART1_INTR_SOURCE                   35
   #define ETS_UART2_INTR_SOURCE                   36
   #define ETS_SDIO_HOST_INTR_SOURCE               37
   #define ETS_ETH_MAC_INTR_SOURCE                 38
   #define ETS_PWM0_INTR_SOURCE                    39
   #define ETS_PWM1_INTR_SOURCE                    40
   #define ETS_PWM2_INTR_SOURCE                    41
   #define ETS_PWM3_INTR_SOURCE                    42
   #define ETS_LEDC_INTR_SOURCE                    43
   #define ETS_EFUSE_INTR_SOURCE                   44
   #define ETS_CAN_INTR_SOURCE                     45
   #define ETS_RTC_CORE_INTR_SOURCE                46
   #define ETS_RMT_INTR_SOURCE                     47
   #define ETS_PCNT_INTR_SOURCE                    48
   #define ETS_I2C_EXT0_INTR_SOURCE                49
   #define ETS_I2C_EXT1_INTR_SOURCE                50
   #define ETS_RSA_INTR_SOURCE                     51
   #define ETS_SPI1_DMA_INTR_SOURCE                52
   #define ETS_SPI2_DMA_INTR_SOURCE                53
   #define ETS_SPI3_DMA_INTR_SOURCE                54
   #define ETS_WDT_INTR_SOURCE                     55
   #define ETS_TIMER1_INTR_SOURCE                  56
   #define ETS_TIMER2_INTR_SOURCE                  57
   #define ETS_TG0_T0_EDGE_INTR_SOURCE             58
   #define ETS_TG0_T1_EDGE_INTR_SOURCE             59
   #define ETS_TG0_WDT_EDGE_INTR_SOURCE            60
   #define ETS_TG0_LACT_EDGE_INTR_SOURCE           61
   #define ETS_TG1_T0_EDGE_INTR_SOURCE             62
   #define ETS_TG1_T1_EDGE_INTR_SOURCE             63
   #define ETS_TG1_WDT_EDGE_INTR_SOURCE            64
   #define ETS_TG1_LACT_EDGE_INTR_SOURCE           65
   #define ETS_MMU_IA_INTR_SOURCE                  66
  #define ETS_MPU_IA_INTR_SOURCE                  67
   #define ETS_CACHE_IA_INTR_SOURCE                68
  //interrupt cpu using table, Please see the core-isa.h
   /*************************************************************************************************************
   *      Intr num                Level           Type                    PRO CPU usage           APP CPU uasge
    *      0                       1               extern level            WMAC                    Reserved
    *      1                       1               extern level            BT/BLE Host VHCI        Reserved
    *      2                       1               extern level            FROM_CPU                FROM_CPU
    *      3                       1               extern level            TG0_WDT                 Reserved
    *      4                       1               extern level            WBB
    *      5                       1               extern level            BT Controller 
    *      6                       1               timer                   FreeRTOS Tick(L1)       FreeRTOS Tick(L1)
    *      7                       1               software                Reserved                Reserved
    *      8                       1               extern level            BLE Controller 
    *      9                       1               extern level            
    *      10                      1               extern edge             Internal Timer
    *      11                      3               profiling
    *      12                      1               extern level
    *      13                      1               extern level
    *      14                      7               nmi                     Reserved                Reserved
  *      15                      3               timer                   FreeRTOS Tick(L3)       FreeRTOS Tick(L3)
    *      16                      5               timer
    *      17                      1               extern level
    *      18                      1               extern level
    *      19                      2               extern level
    *      20                      2               extern level
    *      21                      2               extern level
    *      22                      3               extern edge
    *      23                      3               extern level
    *      24                      4               extern level
    *      25                      4               extern level            Reserved                Reserved
    *      26                      5               extern level            Reserved                Reserved
    *      27                      3               extern level            Reserved                Reserved
    *      28                      4               extern edge             
    *      29                      3               software                Reserved                Reserved
   *      30                      4               extern edge             Reserved                Reserved
    *      31                      5               extern level            Reserved                Reserved
    *************************************************************************************************************
    */
   
   //CPU0 Interrupt number reserved, not touch this.
  #define ETS_WMAC_INUM                           0
   #define ETS_BT_HOST_INUM                        1
   #define ETS_FROM_CPU_INUM                       2
   #define ETS_T0_WDT_INUM                         3
   #define ETS_WBB_INUM                            4
   #define ETS_TG0_T1_INUM                         10 
   //CPU0 Intrrupt number used in ROM, should be cancelled in SDK
   #define ETS_SLC_INUM                            1
   #define ETS_UART0_INUM                          5
   #define ETS_UART1_INUM                          5
   //Other interrupt number should be managed by the user
  
  
   #endif /* _ESP32_SOC_H_ */
