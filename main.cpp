
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string readFile(string path) {
    ifstream file(path.c_str(), ios::binary);
    if (!file) return "";

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string getContentType(string path) {
    if (path.find(".html") != string::npos) return "text/html";
    if (path.find(".css") != string::npos) return "text/css";
    if (path.find(".js") != string::npos) return "application/javascript";
    return "text/plain";
}

void sendResponse(SOCKET client, string body, string type,
                  string status = "200 OK") {
    string header =
        "HTTP/1.1 " + status + "\r\n"
        "Content-Type: " + type + "; charset=utf-8\r\n"
        "Content-Length: " + to_string(body.size()) + "\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n\r\n";

    string response = header + body;
    send(client, response.c_str(), (int)response.size(), 0);
}

void handleClient(SOCKET client) {
    string request;
    char buffer[4096];

    int received;
    while ((received = recv(client, buffer, sizeof(buffer), 0)) > 0) {
        request.append(buffer, received);

        size_t headerEnd = request.find("\r\n\r\n");
        if (headerEnd != string::npos) {
            size_t pos = request.find("Content-Length:");
            if (pos != string::npos && pos < headerEnd) {
                size_t end = request.find("\r\n", pos);
                int length = atoi(request.substr(pos + 15, end - pos - 15).c_str());
                if (request.size() >= headerEnd + 4 + length) break;
            } else {
                break;
            }
        }
    }

    size_t headerEnd = request.find("\r\n\r\n");
    if (headerEnd == string::npos) {
        sendResponse(client, "Bad Request", "text/plain", "400 Bad Request");
        return;
    }

    string headers = request.substr(0, headerEnd);
    string body = request.substr(headerEnd + 4);

    stringstream firstLine(headers);
    string method, path, version;
    firstLine >> method >> path >> version;

    if (path == "/api/submit" && method == "POST") {
        ofstream file("responses.txt", ios::app);

        if (file) {
            file << body << endl;
            sendResponse(client, "{\"message\":\"Response saved successfully\"}",
                         "application/json");
        } else {
            sendResponse(client, "{\"error\":\"Could not save response\"}",
                         "application/json", "500 Internal Server Error");
        }
    }
    else if (path == "/api/responses" && method == "GET") {
        ifstream file("responses.txt");
        string line, result = "[";
        bool first = true;

        while (getline(file, line)) {
            if (line.empty()) continue;
            if (!first) result += ",";
            result += line;
            first = false;
        }

        result += "]";
        sendResponse(client, result, "application/json");
    }
    else if (path == "/api/clear" && method == "POST") {
        ofstream file("responses.txt", ios::trunc);

        if (file) {
            file.close();
            sendResponse(client, "{\"message\":\"All responses cleared\"}",
                         "application/json");
        } else {
            sendResponse(client, "{\"error\":\"Could not clear responses\"}",
                         "application/json", "500 Internal Server Error");
        }
    }
    else if (method == "GET") {
        if (path == "/") path = "/index.html";

        if (path.find("..") != string::npos) {
            sendResponse(client, "Forbidden", "text/plain", "403 Forbidden");
        } else {
            string content = readFile("frontend" + path);

            if (content.empty()) {
                sendResponse(client, "Page not found", "text/plain", "404 Not Found");
            } else {
                sendResponse(client, content, getContentType(path));
            }
        }
    }
    else {
        sendResponse(client, "Not Found", "text/plain", "404 Not Found");
    }
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddress;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Winsock initialization failed." << endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (sockaddr*)&serverAddress,
             sizeof(serverAddress)) == SOCKET_ERROR) {
        cout << "Port 8080 is busy or unavailable." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 5);

    cout << "ResponseHub C++ Server Started!" << endl;
    cout << "Open: http://localhost:8080" << endl;
    cout << "Press Ctrl+C to stop the server." << endl;

    while (true) {
        clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket != INVALID_SOCKET) {
            handleClient(clientSocket);
            closesocket(clientSocket);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
