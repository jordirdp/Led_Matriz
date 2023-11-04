/* Project: Arduino escribe mensajes en matriz LED */
/* Author: Jordi Rodriguez */

#include <MD_MAX72xx.h>
#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 5
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_MAX72XX cartel = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);

String mensaje = "Benvinguts";
String proximo_mensaje;

void setup() {
  // inicializar el objeto mx
  cartel.begin();

  // Establecer intencidad a un valor de 5
  cartel.control(MD_MAX72XX::INTENSITY, 10);

  // Desactivar auto-actualizacion
  cartel.control( MD_MAX72XX::UPDATE, false );

  // inicializar puerto Serie a 9600 baudios
  Serial.begin(9600);
}
void loop() {
  slide_text( 200 );
  actualizar_mensaje();
}

void actualizar_mensaje(){

  while( Serial.available() ){

    char c = Serial.read();

    if( c == '\n' ){
      mensaje = proximo_mensaje;
      proximo_mensaje = "";
      break;
    }
    else
      proximo_mensaje += c;
    
  }
}

void slide_text(int ms_delay){
  int col = 0;
  int last_pos;
  bool completo = false;
  
  cartel.clear();

  while( completo == false ){
    last_pos = printText(col, mensaje);
    delay(ms_delay);
    col++;
    if( last_pos > (int)cartel.getColumnCount() )
      completo = true;
  }
}

int printText(int pos, const String text){
  int w;
  
  for( int i = 0; i < text.length(); i++ ){
    // imprimir letra

    w = cartel.setChar( pos, text[i] );
    // la proxima letra empieza donde termina esta
    pos = pos - w; 
    // Se deja una columna entre letras.
    cartel.setColumn(pos, B00000000);
    
    pos = pos - 1;
    
    if( pos < 0 )
      break;
      
  }
  cartel.update();
  
  return pos;
}
