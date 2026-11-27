const int buttonPin = 2;
const int ledPin = LED_BUILTIN;
const int ledVermelhoPin = 4; // led emergencia
const int ledAmareloPin = 6; // led pwm

int ledVermelhoState = LOW; // estado do led vermelho
float ledAmareloState = 0.0; // estado do led amarelo

const float GANHO = 4.0;

int buttonState = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ledVermelhoPin, OUTPUT);
  pinMode(ledAmareloPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void buttonLogic() //toggle
{
    int checkButtonState = digitalRead(buttonPin);  
    if (checkButtonState == HIGH) {
        buttonState = !buttonState; 
        if(buttonState == HIGH){
            digitalWrite(ledPin, HIGH);
        }else{
            digitalWrite(ledPin, LOW);
        }
    }
}

float temperatura(int a1){
    float v0 = a1 * (5.0 / 1023.0);
    float temp = (v0 / GANHO - 0.5) * 100.0;
    return temp;
}

int verDiaNoite() // dia = 1 | noite = 0
{
    int estadoLuz = 0;
    if (analogRead(A0) < 400) {
        estadoLuz = 0; 
    } else {
        estadoLuz = 1; 
    }
    return estadoLuz;
}

void leds(float temp, int estadoLuz)
{
    if(temp <= 30.0 && temp >= 15.0)
    {
        digitalWrite(ledVermelhoPin, LOW);  
        ledVermelhoState = LOW;
    }else{
        digitalWrite(ledVermelhoPin, HIGH);  
        ledVermelhoState = HIGH;
    }
    if(buttonState == HIGH){
        if(estadoLuz == 1)
        {
            analogWrite(ledAmareloPin, 0);
            ledAmareloState = 0.0;
        }else{
            if(temp < 15.0){
                analogWrite(ledAmareloPin, 255);
                ledAmareloState = 1.0;
            }else{
                analogWrite(ledAmareloPin, 0.25*255);
                ledAmareloState = 0.25;
            }
        }
    }
}

void mostrarDados(float temp, int estadoLuz)
{
    Serial.println("Estado do botao: ");
    if(buttonState == HIGH) {
        Serial.println("Pressionado");
    } else {
        Serial.println("Nao pressionado");
    }
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print(" [C] | Estado da luz: ");
    if (estadoLuz == 1) {
        Serial.println("Dia");
    } else {
        Serial.println("Noite");
    }
    Serial.print("Estado do LED Vermelho: ");
    if (ledVermelhoState == HIGH) {
        Serial.println("Ligado");
    } else {
        Serial.println("Desligado");
    }
    Serial.print("Estado do LED Amarelo: ");
    if (ledAmareloState == 1.0) {
        Serial.println("Ligado a 100%");
    } else if (ledAmareloState == 0.25) {
        Serial.println("Ligado a 25%");
    } else {
        Serial.println("Desligado");
    }
    Serial.println("-----------------------------");
}

void loop()
{
    buttonLogic();
    
    int a1 = analogRead(A1);

    float temp = temperatura(a1);
    int estadoLuz = verDiaNoite();
    
    leds(temp, estadoLuz);
    mostrarDados(temp, estadoLuz);

    delay(1000);
}