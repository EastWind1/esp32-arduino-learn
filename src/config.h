struct Config
{
    const char *wifiHost;                // WIFI SSID
    const char *wifiPassword;            // WIFI 密码
    const unsigned short int serverPort; // 服务器端口
};

struct Config CONFIG = {
    .wifiHost = "WIFI SSID",
    .wifiPassword = "WIFI PASSWORD",
    .serverPort = 9090
};