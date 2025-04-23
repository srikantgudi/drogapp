#include "ProductController.h" // For drogon::app()
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <iomanip>
#include <sstream>
#include <vector>

#include "Product.h"

using namespace std;

void ProductController::home(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

  clientPtr->execSqlAsync(
      "SELECT * FROM categories",
      [callback](const drogon::orm::Result &result) {
        // Json::Value data(Json::arrayValue);  // Array of categories

        stringstream html("");
        stringstream rows("");
        int rowcount = 0;

        for (const auto &row : result) {
          rowcount++;
          rows << "<tr>"
               << "<td style='cursor:pointer' hx-get='/products/"
               << row["categoryid"].as<int>() << "' "
               << "hx-target='#products' title='"
               << row["description"].as<std::string>() << "'>"
               << row["categoryname"].as<std::string>() << "</td></tr>";
        }
        html << "<div>" << rowcount << " rows</div>"
             << "<table>"
             << "<thead><tr><td>Name</td></thead>"
             << "<tbody>" << rows.str();
        html << "</tbody></table>";

        // Insert into view
        HttpViewData viewData;
        viewData.insert("categories", html.str());

        auto resp =
            HttpResponse::newHttpViewResponse("producthome.csp", viewData);

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
  // auto f = clientPtr->execSqlAsyncFuture(
    //     "SELECT * FROM products Where categoryid=$1", catid);
    
  stringstream sqls("");
  sqls << "SELECT * FROM products Where categoryid=" << catid;
  auto f = clientPtr->execSqlAsyncFuture(sqls.str());
  std::vector<Product> vproducts;
  try {
    stringstream html("");
    stringstream rows("");
    auto result = f.get(); // Block until we get the result or catch the exception;
    auto rowcount = result.size();
    int i = 0;
    // Product p;
    // for (auto row : result) {
    //   p = Product{
    //     id : row["productid"].as<int16_t>(),
    //     name : row["productname"].as<string>(),
    //     price : stof(row["unitprice"].as<string>()),
    //     rorlevel : row["reorderlevel"].as<int16_t>()
    //   };
    //   vproducts.push_back(p);
      // cout << p.id << " " << p.name << " " << p.price << " " << p.rorlevel << endl;

      // rows << "<tr>"
      //      << "<td>" << row["productname"].as<string>() << "</td>"
      //      << "<td align='right'>" << setw(12) << fixed << setprecision(2)
      //      << stof(row["unitprice"].as<std::string>()) << "</td>"
      //      << "</tr>";
    // }
    // html << "<div>" << rowcount << " rows</div>"
    //      << "<table><thead>"
    //      << "<tr><td width='400'>Name</td><td align='right'>Unit price</td>"
    //      << "</thead><tbody>" << rows.str() << "</tbody></table>";
    // Insert into view
    HttpViewData viewData;
    viewData.insert("products", result);
    auto resp = HttpResponse::newHttpViewResponse("products.csp", viewData);
    callback(resp);
  } catch (const drogon::orm::DrogonDbException &e) {
    std::cerr << "DB error: " << e.base().what() << std::endl;
  }
}
