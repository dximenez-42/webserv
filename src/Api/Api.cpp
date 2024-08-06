#include "Api.hpp"


Api::Api() {}

Api::~Api() {}

void    Api::setRequest(Request *request)
{
    _request = request;
}

void    Api::setServer(Server *server)
{
    _server = server;
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


void Api::prepareRedirectResponse(const std::string& newLocation) {
    _httpResponse =
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Location: " + newLocation + "\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    std::cout << _httpResponse << std::endl;
}

void Api::prepareHtmlResponse(const std::string& htmlContent) {
    _httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + itos(htmlContent.length()) + "\r\n"
        "\r\n" +
        htmlContent;
    std::cout << _httpResponse << std::endl;
}

void Api::prepareJsonResponse(const std::string& jsonContent) {
    _httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + itos(jsonContent.length()) + "\r\n"
        "\r\n" +
        jsonContent;
    std::cout << _httpResponse << std::endl;
}

void    Api::sendResponse(int client_socket)
{
    if (!_httpResponse.empty()) {
        send(client_socket, _httpResponse.c_str(), _httpResponse.length(), 0);
    } else {
        std::cerr << "Error: No response prepared." << std::endl;
    }
}

void Api::handleRequest() {
    if (checkMethod() == -1)
    {
        //GESTIONAR RESPUESTA PARA MÉTODO NO ACEPTADO
        std::cout << "Método no aceptado" << std::endl;
        return;
    }
    std::vector<Route> routes = _server->getRoutes();

    for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ) {
        if (_request->getNormalizedUri() != it->path) {
            std::cout << _request->getNormalizedUri() << "---" << it->path << std::endl;
            it = routes.erase(it);
        } else {
            std::cout << it->path << "Not deleted" << std::endl;
            ++it;
        }
    }

    if (routes.empty())
    {
        // GESTIONAR ERROR DE RUTA NO ENCONTRADA O NO ACCESIBLE
        std::cout << "Ruta no encontrada" << std::endl;
        return;
    }

    for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++) {
        if (_request->getMethod() == it->method) {
            if (it->location.find("http://") == 0 || it->location.find("https://") == 0)
                prepareRedirectResponse(it->location);
            else if (endsWith(it->location, ".json")) {
                std::string jsonContent = readJsonFile(it->location);
                if (!jsonContent.empty()) {
                    prepareJsonResponse(jsonContent);
                } else {
                    std::cerr << "Error: File not found or empty" << std::endl;
                }
            }
            else if (endsWith(it->location, ".html")) {
                std::string htmlContent = readHtmlFile(it->location);
                if (!htmlContent.empty()) {
                    prepareHtmlResponse(htmlContent);
                } else {
                    std::cerr << "Error: File not found or empty" << std::endl;
                }
            }
        }
        else{
            std::cout << "Method not accepted for: " << it->path << std::endl;
        }
    }
}

