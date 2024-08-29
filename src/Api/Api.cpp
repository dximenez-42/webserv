#include "Api.hpp"

Api::Api() {
    _request = NULL;
    _server = NULL;
    _httpResponse = "";
    _errorPath = "";
    _client_socket = -1;
}

Api::~Api() {
    if (_request != NULL) {
        delete _request;
    }
    if (_server != NULL) {
        delete _server;
    }
}

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
    if (_request != NULL)
        delete _request;
    _request = request;
}

void    Api::setServer(Server *server)
{
    _server = server;
    _errorPath = _server->getErrorsDir();
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

std::string Api::generateUniqueFilename(const std::string& path, const std::string& filename) {
    std::string uniqueFilename = filename;
    std::string filePath = path + "/" + uniqueFilename;
    std::ifstream file(filePath.c_str());

    int counter = 1;
    while (file.good()) {
        std::ostringstream oss;
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::string::npos) {
            oss << filename.substr(0, dotPos) << "(" << counter << ")" << filename.substr(dotPos);
        } else {
            oss << filename << "(" << counter << ")";
        }
        uniqueFilename = oss.str();
        filePath = path + "/" + uniqueFilename;
        file.close();
        file.open(filePath.c_str());
        counter++;
    }

    return uniqueFilename;
}

void Api::handleFile() {
    std::string method = _request->getMethod();

    if (method == "POST") {
        handleFileUpload();
    } else if (method == "GET") {
        handleFileDownload();
    } else if (method == "DELETE") {
        handleFileDelete();
    } else {
        sendError(405);
    }
}

void Api::handleFileUpload() {
    std::cout << "Entra en handle upload" << std::endl;
    if (_request == NULL) {
        std::cout << "Error: request vacía" << std::endl;
        sendError(400);
        return;
    }

    std::vector<FormField> formFields = _request->getForm();

    if (formFields.empty()) {
        std::cout << "Error: celdas vacías" << std::endl;
        sendError(400);
        return;
    }

    bool fileUploaded = false;

	Route route = findRoute();

    for (std::vector<FormField>::iterator it = formFields.begin(); it != formFields.end(); ++it) {
        if (!it->filename.empty()) {
            std::string uniqueFilename = generateUniqueFilename(route.location, it->filename);
            std::string filePath = joinPaths(route.location, uniqueFilename);

            std::ofstream outFile(filePath.c_str(), std::ios::binary);
            if (!outFile) {
                sendError(500);
                return;
            }

            outFile.write(it->value.c_str(), it->value.size());
            outFile.close();
            fileUploaded = true;

			if (endsWith(filePath, ".py") || endsWith(filePath, ".php"))
			{
				handleCGI(filePath);
				return;
			}
        }
    }

    if (fileUploaded) {
        _httpResponse =  "HTTP/1.1 301 Moved Permanently\r\n"
                        "Location: ./uploads \r\n"
                        "Content-Length: 0\r\n"
                        "\r\n";
    } else {
        std::stringstream htmlContent;

        // Crear la estructura HTML
        htmlContent << "<html><head><title>Form Data</title></head><body style='padding:2rem'>";
        htmlContent << "<h1>Submitted Form Data</h1>";
        htmlContent << "<ul>";

        for (size_t i = 0; i < _request->getForm().size(); i++) {
            htmlContent << "<li>";
            htmlContent << "<b>Key:</b> " << _request->getForm()[i].key << "<br>";
            htmlContent << "<b>Value:</b> " << _request->getForm()[i].value << "<br>";
            htmlContent << "</li>";
        }

        htmlContent << "</ul>";
        htmlContent << "</body></html>";
    
        std::string htmlString = htmlContent.str();


        prepareHtmlResponse(htmlString);
        std::cout << _httpResponse << std::endl;              
    }

    sendResponse(_client_socket);
}

std::string getMimeType(const std::string& path) {
    std::map<std::string, std::string> mimeTypes;
    mimeTypes[".html"] = "text/html";
    mimeTypes[".htm"] = "text/html";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".json"] = "application/json";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".ico"] = "image/ico";
    mimeTypes[".gif"] = "image/gif";
    mimeTypes[".svg"] = "image/svg+xml";
    mimeTypes[".txt"] = "text/plain";

    std::string::size_type idx = path.find_last_of('.');
    if (idx != std::string::npos) {
        std::string extension = path.substr(idx);
        if (mimeTypes.find(extension) != mimeTypes.end()) {
            return mimeTypes[extension];
        }
    }

    return "application/octet-stream";
}

void Api::handleFileDownload() {
    std::string filePath = "www/" + _request->getNormalizedUri() + "/" + _request->getBasename();
    std::ifstream inFile(filePath.c_str(), std::ios::binary);

    // std::cout << "File Path: " << filePath << std::endl;
    if (!inFile) {
        std::cerr << "Error: Archivo no encontrado" << std::endl;
        sendError(404);
        return;
    }

    std::ostringstream oss;
    oss << inFile.rdbuf();
    std::string fileContent = oss.str();

    std::string mimeType = getMimeType(filePath);

    _httpResponse = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: " + mimeType + "\r\n"
                    "Content-Length: " + itos(fileContent.size()) + "\r\n"
                    "\r\n" +
                    fileContent;

    sendResponse(_client_socket);
}

