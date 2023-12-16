#include "statement.h"
#include "object.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace Ast {

using Runtime::Closure;

ObjectHolder Assignment::Execute(Closure& closure)
{
    return  closure[var_name] = right_value.get()->Execute(closure);
}

Assignment::Assignment(std::string var, std::unique_ptr<Statement> rv):var_name(var), right_value( move(rv))
{

}

VariableValue::VariableValue(std::string var_name):dotted_ids({var_name})
{

}

VariableValue::VariableValue(std::vector<std::string> dotted_ids):dotted_ids(dotted_ids)
{

}

ObjectHolder VariableValue::Execute(Closure& closure)
{
    Closure *current =&closure;

    for ( auto it =dotted_ids.begin();it!=dotted_ids.end()-1;it++)
    {
        if ((*current).count(*it))
        {
            current =&(*current).at(*it).TryAs<Runtime::ClassInstance>()->Fields();
        }
        else {
            std::runtime_error("VariableError");
        }
    }
    if ((*current).count(dotted_ids.back()))
        return  (*current)[dotted_ids.back()];
    else throw  std::runtime_error("VariableError");

}

unique_ptr<Print> Print::Variable(std::string var)
{
    auto arg = make_unique <VariableValue>(var);
    auto res =make_unique <Print> (move(arg));
    return  res;
}

Print::Print(unique_ptr<Statement> argument)//:args({move(argument)})
{
    args.push_back(move(argument));
}

Print::Print(vector<unique_ptr<Statement>> args_)//:args(move(args))
{
    for (auto &item:args_)
    {
        args.push_back(move(item));
    }
}

ObjectHolder Print::Execute(Closure& closure)
{
    bool is_first = true;
    for (auto &item:args)
    {
        if (is_first) { is_first =false;}
        else { *output <<' ';}
        auto tmp = item.get()->Execute(closure);
        if (tmp.Get()!=nullptr)
        {
            tmp->Print(*output);
        }
        else
        {
            *output <<"None";
        }

    }
    *output<<'\n';
    return  ObjectHolder();
}

ostream* Print::output = &cout;

void Print::SetOutputStream(ostream& output_stream)
{
    output = &output_stream;
}

MethodCall::MethodCall(
        std::unique_ptr<Statement> object
        , std::string method
        , std::vector<std::unique_ptr<Statement>> args
        ):object(move(object)),method(move(method)),args(move(args))
{
}

ObjectHolder MethodCall::Execute(Closure& closure)
{
    std::vector <ObjectHolder> actual;
    for (auto &item:args)
    {
        actual.push_back(item->Execute(closure));
    }
    return object.get()->Execute(closure).TryAs<Runtime::ClassInstance>()->Call(method,actual);
}

ObjectHolder Stringify::Execute(Closure& closure)
{
    ostringstream tmp;
    //Print::SetOutputStream(tmp);
    auto ars = argument.get()->Execute(closure);
    ars->Print(tmp);
    string res = tmp.str();
    return  ObjectHolder::Own(Runtime::String(res));
}

ObjectHolder Add::Execute(Closure& closure)
{
    auto lhs_holder = lhs.get()->Execute(closure);
    auto rhs_holder = rhs.get()->Execute(closure);

    if (lhs_holder.TryAs<Runtime::Number>()&&rhs_holder.TryAs<Runtime::Number>())
    {
        return ObjectHolder::Own(Runtime::Number(lhs_holder.TryAs<Runtime::Number>()->GetValue()
                                                 +rhs_holder.TryAs<Runtime::Number>()->GetValue()));
    }
    else if (lhs_holder.TryAs<Runtime::String>()&&rhs_holder.TryAs<Runtime::String>())
    {
        return ObjectHolder::Own(Runtime::String(lhs_holder.TryAs<Runtime::String>()->GetValue()
                                                 +rhs_holder.TryAs<Runtime::String>()->GetValue()));
    }
    else if (lhs_holder.TryAs<Runtime::ClassInstance>()&&rhs_holder.TryAs<Runtime::String>())
    {
        if (lhs_holder.TryAs<Runtime::ClassInstance>()->HasMethod("__add__",1))
        {
            return   ObjectHolder::Own (Runtime::String(lhs_holder.TryAs<Runtime::ClassInstance>()->Call("__add__",{rhs_holder}).
                                                        TryAs<Runtime::String>()->GetValue()))  ;
        }
        else
        {
            throw runtime_error("Error");
        }

    }
    else if (lhs_holder.TryAs<Runtime::String>()&&rhs_holder.TryAs<Runtime::ClassInstance>())
    {
        if (rhs_holder.TryAs<Runtime::ClassInstance>()->HasMethod("__add__",1))
        {
            return   ObjectHolder::Own (Runtime::String(rhs_holder.TryAs<Runtime::ClassInstance>()->Call("__add__",{lhs_holder}).
                                                        TryAs<Runtime::String>()->GetValue()))  ;
        }
        else
        {
            throw runtime_error("Error");
        }
    }
    else
    {
        throw runtime_error("Error");
    }

}

ObjectHolder Sub::Execute(Closure& closure)
{
    auto lhs_holder = lhs.get()->Execute(closure);
    auto rhs_holder = rhs.get()->Execute(closure);
    if (lhs_holder.TryAs<Runtime::Number>()&&rhs_holder.TryAs<Runtime::Number>())
    {
        return ObjectHolder::Own(Runtime::Number(lhs_holder.TryAs<Runtime::Number>()->GetValue()
                                                 -rhs_holder.TryAs<Runtime::Number>()->GetValue()));
    }
    else
    {
        throw runtime_error("Error");
    }
}

