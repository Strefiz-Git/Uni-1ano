const int buttonPin = 2;
const int ledPin = LED_BUILTIN;
const int ledVermelhoPin = 4; // led emergencia
const int ledAmareloPin = 6;  // led pwm

const float GANHO = 4.0;
const int LIMIAR_LDR = 512;   

int luzLigada = 1;            // 1 = dinamica | 0 = forcada a desligar
int lastButtonState = LOW;    

int ledVermelhoState = LOW;
int pwmValor = 0;             // 0 a 255

unsigned long ultimoPrint = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(ledVermelhoPin, OUTPUT);
    pinMode(ledAmareloPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(ledPin, HIGH); 
}

void buttonLogic() // toggle
{
    int buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && lastButtonState == LOW) {
        if (luzLigada == 0) {
            luzLigada = 1;
            digitalWrite(ledPin, HIGH);
        } else {
            luzLigada = 0;
            digitalWrite(ledPin, LOW);
        }
            delay(30);
    }
    lastButtonState = buttonState;
}

float temperatura(int a1)
{
    float v0 = a1 * (5.0 / 1023.0);
    return (v0 / GANHO - 0.5) * 100.0;
}

int verDiaNoite(int ldr) // dia = 1 | noite = 0
{
    int estadoLuz = 0;
    if (ldr < LIMIAR_LDR) { // ldr < 512 => noite
        estadoLuz = 0;
    } else {
        estadoLuz = 1;
    }
    return estadoLuz;
}

void leds(float temp, int estadoLuz)
{
    int pwm = 0;
    int vermelho = 0;

    if (temp < 15.0 || temp > 30.0) {
        vermelho = 1;
    }
    if (estadoLuz == 0) { // NOITE
        if (temp < 15.0) {
            pwm = 255;
        } else {
            pwm = 64; // 25% 0.25*255        
        }
    }

    if (luzLigada == 0) {
        pwm = 0;
    }

    if (vermelho == 1) {
        digitalWrite(ledVermelhoPin, HIGH);
        ledVermelhoState = HIGH;
    } else {
        digitalWrite(ledVermelhoPin, LOW);
        ledVermelhoState = LOW;
    }

    analogWrite(ledAmareloPin, pwm);
    pwmValor = pwm;
}

void mostrarDados(float temp, int ldr, int estadoLuz)
{
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println(" [C]");

    Serial.print("Tensao LDR: ");
    Serial.print(ldr * (5.0 / 1023.0));
    Serial.println(" [V]");

    Serial.print("Periodo: ");
    if (estadoLuz == 1) {
        Serial.println("Dia");
    } else {
        Serial.println("Noite");
    }

    Serial.print("Luz da estufa (botao): ");
    if (luzLigada == 1) {
        Serial.println("Ligada (dinamica)");   
    } else {
        Serial.println("Desligada (forcada)"); 
    }

    Serial.print("LED vermelho: ");
    if (ledVermelhoState == HIGH) {
        Serial.println("Ligado");
    } else {
        Serial.println("Desligado");
    }

    Serial.print("PWM: ");
    Serial.print(pwmValor);
    Serial.print(" (");
    Serial.print(pwmValor * 100 / 255);
    Serial.println("%)");

    Serial.println("-----------------------------");
}

void loop()
{
    buttonLogic(); 

    float temp = temperatura(analogRead(A1));
    int ldr = analogRead(A0);
    int estadoLuz = verDiaNoite(ldr);

    leds(temp, estadoLuz);
    mostrarDados(temp, ldr, estadoLuz);
    delay(1000);
}