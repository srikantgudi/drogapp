#include "HomeController.h"
#include <drogon/drogon.h>             // For drogon::app()
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <sstream>
#include <vector>

using namespace std;
using namespace drogon;
// using namespace drogon::orm;

void HomeController::home(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

    clientPtr->execSqlAsync("SELECT * FROM categories",
        [callback](const drogon::orm::Result &result) {
            // Json::Value data(Json::arrayValue);  // Array of categories

            stringstream html("");

            html << "<style>* {font-family:helvetica;}\ntd {padding: 0.25rem 0.5rem;}\nthead {background-color:#666;color:beige;}</style>"
                << "<table>"
                << "<thead><tr><td>Id</td><td>Name</td></thead>"
                << "<tbody>"
                ;
            for (const auto &row : result) {
                html << "<tr><td>" << row["categoryid"].as<int>() << "</td>"
                    << "<td>" << row["categoryname"].as<std::string>()
                    << "</td></tr>";
            }
            html << "</tbody></table>";

            // Insert into view
            HttpViewData viewData;
            viewData.insert("data", html.str());
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