#include "Api.hpp"

Api::Api() {}

Api::~Api() {}

Api::Api(const Api& other) : _request(other._request), _server(other._server), _httpResponse(other._httpResponse), _errorPath(other._errorPath), _client_socket(other._client_socket) {}

Api& Api::operator=(const Api& other) {
    if (this == &other) {
        return *this;
    }
    _request = other._request;
    _server = other._server;
    _httpResponse = other._httpResponse;
    _errorPath = other._errorPath;
    _client_socket = other._client_socket;

    return *this;
}

void    Api::setRequest(Request *request)
{
    _request = request;
}

void    Api::setServer(Server *server)
{
    _server = server;
}




Route* Api::findRoute() {
    std::vector<Route> routes = _server->getRoutes();
    std::string path = _request->getNormalizedUri();
    std::string method = _request->getMethod();
    bool pathExists = false;

    for (std::vector<Route>::const_iterator it = routes.begin(); it != routes.end(); ++it) {
        if (it->path == path) {
            if (it->method == method) {
                return const_cast<Route*>(&(*it));
            }
            pathExists = true; 
        }
    }
    if (pathExists){
        sendError(405);
    }
    else
        sendError(404);
    return NULL;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string readJsonFile(const std::string& filepath) {
    std::ifstream file(filepath.c_str(), std::ios::in);
    if (!file.is_open()) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

std::string readHtmlFile(const std::string& filepath) {

    std::ifstream file(filepath.c_str(), std::ios::in);
    if (!file.is_open()) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}




void Api::sendError(int errorCode)
{
    std::ostringstream oss;
        oss << _errorPath << "/" << errorCode << ".html";
        std::string filePath = oss.str();


        std::ifstream errorFile(filePath.c_str(), std::ios::in);
        if (!errorFile.is_open() || errorCode == 500) {
            _httpResponse = "HTTP/1.1 500 Internal Server Error\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 5\r\n"
                            "\r\n"
                            "Error";
            sendResponse(_client_socket);
            return;
        }

        std::stringstream buffer;
        buffer << errorFile.rdbuf();
        std::string fileContent = buffer.str();

        std::ostringstream responseStream;
        responseStream << "HTTP/1.1 " << errorCode << " Error\r\n"
                       << "Content-Type: text/html\r\n"
                       << "Content-Length: " << fileContent.length() << "\r\n"
                       << "\r\n"
                       << fileContent;

        _httpResponse = responseStream.str();

        sendResponse(_client_socket);
}

int  Api::checkMethod()
{    
    std::vector<std::string> methods = _server->getMethods();
    for (std::vector<std::string>::iterator it = methods.begin(); it < methods.end(); it++)
    {
        std::string method = *it;
        if (method == _request->getMethod())
        {
            return 0;
        }
    }
    return -1;
}

bool Api::createDirectory(const std::string& path) {
    struct stat st = {0};

    if (stat(path.c_str(), &st) == -1) {
        if (mkdir(path.c_str(), 0700) != 0) {
            return false;
        }
    }
    return true;
}


void Api::handleFileUpload() {
    _request->printRequest();
    if (_request == NULL) {
        sendError(400);
        return;
    }

    std::string contentType = _request->getHeaderValue("Content-Type");
    if (contentType.find("multipart/form-data") == std::string::npos) {
        sendError(415); // Unsupported Media Type
        return;
    }

    std::string boundary = "--" + _request->getBoundary();
    std::string body = _request->getBody();
    std::string::size_type pos = 0;

    std::string boundaryDelimiter = boundary + "\r\n";
    std::string boundaryEnd = boundary + "--\r\n";

    while ((pos = body.find(boundaryDelimiter, pos)) != std::string::npos) {
        pos += boundaryDelimiter.length();

        std::string::size_type headerEnd = body.find("\r\n\r\n", pos);
        if (headerEnd == std::string::npos) break;

        std::string headers = body.substr(pos, headerEnd - pos);
        pos = headerEnd + 4;  // Skip CRLFCRLF

        std::string::size_type fileEnd = body.find(boundaryDelimiter, pos);
        if (fileEnd == std::string::npos) {
            fileEnd = body.find(boundaryEnd, pos);
        }
        if (fileEnd == std::string::npos) break;

        std::string fileData = body.substr(pos, fileEnd - pos);
        pos = fileEnd + boundaryDelimiter.length();

        std::string fileName;
        std::string line;
        std::istringstream headerStream(headers);
        while (std::getline(headerStream, line)) {
            if (line.find("Content-Disposition:") != std::string::npos && line.find("filename=\"") != std::string::npos) {
                std::string::size_type startPos = line.find("filename=\"") + 10;
                std::string::size_type endPos = line.find("\"", startPos);
                fileName = line.substr(startPos, endPos - startPos);
                break;
            }
        }

        if (!fileName.empty()) {
            if (!createDirectory("uploads")) {
                sendError(500);
                return;
            }

            std::ofstream outFile(("uploads/" + fileName).c_str(), std::ios::binary);
            if (!outFile) {
                sendError(500);
                return;
            }
            outFile.write(fileData.c_str(), fileData.size());
            outFile.close();

            _httpResponse = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 22\r\n"
                            "\r\n"
                            "File uploaded successfully";
            sendResponse(_client_socket);
        }
    }
}





void    Api::prepareRedirectResponse(const std::string& newLocation) {
    _httpResponse =
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Location: " + newLocation + "\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
}

void    Api::prepareHtmlResponse(const std::string& htmlContent) {
    _httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + itos(htmlContent.length()) + "\r\n"
        "\r\n" +
        htmlContent;
}

void    Api::prepareJsonResponse(const std::string& jsonContent) {
    _httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + itos(jsonContent.length()) + "\r\n"
        "\r\n" +
        jsonContent;
}

void    Api::sendResponse(int client_socket)
{
    if (!_httpResponse.empty()) {
        send(client_socket, _httpResponse.c_str(), _httpResponse.length(), 0);
    } else {
        std::cerr << "Error: No response prepared." << std::endl;
    }
}

void Api::handleRequest(int client_socket) {
    _client_socket = client_socket;
    if (checkMethod() == -1)
    {
        //GESTIONAR RESPUESTA PARA MÉTODO NO ACEPTADO
        sendError(405);
        return;
    }
    std::vector<Route> routes = _server->getRoutes();

    Route *route = findRoute();

    if (route != NULL) {
        if (route->location.find("http://") == 0 || route->location.find("https://") == 0)
            prepareRedirectResponse(route->location);
        else if (endsWith(route->location, ".json")) {
            std::string jsonContent = readJsonFile(route->location);
            if (!jsonContent.empty()) {
                prepareJsonResponse(jsonContent);
            } else {
                std::cerr << "Error: File not found or empty" << std::endl;
                sendError(404);
                return;
            }
        }
        else if (endsWith(route->location, ".html")) {
            std::string htmlContent = readHtmlFile(route->location);
            if (!htmlContent.empty()) {
                prepareHtmlResponse(htmlContent);
            } else {
                std::cerr << "Error: File not found or empty" << std::endl;
                sendError(404);
                return;
            }
        }
        else if (route->path == "uploads") {
            handleFileUpload();
        }
        else{
            std::cout << "Error no especificado " << route->path << std::endl;
        }
        sendResponse(client_socket);
    }
}

