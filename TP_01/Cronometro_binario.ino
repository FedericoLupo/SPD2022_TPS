/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *   
 *	El numero 15 (0000001111) se representaria encendiendo los 
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos 
 *	los led anteriores y encenderse uno de los leds VERDES. 
 *	Notese, en el ejemplo los 0 representan los led apagados 
 *	y los 1 los led encendidos). 
 *   
 *	-------------------------------------------------------
 *   
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga. 
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los 
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los 
 *	leds encendidos sea perceptible para el ojo humano y 
 *	documentar cada función creada en el código. 
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/


//--- Defines ---//
#define PRINCIPIO 0         //? To start the counter from 0
#define FIN 1023        //! Max amount of secods to show
#define STOP 13
#define START 4
#define botonStart 2
#define botonReset 3
#define largoLeds 9
#define TIME 1000 //? Secods expresed in miliseconds
//--- End Defines ---//

void setup()
{
  Serial.begin(9600);
  
  for(int i=START-2; i<=STOP; i++)
  {
    pinMode(i,OUTPUT);
  }
}


//VARIABLES
int contador = PRINCIPIO;
int Led[largoLeds];
short botonAntes=0;
unsigned long millisAntes=0;
short status=LOW;
short banderaReset=0;

//FUNCIONES

//setea el array de led en 4 hasta 13
void setearLeds()
{
	for(int i=START; i<=STOP; i++)
  	{
  		Led[i-4]=i;
  	}
}


//muestra un BINARIO con un mensaje, en cierto rango
void mostrarBINARIO(char Mensaje[], int count, int max, int min)
{
	if(count<=max && count>=min)
    {
  		Serial.print(Mensaje);
  		Serial.println(count, BIN);	
    }
}

//muestra un DECIMAL con un mensaje, en cierto rango
void mostrarDECIMAL(char Mensaje1[], int count, char Mensaje2[], int max, int min)
{
	if(count<=max && count>=min)
    {
  		Serial.print(Mensaje1);
  		Serial.print(count);
      	Serial.print(Mensaje2);
    }
}

//Funcion usada para la impresion por consola
void mostrarContador1()
{	
 	mostrarDECIMAL("Seconds: ",contador,"     | ",9,PRINCIPIO);
  	mostrarDECIMAL("Seconds: ",contador,"    | ",99,10);
  	mostrarDECIMAL("Seconds: ",contador,"   | ",999,100);
  	mostrarDECIMAL("Seconds: ",contador,"  | ",FIN,1000);
  	
	mostrarBINARIO("Binary: 000000000",contador,1,PRINCIPIO);
  	mostrarBINARIO("Binary: 00000000",contador,3,2);
  	mostrarBINARIO("Binary: 0000000",contador,7,4);
  	mostrarBINARIO("Binary: 000000",contador,15,8);
  	mostrarBINARIO("Binary: 00000",contador,31,16);
  	mostrarBINARIO("Binary: 0000",contador,63,32);
  	mostrarBINARIO("Binary: 000",contador,127,64);
  	mostrarBINARIO("Binary: 00",contador,255,128);
  	mostrarBINARIO("Binary: 0",contador,511,256);
 	mostrarBINARIO("Binary: ",contador,FIN,512); 	
}

//imprime por consola de manera no estetica
void mostrarContador2()
{
  	int i;
  	int array[10];
  	int binario=0;
  	int aux=contador;
 	for(int i=0;i<10;i++)
  	{
  		array[i]=0;
  	}
  	Serial.print("Seconds: ");
  	Serial.print(contador);
  	Serial.print(" | Binary: ");
	
  	for(i=10;i>0;i--)
    {
        if(aux>=0)
        {
    		binario=aux%2;
        	aux=aux/2;
        	array[i]=binario;
        }
    }
  
  	for(int i=0;i<10;i++)
 	{	
      	Serial.print(array[i]);
    }
  		Serial.println();
}

//funcion para encender el led
void encenderLeds()
{
  int i;
  setearLeds();
  for(i=9;i>=0;i--)
  {
    //retorna el numero de bits de un numero en concreto(contador numero a )
    //bitRead(contador,i); 
    digitalWrite(Led[i],bitRead(contador,i)); 
  }
}


//funcion que controla la secuencia, si status es 1 incia, 0 pausa con MILLIS
void secuencia()
{ 
  unsigned long millisAhora = millis();

  if(millisAhora-millisAntes>=TIME)
  {
    if(status==HIGH)
  	{
      	contador++;
    	mostrarContador1();
    	//mostrarContador2();
  		encenderLeds();
    	if(contador>FIN){contador=PRINCIPIO;}
    	millisAntes=millisAhora;
    }
    else if(status==LOW)
  	{
    	mostrarContador1();
    	//mostrarContador2();
  		encenderLeds();
    	if(contador>FIN){contador=PRINCIPIO;}
    	millisAntes=millisAhora;
    }
  }
}

//lee el boton en el pin 2 y actualiza status en 1 o 0
void botonComienzo()
{
  int botonAHORA = digitalRead(botonStart);

  	if(botonAHORA == HIGH && botonAntes == LOW)
  	{
    	status=HIGH;
  	}
  	else if(botonAHORA == HIGH && botonAntes == HIGH)
  	{
  		status=LOW;
  	}
  	botonAntes=botonAHORA;
 
}

void loop()
{ 
 botonComienzo();
 int boton = digitalRead(botonReset);
 if(boton == HIGH)
 {
    contador=0;
 }
 secuencia();
 delay(100);
}
