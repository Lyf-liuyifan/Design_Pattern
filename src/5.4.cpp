#include "5.3.cpp"
#include <memory>
#include <string>
#include <chrono>
#include <fstream>

using std::unique_ptr;
using std::string;

struct EmployeeFactory
{
private:
    static unique_ptr<Contact> new_employee(const string& name, int suite, const Contact& prototype);
public:
    static unique_ptr<Contact> new_main_office_employee(const string& name, int suite, const Contact& prototype);
};
    
inline unique_ptr<Contact> EmployeeFactory::new_employee(const string& name, int suite, const Contact& prototype)
{
    auto contact = std::make_unique<Contact>(prototype);
    contact->name = name;
    if (contact->address) {
        contact->address->zip = std::to_string(suite);
    }

    // #region agent log
    {
        std::ofstream log("d:\\gitHub\\Design_Pattern\\.cursor\\debug.log", std::ios::app);
        if (log) {
            const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            log << "{\"sessionId\":\"debug-session\",\"runId\":\"pre-fix\",\"hypothesisId\":\"H1\","
                "\"location\":\"src/5.4.cpp:new_employee\",\"message\":\"created contact\","
                "\"data\":{\"name\":\"" << name << "\",\"suite\":" << suite << "},"
                "\"timestamp\":" << ts << "}"
                << std::endl;
        }
    }
    // #endregion agent log

    return contact;
}

inline unique_ptr<Contact> EmployeeFactory::new_main_office_employee(const string& name, int suite, const Contact& prototype)
{
    return new_employee(name, suite, prototype);
}

#ifdef CURSOR_DEBUG_RUN
int main()
{
    Address addr{"Street", "City", "State", "00000"};
    Contact proto{"Prototype", addr};
    auto emp = EmployeeFactory::new_main_office_employee("Alice", 101, proto);
    (void)emp;
    return 0;
}
#endif