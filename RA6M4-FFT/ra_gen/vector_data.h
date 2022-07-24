/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (6)
        #endif
        /* ISR prototypes */
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void gpt_counter_overflow_isr(void);
        void adc_scan_end_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW ((IRQn_Type) 4) /* GPT0 COUNTER OVERFLOW (Overflow) */
        #define GPT0_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 4) /* GPT0 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_ADC0_SCAN_END ((IRQn_Type) 5) /* ADC0 SCAN END (A/D scan end interrupt) */
        #define ADC0_SCAN_END_IRQn          ((IRQn_Type) 5) /* ADC0 SCAN END (A/D scan end interrupt) */
        #endif /* VECTOR_DATA_H */