#pragma once

#include <drogon/HttpController.h>

using namespace drogon;


class HomeController : public drogon::HttpController<HomeController>
{
  public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(HomeController::root, "/", Get);
      ADD_METHOD_TO(HomeController::home, "/home", Get);
      ADD_METHOD_TO(HomeController::about, "/about", Get);
    METHOD_LIST_END

    void root(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void about(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
