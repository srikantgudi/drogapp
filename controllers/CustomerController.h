#pragma once

#include <drogon/HttpController.h>
#include <drogon/drogon.h>

using namespace drogon;

class CustomerController : public drogon::HttpController<CustomerController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CustomerController::home, "/customers", Get);
    ADD_METHOD_TO(CustomerController::orders, "/orders/{1}", Get);
    ADD_METHOD_TO(CustomerController::orderdetails, "/order/{1}/details", Get);
    METHOD_LIST_END

    void home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void orders(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, long &&custid);
    void orderdetails(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, long &&orderid);
};
