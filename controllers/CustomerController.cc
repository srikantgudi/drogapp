#include "CustomerController.h"
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <iomanip>
#include <sstream>
#include <vector>
#include "Customer.h"
#include "Order.h"
#include "Orderdetail.h"

using namespace std;

void CustomerController::home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

    clientPtr->execSqlAsync(
        "SELECT * FROM customers",
        [callback](const drogon::orm::Result &result)
        {
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

void CustomerController::orders(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, long &&custid)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");

    stringstream ordersqls("");
    ordersqls << "SELECT * FROM orders WHERE customerid=" << custid;
    clientPtr->execSqlAsync(
        ordersqls.str(), // orders sql for `custid`
        [callback](const drogon::orm::Result &result)
        {
            std::vector<Order> vorders;
            for (auto row : result)
            {
                Order order{
                    orderid : row["orderid"].as<int>(),
                    orderdate : row["orderdate"].as<std::string>(),
                    shipdate : row["shippeddate"].as<std::string>(),
                };
                vorders.push_back(order);
            }
            // Insert into view
            HttpViewData orderViewData;
            orderViewData.insert("orders", vorders);
            auto resp = HttpResponse::newHttpViewResponse("orders.csp", orderViewData);
            callback(resp);
        },
        [](const drogon::orm::DrogonDbException &e)
        {
            std::cerr << "DB error: " << e.base().what() << std::endl;
        });
}
void CustomerController::orderdetails(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, long &&orderid)
{
    auto clientPtr = drogon::app().getDbClient("pgNorthwindDB");
    stringstream orderdetailsqls("");
    orderdetailsqls << "SELECT p.productname, od.quantity, od.unitprice FROM orderdetails od "
                    << " JOIN products p on p.productid = od.productid WHERE od.orderid=" << orderid;
    
    clientPtr->execSqlAsync(
        orderdetailsqls.str(), // orders sql for `custid`
        [callback](const drogon::orm::Result &result)
        {
            cout << result.size() << " customers" << endl;
            std::vector<Orderdetail> vorderdets;
            for (auto row : result)
            {
                Orderdetail orderdet{
                    productname : row["productname"].as<string>(),
                    quantity : stof(row["quantity"].as<string>()),
                    unitprice : stof(row["unitprice"].as<string>()),
                };
                vorderdets.push_back(orderdet);
            }
            cout << vorderdets.size() << " order-details fetched\n";
            // Insert into view
            HttpViewData orderdetViewData;
            orderdetViewData.insert("orderdetails", vorderdets);
            auto resp = HttpResponse::newHttpViewResponse("orderdetails.csp", orderdetViewData);
            callback(resp);
        },
        [](const drogon::orm::DrogonDbException &e)
        {
            std::cerr << "DB error: " << e.base().what() << std::endl;
        });
}
