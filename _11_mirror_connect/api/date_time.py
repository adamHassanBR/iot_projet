import requests
from datetime import datetime
import time
class dateTime:
    api_key = "xxxxx"
    location = "Europe/Paris"
    url = f"http://api.timezonedb.com/v2.1/get-time-zone?key={api_key}&format=json&by=zone&zone={location}"

    ###### Function for retrieving the date and its formatting ######
    def get_date(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            date_str = data['formatted'].split()[0]
            date = datetime.strptime(date_str, "%Y-%m-%d")
            return date.strftime("%A %d %B %Y")  
        else:
            return "Date"
        

    ###### Function for retrieving the time (hour : minute) and its formatting ######
    def get_time(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            time = data['formatted'].split()[1]
            hour, minute, _ = time.split(':')
            return f"{hour}:{minute}"
        else:
            return "00:00"

    ###### Function to calculate the waiting time before the next minute ######
    def get_waiting_time(self):
        response = requests.get(self.url)
        if response.status_code == 200:
            data = response.json()
            actualy_seconde_time_int = data['timestamp'] % 60
            return 60 - int(actualy_seconde_time_int)
        else:
            return "waiting_time"
    
    ###### Function for retrieving the hour and its formatting ######
    def get_hour(self):
        time.sleep(2)
        hourminute = self.get_time()
        hoursplit = hourminute.split(":")
        if len(hoursplit) > 0:
            hour = hoursplit[0]
        return int(hour)




