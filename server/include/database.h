#ifndef FASTTYPING_DATABASE_H
#define FASTTYPING_DATABASE_H
#include <iostream>
#include <mutex>
#include <pqxx/pqxx>

namespace FastTyping::Server {
    struct AbstractDatabase {
        AbstractDatabase() = default;
        virtual ~AbstractDatabase() = default;
        // USERS table
        virtual std::string getName(int) = 0;
        virtual std::string getPassword(int) = 0;
        virtual void setPassword(int, const std::string &) = 0;
        virtual int getId(const std::string &) = 0;            // or create it
        virtual bool nameExist(const std::string &) = 0;
        // MISTAKES table
        // TODO: ...
    };
    struct Database : AbstractDatabase {
    public:
        Database();
        std::string getPassword(int id) override;
        std::string getName(int) override;
        void setPassword(int, const std::string &) override; // base password == '0000'
        int getId(const std::string &) override;            // or create it
        bool nameExist(const std::string &) override;
        ~Database() override;
    private:
        mutable std::mutex mutex;
        pqxx::connection connect;
        std::string sql;// sql query
    };
}// namespace FastTyping::Server

#endif//FASTTYPING_DATABASE_H
