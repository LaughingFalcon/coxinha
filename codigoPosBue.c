//Acesso por voz no arduino (Bluetooth + Android) criado por Angelo Casimiro (4/27/14) e modificado por nós com autorização do Autor.

  #include <LiquidCrystal.h>
  #include <EEPROM.h>
  #include <SD.h>
  #include <SoftwareSerial.h>

  //  Variables
  int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
  int blinkPin = 7;                // LED verde
  int fadePin = 8;                  // LED amarelo
  int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

  int CS_PIN = 10;
  File arquivoTXT;          // Arquivo a ser escrito para testes
  File arquivo;
  
  LiquidCrystal lcd(6, 9, 18, 17, 16, 15);
  SoftwareSerial BT(0,1); // RX, TX

  // Volatile Variables, used in the interrupt service routine!
  volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
  volatile int Signal;                // holds the incoming raw data
  volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
  volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
  volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

  // Regards Serial OutPut  -- Set This Up to your needs
  static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

  volatile int rate[10];                           // array to hold last ten IBI values
  volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
  volatile unsigned long lastBeatTime = 0;           // used to find IBI
  volatile int P = 512;                          // used to find peak in pulse wave, seeded
  volatile int T = 512;                          // used to find trough in pulse wave, seeded
  volatile int thresh = 525;                     // used to find instant moment of heart beat, seeded
  volatile int amp = 100;                        // used to hold amplitude of pulse waveform, seeded
  volatile boolean firstBeat = true;             // used to seed rate array so we startup with reasonable BPM
  volatile boolean secondBeat = false;           // used to seed rate array so we startup with reasonable BPM

  //testes de memoria
  int meGrave = 18764;
  boolean batata = true;

  //Mais variáveis
  boolean esperandoOpcao = false;
  int atividadeAtual = 0;
  //1 - Glicose
  //2 - BPM
  //3 - Menu
  
  //Usuario de testes
  String usuNome = "Almeida";
  int usuIdade = 23;
  String usuSexo = "M";

  void setup() { // Trá lá lá lá lá lá lá lá lá
    
    pinMode(blinkPin,OUTPUT);         // LED verde
    pinMode(fadePin,OUTPUT);          // LED amarelo
    Serial.begin(115200);             // we agree to talk fast!
    interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
                  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
                  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
                  //   analogReference(EXTERNAL);
    //BT.begin(115200);
  //Ligando o Aparelho
    Serial.println("Olá. Sou C.O.X.I.N.H.A. seu assitente médico pessoal");
    Serial.println("Que coisa incrível vamos fazer hoje?");

  }


  //  É aqui que a mágica acontece
  void loop() {
     serialOutput();

    /*if(batata){
      EEPROM.write(0,(meGrave/256));
      EEPROM.write(1,(meGrave%256));
      Serial.print("Carregando memória");
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.print(".");delay(1000);
      Serial.println();

      int num1 = EEPROM.read(0);
      int num2 = EEPROM.read(1);
      num1 = num1 * 256;
      num1 = num1 + num2;

      Serial.print("Valor esperado: ");
      Serial.print(meGrave);
      Serial.print("Valor gravado: ");
      Serial.println(num1);
      batata = false;
    }*/

    String textin = "";
    Serial.println("Zero - Abrir histórico de saúde");
    Serial.println("Um - Verificar batimentos cardiacos");
    Serial.println("Dois - Medir Glicose no sangue");
  /*while(true){
    
    if(Serial.available() > 0){
      textin = "";
      char letrin;
      while(Serial.available() > 0){
        letrin = Serial.read();
        if(letrin != '\n'){
          textin.concat(letrin);
        }
        delay(10);
      }

    }
    /**while(BT.available()){
      char r = BT.read();
      Serial.print(r);
      
    }*/

        Serial.println("Recebido >"+textin+"<");

        if(textin == "Zero"||textin == "zero"){
            Serial.println("FATAL ERROR");
            //verificarHistoricoDeMedicoes();
        }else if(textin == "Um"||textin == "um"||textin == "*um#"){
            Serial.println("Vamos ver se esse tumtumzinho está firme e forte.");
            medirBatimentosCardiacos();
        }else if(textin == "Dois"||textin == "dois"){
            Serial.println("Será que sua glicose aumentou? Será que ela caiu?");
            medirFalsamenteGlicoseDoSangue();
        }else
            Serial.println("Me desculpe, não compreendo o que deseja fazer.");
  }
  
  while(Serial.available() > 0){
    textin = Serial.read();
    if(Serial.read() == '\n'){
      if(textin == "Zero"||textin == "zero"){
        Serial.println("FATAL ERROR");
        //verificarHistoricoDeMedicoes();
      }else if(textin == "Um"||textin == "um"||textin == "*um#"){
        Serial.println("Vamos ver se esse tumtumzinho está firme e forte.");
        medirBatimentosCardiacos();
      }else if(textin == "Dois"||textin == "dois"){
        Serial.println("Será que sua glicose aumentou? Será que ela caiu?");
        medirFalsamenteGlicoseDoSangue();
      }else
        Serial.println("Me desculpe, não compreendo o que deseja fazer.");
    }
  }
       
  }

  /*void verificarHistoricoDeMedicoes(){
    Serial.println("Que vamos analisar hoje?");
    Serial.println("00 - Batimentos Cardíacos");
    Serial.println("01 - Glicose");
    Serial.println("02 - Nada");
    
    String textin = "";
    boolean escolha = true;
    while(escolha){
    if(Serial.available() > 0){
            textin = "";
            char letrin;

            while(Serial.available() > 0){
                letrin = Serial.read();
                if(letrin != '\n'){
                    textin.concat(letrin);
                }
                delay(10);
            }

        }
    
    if(textin == "00"){
            Serial.println("Bora lá");
            historicoDeMedicoes('B');
        }else if(textin == "01"){
            Serial.println("Vamo que vamo");
            historicoDeMedicoes('G');
        }else if(textin == "02"){
            Serial.println("Vlw flw");
      escolha = false;
        }else
            Serial.println("Qué qui tú qué?");
    
      
    }
    
  }*/
  
  /*void historicoDeMedicoes(char GliBPM){
    String arqNome = ("Gli"+usuNome+".txt");
    chamarArquivo(arqNome);
    
    desinvocarArquivo();
  }*/
  
  
  void medirBatimentosCardiacos(){
      int tempoDeEspera = 30 * 1000;
      long inicio; int medicoes = 0; int valores = 0;
    int BPMtemp = 0;

      Serial.println("");
      Serial.println("Por favor, encoste o dedo no medidor e aguarde 30 segundos");

      inicio = millis();
      while(millis() - inicio < tempoDeEspera){

          if(millis() - inicio > 1000){

          }
          if (QS == true){ // A Heartbeat Was Found
      medicoes++;
          valores = valores + BPM;
            // BPM and IBI have been Determined
      serialOutputWhenBeatHappens(); // A Beat Happened, Output that to serial.
            // Quantified Self "QS" true when arduino finds a heartbeat
            fadeRate = 255; // Makes the LED Fade Effect Happen, Set 'fadeRate' Variable to 255 to fade LED with pulse
            
            QS = false; // reset the Quantified Self flag for next time
          }
          ledFadeToBeat(); // Makes the LED Fade Effect Happen 
          delay(20); 
      }
    if(medicoes>0){
    BPMtemp = (int)valores/medicoes;  
    String estado = estadoBPM(usuNome,usuIdade,'M',BPMtemp);
    Serial.println(usuNome+" Idade: "+usuIdade+" Sexo: "+usuSexo);
    Serial.println(estado);
    gravarBPM(1234, BPMtemp, estado, usuNome);
    }else{
      Serial.println("Nenhum batimento pressentido.");
    }
     

  }
  void medirFalsamenteGlicoseDoSangue(){
  int tempoDeEspera = 30 * 1000;
      long inicio; int resposta = 0;

      Serial.println("");
      Serial.println("Por favor, coloque a palma da mão sobre o Glicosimetro por 30 segundos");

      inicio = millis();
    Serial.println("Medindo...");
      while(millis() - inicio < tempoDeEspera){
      resposta = 42;
          
      }
    if(resposta>42){
    //BPMtemp = (int)valores/medicoes;  
    String estado = estadoGlicose(usuNome,usuIdade,'S',156);
    Serial.println(usuNome+" Idade: "+usuIdade);
    Serial.println(estado);
    gravarGlicose(1234,  estado, usuNome);
    }else{
      Serial.println("Não foi possível medir sua Glicose.");
    }
      
  }

  void ledFadeToBeat(){
     fadeRate -= 15;                         //  set LED fade value
     fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
     analogWrite(fadePin,fadeRate);          //  fade LED
  }

  void interruptSetup(){
    // Initializes Timer2 to throw an interrupt every 2mS.
    TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
    TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER
    OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
    TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
    sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
  }

  void serialOutput(){   // Decide How To Output Serial.
   if (serialVisual == true)
    {
     arduinoSerialMonitorVisual('-', Signal);   // goes to function that makes Serial Monitor Visualizer
    }
   else
    {
      sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
     }
  }

  void acordarSD(){
    pinMode(CS_PIN, OUTPUT);
    
    if (SD.begin()){
    Serial.println("SD tá no esquema para trabalhar");
    }else{
    Serial.println("SD nem saiu da cama para trabalhar");  
      }
  }

  int chamarArquivo(String filename){
  arquivo = SD.open(filename);
  if (arquivo){
    Serial.println("Arquivo chegou");
    return 1;
  }else{
    Serial.println("Arquivo não veio");
    return 0;
  }
}
  
  int invocarArquivo(String filename){
  arquivo = SD.open(filename, FILE_WRITE);

  if (arquivo){
    Serial.println("Arquivo invocado com sucesso.");
    return 1;
  } else{
    Serial.println("Erro ao invocar o arquivo.");
    return 0;
  }
}

  int mandarParaOArquivo(String letras){
  if (arquivo)
  {
    arquivo.println(letras);
  arquivo.println("Deu certo");
    Serial.println("Funfou, mandamos o troço lá para o SD lá");
    return 1;
  } else
  {
    Serial.println("Perdão, falhei em mandar o troço lá para o SD lá");
    return 0;
  }
}

  String lerLinhaArquivo(){
  String received = "";
  char ch;
  while (arquivo.available())
  {
    ch = arquivo.read();
    if (ch == '\n'){
      return String(received);
    }else{
      received += ch;
    }
    }
  return "";
}

  void desinvocarArquivo(){
  if (arquivo)
  {
    arquivo.close();
    Serial.println("Arquivo fechado");
  }
}

  void gravarBPM(int senha, int BPMnovo, String estado, String nome){
  //Testar se o SD tá pronto para a labuta
  acordarSD();
  //Cria o arquivo caso ele n exista
  String arqNome = ("BPM"+nome+".txt");
  invocarArquivo(arqNome);
  //Manda o texto a ser gravado no arquivo TXT
  mandarParaOArquivo(estado);
  //Enviar o arquivo para seu plano de origem
  desinvocarArquivo();
    
  
  }
  void gravarGlicose(int senha, String estado, String nome){
  //Testar se o SD tá pronto para a labuta
  acordarSD();
  //Cria o arquivo caso ele n exista
  String arqNome = ("Gli"+nome+".txt");
  invocarArquivo(arqNome);
  //Manda o texto a ser gravado no arquivo TXT
  mandarParaOArquivo(estado);
  //Enviar o arquivo para seu plano de origem
  desinvocarArquivo();

  }
  /*void gravarUsuario(){
  }*/

  void mensagemSaudeDoDia(int valor){
    Serial.println("Caramba, que fita, sempre dá para melhorar mais!");
  }

  void serialOutputWhenBeatHappens(){
   if (serialVisual == true){ //  Code to Make the Serial Monitor Visualizer Work
    Serial.print("*** Pressentimos um batimento cardiaco *** ");  //ASCII Art Madness
    Serial.print("BPM: ");
    Serial.println(BPM);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("BPM: ");
    lcd.print(BPM);
    
   }
  else
    {
    sendDataToSerial('B',BPM);   // send heart rate with a 'B' prefix
    sendDataToSerial('Q',IBI);   // send time between beats with a 'Q' prefix
    }
 }

  String estadoBPM(String nome, int idade, char sexo, int BPMnovo){
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

  String estadoGlicose(String nome, int idade, char jejum, int glicose){
  String estado = "";
    int *zona = new int[3];
  
  zona[0] = 70;
    zona[1] = 100;
    zona[2] = 126;
  
  if(jejum == 'N' || jejum == 'n'){
    zona[0] = zona[0] + 0;
    zona[1] = zona[1] + 40;
    zona[2] = zona[2] + 74;
  }
      

    //Serial.print(nome+" \n Idade: "+idade+" Sexo: "+sexo+" BPM: "+BPM);
    //Serial.print(":Status:");
    if(glicose<zona[0])
    estado == "Glicose a níveis Hipoglicemicos";
  else
    if(glicose<zona[1])
      estado == "Glicose num nível bom";
    else
      if(glicose<zona[2])
        estado == "Glicose a níveis pré-diabéticos";
      else
        if(glicose>=zona[2])
          estado == "Glicose a níveis Diabéticos";
        else
          estado == "Glicose a níveis desconhecidos";
  
    return estado;
  }
  
  
  void arduinoSerialMonitorVisual(char symbol, int data ){
    const int sensorMin = 0;      // sensor minimum, discovered through experiment
    const int sensorMax = 1024;    // sensor maximum, discovered through experiment
    int sensorReading = data; // map the sensor range to a range of 12 options:
    int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
    // do something different depending on the
    // range value:
   /** switch (range)
    {
    case 0:
      Serial.println("");     /////ASCII Art Madness
      break;
    case 1:
      Serial.println("---");
      break;
    case 2:
      Serial.println("------");
      break;
    case 3:
      Serial.println("---------");
      break;
    case 4:
      Serial.println("------------");
      break;
    case 5:
      Serial.println("--------------|-");
      break;
    case 6:
      Serial.println("--------------|---");
      break;
    case 7:
      Serial.println("--------------|-------");
      break;
    case 8:
      Serial.println("--------------|----------");
      break;
    case 9:
      Serial.println("--------------|----------------");
      break;
    case 10:
      Serial.println("--------------|-------------------");
      break;
    case 11:
      Serial.println("--------------|-----------------------");
      break;
    }*/
  }


  void sendDataToSerial(char symbol, int data ){
     Serial.print(symbol);
     Serial.println(data);
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
      digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
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
      digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
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
