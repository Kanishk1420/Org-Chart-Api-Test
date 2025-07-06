// #define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>


DROGON_TEST(RemoteAPITest)
{
    auto client = drogon::HttpClient::newHttpClient("http://127.0.0.1:8080");
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setMethod(drogon::Get);
    req->setPath("/");
    auto [result, response] = client->sendRequest(req);
    
    #ifdef RUNNING_IN_DOCKER
    // In Docker, we expect a bad server address
    REQUIRE(result == drogon::ReqResult::BadServerAddress);
    #else
    // In normal environment, connection should succeed
    REQUIRE(result == drogon::ReqResult::Ok);
    #endif
}

