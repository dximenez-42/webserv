#!/usr/bin/env python3

import os
import urllib.parse
import cgitb

# Enable debugging
cgitb.enable()

# Get the query string from the environment variable
query_string = os.getenv("QUERY_STRING", "")

# Parse the query string using the correct module
params = urllib.parse.parse_qs(query_string)

# HTML response
print("Content-Type: text/html")  # Specify the content type
print()  # End headers with a blank line

# Start HTML content
print("<html>")
print("<head><title>Simple CGI Script</title></head>")
print("<body>")
print("<h1>Query String Parameters</h1>")
print("<ul>")

# Display each query string parameter
for key, values in params.items():
    for value in values:
        print(f"<li><strong>{key}:</strong> {value}</li>")

print("</ul>")
print("</body>")
print("</html>")
