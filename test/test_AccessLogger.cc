#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include <drogon/plugins/AccessLogger.h>
#include <fstream>
#include <filesystem>

using namespace drogon;
using namespace drogon::plugin;

// Helper function to check if a file exists
bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

// Helper function to delete a file if it exists
void removeFileIfExists(const std::string& path) {
    if (fileExists(path)) {
        std::remove(path.c_str());
    }
}

DROGON_TEST(AccessLoggerInitTest)
{
    // Test basic initialization
    auto logger = std::make_shared<AccessLogger>();
    
    Json::Value config;
    config["log_path"] = "./";
    config["log_file"] = "test_access.log";
    config["log_format"] = "$remote_addr - $method $url $status";
    
    // Should not throw
    CHECK_NOTHROW(logger->initAndStart(config));
    
    // Clean up
    logger->shutdown();
    
    // Remove the test log file
    removeFileIfExists("./test_access.log");
}

DROGON_TEST(AccessLoggerCustomFormatTest)
{
    // Test with custom format
    auto logger = std::make_shared<AccessLogger>();
    
    Json::Value config;
    config["log_path"] = "./";
    config["log_file"] = "test_custom.log";
    config["log_format"] = "$request_date - $method $url [$status] $processing_time";
    config["use_local_time"] = true;
    config["show_microseconds"] = true;
    
    // Should not throw
    CHECK_NOTHROW(logger->initAndStart(config));
    
    // Create a mock request and response
    auto req = HttpRequest::newHttpRequest();
    req->setPath("/test/path");
    req->setMethod(HttpMethod::Get);
    
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Test Body");
    
    // Wait a bit to ensure logging has occurred
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Clean up
    logger->shutdown();
    
    // Remove the test log file
    removeFileIfExists("./test_custom.log");
}

DROGON_TEST(AccessLoggerHeadersTest)
{
    // Test logging of request/response headers
    auto logger = std::make_shared<AccessLogger>();
    
    Json::Value config;
    config["log_path"] = "./";
    config["log_file"] = "test_headers.log";
    config["log_format"] = "$remote_addr - $method $url $status $http_user_agent $upstream_http_content_type";
    
    // Should not throw
    CHECK_NOTHROW(logger->initAndStart(config));
    
    // Create a mock request with headers
    auto req = HttpRequest::newHttpRequest();
    req->setPath("/api/test");
    req->setMethod(HttpMethod::Post);
    req->addHeader("User-Agent", "Drogon Test Client");
    
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k201Created);
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody("{\"result\":\"success\"}");
    
    // Wait a bit to ensure logging has occurred
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Clean up
    logger->shutdown();
    
    // Remove the test log file
    removeFileIfExists("./test_headers.log");
}