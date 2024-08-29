#!/usr/bin/php
<?php
// Enable error reporting
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

header("Content-Type: text/html");

// Display all environment variables (for debugging)
echo "<pre>";
// print_r($_SERVER); // DEBUG
echo "</pre>";

// Start HTML output
echo "<html>";
echo "<head><title>Simple CGI Script</title></head>";
echo "<body>";
echo "<h1>Query String Parameters</h1>";
echo "<ul>";

foreach ($_GET as $key => $value) {
    echo "<li><strong>" . htmlspecialchars($key) . ":</strong> " . htmlspecialchars($value) . "</li>";
}

echo "</ul>";
echo "</body>";
echo "</html>";
?>
