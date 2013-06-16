#ifndef __ARGS_HPP__
#define __ARGS_HPP__

#include <string>
#include <vector>

using namespace std;

class ArgumentInfo {
  public:
    ArgumentInfo(string type, string name, string default_value, string description, bool optional, void* reference);
    void Print();
    bool Assign(const string& value);

    const string& type() { return type_; }
    const string& name() { return name_; }
    bool IsOptional() { return optional_; }

  private:
    string type_;
    string name_;
    string default_value_;
    string description_;
    bool optional_;
    void* reference_;
};

#define DEFINE_int ;
#define DEFINE_bool ;
#define DEFINE_string ;

#define DEFINE_ARG(type, name, description) \
    DEFINE_##type; \
    type ARG_##name; \
    ArgumentInfo _info_##name(#type, #name, "", description, false, &ARG_##name)

#define DEFINE_OPTIONAL_ARG(type, name, default_value, description) \
    DEFINE_##type; \
    type ARG_##name = default_value; \
    ArgumentInfo _info_##name(#type, #name, #default_value, description, true, &ARG_##name)

#define DECLARE_ARG(type, name) extern type ARG_##name

// Extracts parameter values from the passed-in arguments.
int ParseArguments(int argc, const char* argv[]);

#endif // __ARGS_HPP__
