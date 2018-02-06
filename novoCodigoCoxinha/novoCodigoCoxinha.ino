#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

//----------------------------------------------------------------------------
// SESSAO: CODIGOS QUE LIDAM COM SD


// FIM-SESSAO: CODIGOS QUE LIDAM COM SD
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//SESSAO: MENU PRINCIPAL

void exibirBoasVindasNoMonitorSerial()
{
  Serial.println("Ola. Eu sou C.O.X.I.N.H.A.");
  Serial.println("Seu assitente médico pessoal");
  Serial.println("Que coisa incrível vamos fazer hoje?");
}

void exibirMenuPrincipalNoMonitorSerial()
{
  Serial.println();
  Serial.println("******************** MENU PRINCIPAL ********************");
  Serial.println("Informe um dos números abaixo para ativar a opção correspondente:");
  Serial.println("1 - Abrir histórico de saúde;");
  Serial.println("2 - Verificar batimentos cardiacos;");
  Serial.println("3 - Medir Glicose no sangue;");
  Serial.println("4 - Gerar arquivo de log para teste;");
  Serial.println();
}

void aguardarEnvioDeDadosViaBluetooth(){
  while(!Serial1.available()){};
}

char receberOpcaoDeMenuViaBluetooth()
{
  aguardarEnvioDeDadosViaBluetooth();
  byte opcaoEscolhida;

  while(Serial1.available())
  {
    opcaoEscolhida += Serial1.read();
  }
  
  return char(opcaoEscolhida);
}

void realizarOperacaoRequisitada(char operacaoEscolhida)
{
  Serial.println("A opção selecionada foi: >" + String(operacaoEscolhida) + "<");

  switch(operacaoEscolhida)
  {
      case '1':
          Serial.println("Verificando existencia de historico de medicoes...");
          //verificarHistoricoDeMedicoes();
      break;
      case '2':
          Serial.println("Vamos ver se esse tumtumzinho está firme e forte?");
          //medirBatimentosCardiacos();
      break;
      case '3':
          Serial.println("Será que sua glicose aumentou? Será que ela caiu?");
          //medirFalsamenteGlicoseDoSangue();
      break;
      case '4':
          criarLogDeTesteEmChipSD();
      break;
      default:
          Serial.println("Me desculpe, não compreendo o que deseja fazer.");
      break;
  }
}

// FIM-SESSAO: MENU PRINCIPAL
//----------------------------------------------------------------------------

void setup() {

  Serial1.begin(38400); //REPRESENTA AS PORTAS TX1 E RX1 POR ONDE ENTRAM AS INFORMACOES DO BLUETOOTH
  Serial.begin(38400); // REPRESENTA O MONITOR SERIAL
  
  exibirBoasVindasNoMonitorSerial();
}

void loop() {
  
    //definirTipoSaidaSerial();

  exibirMenuPrincipalNoMonitorSerial();

  char opcaoEscolhida = receberOpcaoDeMenuViaBluetooth();
  realizarOperacaoRequisitada(opcaoEscolhida);
}
