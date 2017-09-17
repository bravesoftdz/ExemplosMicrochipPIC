//****************************************************************************
//                EXEMPLO CONVERSOR ANAL�GICO DIGITAL
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              //include do 16F628
#device  ADC=10       //Dispositivo com 10Bits de convers�o (OBRIGAT�RIO!)
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE ME�RIA DOS REGISTRADORES
                                                   
//****************************************************************************
//                    CONFIGURA��ES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=20000000)        //clock usual
#fuses HS,NOWDT,NOLVP,PUT,NOPROTECT

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endere�os da eeprom
                                 
//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************
#define  analogico1       PIN_A0
#define  analogico2       PIN_A1

//*****************************  SA�DAS  *************************************

// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
// caso o pino RW n�o seja utilizado, comente a defini��o lcd_rw
#ifndef lcd_enable
   #define lcd_enable     pin_b5      // pino enable do LCD
   #define lcd_rs         pin_b4      // pino rs do LCD
   //#define lcd_rw      pin_b2      // pino rw do LCD
   #define lcd_d4         pin_b0      // pino de dados d4 do LCD
   #define lcd_d5         pin_b1      // pino de dados d5 do LCD
   #define lcd_d6         pin_b2      // pino de dados d6 do LCD
   #define lcd_d7         pin_b3      // pino de dados d7 do LCD
#endif

//****************************************************************************
//                        CONSTANTES E VARI�VEIS
//****************************************************************************
#define  VREF  4.88E-3;    //Convers�o de 14Bits 5V/2^10

int   w_temp;
int   status_temp;
float ref;
float valor_adc;

//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "c:\dados\projeto\includes\lcd.h"

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line

//****************************************************************************
//                           INTERRUP��ES
//****************************************************************************
#int_global
void interrupcao(){
restart_wdt();
disable_interrupts(global);
#asm
movwf w_temp
swapf status,w
movwf status_temp
#endasm


//***************************************************************************
//                          FIM DA INTERRUPCAO
//***************************************************************************
enable_interrupts(global);
#asm
swapf status_temp,w
movwf status
swapf w_temp,f
swapf w_temp,w
#endasm
}

//****************************************************************************
//                             Sub-rotinas
//****************************************************************************
long int le_adc(int canal){
   set_adc_channel(canal);
   delay_us(10);
   return read_adc();
}

float converte(long int conv){
   return conv*ref;
}

//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURA��ES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno
      setup_wdt(wdt_288ms);
      disable_interrupts(global);
      port_b_pullups(false);    //sem pull-up

//    Configura��o do tris
//    Bit porta: 76543210
      tris_a = 0b00000011;
      tris_b = 0b00000000;
      tris_c = 0b00000000;
      tris_d = 0b00000000;
      tris_e = 0b00000000;
      
//    Inicializa��o do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
 
      setup_adc(ADC_CLOCK_INTERNAL);      //Configura o adc para clock interno
      setup_adc_ports(RA0_RA1_RA3_ANALOG);    //Entradas RA0 e RA1 como anal�gicas
      
//******************** INICIALIZA��O DE VARI�VEIS ***************************
      ref = VREF;
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      
      // Tela 1 - Canal 0
      valor_adc = converte(le_adc(0)); //Converte a tens�o atual na entrada
      lcd_pos_xy(1,1);
      lcd_escreve("ENTRADA ANALOG 1");
      lcd_pos_xy(1,2);
      printf(lcd_escreve,"Valor: %1.3fV",valor_adc);
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');

      // Tela 2 - Canal 1
      valor_adc = converte(le_adc(1)); //Converte a tens�o atual na entrada
      lcd_pos_xy(1,1);
      lcd_escreve("ENTRADA ANALOG 2");
      lcd_pos_xy(1,2);
      printf(lcd_escreve,"Valor: %1.3fV",valor_adc);
      delay_ms(5000);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
      }
//********************************* FIM **************************************
}


















































