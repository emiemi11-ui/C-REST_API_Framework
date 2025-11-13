Cerințe
1) Crearea structurii framework-ului
2) Implementarea unui server REST
	-Acceptarea cererilor HTTP (GET, POST, PUT, DELETE);
	-Trimiterea răspunsurilor către client în format JSON.
3) Definirea endpoint-urilor principale
4) Implementarea stratului de date (Data Layer)
    -Conectarea la o bază de date;
    -Maparea rezultatelor în obiecte C++.
5) Implementarea stratului de servicii (Service Layer)
6) Crearea stratului de control (Controller Layer)

Structura proiectului
```
rest-api-framework/
├── include/
│   ├── core/
│   │   ├── Server.hpp
│   │   ├── Worker.hpp
│   │   └── ThreadPool.hpp
│   ├── sync/
│   │   ├── Semaphore.hpp
│   │   └── Mutex.hpp
│   ├── ipc/
│   │   ├── SharedMemory.hpp
│   │   └── SharedQueue.hpp
│   ├── data/
│   │   ├── DatabaseConnection.hpp
│   │   └── Repository.hpp
│   └── http/
│       ├── Request.hpp
│       └── Response.hpp
├── src/
│   ├── core/
│   ├── sync/
│   ├── ipc/
│   ├── data/
│   └── http/
├── examples/
│   └── blog-api/
├── tests/
└── docs/
    └── documentatie.tex
    ├── README.md
```
