#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SD.h>
#include <SoftwareSerial.h>

int pinoOutputSD = 53;
bool sdJaFoiIniciado;
File arquivo;

//----------------------------------------------------------------------------
// SESSAO: CODIGOS QUE LIDAM COM SD

bool acordarSD()
{
  pinMode(pinoOutputSD, OUTPUT);

  if (SD.begin() || sdJaFoiIniciado)
  {
    Serial.println("Conexão com leitor de SD foi encontrada.");
    Serial.println("Arquivos de log já podem ser criados.");
  
    sdJaFoiIniciado = true;
    return true;
  }
  else{
    Serial.println("ERROR: Conexão com leitor de SD não foi encontrada.");
    return false;
  }
}

bool abrirOuCriarArquivoNoSD(String filename)
{
  arquivo = SD.open(filename, FILE_WRITE);

  if (arquivo)
  {
    Serial.println("Arquivo invocado com sucesso.");
    return true;
  } 
  else{
    Serial.println("ERRO: Não foi possivel abrir o arquivo.");
    return false;
  }
}

bool gravarNoArquivo(String letras)
{
  if (arquivo)
  {
    arquivo.println(letras);
    arquivo.println("Deu certo!"); 
    return true;
  } 
  else{
    Serial.println("ERRO: Não foi possível gravar os dados no arquivo. O arquivo não existe.");
    return false;
  }
}

void finalizarConexaoComArquivo(){
  if (arquivo)
  {
    arquivo.close();
    Serial.println("Conexão com arquivo no dispositivo SD foi encerrada.");
  }
}

void criarLogDeTesteEmChipSD()
{
  Serial.println("Iniciando passo-a-passo de criação de log em chip SD...");
  Serial.println("Iniciando conexão com leito de SD para arduino...");
  bool sdConectado = acordarSD();

  if(sdConectado)
  {
      Serial.println("Tentando abrir conexão com arquivo no chip SD...");
      abrirOuCriarArquivoNoSD("Teste.txt");

      Serial.println("Tentando gravar linha de teste no arquivo no chip SD...");
      bool gravado = gravarNoArquivo("Linha de teste. Se isto foi gravado, tudo está ocorrendo como esperado.");

      if(gravado){
        Serial.println("Dados gravados no dispositivo SD.");
      }
            
      Serial.println("Finalizando conexão com arquivo no chip SD...");
      finalizarConexaoComArquivo();

      Serial.println("Finalizado teste de Chip SD.");
  }
  else{
    Serial.println("ERRO: não foi possível estabelecer conexão com dispositivo SD.");  
  }
}

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