void Api::handleFileDelete() {
    Route route = findRoute();
    if (route.path.empty())
    {
        sendError(404);
        return;
    }
    std::string filePath = joinPaths(route.location, _request->getBasename());

    if (unlink(filePath.c_str()) != 0) {
        std::cerr << "Error filepath" << std::endl;
        sendError(404);
    } else {
        _httpResponse = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 25\r\n"
                        "\r\n"
                        "File deleted successfully";
        sendResponse(_client_socket);
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


void Api::listDirectory(const std::string& directoryName) {
    std::cout << std::endl << "Dirlisting: " << _server->getServerDirListing() << std::endl;
    if (_server->getServerDirListing() == 0)
    {
        sendError(404);
        std::cout << "No debe ejecutar DirList!" << std::endl;
        return;
    }
    std::string directoryPath = "www/" + directoryName;
    std::stringstream html;
    html << "<html><body><h1>Directory Listing</h1><ul>";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directoryPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string name = ent->d_name;
            if (name != "." && name != "..") {
                html << "<li><a href=\"" << directoryName << "/" << name << "\">" << name << "</a></li>";
            }
        }
        closedir(dir);
    } else {
        html << "<li>Error opening directory</li>";
    }
    html << "</ul></body></html>";

    _httpResponse = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: " + itos(html.str().length()) + "\r\n"
                    "\r\n" + html.str();
}


void Api::handleRequest(int client_socket) {
    _client_socket = client_socket;
    std::string normalizedUri = _request->getNormalizedUri();

    if (normalizedUri.empty() || normalizedUri == "index.html" || normalizedUri == "index.htm") {
        serveFile("www/index.html");
        return;
    }

    if (normalizedUri.find("errors") == 0 && !_request->getBasename().empty()) {
        std::cout << "Entra y deberia devolver el archivo" << std::endl;
        serveFile("www/" + normalizedUri + "/" + _request->getBasename());
        return;
    }

    if (checkMethod() == -1) {
        sendError(405);
        return;
    }

    Route route = findRoute();
    if (!route.location.empty() && !route.method.empty() && !route.path.empty()) {
        handleRoute(route);
    } else {
        handleDirectoryOrError(normalizedUri);
    }
}

void Api::handleRoute(const Route& route) {
    if (route.location.find("http://") == 0 || route.location.find("https://") == 0) {
        prepareRedirectResponse(route.location);
    } else if (endsWith(route.location, ".json")) {
        serveJson(route.location);
    } else if (endsWith(route.location, ".html")) {
        serveFile(route.location);
    } else {
		if ((!_request->getBasename().empty() && endsWith(_request->getBasename(), ".py")) || (endsWith(_request->getBasename(), ".php") && _request->getMethod() == "GET"))
		{
			handleCGI(joinPaths(route.location, _request->getBasename()));
			return;
		}
        if (!_request->getBasename().empty() || (_request->getBasename().empty() && (_request->getMethod() == "POST" || _request->getMethod() == "DELETE"))) {
            handleFile();
        } else {
            listDirectory(route.path);
        }
    }
    sendResponse(_client_socket);
}

void Api::handleCGI(std::string path)
{
    std::string script_path = path;
    std::string method = _request->getMethod();
    std::string query_string = _request->getQueryString();
    std::string response;

	if (access(path.c_str(), F_OK) == -1)
	{
		sendError(404);
		return;
	}

    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);

        std::vector<std::string> env_vars;
        env_vars.push_back("REQUEST_METHOD=" + method);
        env_vars.push_back("QUERY_STRING=" + query_string);
        env_vars.push_back("SCRIPT_NAME=" + script_path);

        std::vector<char*> environ;
        for (size_t i = 0; i < env_vars.size(); i++)
            environ.push_back(const_cast<char*>(env_vars[i].c_str()));
        environ.push_back(0);

        if (endsWith(script_path,".py"))
        {
            char* const args[] = {strdup("/usr/bin/python3"), strdup(script_path.c_str()), 0};
            execve("/usr/bin/python3", args, environ.data());
        }
        else if (endsWith(script_path, ".php"))
        {
            char* const args[] = {strdup("/usr/bin/php"), strdup(script_path.c_str()), 0};
            execve("/usr/bin/php", args, environ.data());
        }
    }
    else
    {
	    close(pipe_fd[1]);
        waitpid(pid, 0, 0);

        char buffer[4096];
        ssize_t nbytes;
        while ((nbytes = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            response.append(buffer, nbytes);
        }

        close(pipe_fd[0]);

        _httpResponse = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + itos(response.length()) + "\r\n"
                        "\r\n" + response;
        sendResponse(_client_socket);
    }
}

void Api::handleDirectoryOrError(const std::string& normalizedUri) {
    std::string directoryPath = "www/" + normalizedUri;

    if (!directoryPath.empty() && directoryPath[directoryPath.size() - 1] == '/') {
        directoryPath.erase(directoryPath.size() - 1);
    }

    struct stat info;
    if (stat(directoryPath.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) {
        listDirectory(normalizedUri);
        sendResponse(_client_socket);
    } else {
        std::cerr << "Error: Ruta no encontrada" << std::endl;
        sendError(404);
    }
}


void Api::serveFile(const std::string& path) {
    std::string content = readHtmlFile(path);
    if (!content.empty()) {
        prepareHtmlResponse(content);
    } else {
        std::cerr << "Error: Archivo no encontrado o vacío - " << path << std::endl;
        sendError(404);
    }
    sendResponse(_client_socket);
}

void Api::serveJson(const std::string& path) {
    std::string jsonContent = readJsonFile(path);
    if (!jsonContent.empty()) {
        prepareJsonResponse(jsonContent);
    } else {
        std::cerr << "Error: Archivo JSON no encontrado o vacío - " << path << std::endl;
        sendError(404);
    }
    sendResponse(_client_socket);
}



