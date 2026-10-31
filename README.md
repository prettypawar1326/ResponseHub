# ResponseHub – Response Collection System

## Project Overview
ResponseHub is a web-based response collection system. It allows users to create feedback forms, collect responses, view submitted responses, and check basic analytics.

## Features
- Create feedback forms
- Add form title and description
- Submit responses
- Store responses using C++ backend
- View all submitted responses
- Clear all responses
- View basic analytics

## Technologies Used
- HTML
- CSS
- JavaScript
- C++
- Local Storage
- File Handling
- Winsock2

## Project Structure
```text
ResponseCollectorSystem/
├── frontend/
│   ├── index.html
│   ├── create-form.html
│   ├── fill-form.html
│   ├── success.html
│   ├── responeses.html
│   └── analytics.html
├── backend/
│   └── main.cpp
├── database/
└── README.md
```

## How to Compile
Install MinGW C++ compiler and run:

```bash
g++ backend/main.cpp -o backend.exe -lws2_32
```

## How to Run
Run the backend executable from the main project folder:

```bash
.\backend.exe
```

Open your browser and visit:

http://localhost:8080

## Data Storage
Submitted responses are stored in a text file named `responses.txt` using C++ file handling. Form title and description are stored in browser Local Storage.

## Project Objective
The objective of this project is to demonstrate frontend development, C++ backend programming, HTTP communication, and file handling.