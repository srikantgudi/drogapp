#pragma once

#include <drogon/HttpController.h>

using namespace drogon;


class HomeController : public drogon::HttpController<HomeController>
{
  public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(HomeController::home, "/", Get);
      ADD_METHOD_TO(HomeController::about, "/about", Get);
    METHOD_LIST_END

    void home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void about(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
