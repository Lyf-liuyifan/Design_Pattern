#include <iostream>
#include <string>
#include <utility>
//我们可以复制各个字段来深拷贝

//例如

struct Address{
    std::string street;
    std::string city;
    std::string state;
    std::string zip;
    Address(const std::string& street, const std::string& city, const std::string& state, const std::string& zip)
        : street(street), city(city), state(state), zip(zip) {}
    Address(const Address& other) : street(other.street), city(other.city), state(other.state), zip(other.zip) {}
};

struct Contact{
    std::string name;
    Address* address;
    Contact(const std::string& name, const Address& address) : name(name), address(new Address(address)) {}
    Contact(const Contact& other) : name(other.name), address(new Address(*other.address)) {}                   //这里发生的是深拷贝
    Contact(Contact&& other) noexcept : name(std::move(other.name)), address(std::move(other.address)) {}
    Contact& operator=(const Contact& other) {
        if (this != &other) {
            name = other.name;
            delete address;
            address = new Address(*other.address);
        }
        return *this;
    }
    Contact& operator=(Contact&& other) noexcept {
        if (this != &other) {
            delete address;
            name = std::move(other.name);
            address = other.address;
            other.address = nullptr;
        }
        return *this;
    }
    ~Contact() {
        delete address;
    }
};