#ifndef NOKIA_SERVER_H
#define NOKIA_SERVER_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

/// A program alapesetben minden beallito fuggvenyhivas hatasara elkuld egy xml uzenetet a nokia tablet fele
/// Ez nem mindig a leghatekonyabb modja a kommunikacionak, ugyanis nagy overhead-et okoz
/// Ezert amikor tobb dolgot akarunk egyszerre beallitani, erdemes meghivni a pause_sending() fuggvenyt,
/// melynek hatasara csak a send() fuggveny meghivasakor kerulnek elkuldesre a parancsok, egy csomagban.
/// A send() meghivasa utan ismet az eredeti mukodes lep ervenyre, azaz egyesevel kuldi a parancsokat.

typedef enum MSG_TYPE {NONE = 0, INFO = 1, WARNING = 2, ERROR = 3} msg_type;
typedef enum ROBOT_STATE {CLOSED = 0, OPENED = 1} robot_state;

class nokia_server
{
    public:
        nokia_server(std::string ipaddr = "192.168.2.15");
        ~nokia_server();

        void set_location(double x, double y, double phi); //Aktualis pozicio beallitasa
        void set_speed(double left_wheel, double right_wheel); //Aktualis sebesseg megadasa
        void set_state(robot_state state); //Allapot megadasa (nyitott/csukott)
        void set_opponent(std::string name, double x, double y, double phi, double r); //egy bizonyos nevu ellenfel poziciojanak es meretenek megadasa
        void send_message(std::string msg, msg_type type, int alert_interval); //uzenet kijelzese. amennyiben az alert_interval > 0, az uzenet villogva megjelenik a terkep felett
        void send_variable(std::string name, std::string value); //egy bizonyos nevu valtozo ertekenek elkuldese
        void clear_opponents(); //letorli a palyarol az ellenfeleket.

        void pause_sending();
        void send();

    protected:
    private:
        bool instant_send;
        std::string current_xml;
        struct sockaddr_in address;
        unsigned address_len;
        unsigned short port;
        int sd; //socket descriptor
        void do_send();
        std::string from_int(int n);
};

#endif // NOKIA_SERVER_H
