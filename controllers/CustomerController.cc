#include "CustomerController.h"
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <iomanip>
#include <sstream>
#include <vector>
#include "Customer.h"

using namespace std;

void CustomerController::home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

    clientPtr->execSqlAsync(
        "SELECT * FROM customers",
        [callback](const drogon::orm::Result &result)
        {
            cout << result.size() << " customers" << endl;
            std::vector<Customer> vcustomers;
            for (auto row : result)
            {
                vcustomers.push_back(Customer{
                    id : row["id"].as<int>(),
                    customerid : row["customerid"].as<std::string>(),
                    name : row["companyname"].as<std::string>(),
                    city : row["city"].as<std::string>(),
                    country : row["country"].as<std::string>()
                });
            }
            // Insert into view
            HttpViewData custViewData;
            std::string message = "customers home page";
            custViewData.insert("message", message);
            custViewData.insert("customers", vcustomers);
            auto resp = HttpResponse::newHttpViewResponse("customershome.csp", custViewData);
            callback(resp);
        },
        [](const drogon::orm::DrogonDbException &e)
        {
            std::cerr << "DB error: " << e.base().what() << std::endl;
        });
}

void CustomerController::orders(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, long custid)
{

}
