#include <drogon/drogon.h>
int main() {
    drogon::app().loadConfigFile("../config.yaml");
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);
    std::cout << "\nRunning on http://localhost:5555\n";
    //Load config file
    // drogon::app().loadConfigFile("../config.json");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
