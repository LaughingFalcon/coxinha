
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
    while(command[index] != NULL);

    inicio[index] = 0x00;

    String concatenado((char*)inicio);
    Serial.println("");
    Serial.println("---> Comando Concatenado: " + concatenado);
}

void separateInputByCharacter(char input, char** commands){
    *commands = strtok(input, "&");
}

void findNextToken(char** commands){
    *commands = strtok(NULL,"&");
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
