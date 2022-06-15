#include <iostream>
#include <chrono>
#include "socket.hpp"
#include "client.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <stdio.h>
    #include <sys/select.h>
    #include <termios.h>
    #include <stropts.h>
    int _kbhit() {
        static const int STDIN = 0;
        static bool initialized = false;

        if (! initialized) {
            // Use termios to turn off line buffering
            termios term;
            tcgetattr(STDIN, &term);
            term.c_lflag &= ~ICANON;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            initialized = true;
        }

        int bytesWaiting;
        ioctl(STDIN, FIONREAD, &bytesWaiting);
        return bytesWaiting;
    }
#endif

int main(int argc, char const *argv[])
{
    try
    {   
        if(argc != 4) {
            std::string error = "Number of argument isn't correct. To launch the program please run the command <program> <ip/mdns> <port> <use_mdns>";
            throw std::invalid_argument(error);
        }

        std::string ip_mdns = argv[1];
        int port = std::atoi( argv[2] );
        bool mdns = (strcmp(argv[3],"true") == 0);

        if(!Socket::Start()) {
            std::string error = "Error initialisation " + std::to_string(Socket::GetError()) + " Error occured in file : " + __FILE__ + " at line : " + std::to_string(__LINE__);
            throw std::runtime_error(error);
        }

        TCPSocket client;

        if(mdns) {
            if(!client.Connect_MDNS(ip_mdns, port)) {
                std::string error = "Error while connecting to the server [" + ip_mdns + ',' + std::to_string(port) + "] : " + std::to_string(Socket::GetError()) + " Error occured in file : " + __FILE__ + " at line : " + std::to_string(__LINE__);
                throw std::runtime_error(error);
            }
        }
        else {
            if(!client.Connect_IP(ip_mdns, port)) {
                std::string error = "Error while connecting to the server [" + ip_mdns + ',' + std::to_string(port) + "] : " + std::to_string(Socket::GetError()) + " Error occured in file : " + __FILE__ + " at line : " + std::to_string(__LINE__);
                throw std::runtime_error(error);
            }
        }

        bool end = false;
        client.sendData('I'); ///I stands for init
        auto lastDataSend = std::chrono::steady_clock::now();

        while( !end )
        {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = now-lastDataSend;
            if(_kbhit())
            {
                char c = _getch();
                if(c == 27)
                    goto end;
                client.sendData(c);
                lastDataSend = std::chrono::steady_clock::now();
            }
            if(elapsed_seconds.count() > 1.0)
            {
                client.sendData("0");
            }
        }
end:
        std::cout << "End of the communication, releasing the socket...";
        Socket::Release();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}
