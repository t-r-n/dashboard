#ifndef VALUESERVER_H
#define VALUESERVER_H


#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <string>
#include<QDebug>
using namespace boost::asio;
using boost::system::error_code;
typedef std::shared_ptr<ip::tcp::socket> sock_ptr;
class Client{
public:
    Client(sock_ptr sock):sock_(sock){};
    int do_read(){
        error_code ec;
        read(*sock_,boost::asio::buffer(buffer,32),transfer_at_least(1),ec);
        if(ec){
           is_out=true;
           return -1;
        }
        std::string digits(buffer,32);
        int digit=0;
        try{
            digit=stoi(digits);
        }catch(...){
            return -1;
        }
        return digit;
    }
    ip::tcp::socket& sock(){
        return *sock_;
    }
    bool time_out(){
        return is_out;
    }
private:
    std::shared_ptr<ip::tcp::socket> sock_;
    char buffer[32];
    bool is_out=false;
};
class ValueServer
{
public:

    ValueServer() = delete;
    ValueServer(const ValueServer& s) = delete;
    ValueServer operator=(const ValueServer& s) = delete;
    static ValueServer* getServer( int port = 8006) {
        qDebug()<<__LINE__<<endl;
           if(VServer==nullptr){
               qDebug()<<__LINE__<<endl;
               VServer=new ValueServer(port);
           }
           return VServer;
    }
    void pauseServer(){
        isContinue.store(false);
    }
    void startServer(){
        isContinue.store(true);
    }
    int getValue(){
        return value.load();
    }
private:
    void do_accept(){
        ip::tcp::acceptor acceptor(service,ip::tcp::endpoint(ip::tcp::v4(), port));
        while ( true) {
            while(!isContinue.load()){
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            sock_ptr sock=std::make_shared<ip::tcp::socket>(service);
            auto client_ptr=std::make_shared<Client>(sock);
            acceptor.accept(*sock);
            std::lock_guard<std::mutex>lc(clients_mutex);
            clients.push_back(client_ptr);
        }
    }
    void handle_clients_thread() {
        while ( true) {
            while(!isContinue.load()){
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            std::this_thread::sleep_for( std::chrono::milliseconds(1));
            std::lock_guard<std::mutex>lc(clients_mutex);
            for(auto it = clients.begin(); it!=clients.end(); ++it){
                int ret=(*it)->do_read();
                if(ret!=-1){
                    value.store(ret);
                }
            }

            // 删除已经超时的客户端
            for(auto it = clients.begin(); it!=clients.end(); ){
                if((*it)->time_out()){
                    it=clients.erase(it);
                }else{
                    it++;
                }
            }

        }
    }
private:

    ValueServer(int port):port(port){
        std::thread(&ValueServer::do_accept,this).detach();
        std::thread(&ValueServer::handle_clients_thread,this).detach();
    }
    static ValueServer* VServer;
    io_context service;
    sock_ptr sock;
    int port;
    std::list<std::shared_ptr<Client>>clients;
    std::mutex clients_mutex;
    std::atomic<int>value={0};
    std::atomic<bool>isContinue={true};
};

#endif // VALUESERVER_H
