/*
 * WRITTEN BY: C. ABBEY
 * DATE: APR 5 2024
 * COURSE: ME379: INTRODUCTION TO MECHATRONICS
 *
 * VERSION: 1.1
 *
 * NAME: ARDUINO WX
 *
 * DESCRIPTION:
 * THIS PROGRAM IS INTENDED TO BE RUN ON AN ARDUINO UNO MICROCONTROLLER.
 * THE PROGRAM READS ANALOG VOLTAGE OUTPUT FROM VARIOUS SENSORS AND REFERENCES AN OUTPUT TABLE
 * TO ASSIGN THE CORRECT SENSOR OUTPUT VALUE.
 *
 * THIS IS ACCOMPLISHED BY ASSIGNING A 2-DIMENSIONAL ARRAY THAT CAN BE ACCESSED USING A KEY-VALUE SYSTEM.
 * THE FUNCTION 'READTABLE' IS RESPONSIBLE FOR READING THE CORRECT ROW OF THE TABLE BASED UPON THE
 * SENSOR OUTPUT VOLTAGE. THIS REPRESENTS THE 'KEY' AND ACCESSES THE CORRECT DATA ROW.
 * THE FUNCTION THEN RETURNS A FLOATING POINT DECIMAL THAT CAN BE ACCESSED IN OTHER FUNCTIONS.
 *
 * THE SENSOR READING VALUE IS THEN EXTRACTED FROM THE DATA TABLE WITH THE FUNCTION 'GETSENSORDATA'
 * WHICH REQUIRES TWO ARGUMENTS: THE PIN TO READ, AND THE INDEX VALUE THAT CORRESPONDS TO THE CORRECT
 * COLUMN. THE FUNCTION THEN CALLS 'READTABLE' TO GET THE SENSOR VALUE, AND APPROXIMATE THE CLOSEST OUTPUT VALUE.
 * THE FUNCTION THEN RETURNS AN OUTPUT VALUE FOR THE SENSOR AS A FLOATING POINT.
 *
 * THE TEMPERATURE READING IS NATIVELY IN FARENHEIT, AND NEEDS TO BE CONVERTED TO CELSIUS. THIS IS
 * ACCOMPLISHED IN THE 'CONVERTCELSIUS' FUNCTION. THE CONVERTED VALUE IS RETURNED AS A DOUBLE.
 *
 * I/O REQUIREMENTS:
 * PINS A0 - A3: SENSOR INPUTS
 */

#include <Arduino.h>    //include basic arduino functions header file

#define windDir A0  // define the pin 'windDir' and attach A0
#define windSpd A1  // define the pin 'windSpd' and attach A1
#define temp A2     // define the pin 'temp' and attach A2
#define press A3    // define the pin 'press' and attach A3

// Create the data table as a global 2-D array to make it easier to access in other functions:

float sensorData[][5] = {
        {0.000, 0.000, 0.00, 0.0, 950},
        {0.156, 3.125,11.25, 4.0, 953},
        {0.313, 6.250,22.50, 8.0, 956},
        {0.469, 9.375,33.75, 12.0, 959},
        {0.625, 12.500, 45.00, 16.0, 963},
        {0.781, 15.625, 56.25, 20.0, 966},
        {0.938, 18.750, 67.50, 24.0, 969},
        {1.094, 21.875, 78.75, 28.0, 972},
        {1.250, 25.000, 90.00, 32.0, 975},
        {1.406, 28.125, 101.25, 36.0, 978},
        {1.563, 31.250, 112.50, 40.0, 981},
        {1.719, 34.375, 123.75, 44.0, 984},
        {1.875, 37.500, 135.00, 48.0, 988},
        {2.031, 40.625, 146.25, 52.0, 991},
        {2.188, 43.750, 157.50, 56.0, 994},
        {2.344, 46.875, 168.75, 60.0, 997},
        {2.500, 50.000, 180.00, 64.0, 1000},
        {2.656, 53.125, 191.25, 68.0, 1003},
        {2.813, 56.250, 202.50, 72.0, 1006},
        {2.969, 59.375, 213.75, 76.0, 1009},
        {3.125, 62.500, 225.00, 80.0, 1013},
        {3.281, 65.625, 236.25, 84.0, 1016},
        {3.438, 68.750, 247.50, 88.0, 1019},
        {3.594, 71.875, 258.75, 92.0, 1022},
        {3.750, 75.000, 270.00, 96.0, 1025},
        {3.906, 78.125, 281.25, 100.0, 1028},
        {4.063, 81.250, 292.50, 104.0, 1031},
        {4.219, 84.375, 303.75, 108.0, 1034},
        {4.375, 87.500, 315.00, 112.0, 1038},
        {4.531, 90.625, 326.25, 116.0, 1041},
        {4.688, 93.750, 337.50, 120.0, 1044},
        {4.844, 96.875, 348.75, 124.0, 1047},
        {5.000, 100.000, 360.00, 128.0, 1050},
};

