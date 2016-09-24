// 用来存放STM32寄存器映射的代码

// 外设  perirhral

#define  PERIPH_BASE               ((unsigned int)0x40000000)
#define  APB1PERIPH_BASE           PERIPH_BASE
#define  APB2PERIPH_BASE          (PERIPH_BASE + 0x10000)
#define  AHBPERIPH_BASE           (PERIPH_BASE + 0x20000)


#define  RCC_BASE                (AHBPERIPH_BASE + 0x1000)
#define  GPIOB_BASE              (APB2PERIPH_BASE + 0x0C00)


#define  RCC_APB2ENR            *(unsigned int*)(RCC_BASE + 0x18)
	
//#define  GPIOB_CRL              *(unsigned int*)(GPIOB_BASE + 0x00)
//#define  GPIOB_CRH              *(unsigned int*)(GPIOB_BASE + 0x04)
//#define  GPIOB_IDR              *(unsigned int*)(GPIOB_BASE + 0x08)
//#define  GPIOB_ODR              *(unsigned int*)(GPIOB_BASE + 0x0C)
//#define  GPIOB_BSRR             *(unsigned int*)(GPIOB_BASE + 0x10)
//#define  GPIOB_BRR              *(unsigned int*)(GPIOB_BASE + 0x14)
//#define  GPIOB_LCKR             *(unsigned int*)(GPIOB_BASE + 0x18)
	
typedef unsigned int      uint32_t;
typedef unsigned short    uint16_t;

typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
}GPIO_TypeDef;


#define GPIOB   ((GPIO_TypeDef*)GPIOB_BASE)





