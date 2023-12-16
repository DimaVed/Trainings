#include "object_holder.h"
#include "object.h"

namespace Runtime {

ObjectHolder ObjectHolder::Share(Object& object) {
  return ObjectHolder(std::shared_ptr<Object>(&object, [](auto*) { /* do nothing */ }));
}

ObjectHolder ObjectHolder::None() {
  return ObjectHolder();
}

Object& ObjectHolder::operator *() {
  return *Get();
}

const Object& ObjectHolder::operator *() const {
  return *Get();
}

Object* ObjectHolder::operator ->() {
  return Get();
}

const Object* ObjectHolder::operator ->() const {
  return Get();
}

Object* ObjectHolder::Get() {
  return data.get();
}

const Object* ObjectHolder::Get() const {
  return data.get();
}

ObjectHolder::operator bool() const {
  return Get();
}

bool IsTrue(ObjectHolder object)
{
    if (object.TryAs<Runtime::Bool>())
    {
        return  object.TryAs<Runtime::Bool>()->GetValue();
    }
    else if (object.TryAs<Runtime::Number>())
    {
        return  object.TryAs<Runtime::Number>()->GetValue();
    }
    else if (object.TryAs<Runtime::String>())
    {
        return  object.TryAs<Runtime::String>()->GetValue().size();
    }
    else if (object.TryAs<Runtime::Class>())
    {
        return  true;
    }
    else if (object.TryAs<Runtime::ClassInstance>())
    {
        return  true;
    }
    else return  false;
}

}
