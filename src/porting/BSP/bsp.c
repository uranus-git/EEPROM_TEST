extern void USART_Configuration(void);
extern void NVIC_Configuration(void);
extern void TIM14_PWM_Init(void);

void bsp_init(void)
{
    USART_Configuration();
    NVIC_Configuration();
    TIM14_PWM_Init();
}
