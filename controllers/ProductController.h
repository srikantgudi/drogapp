#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class ProductController : public drogon::HttpController<ProductController>
{
  public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(ProductController::home, "/categories", Get);
      ADD_METHOD_TO(ProductController::products, "/products/{1}", Get);
    METHOD_LIST_END
    
    void home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void products(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, long catid);
    
};
