//  Variables
int pulsePin = 0;                   // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 7;                   // LED verde
int fadePin = 8;                    // LED amarelo
int fadeRate = 0;                   // used to fade LED on with PWM on fadePin

volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduino finds a beat.

void ledFadeToBeat()
{
   fadeRate -= 15;                         //  set LED fade value
   fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
   analogWrite(fadePin,fadeRate);          //  fade LED
}

void serialOutputWhenBeatHappens()
{
  if (serialVisual == true) //  Code to Make the Serial Monitor Visualizer Work
  { 
    Serial.print("*** Pressentimos um batimento cardiaco *** ");  //ASCII Art Madness
    Serial.print("BPM: ");
    Serial.println(BPM);
  }
  else
  {
    // send heart rate with a 'B' prefix
    Serial.print('B');
    Serial.println(BPM);

    // send time between beats with a 'Q' prefix
    Serial.print('Q');
    Serial.println(IBI);
  }
}

 String resultadoLeituraBPM(String nome, int idade, char sexo, int BPMnovo){
  String estado = "";
    int *zona = new int[5];

    if(sexo == 'F' || sexo == 'f')
      zona[0] = 226 - idade;
    else
      zona[0] = 220 - idade;


    zona[1] = (int) (zona[0] * 0.85);
    zona[2] = (int) (zona[0] * 0.75);
    zona[3] = (int) (zona[0] * 0.6);
    zona[4] = (int) (zona[0] * 0.3);

    //Serial.print(nome+" \n Idade: "+idade+" Sexo: "+sexo+" BPM: "+BPM);
    //Serial.print(":Status:");
    if(BPMnovo>zona[0])
      estado = "Batimentos acima do saudável";
    else
      if(BPMnovo>zona[1])
        estado = "Batimentos acima do nível aceitável de atividades pesadas";
      else
        if(BPMnovo>zona[2])
          estado = "Batimentos no nível aceitável de atividades pesadas";
        else
          if(BPMnovo>zona[3])
            estado = "Batimentos no nível aceitável de atividades leves";
          else
            if(BPMnovo>zona[4])
              estado = "Batimentos no nível normal";
            else
              if(BPMnovo>6)
                estado = "Batimentos abaixo do saudável";
              else
                estado = "Morto";
    return estado;

  }

void medirBatimentosCardiacos()
{
    int tempoDeEspera = 30 * 1000;
    long inicio; 
    int medicoes = 0; 
    int valores = 0;
    int BPMtemp = 0;

    Serial.println("");
    Serial.println("Por favor, encoste o dedo no medidor e aguarde 30 segundos");

    inicio = millis();
    while(millis() - inicio < tempoDeEspera)
    {
        if(millis() - inicio > 1000){

        }
        if (QS == true) // A Heartbeat Was Found
        { 
          medicoes++;
          valores = valores + BPM;
          
          // BPM and IBI have been Determined
          serialOutputWhenBeatHappens(); // A Beat Happened, Output that to serial.
          
          fadeRate = 255; // Makes the LED Fade Effect Happen, Set 'fadeRate' Variable to 255 to fade LED with pulse

          // Quantified Self "QS" true when arduino finds a heartbeat
          QS = false; // reset the Quantified Self flag for next time
        }
        ledFadeToBeat(); // Makes the LED Fade Effect Happen
        delay(20);
    }
  if(medicoes > 0)
  {
    BPMtemp = (int)valores/medicoes;
    String estado = resultadoLeituraBPM("Jon Doe", "23", 'M', BPMtemp);
    
    Serial.println("-> Jon Doe, Idade: 23, Sexo: M");
    Serial.println("--> ")
    Serial.print(estado);
  }
  else
  {
    Serial.println("Nenhum batimento pressentido.");
  }
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
