#include "object.h"
#include "statement.h"

#include <sstream>
#include <string_view>

using namespace std;

namespace Runtime {

void ClassInstance::Print(std::ostream& os)
{
    if (cls_.methods_.count("__str__")==1)
    {
        auto item = Call("__str__",{});

        item.Get()->Print(os);
    }
    else
    {
        os<< this;
    }

}

bool ClassInstance::HasMethod(const std::string& method, size_t argument_count) const
{
    if (cls_.GetMethod(method))
    {
        return  cls_.GetMethod(method)->formal_params.size()==argument_count;
    }
}

const Closure& ClassInstance::Fields() const
{
    return closure_;
}

Closure& ClassInstance::Fields()
{
    return  closure_;
}

ClassInstance::ClassInstance(const Class& cls):cls_(cls)
{
   // Fields()["self"] =ObjectHolder::Share(*this);
}

ObjectHolder ClassInstance::Call(const std::string& method, const std::vector<ObjectHolder>& actual_args)
{
    /*Closure tmp= Fields();
    tmp["self"] = ObjectHolder::Share(*this);
    auto method_ptr = cls_.GetMethod(method);
    auto it1 =actual_args.begin();
    auto it2 = method_ptr->formal_params.begin();
    for (;it1!=actual_args.end();it1++,it2++)
    {
        tmp[*it2]=*it1;
    }
    tmp.erase("return");
     auto item = method_ptr->body.get()->Execute(Fields());
    if (tmp.count("return") == 1) return tmp["return"];
      return  item;*/


    Fields()["self"] = ObjectHolder::Share(*this);
    auto method_ptr = cls_.GetMethod(method);
    auto it1 =actual_args.begin();
    auto it2 = method_ptr->formal_params.begin();
    for (;it1!=actual_args.end();it1++,it2++)
    {
        Fields()[*it2]=*it1;
    }
    Fields().erase("return");
     auto item = method_ptr->body.get()->Execute(Fields());
    if (Fields().count("return") == 1) return Fields()["return"];
      return  item;

}

Class::Class(const std::string & name, std::vector<Method> methods, const Class* parent):
    parent_(parent),name_(name)
{
    for (auto &item:methods)
    {
        methods_[item.name]=move(item);
    }
}

const Method* Class::GetMethod(const std::string& name) const
{
    if (methods_.count(name))
    {
        return  &methods_.at(name);
    }
    if (parent_)
    {
        return  parent_->GetMethod(name);
    }

    return  nullptr;

}

void Class::Print(ostream& os)
{
    if (methods_.count("__str__")==1)
    {
        os<< "Class_print";
    }
    else
    {
        os<< this;
    }
}

const std::string& Class::GetName() const
{
    return  name_;
}

void Bool::Print(std::ostream& os)
{
    if (GetValue()) os <<"True";
    else  os <<"False";
}

} /* namespace Runtime */
