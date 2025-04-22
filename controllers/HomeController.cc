#include "HomeController.h"
#include <drogon/drogon.h>             // For drogon::app()
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <sstream>
#include <vector>
#include "Category.h"

using namespace std;
using namespace drogon;
// using namespace drogon::orm;

void HomeController::home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

    clientPtr->execSqlAsync("SELECT * FROM categories",
        [callback](const drogon::orm::Result &result) {
            // Json::Value data(Json::arrayValue);  // Array of categories

            stringstream ss("");

            ss << "<style>* {font-family:helvetica;}\ntd {padding: 0.25rem 0.5rem;}\nthead {background-color:#666;color:beige;}</style>"
            << "<table>"
            << "<thead><tr><td>Id</td><td>Name</td></thead>"
            << "<tbody>"
            ;
            std::vector<Category> categories;

            for (const auto &row : result) {
                categories.push_back(Category{
                    row["categoryid"].as<int>(),
                    row["categoryname"].as<std::string>()
                });
                ss << "<tr><td>" << row["categoryid"].as<int>() << "</td>"
                    << "<td>" << row["categoryname"].as<std::string>()
                    << "</td></tr>";
            }
            ss << "</tbody></table>";

            cout << categories.size() << " rows populated in vector\n";

            // Insert into view
            HttpViewData viewData;
            viewData.insert("data", ss.str());
            viewData.insert("title","Categories");

            auto resp = HttpResponse::newHttpViewResponse("home.csp", viewData);

            callback(resp);
        },
        [](const drogon::orm::DrogonDbException &e) {
            std::cerr << "DB error: " << e.base().what() << std::endl;
        });
}

void HomeController::about(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // HttpViewData data;
    // data.insert("title", "Add new item");

    auto resp = HttpResponse::newHttpViewResponse("about.csp");
    callback(resp);
}