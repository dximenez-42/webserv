#include "Api.hpp"

Api::Api() {
    _errorPath = "www/errors";      //TODO Cambiar a la ruta del config
}

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

Route Api::findRoute()
{
    std::vector<Route> routes = _server->getRoutes();

    std::string uri = _request->getNormalizedUri();
    std::string method = _request->getMethod();

    for (size_t i = 0; i < routes.size(); i++)
    {
        if (routes[i].path == uri && routes[i].method == method)
        {
            return routes[i];
        }
    }
    return Route();
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
    struct stat st = { .st_dev = 0 };

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

    std::string contentType = _request->getContentType();
    if (contentType.find("multipart/form-data") == std::string::npos) {
        sendError(415);
        return;
    }
    //HANDLE UPLOADED FILE
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
        sendError(405);
        return;
    }
    std::vector<Route> routes = _server->getRoutes();

    Route route = findRoute();

    if (!route.location.empty() && !route.method.empty() && !route.path.empty()) {
        if (route.location.find("http://") == 0 || route.location.find("https://") == 0)
            prepareRedirectResponse(route.location);
        else if (endsWith(route.location, ".json")) {
            std::string jsonContent = readJsonFile(route.location);
            if (!jsonContent.empty()) {
                prepareJsonResponse(jsonContent);
            } else {
                std::cerr << "Error: File not found or empty" << std::endl;
                sendError(404);
                return;
            }
        }
        else if (endsWith(route.location, ".html")) {
            std::string htmlContent = readHtmlFile(route.location);
            if (!htmlContent.empty()) {
                prepareHtmlResponse(htmlContent);
            } else {
                std::cerr << "Error: File not found or empty" << std::endl;
                sendError(404);
                return;
            }
        }
        else if (route.path == "uploads") {
            handleFileUpload();
        }
        else{
            std::cout << "Error no especificado " << route.path << std::endl;
        }
        sendResponse(client_socket);
    }
    else
    {
        sendError(500); // TODO Comprobar error
    }
}

