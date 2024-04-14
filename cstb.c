#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

//constant for defining when the second phase starts, see peregrinus-software.ino
#define PHASE_TWO_START 5244180

uint32_t charArrayToUint32(char* byteStart);
uint16_t charArrayToUint16(char* byteStart);
int16_t charArrayToInt16(char* byteStart);
float calc_temp(uint16_t temp);
float calc_mag(int16_t mag);
float calc_accel(int16_t accel);
float calc_gyro(int16_t gyro);

int main(int argc, char* argv[]){
	
	//avoiding a segfault if no file of messages was provided
	if(argc <= 1){
		printf("Please provide a csv file of messages when calling executable");
		return -1;
	}

	//We'll be using two different output files for every phase
	FILE *in_file;

	FILE *phase_one_data_csv;
	phase_one_data_csv = fopen("phase_one_data.csv", "w");
	fprintf(phase_one_data_csv, "timestamp,counts(last second),temp,mag x, mag y, mag z,\n");
	fclose(phase_one_data_csv);

	FILE *phase_two_data_csv;
	phase_two_data_csv = fopen("phase_two_data.csv", "w");
	fprintf(phase_two_data_csv, "timestamp,temp,gyro x,gyro y,gyro z,accel x,accel y,accel z,\n");
	fclose(phase_two_data_csv);

	in_file = fopen(argv[1], "r");
	char line[1000];
	
	//we ignore the first line
	fgets(line, 1000, in_file);
	
	while(fgets(line, 1000, in_file)){
		
		//parsing the csv: we only need the data itself, which can be found in the fourth position
		char *line_token = strtok(line, ",");
		for(int i = 0; i < 3; i++){
			line_token = strtok(NULL, ",");
		}
		char *data = line_token;
		//avoiding another segfault
		if(data == NULL){
			printf("Warning: file seems to have ended on empty line");
			return 0;
		}

		//converting the hexadecimal string to binary data
		char message[326];
		char *dst = message;
		char *end = message + sizeof(message);
		char u;
		while(dst < end && sscanf(data, "%2x", &u) == 1){
			*dst++ = u;
			data += 2;
		}

		//now we need to go byte per byte, convert to an (unsigned) int and if necessary process raw data
		//every distinct measurement is a distinct line in our csv
		char *ptr = message;
		unsigned int timestamp = charArrayToUint32(ptr);
		ptr += 4;
		//the different phases are encoded slightly differently, so we use the timestamp do determine whether
		//we're working with data from the first or seconde phase
		if(timestamp <= PHASE_TWO_START){
			//we count down, to make sure the measurements are in chronological order and timestamped correctly
			//in peregrinus-software.ino the earlier measurements were indeed stored first
			for(int i = 9; i >= 0; i--){
				//this is a bit spaghetti, but we basically put our data in one big string
				char out_string[1000];
				sprintf(out_string, "%lu,", timestamp - i*1000);
				char counts[100];
				sprintf(counts, "%i,", charArrayToUint16(ptr));
				strcat(out_string, counts);
				ptr += 2;
				char temp[100];
				sprintf(temp, "%f,", calc_temp(charArrayToUint16(ptr)));
				strcat(out_string, temp);
				ptr += 2;
				for(int j = 0; j < 3; j++){
					char temp_string[100];
					sprintf(temp_string, "%f,", calc_mag(charArrayToInt16(ptr)));
					strcat(out_string, temp_string);
					ptr += 2;
				}
				//append our string to the output file
				strcat(out_string, "\n");
				phase_one_data_csv = fopen("phase_one_data.csv", "a");
				fprintf(phase_one_data_csv, out_string);
				fclose(phase_one_data_csv);
			}
		} else {
			//zie boven
			for(int i = 22; i >= 0; i--){
				char out_string[1000];
				sprintf(out_string, "%lu,", timestamp - i*435);
				char temp[100];
				sprintf(temp, "%f,", calc_temp(charArrayToUint16(ptr)));
				strcat(out_string, temp);
				ptr += 2;
				for(int j = 0; j < 3; j++){
					char temp_string[100];
					sprintf(temp_string, "%f,", calc_gyro(charArrayToInt16(ptr)));
					strcat(out_string, temp_string);
					ptr += 2;
				}
				for(int j = 0; j < 3; j++){
					char temp_string[100];
					sprintf(temp_string, "%f,", calc_accel(charArrayToInt16(ptr)));
					strcat(out_string, temp_string);
					ptr += 2;
				}
				strcat(out_string, "\n");
				phase_two_data_csv = fopen("phase_two_data.csv", "a");
				fprintf(phase_two_data_csv, out_string);
				fclose(phase_two_data_csv);
			}
		}
	}
	return 0;
}

uint32_t charArrayToUint32(char* byteStart) {
	uint32_t x;
    char number[4] = { *(byteStart + 3), *(byteStart + 2), *(byteStart + 1), *(byteStart)};
	memcpy(&x, number, sizeof(x));
    return x;
}

uint16_t charArrayToUint16(char* byteStart){
	uint16_t x;
	char number[2] = {*(byteStart + 1), *byteStart};
	memcpy(&x, number, sizeof(x));
	return x;
}

int16_t charArrayToInt16(char* byteStart){
	int16_t x;
	char number[2] = {*(byteStart + 1), *byteStart};
	memcpy(&x, number, sizeof(x));
	return x;
}

//temperature calibrated for our specific Flight Model
float calc_temp(uint16_t temp){
	return (temp - 324.31)/1.22;
}

//https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library/blob/master/src/SparkFunLSM9DS1.cpp for values
float calc_mag(int16_t mag){
	return 0.00014 * mag;
}

float calc_accel(int16_t accel){
	return 0.000732 * accel;
}

float calc_gyro(int16_t gyro){
	return 0.07 * gyro;
}

