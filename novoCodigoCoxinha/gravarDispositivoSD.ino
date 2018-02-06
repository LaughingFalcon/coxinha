#include <SD.h>

int pinoOutputSD = 53;
bool sdJaFoiIniciado;
File arquivo;

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
