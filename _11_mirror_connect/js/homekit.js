const { Accessory, Characteristic, Service, uuid, AccessoryEventTypes } = require('hap-nodejs');
const { AccessoryPlugin } = require('homebridge-lib');
const { exec } = require('child_process');

// Initialisez votre accessoire
const accessory = new Accessory('Interrupteur Raspberry Pi', uuid.generate('ABC123'));

// Ajoutez un service (dans ce cas, un service de type interrupteur)
const switchService = new Service.Switch('Interrupteur', 'ABC123');

// Définissez les caractéristiques d'activation/désactivation de l'interrupteur
switchService.getCharacteristic(Characteristic.On)
  .onGet(() => {
    // Définissez la logique pour obtenir l'état actuel (true pour activé, false pour désactivé)
    return votreEtatInterrupteur; // Remplacez par l'état réel de votre interrupteur
  })
  .onSet((value) => {
    // Définissez la logique pour définir l'état de l'interrupteur (value sera true pour activé, false pour désactivé)
    // Mettez à jour votre interrupteur en conséquence

    if (value) {
      // Si l'interrupteur est activé, exécutez la commande 'xrandr' pour activer l'affichage
      exec('DISPLAY=:0 xrandr --output HDMI-1 --mode 1920x1080', (error, stdout, stderr) => {
        if (error) {
          console.error(`Erreur : ${error.message}`);
          return;
        }
        console.log(`Sortie de la commande : ${stdout}`);
      });
    } else {
      // Si l'interrupteur est désactivé, exécutez la commande 'xrandr' pour éteindre l'affichage
      exec('DISPLAY=:0 xrandr --output HDMI-1 --off', (error, stdout, stderr) => {
        if (error) {
          console.error(`Erreur : ${error.message}`);
          return;
        }
        console.log(`Sortie de la commande : ${stdout}`);
      });
    }
  });

// Ajoutez le service à l'accessoire
accessory.addService(switchService);

// Publiez l'accessoire sur le réseau local
accessory.publish({
  username: 'Miror',
  pincode: '11111122',
  category: Accessory.Categories.Switch,
});

// Gérez l'événement 'listening'
accessory.on(AccessoryEventTypes.LISTENING, () => {
  console.log("L'accessoire écoute sur le port 51826");
});
