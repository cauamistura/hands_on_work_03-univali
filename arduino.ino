#include <LiquidCrystal.h>

// Definição de constantes
#define sen_forca A5               // Pino de entrada para o sensor de força
#define AREA_SENSOR_CM2 10.0       // Área do sensor em cm² (exemplo)
#define R_DIV 3230.0               // Medida da resistência do resistor de 3.3k
#define VCC 4.98                   // Tensão medida na linha de 5V do Arduino

// Variáveis globais
float valor_da_forca[] = {0, 0};   // Vetor que armazena o valor da força
String classificacao_anterior = ""; // Variável para armazenar a classificação anterior
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Objeto do display LCD

// Função para calcular a força em gramas
float calcularForca(float fsrADC) {
  // Calcular a voltagem
  float fsrV = fsrADC * VCC / 1023.0;
  
  // Calcular a resistência do FSR
  float fsrR = R_DIV * (VCC / fsrV - 1.0);
  
  // Calcular a condutância (reciprocal da resistência)
  float fsrG = 1.0 / fsrR;

  // Calcular a força com base nos valores fornecidos
  float force;
  if (fsrR <= 600) 
    force = (fsrG - 0.00075) / 0.00000032639;
  else
    force = fsrG / 0.000000642857;

  return force; // Retornar a força em gramas
}

// Função para categorizar a pressão
String categorizarPressao(float pressao) {
  if (pressao < 0.1) {
    return "Pouca/Nula";
  } else if (pressao >= 0.1 && pressao <= 3.5) {
    return "Correta";
  } else {
    return "Excessiva";
  }
}

// Configuração inicial
void setup() {
  pinMode(sen_forca, INPUT);     // Configura sen_force como porta de entrada de dados.
  Serial.begin(9600);           // Inicia a comunicação serial do Arduino
  lcd.begin(16, 2);             // Inicia o display LCD com 16 colunas e 2 linhas
  lcd.print("Treinamento!");    // Exibe uma mensagem inicial no LCD
}

// Loop principal
void loop() {
  valor_da_forca[0] = analogRead(sen_forca); // Lê o valor do sensor
  float forca_gramas = calcularForca(valor_da_forca[0]); // Calcula a força em gramas
  
  String classificacao_atual = categorizarPressao(forca_gramas);
  
  // Imprime a classificação de pressão apenas quando houver alteração
  if (classificacao_atual != classificacao_anterior) {
    lcd.clear(); // Limpa o display
    
    lcd.setCursor(0, 0);
    lcd.print(classificacao_atual); // Exibe a classificação de pressão
    
    lcd.setCursor(0, 1);
    lcd.print("Forca (g): ");
    lcd.print(forca_gramas); // Exibe a força em gramas
    
    classificacao_anterior = classificacao_atual; // Atualiza a classificação anterior
  }

  valor_da_forca[1] = valor_da_forca[0]; // O segundo espaço do vetor recebe a nova leitura
  
  delay(10); // Espera 0,01s
}
