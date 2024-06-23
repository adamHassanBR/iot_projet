
#scp -r /Users/adam_hassan/Desktop/pro/PROJET/iot_projet/ pi@192.168.1.39:/home/pi-hub/Desktop 
import os
os.environ['KIVY_IMAGE'] = 'sdl2'
import time
import kivy
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.config import Config
from kivy.core.text import LabelBase, DEFAULT_FONT


from api.date_time import dateTime
from api.weather import weather

kivy.require('2.0.0')

class IhmView(BoxLayout):
    pass

class ConnectedMiror(App):

    def build(self):
        
        # Définissez la largeur et la hauteur de l'écran
        Config.set('graphics', 'width', 1920)
        Config.set('graphics', 'height', 1080)

        # Désactivez le mode plein écran automatique
        Config.set('graphics', 'fullscreen', '0')
        
        # interval to update labels
        Clock.schedule_interval(self.update_values, 55)
        
        return IhmView()
    
    
    ###### Function run every minute to set label values ######
    def update_values(self, dt):
        instance_dateTime = dateTime()
        instance_wether = weather()
        
        heure_actuel = instance_dateTime.get_hour()
        calcule_heure = heure_actuel
        

        # update labels of time and date
        self.root.ids.heure_label.text = instance_dateTime.get_time()
        
        
        
        # update labels of current values
        self.root.ids.curent_temperature_label.text = instance_wether.get_curent_temperature()
        self.root.ids.curent_icone_image.source = instance_wether.get_curent_condition()
        self.root.ids.curent_precipitation_label.text = instance_wether.get_curent_precipitation()
        self.root.ids.curent_humidity_label.text = instance_wether.get_curent_humidity()
        self.root.ids.curent_wind_label.text = instance_wether.get_curent_wind()
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        
        # update labels of h+1 values
        self.root.ids.plus_un_titre.text = str(calcule_heure)
        self.root.ids.plus_un_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_un_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        # update labels of h+2 values
        self.root.ids.plus_deux_titre.text = str(calcule_heure)
        self.root.ids.plus_deux_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_deux_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+3 values
        self.root.ids.plus_trois_titre.text = str(calcule_heure)
        self.root.ids.plus_trois_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_trois_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+4 values
        self.root.ids.plus_quatre_titre.text = str(calcule_heure)
        self.root.ids.plus_quatre_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_quatre_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+5 values
        self.root.ids.plus_cinq_titre.text = str(calcule_heure)
        self.root.ids.plus_cinq_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_cinq_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+6 values
        self.root.ids.plus_six_titre.text = str(calcule_heure)
        self.root.ids.plus_six_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_six_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+7 values
        self.root.ids.plus_sept_titre.text = str(calcule_heure)
        self.root.ids.plus_sept_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_sept_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        self.root.ids.date_label.text = instance_dateTime.get_date()
        
        
    ###### Function to set values on start programme ######
    def on_start(self):
        instance_dateTime = dateTime()
        waiting_time = instance_dateTime.get_waiting_time()
        instance_wether = weather()
        
        heure_actuel = instance_dateTime.get_hour()
        calcule_heure = heure_actuel
        
        
        # We wait for the next minute before starting the rest of the program
        #time.sleep(waiting_time)
        
        # update labels of time and date
        self.root.ids.heure_label.text = instance_dateTime.get_time()
        self.root.ids.date_label.text = instance_dateTime.get_date()
        
        # update labels of current values
        self.root.ids.curent_temperature_label.text = instance_wether.get_curent_temperature()
        self.root.ids.curent_icone_image.source = instance_wether.get_curent_condition()
        self.root.ids.curent_precipitation_label.text = instance_wether.get_curent_precipitation()
        self.root.ids.curent_humidity_label.text = instance_wether.get_curent_humidity()
        self.root.ids.curent_wind_label.text = instance_wether.get_curent_wind()
        
        # update labels of h+1 values
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        
        self.root.ids.plus_un_titre.text = str(calcule_heure)
        self.root.ids.plus_un_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_un_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        # update labels of h+2 values
        self.root.ids.plus_deux_titre.text = str(calcule_heure)
        self.root.ids.plus_deux_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_deux_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+3 values
        self.root.ids.plus_trois_titre.text = str(calcule_heure)
        self.root.ids.plus_trois_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_trois_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+4 values
        self.root.ids.plus_quatre_titre.text = str(calcule_heure)
        self.root.ids.plus_quatre_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_quatre_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 

        # update labels of h+5 values
        self.root.ids.plus_cinq_titre.text = str(calcule_heure)
        self.root.ids.plus_cinq_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_cinq_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        
        # update labels of h+6 values
        self.root.ids.plus_six_titre.text = str(calcule_heure)
        self.root.ids.plus_six_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_six_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        if calcule_heure+1 > 23 :
            calcule_heure = -1
        
        calcule_heure = calcule_heure + 1 
        
        # update labels of h+7 values
        self.root.ids.plus_sept_titre.text = str(calcule_heure)
        self.root.ids.plus_sept_temperature_label.text = instance_wether.get_temperature(calcule_heure)
        self.root.ids.plus_sept_icone_image.source = instance_wether.get_condition(calcule_heure)
        
        


if __name__ == "__main__":
    connectedMiror = ConnectedMiror()
    connectedMiror.run()
