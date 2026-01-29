https://community.st.com/t5/stm32-mcus-products/did-i-quot-brick-quot-my-quot-blue-pill-quot-board-s-st-link-v2/td-p/72470


### **1. Assurez-vous qu'OpenOCD est en cours d'exécution**
Dans un terminal séparé, lancez OpenOCD avec le fichier de configuration STM32 :

```bash
openocd -f ./stm32f103.cfg
```

Vous devriez voir quelque chose comme ceci dans la sortie :
```plaintext
Info : starting gdb server for stm32f1x.cpu on 3333
Info : Listening on port 3333 for gdb connections
```

Cela confirme qu'OpenOCD est prêt à accepter des connexions sur le port 3333.

---

### **2. Connectez-vous à OpenOCD depuis GDB**
Lancez une session : 
```bash
gdb-multiarch
```

Dans votre session GDB, connectez-vous au serveur GDB d'OpenOCD avec la commande :
```bash
target extended-remote localhost:3333
```

Vous devriez voir une confirmation indiquant que GDB est connecté à OpenOCD.

---

### **3. Charger le Fichier ELF**
Une fois connecté, chargez le fichier ELF dans GDB pour accéder aux symboles et au code source :

```bash
file build/Tutoriel.elf
```

---

### **4. Charger le Firmware dans la Mémoire**
Utilisez la commande suivante pour charger le firmware dans la Flash de la STM32 via GDB :

```bash
load
```

Cela doit fonctionner maintenant que la cible est correctement configurée.

---

### **5. Exécuter ou Déboguer le Programme**
- Pour exécuter le programme depuis le début :
  ```bash
  run
  ```
- Pour insérer des points d'arrêt avant l'exécution :
  ```bash
  break main
  continue
  ```

---

### **6. Envoyer des Commandes OpenOCD**
Utilisez la commande `monitor` pour envoyer des instructions à OpenOCD via GDB. Par exemple :
- Pour réinitialiser et arrêter le microcontrôleur :
  ```bash
  monitor reset halt
  ```
- Pour effacer la mémoire Flash :
  ```bash
  monitor flash erase_sector 0 0 last
  ```

---

### **7. Résolution de l'erreur `Exec format error`**
L'erreur `/bin/bash: line 1: ...: cannot execute binary file` survient parce que vous avez tenté d'exécuter un fichier ELF localement, ce qui n'est pas supporté pour une cible embarquée. Cela se produit généralement si le `target` n'a pas été configuré correctement avec `target extended-remote`.

Assurez-vous de toujours exécuter cette commande avant de charger ou de déboguer le firmware :
```bash
  target extended-remote localhost:3333
```
Dans GDB, on ajoute un break point avec la commande `b [nom du fichier].c:[numéro de la ligne]`

Pour continuer (passer au breakpoint d'après), on utilise `c`.



---

### Commandes Résumées
1. **Terminal 1 : OpenOCD**
   ```bash
   openocd -f ./stm32f103.cfg
   ```

2. **Terminal 2 : GDB**
   ```bash
   gdb-multiarch
   target extended-remote localhost:3333
   file build/Tutoriel.elf
   load
   monitor reset halt
   run
   ```

Ces étapes corrigent la configuration et devraient vous permettre de déboguer efficacement votre microcontrôleur STM32.