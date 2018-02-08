//  -------------------------------------------------------------------------------------------------------------------------
//  PULSE MONITOR VARIABLES  - START
int pulsePin = 0;                   // Pulse Sensor purple wire connected to analog pin 0

volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduino finds a beat.

volatile int rate[10];                           // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find IBI
volatile int P = 512;                          // used to find peak in pulse wave, seeded
volatile int T = 512;                          // used to find trough in pulse wave, seeded
volatile int thresh = 525;                     // used to find instant moment of heart beat, seeded
volatile int amp = 100;                        // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;             // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;           // used to seed rate array so we startup with reasonable BPM
//  PULSE MONITOR VARIABLES - END
//  -------------------------------------------------------------------------------------------------------------------------

void serialOutputWhenBeatHappens(){
    Serial.println();
    Serial.println("*** Pressentimos um batimento cardiaco *** ");  //ASCII Art Madness
    Serial.println("BPM: ");
    Serial.println(BPM);
}

void exibirNaSerialConclusaoDeMedidorCardiaco(String nome, int idade, char sexo, double bpm, String conclusao)
{
    Serial.println();
    Serial.println("******************** RESULTADO MEDIDOR CARDIACO: ********************");
    Serial.println("Nome: " + nome);
    Serial.println("Idade: " + String(idade) + "; Sexo: " + sexo + "; Media de batimentos: "+ String(bpm));
    Serial.println("Conclusão: " + conclusao);
}

String resultadoLeituraBPM(int idade, char sexo, double BPMnovo){
    double *zona = new double[5];

    if(sexo == 'F' || sexo == 'f')
      zona[0] = 226 - idade;
    else
      zona[0] = 220 - idade;

    zona[1] = (zona[0] * 0.85);
    zona[2] = (zona[0] * 0.75);
    zona[3] = (zona[0] * 0.6);
    zona[4] = (zona[0] * 0.3); 
    
    if(BPMnovo > zona[0])
      return "Batimentos acima do saudável";
    
    if(BPMnovo > zona[1])
      return "Batimentos acima do nível aceitável de atividades pesadas";
    
    if(BPMnovo > zona[2])
      return "Batimentos no nível aceitável de atividades pesadas";
    
    if(BPMnovo > zona[3])
      return "Batimentos no nível aceitável de atividades leves";

    if(BPMnovo > zona[4])
      return "Batimentos no nível normal";
            
    if(BPMnovo > 6)
      return "Batimentos abaixo do saudável";

    return "Você está morto.";
}

ISR(TIMER2_COMPA_vect) {        //triggered when Timer2 counts to 124
    cli();                                      // disable interrupts while we do this
    Signal = analogRead(pulsePin);              // read the Pulse Sensor
    sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
                          
    //  find the peak and trough of the pulse wave
    if(Signal < thresh && N > (IBI/5)*3) // avoid dichrotic noise by waiting 3/5 of last IBI
    {
      if (Signal < T) // T is the trough
      {
        T = Signal; // keep track of lowest point in pulse wave
      }
    }

    if(Signal > thresh && Signal > P)
    {          // thresh condition helps avoid noise
      P = Signal;                             // P is the peak
    }                                        // keep track of highest point in pulse wave

    //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
    // signal surges up in value every time there is a pulse
    if (N > 250)
    {                                   // avoid high frequency noise
        if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) )
        {
            Pulse = true;                               // set the Pulse flag when we think there is a pulse
            //digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
            IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
            lastBeatTime = sampleCounter;               // keep track of time for next pulse

            if(secondBeat)
            {                        // if this is the second beat, if secondBeat == TRUE
                secondBeat = false;                  // clear secondBeat flag
                for(int i=0; i<=9; i++) // seed the running total to get a realisitic BPM at startup
                {
                    rate[i] = IBI;
                }
            }

            if(firstBeat) // if it's the first time we found a beat, if firstBeat == TRUE
            {
                firstBeat = false;                   // clear firstBeat flag
                secondBeat = true;                   // set the second beat flag
                sei();                               // enable interrupts again
                return;                              // IBI value is unreliable so discard it
            }
            // keep a running total of the last 10 IBI values
            word runningTotal = 0;                  // clear the runningTotal variable

            for(int i=0; i<=8; i++)
            {                // shift data in the rate array
                rate[i] = rate[i+1];                  // and drop the oldest IBI value
                runningTotal += rate[i];              // add up the 9 oldest IBI values
            }

            rate[9] = IBI;                          // add the latest IBI to the rate array
            runningTotal += rate[9];                // add the latest IBI to runningTotal
            runningTotal /= 10;                     // average the last 10 IBI values
            BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
            QS = true;                              // set Quantified Self flag
            // QS FLAG IS NOT CLEARED INSIDE THIS ISR
        }
    }

    if (Signal < thresh && Pulse == true)
    {   // when the values are going down, the beat is over
        //digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
        Pulse = false;                         // reset the Pulse flag so we can do it again
        amp = P - T;                           // get amplitude of the pulse wave
        thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
        P = thresh;                            // reset these for next time
        T = thresh;
    }

    if (N > 2500)
    {                           // if 2.5 seconds go by without a beat
        thresh = 512;                          // set thresh default
        P = 512;                               // set P default
        T = 512;                               // set T default
        lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
        firstBeat = true;                      // set these to avoid noise
        secondBeat = false;                    // when we get the heartbeat back
    }

    sei();                                   // enable interrupts when youre done!
}// end isr

void setupTimeInterruptionForPulseSensor(){
    // Initializes Timer2 to throw an interrupt every 2mS.
    TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
    TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER
    OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 (0X7C in HEXADECIMAL) FOR 500Hz SAMPLE RATE
    TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
    sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}

void medirBatimentosCardiacosCalculandoMedia(){
    int tempoDeEspera = 30 * 1000;
    long inicio;
    int quantidadeMedicoes = 0;
    int valoresMedidos = 0;

    Serial.println();
    Serial.println("Por favor, encoste o dedo no medidor e aguarde 30 segundos...");

    inicio = millis();
    while(millis() - inicio < tempoDeEspera)
    {
      
        if (QS == true) // A Heartbeat Was Found
        {
            quantidadeMedicoes++;
            valoresMedidos = valoresMedidos + BPM;

            // BPM and IBI have been Determined
            serialOutputWhenBeatHappens(); // A Beat Happened, Output that to serial.

            //fadeRate = 255; // Makes the LED Fade Effect Happen, Set 'fadeRate' Variable to 255 to fade LED with pulse

            // Quantified Self "QS" true when arduino finds a heartbeat
            QS = false; // reset the Quantified Self flag for next time
        }
        //ledFadeToBeat(); // Makes the LED Fade Effect Happen
        delay(20);
    }
    if(quantidadeMedicoes > 0)
    {
        double media = (double)valoresMedidos / quantidadeMedicoes;
        String estado = resultadoLeituraBPM("23", 'M', media);

        exibirNaSerialConclusaoDeMedidorCardiaco("John", "23", 'M', media, estado);
    }
    else
    {
        Serial.println("Nenhum batimento pressentido.");
    }
}
