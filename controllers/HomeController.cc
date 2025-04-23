#include "HomeController.h"
#include <drogon/drogon.h>             // For drogon::app()
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <sstream>
#include <vector>

using namespace std;
using namespace drogon;
// using namespace drogon::orm;

void HomeController::root(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpViewResponse("base.csp");
    callback(resp);
}


void HomeController::home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpViewResponse("home.csp");

    callback(resp);
}

void HomeController::about(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // HttpViewData data;
    // data.insert("title", "Add new item");

    auto resp = HttpResponse::newHttpViewResponse("about.csp");
    callback(resp);
}