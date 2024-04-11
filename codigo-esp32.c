int NUM_PONTOS = 1000;
double pi = 3.141592653;
double passo = (2 * pi) / NUM_PONTOS;
double angulo = 0;

#define LED 2


double potencia(double base, int expoente) {
  double resultado = 1.0;

  for (int i = 0; i < expoente; i++) {
      resultado *= base;
  }

  return resultado;
}

double fatorial(int n) {
    
  if (n == 0 || n == 1) {
      return 1.0;
  }

  double resultado = 1.0;

  for (int i = 2; i <= n; i++) {
    resultado *= i;
  }

  return resultado;
}

double seno(double x) {
    
    double resultado = 0.0;

    for (int i = 0; i < 20; i++) {
        resultado += ((potencia(-1, i) * potencia(x, (2*i) + 1)) / fatorial((2*i) + 1));
    }

    return resultado;
}


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}


void loop() {

  angulo = 0;

  for (int i = 0; i < NUM_PONTOS; i++) {
    
    double valor_seno = seno(angulo);
    if(valor_seno >= 0){
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }

    angulo += passo;
  }
}