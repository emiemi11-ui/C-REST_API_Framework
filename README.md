```
Proiectul-principal este PSOP2.zip ;deziarhivati si rulati in LinuxMint


Cerințe
1) Crearea structurii framework-ului
2) Implementarea unui server REST
	-Acceptarea cererilor HTTP (GET, POST, PUT, DELETE);
	-Trimiterea răspunsurilor către client în format JSON.
3) Definirea endpoint-urilor principale
4) Implementarea stratului de date (Data Layer)
    -Conectarea la o bază de date;
5) Implementarea stratului de servicii (Service Layer)
6) Crearea stratului de control (Controller Layer)
```
```
Structura proiectului
C++ Rest API Framework/
├── Makefile
├── main.cpp
├── server
├── worker
├── controller
├── service
├── repository
├── database
├── shared_queue
├── thread_pool
├── semaphore
├── http_response
├── utils
├── users.db
```

Pentru a se porni serverul se ruleaza ./server:
Din alt terminal se pot da urmatoarele comenzi:
- curl -X GET http://localhost:8080/users (arata lista de useri)
- curl -X POST http://localhost:8080/users -d "name=Gabriel" (adaug un utilizator nou)
- curl -X DELETE http://localhost:8080/users -d "name=Gabriel" (sterg un utilizator din baza de date)
- curl -X PUT http://localhost:8080/users -d "old=Ana&new=AnaMaria" (update la numele utilizatorului)
