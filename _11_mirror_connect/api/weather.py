import requests
from api.date_time import dateTime

class weather:
    api_key = "xxxxx"
    city = "Noisy-Le-Grand"
    url = f"http://api.weatherapi.com/v1/forecast.json?key={api_key}&q={city}&days=1&aqi=no&alerts=no"

    ###### Function for retrieving the curent temperature ######
    def get_curent_temperature(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            temperature_curent = data["current"]["temp_c"]
            temperature_int = int(round(float(temperature_curent)))
            temperature_string = f"{temperature_int}°"
            return temperature_string
        else:
            return "None"

    ###### Function for retrieving the curent condition ######
    def get_curent_condition(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            condition_curent_code = data["current"]["condition"]["code"]
            condition_curent = f'images/{condition_curent_code}.png'
            return condition_curent
        else:
            return "None"
    
    ###### Function for retrieving the curent humidity ######
    def get_curent_humidity(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            humidity_curent = data["current"]["humidity"]
            rounded_curent_humidity = round(humidity_curent)
            return f"{rounded_curent_humidity}%"
        else:
            return "None"
    
    ###### Function for retrieving the curent wind ######
    def get_curent_wind(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            wind_curent = data["current"]["wind_kph"]
            rounded_curent_wind = round(wind_curent)
            return f"{rounded_curent_wind} km/h"
        else:
            return "None"
    
    ###### Function for retrieving the curent precipitation ######
    def get_curent_precipitation(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            instance_dateTime = dateTime()
            hour = instance_dateTime.get_hour()
            precipitation_curent = data["forecast"]["forecastday"][0]["hour"][hour]["chance_of_rain"]
            return f"{precipitation_curent}%"
        else:
            return "None"
    

    



    ###### Function for retrieving the temperature ######
    def get_temperature(self, hour):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            temperature = data["forecast"]["forecastday"][0]["hour"][hour]["temp_c"]
            rounded_temperature= round(temperature)
            return f"{rounded_temperature}°C"
        else:
            return "None"
    
    ###### Function for retrieving the condition ######
    def get_condition(self, hour):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            condition_code = data["forecast"]["forecastday"][0]["hour"][hour]["condition"]["code"]
            condition = f'images/{condition_code}.png'
            return condition
        else:
            return "None"

    ###### Function for retrieving the precipitation ######
    def get_precipitation(self, hour):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            precipitation = data["forecast"]["forecastday"][0]["hour"][hour]["chance_of_rain"]
            return f"{precipitation}%"
        else:
            return "None"
