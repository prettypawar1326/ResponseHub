# ResponseHub – Response Collection System

## Project Overview
ResponseHub is a web-based response collection system that allows users to create forms, collect responses, view submissions, and check basic analytics.

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
ResponseHub/
├── frontend/
│   ├── index.html
│   ├── create-form.html
│   ├── fill-form.html
│   ├── success.html
│   ├── responeses.html
│   └── analytics.html
├── backend/
│   └── main.cpp
└── README.md
```

## How to Compile
```bash
g++ backend/main.cpp -o backend.exe -lws2_32
```

## How to Run
Run the backend executable from the project root:

```bash
.\backend.exe
```

Open in browser:

http://localhost:8080

## Data Storage
Submitted responses are stored in `responses.txt` using C++ file handling. Form title and description are stored in browser Local Storage.

## Project Objective
To demonstrate frontend development, C++ backend programming, HTTP communication, and file handling.
