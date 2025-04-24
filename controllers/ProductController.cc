#include "ProductController.h" // For drogon::app()
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <iomanip>
#include <sstream>
#include <vector>

#include "Product.h"
#include "Category.h"

using namespace std;

void ProductController::home(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

  clientPtr->execSqlAsync(
      "SELECT * FROM categories",
      [callback](const drogon::orm::Result &result) {
        std::vector<Category> vcategories;
        for (const auto &row : result) {
          vcategories.push_back(Category{
            id : row["categoryid"].as<int>(),
            name: row["categoryname"].as<std::string>(),
            description: row["description"].as<std::string>()
          });
        }
        // Insert into view
        HttpViewData viewData;
        viewData.insert("categories", vcategories);
        auto resp = HttpResponse::newHttpViewResponse("producthome.csp", viewData);
        callback(resp);
      },
      [](const drogon::orm::DrogonDbException &e) {
        std::cerr << "DB error: " << e.base().what() << std::endl;
      });
}
void ProductController::products(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback, long catid) {
  auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");
  stringstream sqls("");
  sqls << "SELECT * FROM products Where categoryid=" << catid;
  clientPtr->execSqlAsync(
    sqls.str(), // sql query string
    [callback](const drogon::orm::Result &result) {
      std::vector<Product> vproducts;
      Product p;
      for (auto row : result) {
        p = Product{
          id : row["productid"].as<int16_t>(),
          name : row["productname"].as<string>(),
          price : stof(row["unitprice"].as<string>()),
          rorlevel : row["reorderlevel"].as<int16_t>()
        };
        vproducts.push_back(p);
      }
      // Insert into view
      HttpViewData viewData;
      viewData.insert("products", vproducts);
      auto resp = HttpResponse::newHttpViewResponse("products.csp", viewData);
      callback(resp);
    },
    [](const drogon::orm::DrogonDbException &e) {
      std::cerr << "DB error: " << e.base().what() << std::endl;
    });
}
