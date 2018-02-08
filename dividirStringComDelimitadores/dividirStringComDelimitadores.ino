
// Calculate based on max input size expected for one command
#define INPUT_SIZE 19


void printEachItemFromInput(char* command)
{
    char inicio[10];
    int index = 0;
    do
    {
        Serial.println("Command " + String(index) + ": " + String(command[index]));

        inicio[index] = command[index];
        index++;
    }
    while(command[index] != NULL && command[index] != '\r'  && command[index] != '\n'  && command[index] != "\n\r");

    inicio[index] = 0x00;

    String concatenado((char*)inicio);
    Serial.println("");
    Serial.println("---> Comando Concatenado: " + concatenado);
}

bool listHasElementAtIndex(char* list, int index){
    return list[index] != NULL && list[index] != '\r'  && list[index] != '\n'  && list[index] != "\n\r";
}

int numberOfCharactersInCharList(char* list)
{
    int index = 0;
    
    while(listHasElementAtIndex(list, index))
      index++;

    return index;
}

void separateInputByCharacter(char input, char** commands){
    *commands = strtok(input, "&");
}

void findNextToken(char** commands){
    *commands = strtok(NULL,"&");
}

void definirVariaveisDoInputInicial(char* command, int variableOrder)
{
    int numberOfCharacters = numberOfCharactersInCharList(command);

    switch(variableOrder)
    {
        case 0:
            
          break;

        case 1:

          break;

        case 2:

          break;

        default: Serial.println("Só são aceitáveis até quatro variaveis na comunicação inicial");
    }
}

void separateInputInAnArray()
{
    Serial.println("INICIANDO EXECUÇÃO...");
  
    // Get next command from Serial (add 1 for final 0)
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytes(input, INPUT_SIZE);
    
    // Add the final 0 to end the C string
    input[size] = 0;
    
    // Read each command pair 
    char* commands = strtok(input, "&");;
    //separateInputByCharacter(&input, &command);
  
    int nextTokenCount = 0;
    int maxLength = 4;

    for(int index = 0; index < maxLength; index++)
    {
        int quantidade = numberOfCharactersInCharList(commands);

        Serial.println("Quantidade: " + String(quantidade));
      
        printEachItemFromInput(commands);      
        findNextToken(&commands);

        nextTokenCount++;
    }
}

int occurrencesOfCharacterInText(String text, char character)
{
    int occurrences = 0;
    for (int i = 0; i <= text.length() - 1; i++) 
    {
        if(text.charAt(1) == character)
          occurrences++;
    }
    return occurrences;
}

void separateStringByDelimiter(String fullString, char separator)
{
    Serial.println("Texto completo: " + fullString);
  
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = fullString.length() - 1;

    String response[occurrencesOfCharacterInText(fullString, separator)];
    int separatorFound = 0;
    char commands;

    for (int i = 0; i <= maxIndex; i++) 
    {
        commands = commands + fullString.charAt(i);

        Serial.println("Caracter que estamos lendo: " + String(fullString.charAt(i)));

        
        Serial.println("Variavel Commands: ");
        Serial.println(commands);
      
        if (fullString.charAt(i) == separator || i == maxIndex) 
        {
            separatorFound++;
            response[separatorFound - 1] = String(commands);
          
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    //return found > index ? data.substring(strIndex[0], strIndex[1]) : "";  
}

void executeSecondWay()
{
      String entrada;

    while(Serial.available())
    {
        entrada += Serial.readString();
    }

    Serial.println("Minha entrada: " + entrada);

    separateStringByDelimiter(String(entrada), '&') ;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Insira um comando no formato 1:90&2:80&3:180 para começar.");
    Serial.println("");
}

void loop() {
    while(!Serial.available()){}
    //separateInput();
    separateInputInAnArray();
    //executeSecondWay();
}
