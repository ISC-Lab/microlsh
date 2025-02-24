#define COL 5
#define ROW 3

float *farr;

void setup() {
  delay(5000);
  Serial.println("PROGRAM STARTING...");

  farr = (float*)calloc(ROW, COL * sizeof(float));
  
  if(!farr){
    Serial.println("ERROR: memory allocation failed");
  }
  else{
    Serial.println("SUCCESS: memory allocated!");
  }

  Serial.println("Filling in the data...");
  for(size_t i = 0; i < ROW; i++) {
    for(size_t j = 0; j < COL; j++) {
      farr[i*COL + j] = 3;
    }
  }

  Serial.println("Array content: ");
  for(size_t i = 0; i < ROW; i++) {
    for(size_t j = 0; j < COL; j++) {
      Serial.print(farr[i*COL + j]);
      Serial.print(" ");
    }
    Serial.println("");
  }

  Serial.println("Modifying with memcpy...");
  float data[] = {5.11, 5.11, 5.11, 5.11, 5.11};

  memcpy(&farr[0], data, sizeof(data));
  Serial.println("Array content: ");
 
  for(size_t i = 0; i < ROW; i++) {
    for(size_t j = 0; j < COL; j++) {
      Serial.print(farr[i*COL + j]);
      Serial.print(" ");
    }
    Serial.println("");
  }

  Serial.println("Freeing memory...");
  free(farr);
  Serial.println("PROGRAM TERMINATED");
}

void loop() {}
