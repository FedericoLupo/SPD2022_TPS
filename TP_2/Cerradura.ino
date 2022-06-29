// C++ code
//
#include <LiquidCrystal.h>
#include <Keypad.h>
#include<string.h>

#define VERDE		9
#define ROJO		10
#define LEN 		7
#define LED_TIME	1000
#define TIME_LIMIT	6000
#define BOTTON_C	A4
#define BOTTON_R	12
#define BOTTON_T	11
#define ROWS		4
#define COLS		4

//Definimos leds
int leds[2] = {VERDE, ROJO};
//Definimos botónes
int botones[3]= {BOTTON_C, BOTTON_R, BOTTON_T};

//Definimos los pines de la pantalla
LiquidCrystal lcd(8, 13, A0, A1, A2, A3);//Pines conectados al Arduino

//Definimos los pines del teclado
byte rowPins[ROWS] = { 7,6,5,4 }; 			//Pines conectados al Arduino
byte colPins[COLS] = { 3,2,1,0 }; 

//Definimos el Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//Keypad
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  //Inicia la pantalla
  lcd.begin(16,2);
  
  //Inicia los leds
  for(int i=0;i<2;i++){
    pinMode(leds[i],OUTPUT);
  }
  
  //Inicia los Botones
  for(int i=0;i<3;i++){
    pinMode(botones[i],OUTPUT);
  }
  
  //Seteamos el mensaje
  mensajeUno("PASSWORD:",0,0);
}

//Definimos clave default
char passDefault[LEN]= {'1','C','2','0','2','2'}; // Clave por default
int posicion=0;    							//Necesaria para la clave
int clave=0;       							//Para el LCD
int cursor=7; 								//Posicion inicial de la clave en el LCD

int i=0;
int pulsacion=0;
int contador=2;
int botonAntesC=LOW;
int botonAntesV=LOW;
int botonAntes=LOW;
unsigned long millisAntes = LOW;
int estado = HIGH;
char tecla;
char cIngresada[LEN];
int retorno=2;

void mensajeUno(char* imprime, int c,int f)
{
  lcd.setCursor(c,f);
  lcd.print(imprime);
}

 void imprimealgo(){
  
  for(int j=0;j<LEN;j++){
	lcd.print(passDefault[j]);
  }
}      

void enciendeLed(int pinled)
{
  	digitalWrite(pinled,estado);
  	estado = !estado;
  	
}

void secuenciaLed(int pin)
{
    unsigned long millisAhora = millis();
 
    if (millisAhora < TIME_LIMIT &&
      millisAhora - millisAntes >= LED_TIME  )
    { 
      
      	enciendeLed(pin);
        millisAntes = millisAhora;     	
    }  
}
int guardarNuwPass()
{
  int j;
  for(j=0;j<LEN;j++){
  	passDefault [i]= cIngresada [i];
  }
  imprimealgo();
  return j;
}


int testPass(char* ok, char* error)
{
  int rtn=2;
  
      if(strcmp(passDefault,cIngresada)== 0)
    {
      lcd.clear();
      lcd.print(ok);
      enciendeLed(VERDE);
        
        reset();
	//secuenciaLed(VERDE);
    rtn=0;
    }
    else
    {
      lcd.clear();
      lcd.print(error);
      enciendeLed(ROJO);
      reset();

      rtn=1;
    }    
 return rtn;
}
void reset()
{
    lcd.clear();
    cIngresada[LEN]='\0';
    i=0;
    cursor=7;
    mensajeUno("PASSWORD:",0,0);
}

void botonReset()
{
    int botonAhora = digitalRead(BOTTON_R);
    
    if (botonAhora== HIGH && botonAntes == LOW)
    {
     
        lcd.clear();
      	cIngresada[LEN]='\0';
      	i=0;
      	cursor=7;
     	mensajeUno("PASSWORD:",0,0);
      	
    }

    botonAhora = botonAntes;
}

void botonConfig()
{int algo=1;
    int botonAhora = digitalRead(BOTTON_C);
    
    if (botonAhora == HIGH && botonAntesC == LOW)
    {
       //configuro contraseña;
  
    }

  	botonAntesC	=   botonAhora ;
}

int botonValidaPass()
{
 int rtn=1;
    int botonAhora = digitalRead(BOTTON_T);
    
    if (botonAhora == HIGH && botonAntesV == LOW)
    {
      
        //valida la contraseña ingresada;
     testPass("Password OK", "Password Fail");
     }

  botonAntesV = botonAhora  ;
  
  return rtn;
}
int imprime()
{
  int rtn=1;
  char pulsacion = teclado.getKey() ; 	//Leemos pulsacion
  if (pulsacion != 0 && i <= 5) 		//Si el valor es 0 es que no se ha pulsado ninguna tecla
  {
    cIngresada[i] = pulsacion;
    lcd.print(pulsacion); 				//Imprimimos pulsacion 
    i++; 
    cursor++;             				//Incrementamos el cursor
  	rtn=0;
  }

  return rtn;
}
void loop()
{/*
  char pulsacion = teclado.getKey() ; 	//Leemos pulsacion
  if (pulsacion != 0) 					//Si el valor es 0 es que no se ha pulsado ninguna tecla
  {
    lcd.print(pulsacion); 				//Imprimimos pulsacion
    cursor++;             				//Incrementamos el cursor
  }
  //--- Condicionales para comprobar la clave introducida -----------
  // comparamos entrada con cada uno de los digitos, uno a uno
  if (pulsacion == pass[posicion])
    posicion ++; // aumentamos posicion si es correcto el digito
  if (posicion == LEN)
  { // comprobamos que se han introducido los digitos correctamente
    digitalWrite (VERDE,HIGH);  // encendemos LED
    lcd.setCursor(0,0);      // situamos el cursor el la pos 0 de la linea 0.
    lcd.print("PASSWORD OK     ");// escribimos en LCD
    lcd.setCursor(7,1); // cursor en la posicion 7, linea 1
    clave=1; // indicamos que se ha introducido la clave
    digitalWrite(ROJO,LOW); // apagamos el LED rojo
    digitalWrite(VERDE, HIGH); // encendemos el verde
  }

  //--- Condicionales para resetear clave introducida -------------
  if (pulsacion == '*')
     { // asterisco para resetear el contador
       posicion = 0;
       cursor = 7;
       clave=0;
       posicion=0;
       lcd.setCursor(0,0); // situamos el cursor el la posición 2 de la linea 0.
       lcd.print("NEW PASSWORD:"); // escribimos en LCD
       lcd.setCursor(7,1);
       lcd.print(" "); // borramos de la pantalla los numeros
       lcd.setCursor(7,1);
    
       digitalWrite(ROJO,HIGH); // encendemos el LED rojo
       digitalWrite(VERDE, LOW); // apagamos el verde
  
  }*/
  
 imprime();
  botonReset();
  botonValidaPass();
  botonConfig();
  if(botonAntesC== HIGH)
  {
	 //imprime();
      //configuro contraseña;
      lcd.clear();
      mensajeUno("CONFIGURACION",0,0);
      mensajeUno("NEW PASS:",0,1);
    if(pulsacion==6){
      lcd.clear();
      guardarNuwPass();
        mensajeUno("PASS GUARDADO",0,1);
    }

  }
/*if(botonAntesV == HIGH){
       retorno=testPass("Password OK", "Password Fail");
    if(retorno==0){
        secuenciaLed(VERDE);
}else
{
        enciendeLed(ROJO);
}
    }*/
  
  
  
delay(100);
 }