// Perform a simple calculation to determine the number of table rows. This is used later on in the code.
int tableRows = sizeof(sensorData) / sizeof(sensorData[0]);

// Create the function to read the table value, and approximate the closest value to the sensor reading.
float readTable(float sensorReading, int column) {
    float closestValue = sensorData[0][column]; // initialize variable and set it to row 0, and the column to read.
    // Calculate the difference between the sensor reading and the closest value available.
    float minDifference = abs(sensorReading - closestValue);

    // Create for loop structure to search the table and set the proper values
    for (int i = 1; i < tableRows; i++) {
        float currentDiff = abs(sensorReading - sensorData[i][column]); //calculate the current difference for the row being read.
        if (currentDiff < minDifference) {
            closestValue = sensorData[i][column];
            minDifference = currentDiff;
        }
    }

    return closestValue;    //return the output value of the for loop to be used elsewhere.
}

//Create a function to actually get the data from the table based on argument inputs.
// ARGUMENTS:
// pin is the sensor's arduino pin
// index is the column in the table (which sensor to read)

float getSensorData(int pin, int index) {
    int voltageReading = analogRead(pin);  // read the analog output of the pin argument
    float voltage;
    voltage = map(voltageReading, 0, 1023, 0, 5);   //map the analog output to a voltage scale to match the table

    // get the value from the table by calling the readTable function
    float tableValue = readTable(voltage, index);

    return tableValue;  //return the correct table value as a function output
}

//Create function to convert the F temperature to C. Takes the temperature as argument
double convertCelsius(double temperatureF) {
    double celsius = (temperatureF - 32) / 1.8; // perform conversion calculation

    return celsius; // return the value of the calculation as a double
}

float convertPressure(float pressureMb) {
    float pressureAtm = pressureMb / 1000;

    return pressureAtm;
}


void setup() {
    pinMode(windDir, INPUT);    // set pin as an input
    pinMode(windSpd, INPUT);    // set pin as an input
    pinMode(temp, INPUT);       // set pin as an input
    pinMode(press, INPUT);      // set pin as an input

    Serial.begin(9600);             // start the serial communication (9600 baud)

}

void loop() {
    // create variables and get values from the look-up table:
    // recall: getSensorData(pin to read, column/sensor to read)
    float windDirection = getSensorData(windDir, 1);
    double windSpeed = getSensorData(windSpd, 2);
    double temperature = convertCelsius(getSensorData(temp, 3));
    float pressure = convertPressure(getSensorData(press, 4));

    // Print all information to the serial monitor
    Serial.print("WIND DIRECTION= ");
    Serial.print(windDirection);
    Serial.println(" DEGREES");

    Serial.print("WIND SPEED= ");
    Serial.print(windSpeed);
    Serial.println(" MPH");

    Serial.print("TEMPERATURE= ");
    Serial.print(temperature);
    Serial.println(" Degrees C");

    Serial.print("PRESSURE= ");
    Serial.print(pressure);
    Serial.println(" ATM");

    Serial.println("");

    delay(2000);    //wait 2 seconds between readings
}