ObjectHolder Mult::Execute(Runtime::Closure& closure)
{
    auto lhs_holder = lhs.get()->Execute(closure);
    auto rhs_holder = rhs.get()->Execute(closure);
    if (lhs_holder.TryAs<Runtime::Number>()&&rhs_holder.TryAs<Runtime::Number>())
    {
        return ObjectHolder::Own(Runtime::Number(lhs_holder.TryAs<Runtime::Number>()->GetValue()
                                                 *rhs_holder.TryAs<Runtime::Number>()->GetValue()));
    }
    else
    {
        throw runtime_error("Error");
    }
}

ObjectHolder Div::Execute(Runtime::Closure& closure)
{
    auto lhs_holder = lhs.get()->Execute(closure);
    auto rhs_holder = rhs.get()->Execute(closure);
    if (lhs_holder.TryAs<Runtime::Number>()&&rhs_holder.TryAs<Runtime::Number>())
    {
        return ObjectHolder::Own((Runtime::Number(lhs_holder.TryAs<Runtime::Number>()->GetValue()
                                                  /rhs_holder.TryAs<Runtime::Number>()->GetValue())));
    }
    else
    {
        throw runtime_error("Error");
    }
}

ObjectHolder Compound::Execute(Closure& closure)
{
    for (auto &item:statements)
    {
        item.get()->Execute(closure);
    }
    return ObjectHolder();
}


ObjectHolder Return::Execute(Closure& closure)
{
    if (statement.get())
    {
        closure["return"] = statement.get()->Execute(closure);
        return closure["return"];
    }
    else
    {
        return  ObjectHolder();
    }
}

ClassDefinition::ClassDefinition(ObjectHolder class_):cls(class_),
    class_name(class_.TryAs<Runtime::Class>()->name_)
{

}

ObjectHolder ClassDefinition::Execute(Runtime::Closure& closure)
{
    return  cls;
}

FieldAssignment::FieldAssignment(
        VariableValue object, std::string field_name, std::unique_ptr<Statement> rv
        )
    : object(std::move(object))
    , field_name(std::move(field_name))
    , right_value(std::move(rv))
{
}

ObjectHolder FieldAssignment::Execute(Runtime::Closure& closure)
{
    Closure* current_closure = &closure;
    for (auto &item:object.dotted_ids)
    {

        if (current_closure->count(item))
        {
            current_closure =  &current_closure->at(item).TryAs<Runtime::ClassInstance>()->Fields();
        }
        else
        {
            break;
        }

    }
    auto val =  right_value.get()->Execute(*current_closure);
    (*current_closure)[field_name] = move(val);
    return  (*current_closure)[field_name];
}

IfElse::IfElse(
        std::unique_ptr<Statement> condition,
        std::unique_ptr<Statement> if_body,
        std::unique_ptr<Statement> else_body
        ):condition(move(condition)),if_body(move(if_body)),
    else_body(move(else_body))
{}

ObjectHolder IfElse::Execute(Runtime::Closure& closure)
{
    if (Runtime::IsTrue(condition->Execute(closure)))
    {
        return if_body->Execute(closure);
    }
    else
    {
        if (else_body)
        {
            return else_body->Execute(closure);
        }
        else
        {
            return  ObjectHolder();
        }
    }
}

ObjectHolder Or::Execute(Runtime::Closure& closure)
{
    return ObjectHolder::Own(Runtime::Bool(Runtime::IsTrue(lhs.get()->Execute(closure))||
                                           Runtime::IsTrue(rhs.get()->Execute(closure)) ));
}

ObjectHolder And::Execute(Runtime::Closure& closure)
{
    return ObjectHolder::Own(Runtime::Bool(Runtime::IsTrue(lhs.get()->Execute(closure))&&
                                           Runtime::IsTrue(rhs.get()->Execute(closure)) ));
}

ObjectHolder Not::Execute(Runtime::Closure& closure)
{
    return ObjectHolder::Own(Runtime::Bool(!Runtime::IsTrue(argument->Execute(closure))));
}

Comparison::Comparison(
        Comparator cmp, unique_ptr<Statement> lhs, unique_ptr<Statement> rhs
        ):comparator(cmp),left (move(lhs)),right(move(rhs))
{
}

ObjectHolder Comparison::Execute(Runtime::Closure& closure)
{
    return  ObjectHolder::Own(Runtime::Bool(comparator(left->Execute(closure),right->Execute(closure))));
}

NewInstance::NewInstance(
        const Runtime::Class& class_, std::vector<std::unique_ptr<Statement>> args
        )
    : class_(class_)
    , args(std::move(args))
{

}

NewInstance::NewInstance(const Runtime::Class& class_) : NewInstance(class_, {})
{
}

ObjectHolder NewInstance::Execute(Runtime::Closure& closure)
{
    auto item = Runtime::ClassInstance(class_);
    if (item.HasMethod("__init__",args.size()))
    {
        std::vector <ObjectHolder> arg_holder;
        for (auto &item:args)
        {
            arg_holder.push_back(item->Execute(closure));
        }
        item.Call("__init__",arg_holder);
    }
    //closure["self"] =ObjectHolder::Share(item);
    //return  closure["self"];

    return ObjectHolder::Own(move(item));
}


} /* namespace Ast */
