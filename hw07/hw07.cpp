// Задание 7 - реализовать асинхронный сервер
// Клиент взаимодействует с сервером через утилиту telnet
// Сервер должен поддерживать и обрабатывать следующие команды:
// 1. username - возвращает имя пользователя
// 2. hostname - возвращает имя компьютера
// 3. serverdatetime - возвращает время и дату на сервере
// 4. ls - содержимое папки на сервере ( пример вызова ls ~/ )
// 5. mkfile - создать файл на сервере ( пример вызова mkfile ~/new_file.txt )
// 6. mkdir - создать папку на сервере ( пример вызова mkdir ~/new_dir )
// 7. rmfile - удалить файл на сервере ( пример вызова rmfile ~/new_file.txt )
// 8. rmdir - удалить папку на сервере ( пример вызова rmdir ~/new_dir )
// 9. readfile - прочитать содержимое файла на сервере ( пример (прочитать 10 байт, по-умолчанию весь файл):
														// readfile ~/new_file.txt 10 )
// 10. quit - завершить сеанс
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <boost/algorithm/string.hpp>

using btcp = boost::asio::ip::tcp;

struct Connection
{
    btcp::socket socket;
    boost::asio::streambuf read_buffer;
    Connection(boost::asio::io_service &io_service) :
    socket(io_service),
    read_buffer(){}
};

class Server
{
    boost::asio::io_service ioservice;
    btcp::acceptor acceptor;
    std::list<Connection> connections;

public:
    Server(uint16_t port) : acceptor(ioservice, btcp::endpoint(btcp::v4(), port)) { start_accept(); }


    void send_response(std::list<Connection>::iterator con_handle, const std::string& message){
        auto buff = std::make_shared<std::string>(message+"\n");
        auto handler =boost::bind(&Server::on_write, this, con_handle, buff,boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff),handler);

    }

    void on_read(std::list<Connection>::iterator con_handle, boost::system::error_code const &err, size_t bytes_transfered)
    {
        if (bytes_transfered > 0 )
        {
            std::istream is(&con_handle->read_buffer);
            std::string line;
            std::getline(is, line);
            boost::algorithm::trim(line);
            std::vector<std::string> args;
            boost::split(args, line, boost::is_any_of(" "), boost::token_compress_on);

            if (args[0] == "username") {
                auto username = boost::asio::ip::host_name();
                send_response(con_handle, username );
            } else if (args[0] == "hostname") {
                auto hostname = boost::asio::ip::host_name();
                send_response(con_handle, hostname );
            } else if(args[0] == "serverdatetime") {
                std::time_t now = std::time(nullptr);
                std::string datetime = std::asctime(std::localtime(&now));
                send_response(con_handle, datetime);
            } else if (args[0] == "ls") {
                    std::string result;
                    try {
                        for (boost::filesystem::directory_iterator itr(args[1]);
                        itr != boost::filesystem::directory_iterator(); ++itr) {
                            result += itr->path().filename().string() + "\n";
                        }
                    } catch (const boost::filesystem::filesystem_error& ex) {
                        result = ex.what();
                    }
                    send_response(con_handle, result);
            } else if (args[0] == "mkfile"){
                boost::filesystem::path file_path(args[1]);
                boost::filesystem::ofstream ofs(file_path);
                if(ofs.is_open()){
                    std::string result = "File created successfully";
                    send_response(con_handle, result);
                } else{
                    std::string result = "Failed to create file";
                    send_response(con_handle, result);
                }
            } else if (args[0] == "mkdir"){
                boost::filesystem::path dir_path(args[1]);
                if (boost::filesystem::create_directory(dir_path)) {
                    std::string result = "Directory created successfully";
                    send_response(con_handle, result);
                } else {
                    std::string result = "Failed to create directory";
                    send_response(con_handle, result);
                }
            }  else if (args[0] == "rmfile") {
                boost::filesystem::path file_path(args[1]);
                std::string result;
                try {
                    boost::filesystem::remove(file_path);
                    result = "File " + args[1] + " successfully deleted.";
                }
                catch (const boost::filesystem::filesystem_error& ex) {
                    result = "Failed to delete file " + args[1] + ". " + ex.what();
                }
                send_response(con_handle, result);
            } else if (args[0] == "rmdir") {
                boost::system::error_code ec;
                boost::filesystem::remove_all(args[1], ec);
                if (ec) {
                    send_response(con_handle, ec.message());
                } else {
                    send_response(con_handle, "Directory removed successfully");
                }
            } else if (args[0] == "readfile"){
                std::string response;
                boost::filesystem::path file_path(args[1]);
                std::ifstream file(args[1]);
                size_t num_bytes;
                if (file.is_open()) {
                    if (args.size() == 2) {
                        num_bytes = boost::filesystem::file_size(args[1]);
                        response.resize(num_bytes);
                        file.read(&response[0], num_bytes);

                    } else {
                        num_bytes = std::stoi(args[2]);
                        response.resize(num_bytes);
                        file.read(&response[0], num_bytes);
                    }
                    file.close();
                }
                send_response(con_handle, response);
            } else if (args[0] == "quit") {
                std::string goodbye_msg = "Goodbye!";
                send_response(con_handle,goodbye_msg);
                con_handle->socket.close();
            } else  {
                std::cout << "Message Received: " << line << std::endl;
            }


        }

        if (!err){
            do_async_read(con_handle);
        } else if (!con_handle->socket.is_open()){
            std::cout << "Session closed\n" ;
        } else{
            std::cerr << "Error: " << err.message() << std::endl;
            connections.erase(con_handle);
        }
    }

    void do_async_read(std::list<Connection>::iterator con_handle)
    {
        auto handler = boost::bind(&Server::on_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
        boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer, "\n", handler);
    }

    void on_write(std::list<Connection>::iterator con_handle, std::shared_ptr<std::string> msg_buffer, boost::system::error_code const &err)
    {
        if (!err)
        {
            std::cout << "Finished sending message\n";
        }
        else
        {
            std::cerr << "Error: " << err.message() << std::endl;
            connections.erase(con_handle);
        }
    }

    void on_accept(std::list<Connection>::iterator con_handle, boost::system::error_code const &err)
    {
        if (!err)
        {
            std::cout << "Connection from: " << con_handle->socket.remote_endpoint().address().to_string() << "\n";
            std::cout << "Sending message\n";
            auto buff = std::make_shared<std::string>("Hello Client!\n\n\n");
            auto handler = boost::bind(&Server::on_write, this, con_handle, buff, boost::asio::placeholders::error);
            boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff), handler);
            do_async_read(con_handle);
        }
        else
        {
            std::cerr << "Error: " << err.message() << std::endl;
            connections.erase(con_handle);
        }
        start_accept();
    }
    void start_accept()
    {
        auto con_handle = connections.emplace(connections.begin(), ioservice);
        auto handler = boost::bind(&Server::on_accept, this, con_handle, boost::asio::placeholders::error);
        acceptor.async_accept(con_handle->socket, handler);
    }

    void run()
    {
        ioservice.run();
    }
};

int main()
{
    Server srv(12345);
    srv.run();
    return 0;
}
