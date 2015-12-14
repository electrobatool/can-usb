#define BUF_SIZE_RX 256 

#define Queue_Ready(q)  ((q).in != (q).out)

#define CMD_BUFFER_LEN 30  // Lenght command buffer
#define ERR         7      // Error (ASCII BEL)

#define LED_PIN 13

//Буфер приема и его индексы:
struct queue {
    uint8_t buffer[BUF_SIZE_RX];
    uint8_t in,
    uint8_t out;
} rxQueue;

uint8_t cmdBuf[CMD_BUFFER_LEN];  // command buffer
uint8_t bufIdx = 0;

void writeBuf(struct queue *q, uint8_t value) {
    q->buffer[q->in++] = value;
}

uint8_t readBuf(struct queue *q) {
    return q->buffer[q->out++];
}

uint8_t execCmd(uint8_t * cmdBuf) {
  
  switch (cmdBuf[0]) {
    case 'V':
      Serial.print("V0101");
      return '\r';
    
    case 't':
      Serial.print("t03680102030405060708");
      return '\r';
    
    default:
      return ERR;
  }
  
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT); 
}

void loop() {
  
  if (Queue_Ready(rxQueue)) {
    uint8_t rxChar = readBuf(&rxQueue);
//    Serial.write(rxChar);
    if (rxChar == '\r') {    // End command
      cmdBuf[bufIdx] = '\0'; // End string
      Serial.write(execCmd(cmdBuf));
      bufIdx = 0; 
    }
    else if (rxChar != 0) {
      cmdBuf[bufIdx++] = rxChar;
    } 
  }
}

void serialEvent() {
    writeBuf(&rxQueue, Serial.read());
}  
