#include <wiringPi.h>
#include "bme280.h"
#include <wiringSerial.h>
int main(void)
{
	bme280_raw_data raw;
	bme280_calib_data cal;
	int fd = wiringPiI2CSetup(BME280_ADDRESS);//on associe une variable a une broche I2C
	init_capteur(fd);
	if (fd < 0)//test de fonctionnalité du capteur 
	{
		printf("Device not found");
	}
	else
	{
		printf("Device OK");

		getRawData(fd, &raw);//recuperation des donnee brute
		printf("raw temp: %d C\n raw pressure:%d hpa\n raw humidity: %d % \n", raw.temperature, raw.pressure, raw.humidity);
		readCalibrationData(fd, &cal);//lire le calibrage des donnee
		int32_t t_fine = getTemperatureCalibration(&cal, raw.temperature);//variable t_fine =le calibrage de la donnee temperature
		float temp = compensateTemperature(t_fine);//temp = la valeur finale de la temperature 
		printf("temp %f C\n", temp);
		float pressure = compensatePressure(raw.pressure, &cal, t_fine) / 100;//valeur calculer de la presion avec la valeur brute de la pression,et pointeur de la structure calibrage et le t_fine /100 pour avoir des hecto pascal
		printf("pressure %f hpa\n", pressure);
		float humidity = compensateHumidity(raw.humidity, &cal, t_fine);//valeur calculer de l'humiditer avec la valeur brute de l'humidité et &cal et t_fine
		printf("humidity %f %\n", humidity);
	}
	return 0;
}
